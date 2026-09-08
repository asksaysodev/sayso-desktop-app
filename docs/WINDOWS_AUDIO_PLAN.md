# Windows Native Audio Implementation Plan — Repository Archaeology & Design

Based on full reads of `NATIVE_AUDIO_CONTRACT.md`, `AUDIO_MODULE_WINDOWS_ASSESSMENT.md`,
`audio_device_manager.mm` (2474 lines, the macOS reference), the current
`audio_device_manager_win.cpp` stub (148 lines, 11 of 13 methods), `index.js`,
`binding.gyp`, and `package.json`. This is a planning document only — no
implementation files have been modified.

**Scope restriction:** this plan implements the existing Windows native C++ audio
addon only. It does not touch UI, React/frontend, Electron application logic,
backend, IPC, `native-audio/index.js`, `audioStreamer.js`, `audioBuffer.js`,
`audioConverter.js`, `recorder.ts`, `binding.gyp`, or `package.json`.

**Decisions locked in for this plan:**
- **Mic strategy: Option B — full hand-rolled WASAPI.** No new native dependency
  (no PortAudio/miniaudio). `binding.gyp`/`package.json` need no changes beyond
  what's already present (`ole32`, `mmdevapi`, `ksuser` already wired for
  `OS=='win'`). Everything (loopback + mic) lives in one hand-written
  `audio_device_manager_win.cpp`, structurally mirroring the `.mm` file.
- **Render-endpoint tracking: yes, mirror mic-side resilience.** Loopback gets
  its own generation counter, debounce/stability/backoff chain, and recovery
  state, structurally identical to the mic-side design, driven by
  `OnDefaultDeviceChanged(eRender, eConsole, ...)` instead of `eCapture`. Two
  independent recovery state machines exist side by side, each with its own
  generation token, so a render-device flap can never invalidate an in-flight
  mic recovery chain or vice versa — implemented as **two separate
  single-purpose restart-worker threads**, not one shared queue interleaving
  both paths' work.

---

## 1. The exact 13 NAN methods

Per `NATIVE_AUDIO_CONTRACT.md` lines 21–39, case-sensitive:

| # | Method |
|---|---|
| 1 | `initialize` |
| 2 | `checkScreenRecordingGranted` |
| 3 | `requestScreenRecordingPermission` |
| 4 | `startSystemAudioCapture` |
| 5 | `stopSystemAudioCapture` |
| 6 | `isSystemAudioCaptureActive` |
| 7 | `startMicrophoneCapture` |
| 8 | `stopMicrophoneCapture` |
| 9 | `isMicrophoneCaptureActive` |
| 10 | `isMicRouteRecovering` |
| 11 | `setStreamingCallback` |
| 12 | `setMicrophoneStreamingCallback` |
| 13 | `setLifecycleEventCallback` |

**Gap found:** the current `audio_device_manager_win.cpp` only registers **11**
of these in its `NAN_MODULE_INIT(Init)` (lines 123–146) — `isMicRouteRecovering`
and `setLifecycleEventCallback` are **not exported at all** yet, not even as
stubs. `index.js` already guards both with
`typeof nativeAudio.X === 'function'` checks (index.js:220, index.js:155), so
today they silently no-op on Windows. The mac `.mm` exports all 13 plus the 3
dead legacy methods (`listOutputDevices`, `createMultiOutputDevice`,
`deleteMultiOutputDevice`), which the contract explicitly says **not** to port.

---

## 2. Per-method contract table

| Method | JS caller (`index.js`) | Args | Returns | Sync/Async | Error behavior |
|---|---|---|---|---|---|
| `initialize` | `AudioDeviceManager.initialize()`, guarded by `this.isInitialized` | none | `Promise<void>` | async | Reject if audio subsystem can't come up (COM init fail, enumerator fail, listener registration fail). JS treats throw as fatal for the Cue session. |
| `checkScreenRecordingGranted` | `checkScreenRecordingGranted()`, no `initialize()` required | none | `boolean` | sync | Must never throw, never prompt, never block. Windows: always `true`. |
| `requestScreenRecordingPermission` | `requestScreenRecordingPermission()`, after `initialize()` | none | `Promise<void>` | async | Windows: no-op resolve. |
| `startSystemAudioCapture` | `AudioDeviceManager.startSystemAudioCapture(options)` → really driven via `startProspectStreaming()` | `{ streamingOnly: true }` (only field JS sends) | `Promise<boolean>` | async | Reject if: already active, start already in-flight, stop still in-flight, or WASAPI activation/start fails. Must mirror mac's three specific overlap-guard messages so `stopSystemAudioCapture` can cancel a pending start. |
| `stopSystemAudioCapture` | `stopSystemAudioCapture()` | none | `Promise<{success, filePath?, actualStartMs?, error?}>` | async | Idle-stop must resolve `{success:false}`, never throw. Must first cancel any pending start before tearing down. Resolves only after full native teardown. `filePath`/`actualStartMs` may be omitted/null. |
| `isSystemAudioCaptureActive` | `isSystemAudioCaptureActive()` | none | `boolean` | sync | Must return `true` while a start is still settling, not only once fully running. |
| `startMicrophoneCapture` | `startMicrophoneCapture(options)` | `{}` (streamingCallback stripped in JS) | `boolean \| {ok:false, reason}` | native call synchronous, JS awaits | Success `true`; failure returns one of 6 reasons: `callback_empty`, `already_active`, `no_input_node`, `tap_install_failed`, `engine_start_failed`, `no_tap_buffers`. Never collapse these. |
| `stopMicrophoneCapture` | `stopMicrophoneCapture()` | none | `boolean` | native call synchronous, JS awaits | Must NOT touch system-audio state. Always `true` on clean stop. |
| `isMicrophoneCaptureActive` | `isMicrophoneCaptureActive()` | none | `boolean` | sync | Stays `true` throughout a recovery episode. |
| `isMicRouteRecovering` | `isMicRouteRecovering()`, guarded by `typeof === 'function'` | none | `boolean` | sync | `true` only while the backoff recovery loop is actively retrying. Distinct from #9. |
| `setStreamingCallback` | `setStreamingCallback(callback)` | `Function \| null` | `void` | sync | `null` safe at any time; clears callback only, never touches `uv_async_t` handles (UAF class — SAYSO-349). |
| `setMicrophoneStreamingCallback` | called by `startMicrophoneCapture`/`stopMicrophoneCapture` in index.js, guarded by feature check | `Function \| null` | `void` | sync | Same UAF constraint; independent of `setStreamingCallback`. |
| `setLifecycleEventCallback` | `setLifecycleEventCallback(callback)`, guarded by feature check | `Function \| null` | `void` | sync | `null` clears callback but the `uv_async_t` must be allocated once and **never freed** — the exact bug class SAYSO-349 documents. |

---

## 3. macOS → Windows/WASAPI responsibility map

| macOS responsibility | Windows/WASAPI equivalent |
|---|---|
| `SCStream` + `SCStreamConfiguration` + `SCContentFilter` | `IMMDevice` (default render endpoint) → `IAudioClient::Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_LOOPBACK, ...)` → `IAudioCaptureClient` |
| `SCShareableContent.getShareableContentWithCompletionHandler` (async, multi-stage start) | Not needed as-is — WASAPI activation is more direct, but the generation-guarded pending-struct *pattern* is still used since `IAudioClient::Initialize`/`Start()` can also block/fail on a worker thread |
| `AudioCaptureDelegate` (`SCStreamOutput`/`SCStreamDelegate`) callback | The WASAPI capture thread's poll loop (`IAudioCaptureClient::GetBuffer`/`ReleaseBuffer`) on a dedicated worker thread (event-driven via `SetEventHandle` + `WaitForSingleObject`, or timer-driven poll) |
| `AVAudioEngine` + `AVAudioInputNode.installTapOnBus` (mic capture) | `IMMDevice` (default capture endpoint) → `IAudioClient::Initialize(AUDCLNT_SHAREMODE_SHARED, 0, ...)` → `IAudioCaptureClient`, same poll/event pattern on its own worker thread |
| CoreAudio HAL property listener on `kAudioHardwarePropertyDefaultInputDevice` | `IMMNotificationClient::OnDefaultDeviceChanged(eCapture, eConsole, ...)` |
| `AVAudioEngineConfigurationChangeNotification` (secondary route-change signal) | `IMMNotificationClient::OnDeviceStateChanged` / `OnPropertyValueChanged` on the currently-opened endpoint — synthesized from device-state + property notifications since Windows has no single "engine config changed" event |
| `kAudioDevicePropertyTransportType` (Bluetooth detection for widened timing) | `IPropertyStore::GetValue(PKEY_AudioEndpoint_FormFactor)` / endpoint FormFactor heuristics — no single canonical "is this Bluetooth" property as clean as CoreAudio's |
| `AudioObjectGetPropertyData(kAudioHardwarePropertyDefaultInputDevice)` | `IMMDeviceEnumerator::GetDefaultAudioEndpoint(eCapture, eConsole)` |
| Nothing (SCK captures "the display's audio" implicitly) | Explicit default **render** endpoint selection via `GetDefaultAudioEndpoint(eRender, eConsole)` — tracked with its own resilience path per the locked-in decision above |
| `dispatch_queue_t` serial queues (`com.sayso.audio`, `com.sayso.micRouteRestart`) | Dedicated worker threads per capture path, pinned to `MMCSS` (`AvSetMmThreadCharacteristics("Audio")`) for realtime priority; two separate serial restart-worker threads (mic, loopback) |
| `dispatch_after` debounce/stability timers | Timer queue timers (`CreateTimerQueueTimer`) or a lightweight scheduled-callback abstraction on each restart worker thread |
| `g_micEngineObserverAttachedAtMs` cooldown against self-triggered notifications | Same pattern: timestamp the moment we (re)opened the endpoint; ignore notifications inside a cooldown window |
| ARC-off explicit `release`/`retain` discipline (SAYSO-361 UAF class) | COM `Release()`/`AddRef()` discipline — same class of bug, same fix |
| `uv_async_send` / `uv_async_t` threading pattern | **Identical** — libuv is portable, ports unchanged |
| No self-exclusion needed | No self-exclusion needed — plain render-endpoint loopback, no `PROCESS_LOOPBACK`/`ActivateAudioInterfaceAsync` |

---

## 4. Persistent native state Windows will need

**COM / enumeration:**
- `IMMDeviceEnumerator*` — held per-thread (see §10/§11), no cross-thread sharing
- `IMMNotificationClient*` (custom COM class) — one, registered once, unregistered only at final module teardown
- COM apartment state per thread that touches WASAPI

**System-audio (loopback) path:**
- `IMMDevice*` for the currently-opened default render endpoint
- `IAudioClient*`, `IAudioCaptureClient*` (loopback mode)
- negotiated `WAVEFORMATEX`/`WAVEFORMATEXTENSIBLE`
- capturing/idle/starting/stopping state flag (atomic)
- pending-start struct (generation, stage, settled flag, mutex) — analog of `PendingSckStart`
- pending-stop struct (resolvers list, mutex) — analog of `PendingSckStop`
- monotonic start-generation counter
- worker thread handle + stop-event handle for the capture loop
- `g_loopbackOpenedEndpointId`, `g_loopbackRouteRestartGeneration`, `g_loopbackRouteRecovering`, `g_loopbackRouteRecoveryStartMs` (new, per the render-tracking decision)
- `uv_async_t* g_streamingAsyncHandle`

**Microphone path:**
- `IMMDevice*` for the currently-opened default capture endpoint
- `IAudioClient*`, `IAudioCaptureClient*`
- negotiated format
- `g_isMicCapturing` (atomic bool)
- `g_micOpenedEndpointId` (Windows: endpoint ID string via `IMMDevice::GetId()`, stored as `std::wstring`)
- `g_micRouteRestartGeneration` (atomic uint64)
- `g_micFirstTapSeen` (atomic bool)
- `g_micRouteRecovering` (atomic bool) + `g_micRouteRecoveryStartMs`
- worker thread handle + stop-event handle for the mic capture loop
- mic restart-worker thread + its own timer/queue state
- `uv_async_t* g_micStreamingAsyncHandle`

**Lifecycle diagnostics channel:**
- `g_lifecycleMutex`, `g_lifecycleQueue` (vector<string>), `g_lifecycleAsyncHandle` (allocated once, never freed)

**Callbacks:**
- `Nan::Persistent<v8::Function>` × 3 (streaming, mic-streaming, lifecycle)

---

## 5. System-audio capture lifecycle design

1. **Idle → Start requested.** `startSystemAudioCapture()` checks: (a) not already capturing, (b) no pending start, (c) no pending stop — reject with the three mac-equivalent messages if any hold.
2. Allocate a `PendingLoopbackStart` (generation, `uv_async_t`, resolver, stage enum, settled flag), publish it under a mutex, return its promise immediately.
3. Arm a 10s watchdog timer that rejects the generation with a `wasapi_start_timeout` reason if nothing settles it.
4. On a dedicated **loopback worker thread**: `CoInitializeEx(COINIT_MULTITHREADED)` → `GetDefaultAudioEndpoint(eRender, eConsole)` → `Activate(IID_IAudioClient)` → `IsFormatSupported` negotiation → `Initialize(..., AUDCLNT_STREAMFLAGS_LOOPBACK, ...)` → `GetService(IID_IAudioCaptureClient)` → `Start()`.
5. Each stage advances a stage marker so a watchdog firing mid-flight can report *where* it hung.
6. On success, publish the pipeline objects to the globals only after winning settlement — an abandoned/late-arriving start must tear down its own orphaned `IAudioClient` rather than touch globals that may belong to a newer start.
7. The worker thread enters its capture loop: `GetBuffer`/`ReleaseBuffer`, copying PCM into a `StreamingData` payload and `uv_async_send`-ing it — never touching V8.
8. **Stop requested.** Cancels any pending start first (generation-based cancel-and-reject), then if idle resolves `{success:false}` immediately, else signals the worker thread's stop event, waits for full exit + `IAudioClient::Stop()`, releases all COM interfaces, closes/nulls the `uv_async_t`, resolves the stop promise. Concurrent stop calls coalesce onto the same pending-stop struct's resolver list.

---

## 6. Microphone capture lifecycle design

1. `startMicrophoneCapture()` requires `g_micStreamingCallback` already set (else `callback_empty`), guards `g_isMicCapturing` (else `already_active`).
2. Runs the actual open synchronously on the **mic restart worker thread**, matching mac's `dispatch_sync` so mic-engine touches only ever happen from one thread context and the JS-facing call stays synchronous.
3. `TryStartCaptureOnce(waitForFirstTapMs, &failReason)`: `GetDefaultAudioEndpoint(eCapture, eConsole)` → `Activate` → `IsFormatSupported` → `Initialize` → `GetService(IAudioCaptureClient)` → `Start()`; failures map to `no_input_node`, `tap_install_failed`, `engine_start_failed` (HRESULT logged), or `no_tap_buffers` (first-buffer wait timeout).
4. Retry once with a longer wait, identical to mac's two-attempt pattern.
5. On success, record the opened endpoint's ID string and start the capture worker thread's poll loop, delivering via `g_micStreamingAsyncHandle`.
6. `stopMicrophoneCapture()` bumps the restart generation first, then on the restart-worker thread tears down the capture client/thread only, resets `g_isMicCapturing`, opened-endpoint-id, and `g_micRouteRecovering` — must not touch system-audio state.

---

## 7. Separate system/microphone PCM queues

No true queue data structure in the mac code — each is a single-slot mailbox
per `uv_async_t` (`StreamingData*` assigned to `handle->data`). Windows
replicates that shape, twice, independently:

- `g_streamingAsyncHandle` (system audio) with its own `StreamingData*` mailbox
- `g_micStreamingAsyncHandle` (microphone) with its own `StreamingData*` mailbox

Each capture worker thread allocates a `new StreamingData` per buffer, assigns
it, calls `uv_async_send`. The JS-thread callback reads `handle->data`, builds
the `Buffer`+`format`, invokes the JS callback, `delete`s the payload. **Do not
share one queue between the two paths** — stopping one must not silence the
other.

**Flag (not to silently fix):** a single-slot mailbox can drop a buffer if a
second `uv_async_send` fires before the JS thread drains the first (libuv
coalesces multiple sends into one wakeup; with a single-pointer mailbox the
earlier payload is overwritten and leaked). This is a latent behavior already
present on mac — carry it forward as-is rather than silently diverging.

---

## 8. Separate `uv_async_t` handles

Three, matching mac:
- `g_streamingAsyncHandle` → `StreamingAsyncCallback` (system audio)
- `g_micStreamingAsyncHandle` → `MicStreamingAsyncCallback` (microphone)
- `g_lifecycleAsyncHandle` → `LifecycleAsyncCallback` (diagnostics, allocated once, **never freed**)

Plus per-operation ones for promise settlement: one embedded in each
`PendingLoopbackStart`, one in each `PendingLoopbackStop`.

All are only touched (created, `uv_async_send`'d, `uv_close`'d) following the
same rule mac follows: creation and `uv_close` happen on/from the JS thread;
`uv_async_send` is the only cross-thread-safe operation; the two long-lived
streaming/lifecycle handles are **closed only on Stop**, never on
callback-clear.

---

## 9. Thread responsibilities

| Thread | Operations |
|---|---|
| **Node/JS thread** | All NAN method bodies' synchronous parts; promise creation/resolution; `uv_async_t` creation/`uv_close`; building `Buffer`/`format` and invoking `Nan::Callback`s; reading/writing `Nan::Persistent` slots |
| **WASAPI loopback capture thread** (per session) | `CoInitializeEx`/`CoUninitialize`; `IAudioClient`/`IAudioCaptureClient` lifecycle; `GetBuffer`/`ReleaseBuffer` poll loop; `StreamingData` copy; `uv_async_send`; MMCSS registration — never touches V8 |
| **WASAPI mic capture thread** (per session) | Same shape, independent lifecycle, own MMCSS registration, own `uv_async_send` |
| **Mic-route-restart worker thread** (single, long-lived, serial) | All mic engine open/close/restart logic; debounce/stability/backoff timer chains; the only thread allowed to touch mic `IAudioClient`/`IAudioCaptureClient` pointers |
| **Loopback-route-restart worker thread** (single, long-lived, serial — new, per the render-tracking decision) | Same role as the mic restart worker, but for the render/loopback path — independent generation token, independent timer chains, so a render-device flap never blocks or interleaves with mic recovery |
| **Device notification thread** (COM-owned, not created by Sayso) | Minimum possible inline work: read the new default device id/role, then hand off to the relevant restart-worker thread. Must not block, must not call back into the enumerator synchronously, must not touch `IAudioClient` pointers directly |

---

## 10. COM initialization and teardown/lifetime

- Each thread that touches WASAPI/MMDevice COM interfaces calls
  `CoInitializeEx(nullptr, COINIT_MULTITHREADED)` once at thread start and
  `CoUninitialize()` once at thread exit: loopback capture thread, mic capture
  thread, mic restart-worker thread, loopback restart-worker thread.
- **Do not call `CoInitializeEx` on the Node/JS thread at all** — Electron/
  Chromium already initializes COM on the main thread in
  `COINIT_APARTMENTTHREADED` mode for its own needs; mixing apartment models on
  the same thread is the classic Windows COM footgun. `initialize()`'s
  enumerator-creation work is handed to a worker thread and awaited via
  `uv_async_t`, matching the async-settle pattern already used for start/stop.
- `IMMDeviceEnumerator` instances are created once per long-lived worker thread
  (mic restart worker, loopback restart worker) and held for that thread's
  lifetime. Per-session capture threads (loopback, mic) create and release
  their own short-lived enumerator instance scoped to their own COM apartment.

---

## 11. `IMMDeviceEnumerator` ownership

- **No cross-thread sharing.** Each thread that needs one (mic restart worker,
  loopback restart worker, loopback capture thread, mic capture thread) creates
  its own via `CoCreateInstance(__uuidof(MMDeviceEnumerator), ...)` after its
  own `CoInitializeEx`, and releases it at thread exit. This sidesteps COM
  apartment/marshaling complexity at the cost of one extra cheap
  `CoCreateInstance` per thread.
- The `IMMNotificationClient` registration is process-wide and singular (§12) —
  registered once against one `IMMDeviceEnumerator` instance during
  `initialize()`, unregistered only at that instance's teardown.

---

## 12. `IMMNotificationClient` ownership and callbacks

- One custom COM class (`SaysoNotificationClient : public IMMNotificationClient`)
  with manual `QueryInterface`/`AddRef`/`Release`.
- Registered exactly once during `initialize()` via
  `RegisterEndpointNotificationCallback`, unregistered only if/when that
  enumerator instance is torn down (in practice: never, during normal app
  lifetime — matches mac never unregistering its HAL listener).
- Implements all five methods; two matter functionally:
  - `OnDefaultDeviceChanged(EDataFlow flow, ERole role, LPCWSTR pwstrDeviceId)`
    — filter to `flow == eCapture && role == eConsole` to trigger the mic
    restart-worker; filter to `flow == eRender && role == eConsole` to trigger
    the loopback restart-worker (per the locked-in render-tracking decision).
  - `OnDeviceStateChanged` — secondary signal mirroring
    `AVAudioEngineConfigurationChangeNotification`, for the specific endpoint
    currently open on either path, in case a device drops to
    `DEVICE_STATE_NOTPRESENT`/`DEVICE_STATE_UNPLUGGED` without a default-device
    change.
- Callback bodies are minimal and non-blocking: read the changed device id,
  post/queue to the relevant restart-worker thread. **Never call back into
  `IAudioClient` methods directly from inside the notification callback.**

---

## 13. Default render endpoint tracking

- On loopback start: `GetDefaultAudioEndpoint(eRender, eConsole)`, store the
  endpoint id string (`g_loopbackOpenedEndpointId`).
- Per the locked-in decision, this is tracked with the **same resilience
  design as the mic path** (§15–16, mirrored): a default-render change while
  capturing triggers the same debounce/stability/backoff chain, running on its
  own dedicated loopback restart-worker thread with its own generation token,
  so a user swapping output devices mid-call (e.g. plugging in headphones)
  gets loopback re-bound to the new default render endpoint rather than
  silently continuing to capture a now-idle endpoint.

---

## 14. Default capture endpoint tracking

- Direct analog of mac's `g_micOpenedInputDeviceId`/`g_lastKnownDefaultInputDevice`:
  store the currently-open capture endpoint's id string, updated only by the
  mic restart-worker thread, compared against a fresh
  `GetDefaultAudioEndpoint(eCapture, eConsole)` read whenever
  `OnDefaultDeviceChanged` fires for `eCapture`.
- Same "changed vs. same-id-but-stuck" fork as mac (§15).

---

## 15. Device-change debounce/coalescing/recovery design

Faithful port of mac's three-tier mechanism, applied **independently to both
the mic and loopback paths** (each with its own generation token and worker
thread, per the locked-in decision):

1. **Generation token** (atomic uint64, one per path) bumped on every
   notification and on Stop — invalidates all in-flight debounce/stability/
   backoff steps whose captured generation no longer matches.
2. **Fast/slow/leaving-transport path selection**: on notification, read
   current default endpoint for that path; if it differs from the opened one,
   classify as fast-path (80ms debounce/60ms stability) unless the previously
   opened endpoint looks like a wireless transport (widen to ~450ms), else
   same-id slow-path (600ms/150ms).
3. **Two-sample stability check**: after the debounce delay, read the default
   endpoint; wait the stability delay; read again; only proceed if it settled,
   else recurse into a fresh debounced-restart call.
4. **Immediate restart**: teardown + `TryStartCaptureOnce` with a widened
   first-tap-wait if leaving a wireless-classified transport, one retry with a
   1500ms wait.
5. **Backoff recovery** on double-failure: fixed schedule
   `{250, 500, 1000, 2000, 4000}` ms, holding at 4000ms after exhaustion, up to
   a 30s ceiling measured from the first failure of the episode — the
   recovering flag is set only on entering this state, cleared on success or
   explicit Stop/Start, and a `_recovery_failed`/`_succeeded`/`_started`
   lifecycle event is emitted at each transition.
6. **Windows-specific transport detection substitute**: since there's no
   single canonical Windows property like `kAudioDevicePropertyTransportType`,
   use `IPropertyStore::GetValue(PKEY_AudioEndpoint_FormFactor)` (values like
   `Headset`, `Headphones`), with query-failure treated as "assume
   wireless" (the safer, wider-timing default).

---

## 16. Microphone recovery state and generation handling

- `g_isMicCapturing` (atomic bool) — capture intended/logical-active flag,
  spans the entire recovery episode.
- `g_micRouteRecovering` (atomic bool) — true only during active backoff retry.
- `g_micRouteRecoveryStartMs` (plain int64, touched only on the mic
  restart-worker thread) — anchors the 30s ceiling.
- `g_micRouteRestartGeneration` (atomic uint64) — the coalescing token.
- Authoritative resetters: an explicit `startMicrophoneCapture()` call always
  clears `g_micRouteRecovering` on entry; an explicit `stopMicrophoneCapture()`
  always clears it too and bumps the generation first.
- `g_micOpenedEndpointId` is **deliberately not reset** to "unknown" on a
  failed restart attempt — it must stay stale so the next notification's
  classification still compares against the last known-good target.
- The loopback path gets an exact structural mirror of all of the above
  (`g_loopbackRouteRecovering`, `g_loopbackRouteRecoveryStartMs`,
  `g_loopbackRouteRestartGeneration`, `g_loopbackOpenedEndpointId`) with its
  own authoritative resetters tied to `startSystemAudioCapture`/
  `stopSystemAudioCapture`.

---

## 17. WASAPI format negotiation design

- Request the endpoint's mix format via `IAudioClient::GetMixFormat()` first —
  the format the shared-mode engine actually wants, mirroring mac's "pass nil,
  let the HAL decide" philosophy and the contract's explicit "Windows is not
  required to match 48kHz/float exactly — report the format accurately."
- Call `IsFormatSupported(AUDCLNT_SHAREMODE_SHARED, mixFormat, &closestMatch)`
  to confirm.
- No fallback chain to a fixed format is required — whatever comes back from
  `GetMixFormat` is reported truthfully via `{sampleRate, channels, bitDepth,
  isFloat}`, and `audioConverter.js` handles the rest. Exclusive-mode fallback
  logic (what the assessment doc worried about) is not needed since nothing
  here requires exclusive mode.
- `isFloat` derived from `WAVE_FORMAT_EXTENSIBLE` + `KSDATAFORMAT_SUBTYPE_IEEE_FLOAT`
  or plain `WAVE_FORMAT_IEEE_FLOAT`; `bitDepth` from `wBitsPerSample`;
  `channels` from `nChannels`; `sampleRate` from `nSamplesPerSec`.

---

## 18. Raw PCM → Node Buffer without touching V8 from the audio thread

Identical pattern to mac, ported directly:

1. Capture thread calls `IAudioCaptureClient::GetBuffer(&pData, &numFrames, &flags, ...)`.
2. Copies `numFrames * blockAlign` bytes into a heap-allocated `StreamingData`
   (a `std::vector<char>` plus format fields) — required because `pData` is
   only valid until `ReleaseBuffer`, which must happen promptly (audio-thread
   realtime constraints — no blocking, no waiting on the JS thread).
3. Calls `ReleaseBuffer(numFrames)`.
4. Assigns the `StreamingData*` to the relevant `uv_async_t`'s `.data` field
   and calls `uv_async_send()` — the only cross-thread signal.
5. On the JS thread, the registered callback reads `handle->data`, builds
   `Nan::CopyBuffer` + the format object, invokes the stored `Nan::Callback`,
   then `delete`s the payload.

This piece is already platform-agnostic (per the assessment doc) and needs no
Windows-specific redesign.

---

## 19. Start/stop synchronization against overlapping calls

- **System audio:** a `PendingLoopbackStart` struct carries a generation
  number and a mutex-guarded `settled` bool; `SettleForGeneration(gen, reject,
  message)` is the only transition, idempotent across the watchdog timer, a
  genuine WASAPI failure, and a concurrent stop-cancel racing to call it. A
  `stopSystemAudioCapture()` call arriving while a start is pending must settle
  (reject) that pending start's generation *before* proceeding to teardown, so
  a wedged/slow WASAPI activation can never outlive a stop.
- A second start call while one is pending rejects immediately with a "start
  already in progress" message. A second stop call while one is in progress
  coalesces onto the same pending-stop struct's resolver list.
- **Microphone:** `startMicrophoneCapture`/`stopMicrophoneCapture` route
  through the mic restart-worker thread (post a work item, block the calling
  JS thread on a Windows event/condition-variable until it completes),
  guaranteeing only one mic-engine mutation is ever in flight with natural
  FIFO ordering.
- **Loopback worker thread lifetime** (new relative to mac — SCK manages its
  own internal threading, WASAPI does not): `Stop()` signals the thread's
  stop-event and waits (bounded timeout) for the thread to fully exit before
  releasing the `IAudioClient`/`IAudioCaptureClient` COM pointers those threads
  reference; a subsequent `Start()` cannot begin creating a new thread/client
  pair until the old thread has fully joined. This is the single riskiest new
  piece of the Windows port.

---

## 20. Potential race conditions and lifetime/UAF issues

1. **Loopback/mic worker thread join ordering** (new to Windows): releasing
   COM pointers before the capture thread has exited its poll loop → crash.
   Must join-before-release, always.
2. **`uv_async_t` single-slot mailbox overwrite** (§7): a dropped/leaked buffer
   if two sends race ahead of one JS-thread drain — present on mac today too.
3. **`IMMNotificationClient` callback reentrancy**: calling back into
   `IMMDeviceEnumerator`/`IAudioClient` synchronously from inside a
   notification callback risks deadlock — must always hand off to the
   relevant restart-worker thread.
4. **Missed `Release()` on error-return paths**: `IsFormatSupported`,
   `Initialize`, `GetService` can each fail mid-sequence; every failure branch
   needs an explicit cleanup path mirroring `MicEngineTeardownOnly`.
5. **Cooldown-window self-triggered notification loop**: Windows device-state
   notifications can fire as a side effect of Sayso's own `Initialize`/`Start`
   calls on some drivers — needs the same attach-timestamp cooldown guard mac
   uses, on both the mic and loopback restart paths.
6. **Generation-token races between debounce and backoff-recovery chains**:
   the "don't reset the opened-endpoint-id on failure" subtlety (§16) must be
   preserved exactly on both paths, not "cleaned up."
7. **`settled`-flag races on pending-start structs**: three concurrent writers
   (watchdog, WASAPI completion/failure, stop-cancel) — must be a single
   mutex-guarded check-and-set.
8. **Callback-clear vs. async-handle lifetime** (SAYSO-349's exact class):
   `setStreamingCallback(null)`/`setMicrophoneStreamingCallback(null)`/
   `setLifecycleEventCallback(null)` must only `.Reset()` the
   `Nan::Persistent`, never `uv_close` the associated handle.
9. **COM apartment mismatch**: any WASAPI-touching thread left without
   `CoInitializeEx`, or initialized in the wrong apartment model, can fail
   intermittently with `CO_E_NOTINITIALIZED`/`RPC_E_WRONG_THREAD` — every new
   thread must be audited for this at creation.
10. **Two independent restart-worker threads (mic + loopback) must not share
    state**: since the render-tracking decision adds a second recovery state
    machine, verify no global is accidentally shared between the mic and
    loopback recovery paths — each needs its own generation token, its own
    recovering flag, its own opened-endpoint-id.

---

## 21. Resources to release at each teardown point

**System-audio stop (`stopSystemAudioCapture` teardown):**
- Signal + join the loopback capture worker thread
- `IAudioCaptureClient::Release()`
- `IAudioClient::Stop()` then `Release()`
- `IMMDevice::Release()` (the opened render endpoint)
- Any per-thread `IMMDeviceEnumerator` created for that thread → `Release()`
- `CoUninitialize()` on the capture thread before it exits
- `g_streamingCallback.Reset()`
- Close (`uv_close`) and free `g_streamingAsyncHandle`, then null the pointer
- Clear the pending-start/pending-stop struct pointers once settled
- Reset the capturing/active flag last
- Also tear down the loopback restart-worker's device-tracking state
  (`g_loopbackOpenedEndpointId`, recovery flags) — new, per the render-tracking
  decision

**Mic stop (`stopMicrophoneCapture` teardown):**
- Bump `g_micRouteRestartGeneration` first
- On the mic restart-worker thread: signal + join the mic capture worker
  thread, `IAudioCaptureClient::Release()`, `IAudioClient::Stop()`+`Release()`,
  `IMMDevice::Release()`
- Unregister/detach any per-engine secondary device-state observer analog
- Reset `g_isMicCapturing`, `g_micOpenedEndpointId` (to "unknown" — an explicit
  Stop *is* authoritative), `g_micRouteRecovering`
- **Must NOT** touch `g_streamingCallback`/`g_streamingAsyncHandle` or the
  lifecycle channel
- Mic callback itself is cleared by `index.js` calling
  `setMicrophoneStreamingCallback(null)` separately

**Final/process-level cleanup (no explicit NAN method exists for this):**
- `IMMNotificationClient` unregistration
- Any long-lived `IMMDeviceEnumerator` `Release()`
- `g_lifecycleAsyncHandle` — deliberately **never** closed during normal
  operation; only reclaimed by process exit
- Join both restart-worker threads (mic, loopback) if they are long-lived
  dedicated threads

---

## 22. How this satisfies `NATIVE_AUDIO_CONTRACT.md`

- **Frozen 13-method surface**: §1–2 enumerate exactly what must be added to
  the current 11-method stub (`isMicRouteRecovering`, `setLifecycleEventCallback`).
- **`index.js` as authority, never modified**: every arg/return shape in §2 is
  treated as fixed input.
- **No self-exclusion needed**: §5 uses plain render-endpoint loopback with no
  `PROCESS_LOOPBACK` activation path.
- **Buffer format contract**: §17–18 report the WASAPI mix format truthfully
  rather than forcing 48kHz/float.
- **Callback threading**: §18 is a direct, unmodified port of the mac
  copy-then-`uv_async_send` pattern.
- **Two separate `uv_async_t` handles + no free-on-clear**: §8 and §21's "must
  NOT" bullets directly encode SAYSO-349.
- **Device-change resilience bar**: §15–16 is a faithful port of the
  documented debounce/coalesce/backoff/generation design, extended
  symmetrically to the loopback/render side per the locked-in decision.
- **Six-way mic-start failure taxonomy (SAYSO-347)**: §2/§6 preserve all six
  distinguishable reasons rather than collapsing any of them.
- **No silent latch-off (SAYSO-353)**: §16's recovery state machine keeps the
  capturing flag true throughout an episode and emits a `_recovery_failed`
  event rather than silently going quiet, on both paths.
- **Two error channels split by timing**: §2 and §19 keep start/settle
  failures in promise rejections and post-settle anomalies on the lifecycle
  channel exclusively.
- **Build**: `binding.gyp`'s existing `OS=='win'` conditional
  (`ole32`/`mmdevapi`/`ksuser`) is sufficient — no build-system change is
  needed, consistent with the Option B (no new dependency) decision.

---

## A. Proposed internal structs/classes/helpers

```
SaysoNotificationClient          — IMMNotificationClient impl (QueryInterface/AddRef/Release + 5 callbacks)
PendingLoopbackStart              — generation, uv_async_t, resolver, stage enum, settled flag, mutex-guarded
PendingLoopbackStop                — resolvers list, saved-state snapshot, mutex-guarded
StreamingData                      — unchanged from mac: audioData vector + format fields
LoopbackCaptureThreadContext        — IAudioClient*/IAudioCaptureClient*/IMMDevice*, stop-event handle, thread handle
MicCaptureThreadContext             — same shape, mic-specific
MicRouteRestartWorker                — dedicated thread + work-item/timer-queue wrapper, mic path
LoopbackRouteRestartWorker           — dedicated thread + work-item/timer-queue wrapper, loopback path (new)
LoopbackStartStage / MicStartStage (enum)  — stage-tracking analog of SckStartStage, per path
Helper: GetDefaultEndpointId(EDataFlow)      — wraps GetDefaultAudioEndpoint + GetId
Helper: IsLikelyWirelessEndpoint(IMMDevice*) — FormFactor-based substitute for transport-type check
Helper: EmitLifecycleEvent(message)          — unchanged shape from mac
Helper: NowMs()                              — GetTickCount64 or QueryPerformanceCounter-based wall clock
```

## B. Proposed state variables

Enumerated fully in §4 — COM enumerator/notification-client singletons;
per-path `IMMDevice*`/`IAudioClient*`/`IAudioCaptureClient*`; per-path
capturing/active atomics; per-path opened-endpoint-id strings; generation
counters (loopback start, mic restart, loopback restart); recovery-in-progress
atomics + start-timestamps (mic and loopback, independent); three
`uv_async_t*` (streaming, mic-streaming, lifecycle) plus per-operation ones for
start/stop settlement; three `Nan::Persistent<Function>` callback slots;
lifecycle mutex + queue.

## C. Proposed thread model

Six thread roles, detailed in §9: JS/libuv thread (V8 + NAN only), loopback
capture thread (per session), mic capture thread (per session), mic-route-
restart worker thread (single, long-lived), loopback-route-restart worker
thread (single, long-lived — new, per the render-tracking decision), and the
COM-internal device-notification thread (not owned by Sayso — callbacks must
hand off immediately).

## D. Proposed queue model

Two independent single-slot `uv_async_t` mailboxes (system-audio, mic)
carrying heap-allocated `StreamingData*`, plus one mutex-guarded
`std::vector<std::string>` queue for lifecycle events drained in batch — all
three ported unchanged in shape from mac (§7–8).

## E. Proposed device-recovery state machine

Applies independently to both the mic (capture) and loopback (render) paths,
each with its own generation token and worker thread:

```
IDLE (not capturing)
  → Start requested → OPENING → RUNNING
RUNNING
  → OnDefaultDeviceChanged(matching flow) → DEBOUNCING (fast/slow/leaving-wireless path selection)
DEBOUNCING → STABILITY_CHECK (two-sample read) → [unstable: back to DEBOUNCING] / [stable: RESTARTING]
RESTARTING → [immediate attempt ok: RUNNING] / [both immediate attempts fail: RECOVERING]
RECOVERING (backoff loop, 250ms→4000ms steps, 30s ceiling from first failure)
  → [attempt succeeds: RUNNING] / [ceiling reached: RECOVERY_FAILED — logically-active-but-silent, emits _failed event, waits for next notification]
Any state → Stop() → IDLE (bumps generation, invalidates all in-flight timers unconditionally)
Any state → Start() while already RUNNING → reject/no-op per state (authoritative reset of RECOVERING)
```

## F. Files believed to need modification (implementation phase, not now)

- `sayso-desktop-app/electron/native-audio/src/audio_device_manager_win.cpp` —
  the entire implementation (currently a 148-line stub; will grow
  substantially, likely 1500–2000+ lines given the mac reference is 2474 lines
  minus the ~600 lines of file-writing/CAF/WAV code that has no Windows
  streaming-only equivalent)

No other file requires modification to satisfy this contract:
`binding.gyp`'s Windows conditional already lists the needed libraries
(`ole32`, `mmdevapi`, `ksuser`); `index.js` is explicitly frozen; no new native
dependency is introduced, per the locked-in Option B decision.
