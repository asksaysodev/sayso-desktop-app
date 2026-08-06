# Audio Module: Architecture & Windows Support Assessment

## Current macOS Architecture

The entire audio stack lives in a single **Objective-C++ native addon** (`client/electron/native-audio/src/audio_device_manager.mm`, ~1929 lines), compiled against five Apple-only frameworks and exposed to the Electron main process via NAN bindings.

### System Audio Capture (Lead / Prospect)

Captures the macOS system audio output — what the lead hears — using **ScreenCaptureKit (SCK)**, available only on macOS 13+.

| Step | Detail |
|------|--------|
| API | `SCStream` with audio-only configuration |
| Format | 48 kHz, stereo, 32-bit PCM |
| Permission | Screen Recording (required by Apple for SCK) |
| Self-exclusion | macOS sets `excludesCurrentProcessAudio = YES` as a defensive default — Sayso emits no audio, so it's effectively a no-op. **Not needed on Windows**: use plain render-endpoint loopback (no per-process exclusion). |
| Callback path | SCK dispatch queue → `uv_async_send` → Node.js event loop → JS callback |

Key globals: `g_stream`, `g_config`, `g_filter`, `g_isCapturing`, `g_streamingCallback`, `g_streamingAsyncHandle`.

### Microphone Capture (Agent / User)

Captures the agent's microphone using **AVAudioEngine** with an input node tap.

| Step | Detail |
|------|--------|
| API | `AVAudioEngine` + `AVAudioInputNode.installTapOnBus:bufferSize:format:block:` |
| Format | Auto-negotiated (nil format handed to tap); typically 48 kHz mono Float32 |
| Buffer size | 4096 frames |
| Device tracking | CoreAudio HAL property listener on `kAudioHardwarePropertyDefaultInputDevice` |
| Bluetooth handling | Debounced restart — 80 ms fast path (device ID changed) / 600 ms slow path (same ID churn) |
| Retry logic | Up to 2 retries with 1200 ms / 1500 ms wait on engine cold-start failure |

Key globals: `g_micEngine`, `g_micInputNode`, `g_isMicCapturing`, `g_micOpenedInputDeviceId`, `g_micRouteRestartGeneration`.

### Linked Apple Frameworks

| Framework | Purpose |
|-----------|---------|
| ScreenCaptureKit | System audio loopback capture |
| AVFoundation | Microphone capture via AVAudioEngine |
| CoreAudio | HAL — device enumeration, default device queries, property listeners |
| CoreMedia | CMTime for frame timing |
| CoreFoundation / Foundation | Utilities, string handling, Objective-C runtime |

### Audio Pipeline (JS Layer)

After the native layer delivers raw PCM buffers via JS callbacks, the streaming pipeline in `client/electron/streaming/` handles the rest:

```
Native PCM buffer (48 kHz / stereo / Float32 or Int32)
    ↓ audioBuffer.js     — accumulate into ~100 ms chunks, detect format changes
    ↓ audioConverter.js  — bit depth → PCM-S16 | stereo → mono | resample → 16 kHz
    ↓ audioStreamer.js   — dual WebSocket streams (user mic + prospect audio)
    ↓ AssemblyAI         — 16 kHz / mono / PCM-S16LE
```

### Platform Guard

```typescript
// main.ts
if (process.platform === 'darwin') {
  nativeAudio = require('./native-audio');
}
```

The native module is never loaded on non-macOS platforms. Windows users currently get no audio capture at all.

---

## What a Windows Port Would Require

None of the native code compiles on Windows. Objective-C++ and all five Apple frameworks are macOS-only. A Windows implementation means a new file — `audio_device_manager_win.cpp` — written in C++, using the Windows audio stack.

### Windows API Equivalents

| Mac API | Windows Equivalent |
|---------|-------------------|
| ScreenCaptureKit (`SCStream`) | **WASAPI loopback** (`AUDCLNT_STREAMFLAGS_LOOPBACK`) |
| AVAudioEngine + AVAudioInputNode | **WASAPI** input endpoint (`IAudioCaptureClient`) |
| CoreAudio HAL property listener | **`IMMNotificationClient`** (COM interface) |
| `kAudioHardwarePropertyDefaultInputDevice` | `IMMDeviceEnumerator::GetDefaultAudioEndpoint` |
| `uv_async_send` threading | Same libuv pattern — portable |

WASAPI (Windows Audio Session API) has been mandatory for all consumer audio hardware since Windows Vista. It is the correct target for a production consumer app — it abstracts individual hardware drivers just as AVFoundation does on Mac.

### Build System Changes

`binding.gyp` would need platform conditionals:

```gyp
{
  "conditions": [
    ["OS=='mac'", {
      "sources": ["src/audio_device_manager.mm"],
      "link_settings": { "libraries": ["-framework ScreenCaptureKit", ...] }
    }],
    ["OS=='win'", {
      "sources": ["src/audio_device_manager_win.cpp"],
      "libraries": ["-lole32", "-lmmdevapi", "-lksuser"]
    }]
  ]
}
```

---

## Honest Assessment: Is Windows Harder Than Mac?

### Where the Difficulty Is Comparable

The WASAPI core API is well-documented and has been stable for 15+ years. Loopback capture (`AUDCLNT_STREAMFLAGS_LOOPBACK`) is a well-understood pattern. The microphone capture path via WASAPI input endpoint is structurally similar to AVAudioEngine. The threading model (libuv) is the same on both platforms.

### Where Windows Is Genuinely Harder

**1. COM boilerplate overhead**
Device enumeration and change notifications on Windows require implementing a full COM class (`IMMNotificationClient`). The Mac equivalent is a ~20-line CoreAudio property listener. On Windows it is a verbose class with `QueryInterface` / `AddRef` / `Release` and five notification methods.

**2. Audio software ecosystem fragmentation**
This is the real production risk. Windows users commonly have one or more of:

- **Nahimic** (pre-installed on MSI, ASUS, Dell, HP gaming laptops)
- **Realtek Audio Manager / Realtek HD Audio**
- **DTS Sound Unbound / Dolby Atmos / Sonic Studio**
- **Nvidia RTX Voice / Nvidia Broadcast**
- **VoiceMeeter / VB-Cable** (popular with streamers and power users)
- **Discord's virtual audio device**
- **Gaming headset companion software** (SteelSeries, Razer, Corsair, HyperX)

These install virtual audio devices and audio enhancement layers. Your loopback capture might silently pick up the wrong endpoint, receive enhanced/processed audio, receive silence (if the enhancement layer sits between the render endpoint and WASAPI), or enumerate duplicate devices with confusing names. Endpoint selection logic needs to handle this deliberately — it is not handled automatically by WASAPI.

**3. Loopback endpoint selection**
Unlike SCK which captures the display's audio session, WASAPI loopback requires selecting a specific render endpoint (output device). You must determine which output device is the "real" default, handle the case where it is a virtual device (VoiceMeeter mix, etc.), and update when the user changes their default output. This logic does not exist in the current Mac code because SCK handles it implicitly.

**4. Device hot-plug reliability**
The Mac code contains 80 ms / 600 ms debounce logic, generation tokens to invalidate stale timers, retry loops with 1200 ms / 1500 ms waits, and a "first tap seen" diagnostic. This code was written in response to real-world failures with Bluetooth headsets. WASAPI has the same class of problems — audio engine restart on device change, format renegotiation, cold-start delays. A first-pass implementation will not include this defensive logic; it accumulates through testing and production incidents.

**5. Format negotiation**
WASAPI shared mode (the correct mode for a consumer app) requires negotiating a mix format with the audio engine. Not all drivers accept arbitrary formats. The current Mac code uses `nil` format on the AVAudioEngine tap and lets the HAL negotiate — on Windows, explicit format negotiation with `IsFormatSupported` and fallback logic is required.

---

## On "Vibecoding" the Windows Module

An AI-generated WASAPI implementation will produce structurally correct code that works on a clean Windows 11 machine with standard audio drivers. The problem is what it **won't** include:

- Correct COM initialization and teardown in the right thread context
- Loopback endpoint selection logic accounting for virtual devices
- Device hot-plug handling that does not deadlock or silently stop capturing
- Retry/debounce logic for Bluetooth device switches
- `IsFormatSupported` negotiation and fallback

These are not AI limitations — they are issues that only surface during testing on real hardware. The Mac code is 1929 lines not because the happy path is complex, but because the defensive code around edge cases is. Windows will require the same iteration.

**Conclusion:** Vibecoding gets you a working demo in a day. Getting to production reliability requires weeks of testing across hardware configurations, audio enhancement software combinations, and device switching scenarios. Skipping that work means shipping a module that works for reviewers and breaks for customers.

---

## Recommended Approach

### Option A: Use a Cross-Platform Library (Lower Risk)

For the **microphone** side, use a battle-tested cross-platform audio library:

- **[PortAudio](https://www.portaudio.com/)** — 15+ years old, WASAPI + DirectSound backends, used in Audacity and VLC. The WASAPI backend handles format negotiation and device enumeration reliably.
- **[miniaudio](https://miniaud.io/)** — Single-header C library, WASAPI + DirectSound + WinMM backends, modern and actively maintained. Much simpler to integrate than PortAudio.

For **system audio loopback** (the harder part), no cross-platform library handles this well. Write a focused WASAPI loopback capture module (~400–600 lines of careful C++) specifically for Windows.

This approach:
- Reduces microphone implementation to mostly configuration and callback wiring
- Focuses custom effort on the one piece with no good library (loopback)
- Gives you a battle-tested device change handling baseline for free

### Option B: Write Full WASAPI Implementation

Write `audio_device_manager_win.cpp` from scratch covering both microphone and loopback. Structurally mirrors the Mac `.mm` file. More control, more maintenance surface.

Requires:
- COM setup (`CoInitializeEx`, `CoUninitialize`)
- `IMMDeviceEnumerator` for device enumeration and default endpoint selection
- `IMMNotificationClient` subclass for device change events
- `IAudioClient` + `IAudioCaptureClient` for both loopback and microphone
- Format negotiation with `IsFormatSupported`
- Debounce/restart logic for device changes
- Threading model matching the Mac implementation (libuv async callbacks)

### What Is Not Negotiable for Production Quality

Regardless of approach, Windows shipping requires:

- [ ] Testing on machines with Nahimic installed (common on gaming laptops)
- [ ] Testing with VB-Cable / VoiceMeeter installed (common among power users)
- [ ] Testing Bluetooth headset connect/disconnect mid-session
- [ ] Testing audio device removal mid-capture
- [ ] Testing on Windows 10 (22H2) and Windows 11
- [ ] Testing with multiple virtual audio devices in the device list
- [ ] Testing with Realtek, Intel, and AMD audio drivers
- [ ] Validating loopback captures the correct endpoint when non-default output devices exist

---

## Required Cleanup Before Windows Work Begins

The codebase has several structural issues that must be resolved before a Windows developer can work effectively on the native module. These are not cosmetic — items 1 and 2 are direct preconditions for writing a Windows native module without reverse-engineering 2000 lines of unrelated code to understand what interface to implement.

### 1. `main.ts` Is Doing Too Much (2108 lines)

`main.ts` currently owns: window creation, all IPC handlers, audio lifecycle, auth token management, auto-updater, global shortcuts, logging setup, and environment loading — all in one file. The audio cleanup function alone (`cleanupAllAudioCapture`) is ~130 lines inside it.

Audio-related state is also scattered as loose globals throughout the file:

```
global.userFullRecordingProcess
global.mediaRecorder
global.userMediaRecorder
global.userAudioStream
global.prospectAudioStream
global.screenCapture
global.userRecordingFile
global.prospectRecordingFile
global.userActualStartMs
```

These need to live in a dedicated audio manager, not in a process entry point.

### 2. `recorder.ts` Has No Platform Abstraction (Blocker)

Line 1 of `recorder.ts`:

```ts
const nativeAudio = require('./native-audio');
```

No platform guard. On Windows this crashes immediately. `main.ts` has a `process.platform === 'darwin'` check before loading `nativeAudio`, but `recorder.ts` does not. Before Windows work can begin, a platform abstraction layer is needed — a `nativeAudioProvider.ts` that loads the correct native module per platform and exports a unified interface. No other file should `require` the native module by name directly.

### 3. `audioQueue.ts` Is Dead Code in the Current Cue Path

`AudioQueue` is built around file paths — its items carry a `filePath` field and call `onChunk(filePath, speaker)`. The current Cue path streams raw buffers directly through `cueAudioStreamer.addUserAudio()` / `addProspectAudio()`. The queue is imported in `main.ts`, its events are wired up, and a `get-audio-queue-status` IPC handler is registered — but `enqueue()` is never called in the streaming flow. It is dead code that will mislead a Windows developer trying to understand the audio path.

### 4. `_getSessionId` in `audioStreamer.js` Is Dead Code

```js
async _getSessionId(token) {
  return `temp-session-${Date.now()}`; // real implementation commented out
}
```

The `sessionId` is always passed in from the `start-cue` IPC handler in `main.ts` — the `if (!this.sessionId)` branch never executes. The `axios` import at the top of `audioStreamer.js` exists only for this dead method. Both should be removed.

### 5. Two Streamers With Unclear Ownership

`main.ts` contains both `audioStreamer` (older variable, referenced only by `get-streaming-status`) and `cueAudioStreamer` (the live production path). The relationship between them is not documented. Before a Windows developer touches anything, this should resolve to one clear audio streamer with an explicit lifecycle.

---

### Cleanup Tasks (In Priority Order)

| Priority | Task | Why It Matters for Windows |
|---|---|---|
| 1 | Create `nativeAudioProvider.ts` — loads `./native-audio` on macOS, `./native-audio-win` on Windows, exports one interface | Without this, Windows crashes on `recorder.ts` import; no clean seam to insert the new module |
| 2 | Extract `audioManager.ts` — owns `cueAudioStreamer`, teardown, cleanup, all audio globals, and `start-cue`/`stop-cue` IPC handlers | Gives Windows developer a single file to understand the audio contract |
| 3 | Delete `audioQueue.ts` and its IPC handler from `main.ts` | Removes a misleading file-path-based code path that has nothing to do with streaming |
| 4 | Remove dead code in `audioStreamer.js` — `_getSessionId`, commented axios call, `axios` import, stale TODOs | Reduces noise in the one file a Windows developer will study most |
| 5 | Resolve the `audioStreamer` vs `cueAudioStreamer` split in `main.ts` | Establishes unambiguous ownership of the live audio path |

---

## File Reference

| File | Purpose | Status |
|------|---------|--------|
| `client/electron/native-audio/src/audio_device_manager.mm` | Full macOS implementation (1929 lines) | macOS only |
| `client/electron/native-audio/binding.gyp` | Node-gyp build config (macOS only currently) | Needs platform conditionals for Windows |
| `client/electron/native-audio/index.js` | JS wrapper (`AudioDeviceManager` class) | macOS only |
| `client/electron/streaming/audioStreamer.js` | Dual-stream orchestrator | Cross-platform, needs dead code removed |
| `client/electron/streaming/audioBuffer.js` | Chunk buffering + format change detection | Cross-platform, no changes needed |
| `client/electron/streaming/audioConverter.js` | PCM format conversion (bit depth, resample, mono) | Cross-platform, no changes needed |
| `client/electron/streaming/streamingConfig.js` | Centralized format and endpoint config | Cross-platform, no changes needed |
| `client/electron/recorder.ts` | Mic capture wrapper — directly requires native-audio | Needs platform abstraction before Windows |
| `client/electron/audioQueue.ts` | File-based audio queue (EventEmitter) | Dead code in current streaming path — remove |
| `client/electron/main.ts` | IPC handlers + `process.platform === 'darwin'` guard | Needs audio logic extracted to `audioManager.ts` |
