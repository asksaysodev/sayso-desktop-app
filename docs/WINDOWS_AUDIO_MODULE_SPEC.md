# Windows Native Audio Module — Interface Specification

**Role:** implement a Windows native audio-capture module in C++ (WASAPI),
packaged as a Node.js/Electron native addon (`native_audio.node`, built with
`node-gyp` + NAN/N-API).

The module captures **two independent audio streams** during a live call — the
system output (the far-end voice) and the local microphone — and delivers each as
raw PCM to JavaScript. A JavaScript wrapper (provided) loads the compiled
`.node` and calls these methods; **implement to match that calling convention —
the interface is fixed.** A complete, production implementation of the same
interface already exists for macOS and is available as a behavioral reference.

There is nothing to build outside this one module: no UI, no app logic, no
networking — the host application owns all of that. Your surface is a single
`.cpp` file plus the Windows section of the build config.

---

## The interface — 13 methods

`Init` must export exactly these. Names are case-sensitive.

| # | Method | Returns | Async |
|---|---|---|---|
| 1 | `initialize` | `Promise<void>` | yes |
| 2 | `checkScreenRecordingGranted` | `boolean` | no |
| 3 | `requestScreenRecordingPermission` | `Promise<void>` | yes |
| 4 | `startSystemAudioCapture` | `Promise<boolean>` | yes |
| 5 | `stopSystemAudioCapture` | `Promise<{ success, error? }>` | yes |
| 6 | `isSystemAudioCaptureActive` | `boolean` | no |
| 7 | `startMicrophoneCapture` | `boolean \| { ok: false, reason? }` | sync value; JS awaits |
| 8 | `stopMicrophoneCapture` | `boolean` | JS awaits |
| 9 | `isMicrophoneCaptureActive` | `boolean` | no |
| 10 | `isMicRouteRecovering` | `boolean` | no |
| 11 | `setStreamingCallback` | `void` | no |
| 12 | `setMicrophoneStreamingCallback` | `void` | no |
| 13 | `setLifecycleEventCallback` | `void` | no |

The two capture paths (system audio, microphone) run **independently** and stream
through **separate callbacks** — stopping or losing one must never disturb the
other.

---

## Method contracts

### 1. `initialize()` → `Promise<void>`
One-time, idempotent setup: allocate audio clients, register the device-change
listener. The caller awaits this before every capture call. **Reject** if the
audio subsystem cannot be brought up — the caller treats that as fatal for the
session.

### 2. `checkScreenRecordingGranted()` → `boolean`
Non-prompting status read. **Windows has no screen-recording gate for loopback
capture — return `true`.** Must not block, prompt, or require `initialize()`.
(This method exists because the macOS backend needs a screen-recording grant for
its capture API; Windows does not.)

### 3. `requestScreenRecordingPermission()` → `Promise<void>`
On macOS this triggers the OS permission prompt. **On Windows it is a no-op —
just resolve.**

### 4. `startSystemAudioCapture(options)` → `Promise<boolean>`
Start **loopback capture of the system output** (the far-end call audio) on the
default render endpoint.

- Resolve `true` once capture is genuinely running and audio is flowing.
- **Reject** if it can't start. Also reject if a start or stop is already in
  flight (guard against overlapping calls corrupting state), with a short,
  descriptive reason in the error message (e.g. a timeout or a
  cancelled-by-stop condition — see §Errors).
- **No self-exclusion needed.** The host app emits no audio of its own, so plain
  render-endpoint loopback is correct. You do **not** need the per-process
  loopback API (`ActivateAudioInterfaceAsync` + `PROCESS_LOOPBACK`).
- Deliver audio through the **system-audio** callback (see §Buffer format,
  §Threading).

### 5. `stopSystemAudioCapture()` → `Promise<{ success, error? }>`
Stop system capture and resolve **after** teardown fully completes, so the next
start is immediately safe.
- `{ success: true }` on a clean stop; `{ success: false, error }` otherwise.
- Calling stop while idle must resolve safely (`{ success: false }`) — never
  throw.
- Must be able to **cancel a start that is still settling** (see method 6).

### 6. `isSystemAudioCaptureActive()` → `boolean`
Synchronous liveness flag. **Must also report `true` while a start is still
settling** — callers use this to decide whether a teardown is needed before a
fresh start, and a pending start needs one. `stopSystemAudioCapture()` must be
able to cancel such a pending start rather than reporting idle around it.

### 7. `startMicrophoneCapture(options)` → `boolean | { ok: false, reason }`
Start capture of the **local microphone** on the default capture endpoint.
- Return `true` when the device is delivering buffers.
- **On failure, return `{ ok: false, reason }`** with a short, distinct `reason`
  string identifying the failure mode — never a bare `false` that collapses
  several causes into one. Different causes need different `reason` values so the
  host app can tell them apart in diagnostics. Representative Windows failure
  modes to distinguish: device could not be opened, requested format
  unsupported, capture already active, callback not registered, or the device
  started but produced no buffers within the cold-start window. (The exact
  strings are free-form; the host app only checks `result !== true` and reports
  the `reason`.)
- Deliver audio through the **microphone** callback (separate from system audio).

### 8. `stopMicrophoneCapture()` → `boolean`
Stop the mic path **only** — must not tear down system audio. Return `true` on a
clean stop.

### 9. `isMicrophoneCaptureActive()` → `boolean`
Synchronous liveness flag for the mic path. Stays `true` throughout a
device-recovery episode (capture is still logically intended, just momentarily
not producing data).

### 10. `isMicRouteRecovering()` → `boolean`
Synchronous flag: `true` while the module is actively retrying after a failed mic
route restart (a device change mid-session). Lets host-side watchdogs stand down
while native recovery is already in flight instead of racing it. Distinct from
method 9.

### 11. `setStreamingCallback(fn | null)`
Register the JS callback for **system-audio** chunks, invoked as
`callback(buffer, format)` (see §Buffer format). `null` clears it and must be
safe to call at any time.

### 12. `setMicrophoneStreamingCallback(fn | null)`
Same, for **microphone** chunks. Separate channel so stopping one path never
silences the other.

### 13. `setLifecycleEventCallback(fn | null)`
A **diagnostics channel**: register a JS callback invoked with a single short
`snake_case` string per capture-lifecycle anomaly that happens *after* a start
has already succeeded (e.g. a watchdog firing, an orphaned stream being stopped,
an unrecoverable restart). The host app escalates events whose name ends in
`_failed`; the rest are breadcrumb-level logs.
- Passing `null` clears the callback **but must not free the underlying async
  plumbing** — those handles are torn down on stop, not on callback-clear.
  Freeing them here is a use-after-free.
- Start/settle failures are **not** delivered here — those travel in the start
  promise's rejection (method 4). This channel is only for events after a start
  has settled.

---

## Buffer format

Every callback delivers two arguments:

```js
callback(buffer, format)
```

- `buffer` — a Node.js `Buffer` of **raw interleaved PCM**, no WAV/container
  header.
- `format` — `{ sampleRate: number, channels: number, bitDepth: number, isFloat: boolean }`

**You do not have to hit a specific rate or format.** The host app resamples and
downmixes to its wire format (16 kHz mono 16-bit) using the `format` you report.
The one hard requirement: **`format` must truthfully describe `buffer`.** Report
what WASAPI actually gives you.

For reference, the macOS build reports:

| Path | sampleRate | channels | bitDepth | isFloat |
|---|---|---|---|---|
| System audio | 48000 | 2 | 32 | true |
| Microphone | 48000 | 1 | 32 | true |

---

## Threading (critical)

**Never call into JavaScript from the audio thread.** Audio arrives on an OS
real-time thread; invoking the JS callback from there will crash or corrupt the
process. Use the standard libuv hand-off:

1. The audio thread copies the PCM into a queued payload.
2. It signals the Node loop via `uv_async_send` on a pre-initialized `uv_async_t`.
3. The async handler runs **on the JS thread**, builds the `Buffer` + `format`,
   and invokes the stored callback.

Use **separate** `uv_async_t` handles for the system-audio and microphone paths.
Tear these down on stop.

---

## Device-change resilience (required)

Capture must **survive a mid-call device change** — the user switching from
Bluetooth earbuds to a wired headset, unplugging hardware, or the OS changing the
default device — **without dropping the session.** This is a core requirement,
not a stretch goal: a silent, unrecoverable capture death here is the worst
possible failure (the call looks fine while half the audio is missing).

Approach:
- Subscribe to device changes via `IMMNotificationClient`
  (`OnDefaultDeviceChanged` / `OnDeviceStateChanged`).
- **Debounce and coalesce** — a single swap fires a burst of notifications;
  collapse them into one clean restart rather than restarting per event.
- Verify the new device is stable before committing, and **retry** if the first
  attempt delivers no buffers.
- Re-initialize only the affected path without interrupting the other.

For reference, the macOS build debounces on two timescales (a fast path when the
default device genuinely changed, a slower path for spurious churn), uses a
generation token to invalidate stale in-flight restarts, does a two-sample
stability check before committing, and retries once if the restarted path
delivers no buffers within a short window. Aim to match that "user swaps devices
mid-call and capture keeps running" bar.

---

## Errors & diagnostics

Failures must be **legible across the boundary** so the host app can log and
report them. The module itself does no reporting — it makes each failure
*identifiable*. Two channels, by timing:

1. **Start/settle failures** travel in the **start-promise rejection** (methods
   1 and 4): reject with a short, specific reason in the message. One failure
   mode → one reason.
2. **Post-start anomalies** travel on **`setLifecycleEventCallback`** (method
   13): a short `snake_case` event; names ending in `_failed` are escalated by
   the host.

Rules:
- **One failure mode, one identity.** Never flatten distinct failures into a bare
  `false` (see method 7) — distinguishable causes need distinguishable reasons.
- **No silent latch-off.** If a running path dies and cannot recover, emit a
  `_failed` lifecycle event — do not merely stop delivering buffers.
- **Never free async plumbing on callback-clear** (see method 13).
- **Distinguish transient from fatal** — a single dropped buffer or a brief gap
  during a device swap is transient (recovered, not reported); an unrecoverable
  teardown is fatal (`_failed`).

Observability is a prerequisite for the resilience requirement above: "survives a
swap" is only *testable* if a failed recovery is visible.

---

## Build

The addon is built with `node-gyp`. The build config selects sources per
platform; the Windows target compiles the single Windows `.cpp` and links the
Windows audio libraries:

- Sources: `src/audio_device_manager_win.cpp` (add more files here if you split
  the implementation).
- Libraries: `ole32`, `mmdevapi`, `ksuser`.

The module compiles per-architecture and per Electron version — a rebuild is
required when either changes. A pre-scaffolded, compiling stub of this exact
surface (every method present, returning "not implemented") is provided as the
starting point.

---

## Out of scope

- Three legacy methods exported by the macOS build (`listOutputDevices`,
  `createMultiOutputDevice`, `deleteMultiOutputDevice`) are **dead** and must
  **not** be implemented.
- No disk recording — capture is streaming-only.
- No UI, no application logic, no networking — the host app owns all of it.
