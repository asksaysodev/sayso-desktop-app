# Native Audio Module — Interface Contract

This is the **frozen interface** the native audio addon (`native_audio.node`)
must implement. macOS implements it today in Objective-C++
(`src/audio_device_manager.mm`, ScreenCaptureKit + AVAudioEngine). The Windows
port implements the same NAN surface in `src/audio_device_manager_win.cpp`
(WASAPI). Both compile to `build/Release/native_audio.node`, loaded by
`native-audio/index.js` on both platforms.

Read alongside [`AUDIO_MODULE_WINDOWS_ASSESSMENT.md`](./AUDIO_MODULE_WINDOWS_ASSESSMENT.md)
(architecture + WASAPI scoping) and [`IPC_CONTRACT.md`](./IPC_CONTRACT.md)
(how `start-cue`/`stop-cue` reach this module).

> **Contract stability:** the JS wrapper `native-audio/index.js` is the
> authority for how these methods are called (arguments, `await`, return
> handling). Implement to match what `index.js` expects — do not change
> `index.js` to fit a new native shape.

---

## The NAN surface (12 methods)

`Init` (`NAN_MODULE_INIT`) must export exactly these. Names are case-sensitive.

| # | Method | Returns | Async? |
|---|---|---|---|
| 1 | `initialize` | `Promise<void>` | yes |
| 2 | `checkScreenRecordingGranted` | `boolean` | no (sync) |
| 3 | `requestScreenRecordingPermission` | `Promise<void>` | yes |
| 4 | `startSystemAudioCapture` | `Promise<boolean>` | yes |
| 5 | `stopSystemAudioCapture` | `Promise<{success, filePath?, actualStartMs?, error?}>` | yes |
| 6 | `isSystemAudioCaptureActive` | `boolean` | no (sync) |
| 7 | `startMicrophoneCapture` | `Promise<boolean>` \| `boolean` | native returns sync bool; JS awaits |
| 8 | `stopMicrophoneCapture` | `boolean` | JS awaits |
| 9 | `isMicrophoneCaptureActive` | `boolean` | no (sync) |
| 10 | `setStreamingCallback` | `void` | no (sync) |
| 11 | `setMicrophoneStreamingCallback` | `void` | no (sync) |
| 12 | `setLifecycleEventCallback` | `void` | no (sync) |

> The macOS `.mm` also still exports the dead trio `listOutputDevices`,
> `createMultiOutputDevice`, `deleteMultiOutputDevice` — **do not implement
> these on Windows.** They have zero JS callers (removed in SAYSO-327) and are
> not part of this contract; the `.mm` copies are legacy and left untouched only
> to avoid a native rebuild.

`startSystemAudioCapture` is **internal** — the public system-audio entry point
is `startProspectStreaming` in the JS wrapper, which calls it with
`{ streamingOnly: true }`. Windows must still export the native method; JS drives
it.

---

## Method contracts

### `initialize()`
Idempotent one-time setup (allocate engines, register device listeners). JS
guards with its own `isInitialized` flag and `await`s this before every capture
call. **Throw** (reject) if the audio subsystem can't be brought up — the caller
treats a throw as fatal for that Cue session.

### `checkScreenRecordingGranted()` → `boolean`
Non-prompting status read. On macOS this is `CGPreflightScreenCaptureAccess`
(never shows a dialog). **Windows:** there is no OS screen-recording gate for
loopback capture — return `true`. Must not block, prompt, or require
`initialize()`.

### `requestScreenRecordingPermission()`
Triggers the OS permission prompt / opens the relevant settings pane. macOS uses
`CGRequestScreenCaptureAccess` (required for ScreenCaptureKit). **Windows:**
no-op (resolve). If a future WASAPI path needs a mic/loopback consent, surface it
here.

### `startSystemAudioCapture(options)` → `Promise<boolean>`
Starts loopback capture of **system output** ("what the prospect says", i.e. the
far-end call audio). `options.streamingOnly` is always `true` from JS.

- Resolves `true` once capture is genuinely running and audio output is attached.
- **Reject** if capture can't start. macOS additionally rejects if a start/stop
  is already in flight ("already active", "start already in progress", "stop
  still in progress") — mirror this guard so overlapping `start-cue` calls can't
  corrupt state.
- **Self-exclusion is mandatory:** the module must NOT capture Sayso's own audio
  output (otherwise the app's own coaching/UI sounds loop back into the
  transcript). macOS sets `excludesCurrentProcessAudio = YES`. The Windows
  loopback path must exclude Sayso's own render session equivalently.
- Delivers audio via the streaming callback (see **Buffer format** + **Callback
  threading** below).

### `stopSystemAudioCapture()` → `Promise<{ success, filePath?, actualStartMs?, error? }>`
Stops system capture and resolves **after** native teardown fully completes, so
the next `startSystemAudioCapture` is safe to call immediately.

- `success: true` on clean stop; `success: false` + `error` string otherwise.
- `filePath` / `actualStartMs`: **always `null`** — legacy disk-recording fields
  from the removed file-save path (SAYSO-327 made Cue streaming-only). Kept in
  the shape only for wire-compatibility; Windows should return
  `{ success: true }` (or `{ success: false, error }`) and may omit them.
- Calling stop while idle must resolve safely (`success: false`, not throw) —
  see the macOS idle-stop branch.

### `isSystemAudioCaptureActive()` → `boolean`
Sync liveness flag for the system-audio path. **Includes a start still settling**
(SAYSO-355): callers use this probe to decide whether teardown is needed before a
fresh start, and a pending start needs teardown. `stopSystemAudioCapture()` must
be able to cancel such a pending start (reject its promise with
`sck_start_canceled_by_stop…`), never report idle around it.

### `startMicrophoneCapture(options)` → `boolean`
Starts capture of the **agent's microphone** ("what the user says"). macOS uses
`AVAudioEngine` with an input-node tap (4096-frame buffer, nil/auto format).
Returns `true` when the tap is delivering buffers, `false` if it can't start.
Mic chunks are delivered through the **microphone** callback
(`setMicrophoneStreamingCallback`), a separate channel from the system-audio
callback — the two capture paths stream independently.

### `stopMicrophoneCapture()` → `boolean`
Stops the mic tap/engine. Must NOT tear down the system-audio path (they stop
independently; the user mic often stops before the prospect stream). Returns
`true` on clean stop.

### `isMicrophoneCaptureActive()` → `boolean`
Sync liveness flag for the mic path.

### `setStreamingCallback(fn | null)`
Registers the JS callback for **system-audio** chunks. `null` clears it. Called
with `(buffer, format)` — see below. Passing `null` must be safe at any time.

### `setMicrophoneStreamingCallback(fn | null)`
Same, for **microphone** chunks. Kept separate so stopping one path doesn't
silence the other. `null` clears only the mic callback.

### `setLifecycleEventCallback(fn | null)`
Diagnostics channel (SAYSO-355). Registers a JS callback invoked with a single
short snake_case string per capture-lifecycle anomaly — e.g.
`sck_start_watchdog_fired gen=3 timeout_ms=10000`, `sck_orphan_stream_stopped
gen=3`. Events ending in `_failed` are escalated by the consumer (Sentry).
`null` clears the callback but must NOT free the underlying async plumbing —
freeing on callback-clear is the UAF class documented in SAYSO-349. JS callers
must tolerate the method being absent (older native builds): guard with a
feature check, never call unconditionally. Start/settle failures themselves are
NOT delivered here — they travel in the start promise rejection message
(`sck_start_timeout…`, `sck_start_canceled_by_stop…`); this channel is for
events that occur after a promise has already settled.

---

## Buffer format contract

Every callback invocation delivers **two arguments**:

```js
callback(buffer, format)
```

- `buffer`: a Node.js `Buffer` of **raw interleaved PCM** (no WAV/container
  header).
- `format`: `{ sampleRate: number, channels: number, bitDepth: number, isFloat: boolean }`

macOS today delivers:

| Path | sampleRate | channels | bitDepth | isFloat |
|---|---|---|---|---|
| System audio (SCK) | 48000 | 2 (stereo) | 32 | true |
| Microphone (AVAudioEngine) | 48000 (auto) | 1 (mono) | 32 | true |

**Windows is not required to match 48 kHz / float exactly.** Downstream
`streaming/audioConverter.js` resamples/downmixes to the wire format
(16 kHz mono S16) using the `format` you report — so report the format
**accurately** and the converter handles the rest. The one hard requirement:
`format` must truthfully describe `buffer`.

---

## Callback threading (critical)

**Never call into V8/JS from the audio thread.** Audio arrives on an OS
realtime/dispatch thread; invoking the JS callback there will crash or corrupt
the isolate.

macOS pattern (replicate the equivalent on Windows with libuv):

1. Audio thread copies the PCM into a queued payload.
2. Signals the Node loop via `uv_async_send` on a pre-initialized `uv_async_t`.
3. The async callback (`StreamingAsyncCallback` / `MicStreamingAsyncCallback`)
   runs **on the JS thread**, constructs the `Buffer` + `format`, and invokes the
   stored `Nan::Callback`.

Separate `uv_async_t` handles back the system-audio vs mic paths
(`g_streamingAsyncHandle`, `g_micStreamingAsyncHandle`). Tear these down on stop.

---

## Device-change resilience (the reliability bar)

Not required for the first compiling stub, but this is the bar the Windows
module must eventually meet — capture must survive the user switching
input/output devices mid-call (AirPods connect, headset unplug, default device
change) **without dropping the Cue session**.

macOS mic-route handling, as the reference behavior to match:

- A CoreAudio HAL listener on the default-input device fires on route changes.
- Restart is **debounced + coalesced**, not immediate:
  - **Fast path** (HAL default ≠ the device we opened): 80 ms debounce + 60 ms
    stability recheck.
  - **Slow path** (same id — stuck engine / spurious notify): 600 ms debounce +
    150 ms stability recheck.
- A **generation token** (`g_micRouteRestartGeneration`, bumped on each notify
  and on Stop) invalidates in-flight debounce/stability closures, so a burst of
  notifications collapses to a single restart.
- **Two-sample stability check:** read the default input, wait the stability
  delay, read again; only restart if it settled (else coalesce again).
- Mic tap start has a **retry**: wait up to 1500 ms for tap buffers; if none
  arrive, tear down and retry once before reporting failure.

Windows/WASAPI equivalent: subscribe to `IMMNotificationClient`
(`OnDefaultDeviceChanged` / `OnDeviceStateChanged`), debounce/coalesce the same
way, and re-initialize the affected capture client without interrupting the other
path. Aim to match the "user swaps devices mid-call and Cue keeps running" bar.

---

## Error & observability

Failures must be **legible across the boundary** so the JS layer can escalate
them. The native module does not call Sentry itself — it makes each failure
*identifiable*; `audio/audioManager.ts` and `src/config/sentry.ts` own the actual
reporting, severity, and transient-vs-fatal filtering.

There are **two error channels, split by timing:**

1. **Start / settle failures travel in the start-promise rejection.**
   `initialize` and `startSystemAudioCapture` reject with a specific snake_case
   reason in the message (`sck_start_timeout…`, `sck_start_canceled_by_stop…`) so
   the caller's `catch` can report it. One failure mode → one reason string.
2. **Post-settle anomalies travel on `setLifecycleEventCallback`.** Once a start
   promise has resolved, later trouble (watchdog fired, orphaned stream stopped,
   an unrecoverable restart) is emitted as a short snake_case event. Events ending
   in `_failed` are escalated to Sentry by the consumer; the rest are
   breadcrumb-level diagnostics.

The bar, learned from real macOS incidents:

- **One failure mode, one identity (SAYSO-347).** Never flatten distinct failures
  into a bare `false`. If `startMicrophoneCapture` can fail three ways (device
  open denied, format negotiation failed, no buffers within the cold-start
  window), those are three *distinguishable* reasons — surfaced via a `_failed`
  lifecycle event or a rejection message — not one opaque boolean.
- **No silent latch-off (SAYSO-353).** If a running path dies and cannot recover
  (a device change kills the mic and it never comes back), emit a `_failed`
  lifecycle event — do not merely stop delivering buffers. A path that goes quiet
  with no signal is the worst case: the session ends looking clean while half the
  audio is missing.
- **Never free async plumbing on callback-clear (SAYSO-349).** Passing `null` to
  a callback setter clears the callback only; the `uv_async_t` handles are torn
  down on Stop, not on clear. Freeing them on clear is a use-after-free.
- **Distinguish transient from fatal.** A single dropped buffer or a mid-swap gap
  is transient (the resilience layer recovers — no Sentry); an unrecoverable
  teardown is fatal (`_failed` → Sentry). Labeling them keeps the signal clean.

> Observability is a prerequisite for the device-change resilience bar above:
> "survives a mid-call swap" is only *testable* if a failed recovery emits a
> visible `_failed` event. Build the two together.

---

## Build

`binding.gyp` selects sources per platform (see the `conditions` block):

- **mac** (`OS=="mac"`): `src/audio_device_manager.mm` + CoreAudio,
  CoreFoundation, Foundation, ScreenCaptureKit, AVFoundation, CoreMedia; C++20 /
  libc++, deployment target 13.0.
- **win** (`OS=="win"`): `src/audio_device_manager_win.cpp` + `ole32`,
  `mmdevapi`, `ksuser`.

Verify: `node-gyp rebuild` (or `npm run rebuild-native-both`) still produces
`build/Release/native_audio.node` on macOS; on Windows the stub compiles + links
and every method resolves to a not-implemented error/false until the WASAPI work
lands.
