# Windows Native Audio — QA Test Plan (SAYSO-392 / SAYSO-332)

**Branch under test:** `test/sayso-392-windows-audio-qa`
**Contents:** PR #55 (`WindowsAppV1` — the WASAPI native audio backend) merged with
`development` (which already carries the SAYSO-382 Windows window/tray/lifecycle
seams). Both are needed for a usable Windows build: #55 alone boots on an older
`development` that still has native window chrome and no tray sizing fixes.

**What is actually new here:** `electron/native-audio/src/audio_device_manager_win.cpp`
(+2.5k lines, hand-rolled WASAPI loopback + mic, two independent recovery state
machines) and `electron/audio/WindowsAudioProvider.ts` (placeholder → real
`require('../native-audio')` pass-through). Nothing in it has ever been built or
run on Windows.

Reference for expected behavior: `docs/NATIVE_AUDIO_CONTRACT.md` (the frozen
13-method contract, buffer format, and the device-change resilience bar) and
`docs/WINDOWS_AUDIO_PLAN.md` (the implementer's design notes).

---

## 1. Environment setup

Run the matrix on **two machines**: Windows 10 (1809+) x64 and Windows 11 x64.

Prerequisites:

- Node 20+, Python 3, and **Visual Studio Build Tools** with the "Desktop
  development with C++" workload (MSVC v143 + Windows 10/11 SDK). The module is
  C++20 and links `ole32` / `mmdevapi` / `ksuser`.
- Git, and the `.env.hestabit` env file (ask Franco — it is not in the repo).

```powershell
git clone https://github.com/asksaysodev/sayso-desktop-app.git
cd sayso-desktop-app
git checkout test/sayso-392-windows-audio-qa
npm ci
npm run rebuild-native-win     # node-gyp against the Electron 36 headers, x64
npm run dev:hestabit           # dev build against the Hestabit env
```

`npm run rebuild-native-win` must be re-run after any `.cpp` change — a stale
`build/Release/audio_device_manager.node` is the most common cause of a
"my fix didn't do anything" report.

Hardware needed:

- A built-in or USB microphone.
- A Bluetooth headset (with mic) that can be connected/disconnected on demand.
- A wired headset/USB device for the endpoint-switch cases.
- Speakers/output able to play audio for the loopback tests.

## 2. How to observe a result

A test only passes if audio actually reaches the transcript — a green UI with no
words is a **fail**, not a pass (that is the exact "silent latch-off" failure the
contract calls the worst case).

Evidence to capture on every run, pass or fail:

1. **Live transcript in the Cue window** — the end-to-end proof that PCM reached
   AssemblyAI. Prospect (system audio) and You (mic) are separate lanes; note
   which lane died.
2. **DevTools console** — `Ctrl+Shift+I` (restored on Windows dev builds by
   SAYSO-382). Watch for `startMicrophoneCapture` / `startProspectStreaming`
   results and any snake_case lifecycle events. Anything ending in `_failed` is a
   real defect even if the UI looks fine.
3. **Log file** — `%APPDATA%\Sayso\logs\` (packaged builds).
4. For crashes: the Windows Event Viewer entry (Application log) plus whatever
   the console printed immediately before.

Record for each case: OS build number, device names involved, pass/fail, and the
console excerpt.

---

## 3. Test cases

Ten cases. Fill in Result / Notes as you go.

### TC-01 — Microphone audio capture (Functional)

| | |
|---|---|
| **Steps** | 1. Connect a microphone. 2. Start a Cue session (mic capture). 3. Speak into the microphone. 4. Stop capture. |
| **Expected** | Mic audio is captured and raw PCM is streamed. Your speech appears in the "You" transcript lane within a couple of seconds. The `format` reported by the module truthfully describes the buffer (sample rate / channels / bit depth / float) — the converter downmixes to 16 kHz mono S16 from whatever is reported, so a wrong `format` shows up as garbled or chipmunk transcript text. |
| **Result** | |
| **Notes** | |

### TC-02 — System audio / loopback capture (Functional)

| | |
|---|---|
| **Steps** | 1. Connect an output device. 2. Play audio on the system (a video call, or a YouTube clip of speech). 3. Start system audio capture. 4. Stop capture. |
| **Expected** | System audio is captured through WASAPI loopback and lands in the "Prospect" transcript lane. Sayso's own output must not be captured back into the loopback (no self-echo loop). |
| **Result** | |
| **Notes** | |

### TC-03 — Start / stop capture (Functional)

| | |
|---|---|
| **Steps** | 1. Start capture. 2. Verify audio data is flowing (both lanes). 3. Stop capture. 4. Start capture again. |
| **Expected** | Capture starts and stops cleanly with no error dialogs, and the second start behaves exactly like the first — no stale state, no transcript text carried over from the previous session, no "already active" rejection. `isMicrophoneCaptureActive()` / `isSystemAudioCaptureActive()` must read false between the runs. |
| **Result** | |
| **Notes** | |

### TC-04 — Repeated start/stop (Lifecycle)

| | |
|---|---|
| **Steps** | 1. Start capture. 2. Stop capture. 3. Repeat at least 10 times, including a few rapid stop-immediately-after-start cycles. |
| **Expected** | No crash, no resource leak, no duplicate stream, no stale capture state. Watch Task Manager: the app's memory and handle count should return to roughly the same level after each cycle rather than climbing every iteration. Two overlapping streams would show as doubled/echoed transcript text. |
| **Result** | |
| **Notes** | |

### TC-05 — Bluetooth headset connected during active capture (Edge case)

| | |
|---|---|
| **Steps** | 1. Start capture on the current device. 2. Connect a Bluetooth headset mid-session. 3. Observe capture behavior. |
| **Expected** | The default-device change is detected and the route transition is handled safely: capture continues, the session is **not** dropped, and after a brief gap audio resumes from the new route. A short gap while the device settles is acceptable (transient); permanent silence in either lane is a failure and must have produced a `_failed` lifecycle event in the console. Windows switching both the render and capture default at once (typical for a headset) must not let one recovery invalidate the other. |
| **Result** | |
| **Notes** | |

### TC-06 — Bluetooth headset disconnected during active capture (Edge case)

| | |
|---|---|
| **Steps** | 1. Start capture. 2. Connect a Bluetooth headset and make it the active device. 3. Disconnect the headset mid-capture (power it off / move it out of range as well as unpairing). |
| **Expected** | Device removal is detected and capture recovers onto the fallback device without crashing. Same bar as TC-05: recovery or an explicit `_failed` event — never silent death. Test both an ordered disconnect and an abrupt power-off, since they raise different WASAPI events. |
| **Result** | |
| **Notes** | |

### TC-07 — Wired device handling (Compatibility)

| | |
|---|---|
| **Steps** | 1. Plug in a wired device. 2. Keep the physical device available. 3. Start capture. 4. Switch endpoints (Windows Sound settings → change default input/output, including switching between the jack's virtual endpoints). |
| **Expected** | Capture handles the virtual endpoint correctly and stays stable across the switch; audio follows the new endpoint. |
| **Result** | |
| **Notes** | |

### TC-08 — Windows 10 compatibility (Compatibility)

| | |
|---|---|
| **Steps** | Run TC-01, TC-02 and a device switch **on Windows 10** (1809 or later — Electron 36 does not support anything older). |
| **Expected** | Mic capture, loopback capture and device handling all work as they do on Windows 11. |
| **Result** | |
| **Notes** | The source spreadsheet's steps for this row said "Windows 11" — that is a copy-paste error; this row is the Windows 10 pass. |

### TC-09 — Windows 11 compatibility (Compatibility)

| | |
|---|---|
| **Steps** | Run TC-01, TC-02 and a device switch **on Windows 11**. |
| **Expected** | Mic capture, loopback capture and device handling all work as expected. |
| **Result** | |
| **Notes** | |

### TC-10 — Mic and system capture after application restart (Recovery / Regression)

| | |
|---|---|
| **Steps** | 1. Start both captures. 2. Quit the app (also try killing it from Task Manager while capture is running) and relaunch. 3. Start both captures again. |
| **Expected** | Both capture types initialize correctly after restart. A hard kill mid-capture must not leave an endpoint locked — the next launch starts capture normally rather than failing to open the device. |
| **Result** | |
| **Notes** | |

---

## 4. Areas worth extra attention

These come out of the macOS incidents the contract was written from; the Windows
port is structurally a mirror of the macOS module, so it may have inherited the
same shapes:

- **Silent latch-off (SAYSO-353).** A path that dies and never recovers must emit
  a `_failed` lifecycle event. If a lane goes quiet with a clean-looking UI, that
  is the highest-priority bug class here — call it out explicitly.
- **Teardown ordering.** Stop while a device-change recovery is in flight
  (start capture → yank the headset → hit stop within a second). macOS has open
  work here (SAYSO-362 async-handle teardown UAF, SAYSO-363 single-slot handoff);
  this is the most likely place to find a Windows crash.
- **Distinguishable failures (SAYSO-347).** When a start fails, the console must
  say *which* way it failed, not just `false`.

## 5. Reporting

File one Linear issue per distinct defect in the SAYSO team, linked to
SAYSO-392, with: OS + build number, device make/model, the test case ID, the
console excerpt, and whether it reproduces every time or intermittently. Attach
the log file for crashes.
