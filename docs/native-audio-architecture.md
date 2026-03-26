# Native Audio Architecture

> Last updated: March 2026  
> Covers: current implementation + next-sprint work items for user-selectable microphone

---

## 1. Overview

Sayso captures two independent audio streams simultaneously:

| Stream | Source | Technology | Purpose |
|---|---|---|---|
| **User mic** | macOS input device (AirPods, built-in, USB mic…) | `AVAudioEngine` + tap | Transcribe the Sayso user's speech |
| **System audio** | macOS audio output (prospect's voice via call app) | `ScreenCaptureKit` (SCK) | Transcribe the prospect's speech |

Both streams are captured in the native addon (`audio_device_manager.mm`) compiled as a Node.js native module via `nan`. The addon runs inside the Electron main process and pushes raw PCM buffers to JavaScript over `libuv` async handles.

---

## 2. File Map

```
client/electron/native-audio/
├── src/
│   └── audio_device_manager.mm   ← All native capture logic (Obj-C++)
├── index.js                      ← JS wrapper class (NativeAudioManager)
└── binding.gyp                   ← Build config

client/electron/
├── main.ts                       ← IPC surface; starts/stops capture
└── streaming/
    ├── audioStreamer.js           ← Orchestrates WebSocket streaming
    ├── audioBuffer.js             ← Accumulates chunks, detects format changes
    └── websocketClient.js         ← WebSocket helpers (sendJson, etc.)
```

---

## 3. User Microphone Capture — Current Implementation

### 3.1 Session lifecycle

```
main.ts                    NativeAudioManager (index.js)       audio_device_manager.mm
  │                                │                                    │
  │  startMicrophoneCapture()      │                                    │
  │──────────────────────────────►│                                    │
  │                                │  setMicrophoneStreamingCallback()  │
  │                                │───────────────────────────────────►│  stores g_micStreamingCallback
  │                                │  startMicrophoneCapture()          │
  │                                │───────────────────────────────────►│
  │                                │                                    │  TryStartMicrophoneCaptureOnce(1200ms)
  │                                │                                    │    → new AVAudioEngine
  │                                │                                    │    → installTapOnBus:0 (AVAudioInputNode)
  │                                │                                    │    → engine.startAndReturnError()
  │                                │                                    │    → WaitForMicFirstTapMs(1200ms)
  │                                │                                    │
  │                                │           true / false             │
  │                                │◄───────────────────────────────────│
  │                                │                                    │  if ok: g_isMicCapturing = true
  │                                │                                    │         g_micOpenedInputDeviceId = HAL default id
```

**Key globals (`.mm`):**

| Variable | Type | Meaning |
|---|---|---|
| `g_micEngine` | `AVAudioEngine*` | The running engine; `nil` when stopped |
| `g_micInputNode` | `AVAudioInputNode*` | Tap target; `nil` when stopped |
| `g_isMicCapturing` | `bool` | True while capture should be active |
| `g_micOpenedInputDeviceId` | `AudioDeviceID` | HAL device ID the current engine was started against |
| `g_micRouteRestartGeneration` | `atomic<uint64_t>` | Coalescing token; bumped on every new default-input notification or explicit stop |

### 3.2 Audio tap and data flow

The tap is installed on `AVAudioInputNode` bus 0 with an **explicit nil format** (accepts whatever format the engine negotiates with the hardware). This gives:

- **AirPods (HFP):** 24 kHz, 1 ch, Float32
- **Built-in mic:** 48 kHz, 1 ch, Float32

On each tap buffer the native code:
1. Copies the raw PCM samples into a `StreamingData` heap struct
2. Records `sampleRate`, `channels`, `bitDepth`, `isFloat`
3. Signals `uv_async_send(g_micStreamingAsyncHandle)` — safe cross-thread wake

The `libuv` callback runs on the Node.js event loop, converts the struct into a `Buffer` + format object, and calls the JS callback registered by `index.js`.

### 3.3 Format conversion (JS side)

`audioStreamer.js` receives raw PCM buffers and passes them to `audioBuffer.js`, which accumulates them. When chunks are ready, `audioStreamer._processUserChunks()` converts each chunk to **16 kHz / mono / PCM-S16LE** (AssemblyAI's required format) via `convertToAssemblyAIFormat()` and sends the result over the user WebSocket.

### 3.4 Default-input device listener

At initialization (`Initialize` NAN_METHOD), the native code registers a CoreAudio property listener on `kAudioHardwarePropertyDefaultInputDevice`:

```
SaysoDefaultInputDeviceListenerProc
  └─► logs old → new device names
  └─► calls SaysoScheduleMicRouteDebouncedRestart()
```

This listener fires whenever macOS changes which device is the system default input (e.g. AirPods connect/disconnect, user changes Sound settings).

### 3.5 Debounced restart — the full state machine

Bluetooth device changes often cause multiple rapid default-input notifications in quick succession (OS churn). The restart scheduler coalesces them using an atomic generation token:

```
Default-input notification fires
  │
  ▼
SaysoScheduleMicRouteDebouncedRestart()  [on SaysoMicRouteRestartQueue — serial GCD queue]
  │
  ├─ reads HAL default now  (halNow)
  ├─ compares to g_micOpenedInputDeviceId
  │
  ├─ FAST PATH  (halNow ≠ opened id)          SLOW PATH  (halNow == opened id)
  │  debounce = 80 ms                          debounce = 600 ms
  │  stability = 60 ms                         stability = 150 ms
  │
  ├─ bump g_micRouteRestartGeneration  (wave N)
  │
  ▼
  dispatch_after(debounce)
    │  if wave stale → no-op (newer notification took over)
    │  if g_isMicCapturing == false → no-op
    │
    ▼  read HAL default (firstDefault)
    dispatch_after(stability)
      │  if wave stale → no-op
      │  if firstDefault ≠ current HAL default → still churn → reschedule
      │
      ▼  device stable
      SaysoPerformMicRestartIfCapturing(YES)
        │
        ├─ MicEngineTeardownOnly()  [removeTapOnBus + engine stop + nil refs]
        ├─ TryStartMicrophoneCaptureOnce(1200ms)
        │    └─ on fail: TryStartMicrophoneCaptureOnce(1500ms)
        │
        ├─ if ok:  g_micOpenedInputDeviceId = currentDefault
        │          log "✅ Mic route restart succeeded"
        └─ if fail: g_isMicCapturing = false
                    log "❌ Mic route restart FAILED"
```

**Why two paths?** On AirPods _disconnect_ the HAL default changes immediately to built-in mic (different ID) → fast path minimises the dead-audio window to ~140 ms. On spurious same-ID notifications (Bluetooth re-route, aggregate device churn) the slow path prevents thrashing.

### 3.6 JS-side resilience added in this sprint

**`audioBuffer.js` — format change detection**  
`addAudioData()` returns `routeFormatChanged = true` on the first chunk whose format differs from the established baseline (sampleRate, channels, bitDepth, isFloat). The buffer is cleared and adopts the new format.

**`audioStreamer.js` — route change handling**  
When `routeFormatChanged` is `true`:
- Near-silent chunks (`maxSample < 500`) are **dropped for 5 s** — this prevents AirPods mic-init noise from reaching AssemblyAI and crashing the streaming session
- A 1500 ms debounce timer fires `reset_transcription` to the server, requesting a fresh STT session once the new device has stabilised

**`cueStreamHandlers.js` — server-side recovery**  
Two mechanisms ensure transcription continues even if the AssemblyAI WebSocket closes mid-session:

1. **Non-destructive session swap** (`reset_transcription`): creates a new `StreamingTranscriber`, buffers incoming audio while it connects, flushes the buffer on `onOpen`, then closes the old session — no audio gap
2. **Auto-reconnect** (`scheduleReconnect`): if AssemblyAI closes the session unexpectedly (e.g. due to mic-init noise before the client's debounce fires), a new session is created automatically after 400 ms without any client action needed

---

## 4. System Audio Capture (Prospect)

Uses **ScreenCaptureKit** (`SCStream`). Captures the macOS audio output mix at 48 kHz stereo Float32. The tap format is fixed; no route-change listener is needed since system audio output routing is handled by the OS and doesn't affect the SCK stream.

The prospect stream is isolated from the user mic stream — they use separate native callbacks (`g_streamingCallback` vs `g_micStreamingCallback`) and separate WebSocket connections.

---

## 5. IPC Surface (current)

`main.ts` talks to the native addon through `NativeAudioManager` (`index.js`):

| JS method | NAN export | Notes |
|---|---|---|
| `startMicrophoneCapture({ streamingCallback })` | `startMicrophoneCapture` | Always captures OS default input |
| `stopMicrophoneCapture()` | `stopMicrophoneCapture` | Clears mic callback + stops engine |
| `isMicrophoneCaptureActive()` | `isMicrophoneCaptureActive` | Returns `g_isMicCapturing` |
| `startSystemAudioCapture(options)` | `startSystemAudioCapture` | SCK stream |
| `stopSystemAudioCapture()` | `stopSystemAudioCapture` | — |
| `listOutputDevices()` | `listOutputDevices` | Used for multi-output device creation |
| `createMultiOutputDevice(...)` | `createMultiOutputDevice` | Aggregate device for output routing |
| `deleteMultiOutputDevice(id)` | `deleteMultiOutputDevice` | — |

There is currently **no `listInputDevices` export** and no concept of a preferred input device UID.

---

## 6. Next Sprint — User-Selectable Microphone

### 6.1 Goal

Let users pick a preferred input device (persisted by stable Core Audio UID). When the preferred device is unavailable, fall back to the OS default transparently and notify the UI. The same `ResolveEffectiveInput` logic drives both session start and mid-session recovery.

### 6.2 New concept: `ResolveEffectiveInput`

```
ResolveEffectiveInput(preferredUID: string | null)
  → { deviceID: AudioDeviceID, source: "user_preference" | "system_default" | "fallback" }
```

- If `preferredUID` is null or empty → return OS default, source = `system_default`
- If `preferredUID` matches a currently-present device → return that device, source = `user_preference`
- If `preferredUID` set but device not found → return OS default, source = `fallback`, and signal JS with `mic-fallback-activated` IPC event

This function should be called:
- At `startMicrophoneCapture` start
- At each restart attempt inside `SaysoPerformMicRestartIfCapturing`

### 6.3 Required native changes (`audio_device_manager.mm`)

#### a) `ListInputDevices` NAN_METHOD (new export)

Returns a JS array of objects `{ id: number, uid: string, name: string }` for all CoreAudio devices that have input streams (`kAudioDevicePropertyStreams`, `kAudioDevicePropertyScopeInput`).

Pattern: clone `ListOutputDevices` NAN_METHOD (already exists), change scope to `kAudioDevicePropertyScopeInput`.

#### b) Preferred device globals

```objc
static NSString* g_preferredInputUID = nil;  // nil = follow OS default
static std::mutex g_preferredInputMutex;      // guards g_preferredInputUID reads/writes
```

#### c) `SetPreferredInputDevice` NAN_METHOD (new export)

Accepts `(uid: string | null)` from JS. Writes to `g_preferredInputUID` under the mutex. Does **not** restart capture immediately — the mode takes effect on the next `startMicrophoneCapture` call (or on the next debounce restart if a session is already active and the implementation opts for immediate switch).

#### d) `ResolveEffectiveInput` implementation

```objc
static AudioDeviceID ResolveEffectiveInput(BOOL* outFallbackUsed) {
    std::lock_guard<std::mutex> lk(g_preferredInputMutex);
    *outFallbackUsed = NO;

    if (!g_preferredInputUID || g_preferredInputUID.length == 0) {
        return SaysoGetCurrentDefaultInputDeviceID();
    }

    // Scan device list for matching UID
    // (enumerate kAudioHardwarePropertyDevices, call SaysoCopyAudioDeviceUID per device)
    AudioDeviceID found = SaysoFindInputDeviceByUID(g_preferredInputUID);
    if (found != kAudioObjectUnknown) {
        return found;
    }

    // Preferred device not found → fallback
    *outFallbackUsed = YES;
    return SaysoGetCurrentDefaultInputDeviceID();
}
```

#### e) Update `TryStartMicrophoneCaptureOnce`

Replace the hardcoded `SaysoGetCurrentDefaultInputDeviceID()` call with `ResolveEffectiveInput()`. Pass the resolved device ID to `AVAudioEngine`'s input node via:

```objc
AVAudioSession or AudioUnit input device selection
// On macOS desktop: use kAudioHardwarePropertyDefaultInputDevice override is not needed —
// AVAudioEngine follows the HAL default; to target a specific device, set it as the HAL
// default for the process, or use AudioUnit directly.
// Practical approach: call AudioDeviceSetProperty / set kAudioHardwarePropertyDefaultInputDevice
// on the HAL for the session scope, or use an audio unit with kAudioDevicePropertyDeviceUID.
```

> **Note:** On macOS (non-iOS), `AVAudioEngine`/`AVAudioInputNode` always routes to the system default input. To target a specific device you must either (a) set the process default before starting the engine, or (b) use an `AudioUnit` (`kAudioUnitSubType_HALOutput`) with explicit device selection. Option (a) is simpler and sufficient for Sayso's single-capture-at-a-time model.

#### f) Mode-based listener strategy

| Mode | Listener needed |
|---|---|
| Follow OS default (`preferredUID` = null) | Existing `kAudioHardwarePropertyDefaultInputDevice` listener ✅ |
| Fixed preferred UID | Also watch `kAudioHardwarePropertyDevices` (device list changes) to detect when the preferred device appears or disappears |

When in fixed-preferred mode: default-input churn does not require a restart **unless** the preferred device has disappeared (i.e. `ResolveEffectiveInput` would now return fallback). Saves unnecessary restarts when the user has a USB mic and AirPods connect/disconnect.

#### g) Fire IPC event on fallback

When `ResolveEffectiveInput` returns `outFallbackUsed = YES`, notify JS via a new async event callback (similar to how existing audio buffers are delivered), or emit through the existing Node.js EventEmitter surface. The event payload:

```json
{ "type": "mic-fallback-activated", "preferredUID": "DC-80-84-9C-C8-35:input", "fallbackDeviceName": "MacBook Pro Microphone" }
```

### 6.4 Required JS / IPC changes

#### `index.js` (NativeAudioManager)

Add methods:
```js
async listInputDevices()
  → [{ id: number, uid: string, name: string }]

async setPreferredInputDevice(uid: string | null)
  → void (persists to Electron store, passes uid to native)

async getPreferredInputDevice()
  → string | null  (reads from Electron store)
```

Update `startMicrophoneCapture` to pass `preferredUID` into the native call.

#### `main.ts`

- IPC handler `list-input-devices` → calls `nativeAudioManager.listInputDevices()`
- IPC handler `set-preferred-input-device` → validates + persists UID, calls `nativeAudioManager.setPreferredInputDevice(uid)`
- IPC handler `get-preferred-input-device` → returns stored preference
- Listen for `mic-fallback-activated` from native → forward to renderer via `win.webContents.send('mic-fallback-activated', payload)` for non-blocking UI copy

Persist preference using Electron's `electron-store` (or equivalent), keyed by UID (stable across renames/reconnects).

### 6.5 Required UI changes

#### Settings / coach-adjacent dropdown

```
Input Device
┌─────────────────────────────────────────────────────────┐
│ ✓  System default (recommended)                         │
│    AirPods de Franco  [DC-80-84:input]                  │
│    MacBook Pro Microphone  [BuiltInMicrophoneDevice]    │
└─────────────────────────────────────────────────────────┘
```

- Populate from `list-input-devices` IPC, refresh on focus
- Store selection as UID, display label fetched from OS
- "System default" = `null` preference (always present at top)
- When `mic-fallback-activated` event received: show non-blocking toast/copy "Previous microphone unavailable — using system default"

### 6.6 Mid-session behavior decision

**Recommended (Option A — simplest):** Preference only applies on next `startMicrophoneCapture`. If user changes preference mid-session, they see a note "Change applies on next session start." This avoids tearing down a live streaming session for a settings change.

**Alternative (Option B):** If preference changes while a session is active, call `SaysoScheduleMicRouteDebouncedRestart()` immediately. More seamless UX but requires careful interaction with the existing debounce state.

Document whichever is chosen in this file.

### 6.7 Acceptance criteria checklist

- [ ] `listInputDevices` returns `[{ id, uid, name }]` for all present input devices
- [ ] Selected UID persists in Electron store; survives app restart
- [ ] With a valid preferred device present: capture opens that device
- [ ] With preferred UID set but device absent: capture opens OS default, `mic-fallback-activated` fires, UI shows copy
- [ ] With `null` preference (system default mode): changing OS default mid-session triggers existing debounced restart and audio continues after a short blip
- [ ] Removing preferred device mid-session (disconnect USB mic, AirPods, etc.) falls back and continues capture when possible
- [ ] All new native symbols exported in `NAN_MODULE_INIT(Init)` and surfaced in `index.js`

---

## 7. Known Limitations

- **Aggregate / virtual devices:** Core Audio aggregate devices created by third-party apps (Loopback, Rogue Amoeba, etc.) may have unstable UIDs across sessions. Documented limitation for v1.
- **iOS / mobile:** This architecture is macOS-desktop-only. No mobile path.
- **Hot-plug USB mics:** The `kAudioHardwarePropertyDevices` listener (needed for fixed-preferred mode) should catch these; if the device is preferred and disappears the fallback path fires. Test manually.
- **Multiple simultaneous input devices:** Sayso captures exactly one user mic at a time. Aggregate input (mix two mics) is out of scope.
