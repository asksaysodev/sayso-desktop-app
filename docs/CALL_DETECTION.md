# Call detection (SAYSO-528)

When another app, in v1 a browser, holds the mic for 3s, the main process emits `call-candidate`, which shows the "Launch Sayso?" widget (SAYSO-529).

## Pieces

| Layer | File | Job |
|---|---|---|
| Native | `electron/mic-activity/src/mic_activity_mac.mm`, `mic_activity_win.cpp` | Per-process mic start/stop events, delivered to JS through `uv_async_send` |
| Glue | `electron/mic-activity/src/mic_activity.cc`, `index.js` | NAN `start(cb)` / `stop()` / `mode()` |
| Service | `electron/callDetection/micActivityMonitor.ts` | Resolve the app, exclude Sayso, filter to browsers, debounce, emit `call-candidate` / `call-ended` |
| Allowlist | `electron/callDetection/browserAllowlist.ts` | Browser bundle ids (macOS) and exe names (Windows) |
| Prompt | `electron/callDetection/callPrompt.ts` | Show/hide rules, 10s auto-hide, dismissed sessions |
| Widget | `call-detected-window.html`, `src/callDetectedWindow/` | The SAYSO-530 design: dark card, Dismiss / Launch Coach, timer band |
| Wiring | `electron/main.ts` | Start on `signed-in`. Stop on sign-out, session expiry and quit. Per-call subscription check. Creates the widget window |

The addon is separate from `electron/native-audio/` on purpose, because another team owns that one. Packaging builds the native-audio addon first, unchanged, and builds this one afterwards. Any failure here is a warning, and the app ships without detection.

## Detection by OS

- **macOS 14.2+ (`per-process`):** a listener on `kAudioHardwarePropertyProcessObjectList`, plus a per-process listener on `kAudioProcessPropertyIsRunningInput`. The PID and bundle id come from `kAudioProcessPropertyPID` / `kAudioProcessPropertyBundleID`. The owning app is found in three steps:
  1. `responsibility_get_pid_responsible_for_pid`, loaded through `dlsym` and optional.
  2. `NSRunningApplication` for the bundle id and URL.
  3. If that fails, the first `.app/` in the executable path.
- **macOS 13–14.1 (`unsupported`):** no detection. The only signal is `kAudioDevicePropertyDeviceIsRunningSomewhere`, which has no attribution and which Hey Siri keeps on permanently (see Findings).
- **Windows:** WASAPI `IAudioSessionManager2` on every active capture endpoint. Events come from session notifications and per-session `OnStateChanged`, aggregated per PID. The owning app is the topmost ancestor process running the same exe.

## Findings

Verified on macOS 26.5.2 (arm64), 2026-10-26:

- **`kAudioProcessPropertyIsRunningInput` sends no notification of its own.** When capture starts, only `kAudioProcessPropertyDevices` (`pdv#`) fires on the process object. The watcher therefore listens to every property on each process (wildcard) and re-reads the flag. The first version listened to the flag alone and never saw a start.
- **Terminal recording:** an `ffmpeg` capture started from a terminal resolves to the terminal app (`com.mitchellh.ghostty`) through the responsible PID. Start and stop each arrived within about 100ms.
- **No permission needed.** Reading the process list and its properties from an unsigned CLI showed no TCC prompt, without any mic grant.
- **Chrome:** the mic users are `com.google.Chrome.helper` processes. The responsible PID is the Chrome main process. Chrome can run from a code-sign clone path (`…/code_sign_clone/…/Google Chrome.app.bundle/…`), so the path-based lookup breaks. `NSRunningApplication` on the responsible PID returns `com.google.Chrome` and `/Applications/Google Chrome.app`, which is why it is the primary lookup.
- **Safari:** audio runs in the shared `com.apple.WebKit.GPU` process. During a Meet call the responsible process is Safari, which resolves to `com.apple.Safari` ✅.
- **Browsers briefly close and reopen the mic.** Safari did stop → start about 50ms apart while joining. The monitor keeps a session alive for `MIC_STOP_GRACE_MS` (1.5s) after the last stop, so a flicker doesn't end the call and start a new one. If the debounce elapses inside the grace period, the call is detected when the mic comes back.
- **Hey Siri holds the mic permanently.** `com.apple.CoreSpeech` (no app path) shows `IsRunningInput = 1` whenever voice activation is on. It isn't a browser, so per-process mode ignores it. But it pins the device-wide `IsRunningSomewhere` flag on, so the macOS 13–14.1 fallback can never see a call start for those users. **Decision (10-26): no detection below 14.2.**
- **Meet opens the mic on the pre-join screen**, before "Join" is clicked. Detection, and the 529 prompt, fire there, which suits "about to take a call".
- **Self-exclusion:** Cue captures in the main process (`process.pid`). The Settings mic test (`getUserMedia`) runs in a Sayso helper process, and both are covered by `app.getAppMetrics()` PIDs. The execPath check also covers dev Electron.
- **Windows:** verified on Windows 11 with Chrome, Edge and Brave (see the Windows browser matrix below). The widget also shows and works there (10-27, dev build).

### Browser matrix: to do

Run `node scripts/mic-activity-watch.js` (plain-Node build, see `CLAUDE.md`). Start a Google Meet test call, end it, and record:

| Browser | Mic process (bundleId) | Resolved appBundleId | Start latency | Stop latency | Notes |
|---|---|---|---|---|---|
| Chrome | `com.google.Chrome.helper` (responsible = Chrome main) | `com.google.Chrome` ✅ | immediate (Meet pre-join screen opens the mic) | immediate on leaving | Mic stayed open from pre-join through the call: one start, one stop |
| Arc | | | | | |
| Edge | | | | | |
| Safari | `com.apple.WebKit.GPU` (responsible = Safari) | `com.apple.Safari` ✅ | immediate | n/a | Lives in a cryptex (`/System/Volumes/Preboot/Cryptexes/App/System/Applications/Safari.app`). Mic stopped and started again about 140ms apart while joining |
| Firefox | `org.mozilla.firefox` (main process, no helper) | `org.mozilla.firefox` ✅ | immediate | immediate | Two 25–40ms blips during Meet setup (below the debounce), then a steady hold. The second blip and the call were about 140ms apart, so the grace period merges them |
| Zoom (desktop, not watched) | `us.zoom.xos` (main process, no helper) | `us.zoom.xos` ✅ (user-level install, `~/Applications/zoom.us.app`) | immediate | — | Correctly ignored (not a browser). Ran alongside an open Safari call: two independent sessions |

Check these for false positives: Dictation, Siri, Loom, Google voice search.

### Windows browser matrix

Same procedure on Windows 11 (`mode` = `per-process`).

| Browser | Mic process (exe) | Resolved owner | Start latency | Stop latency | Notes |
|---|---|---|---|---|---|
| Chrome | `chrome.exe` | ✅ | | | Works. Latencies not recorded |
| Brave | `brave.exe` (child pid, responsible = Brave main) | `%LOCALAPPDATA%\BraveSoftware\Brave-Browser\Application\brave.exe` ✅ | immediate | n/a | Per-user install. Held ~3.8s, then a stop → start → stop → start burst while joining (gaps ~190ms, 1ms, ~280ms, all inside the grace period, so one session), then held |
| Edge | `msedge.exe` (child pid, responsible = Edge main) | `C:\Program Files (x86)\Microsoft\Edge\Application\msedge.exe` ✅ | immediate | immediate | Installs under `Program Files (x86)` even on x64. A 0ms start/stop blip, then the mic reopened ~800ms later (inside the grace period, so merged) and held ~10s until leaving |

Packaged check: run `npm run package:staging`, sign in and start a Chrome call. Confirm the log shows `[CallDetection] call detected`, no prompt appears, and `codesign -dv` on `app.asar.unpacked/electron/mic-activity/build/Release/mic_activity.node` passes.

## Prompt (SAYSO-529)

`showCallDetectedPrompt(session)` in `callPrompt.ts` is the single entry point; `call-candidate` reaches it through `onCallCandidate`, which first checks `canPrompt()` (signed in, subscribed, not reconnecting, and the "Meeting detection" setting on). It shows nothing if the coach window is open, Cue is active, a prompt is already up, or the user already dismissed this session.

- Window: top-right of the display under the cursor, inside the work area: the card sits 31px from the right and 52px below the top (Figma), the window adding 16px of transparent room for its shadow (`WINDOW_CONFIG.CALL_PROMPT`). `focusable: false` + `showInactive()` so it never takes focus from the call; `setAlwaysOnTop(true, 'screen-saver')`, plus `visibleOnFullScreen` on macOS, so it sits over a full-screen browser. Created on show, destroyed on hide.
- Look (SAYSO-530): the countdown is a lighter fill sweeping the Dismiss button left to right over the 10s; its CSS duration mirrors `CALL_PROMPT_AUTO_HIDE_MS`.
- Hover: the pointer over the card pauses the auto-hide (`call-prompt:hover` → `setCallPromptHovered`); leaving resumes it with the time that was left. The countdown fill pauses with it.
- Enter: the card slides in from past the right screen edge (250ms ease-out, `CALL_PROMPT_ENTER_MS`).
- Exit: `timeout` and `dismiss` slide the card back off the right edge (200ms ease-in, `CALL_PROMPT_EXIT_MS`); the other reasons close instantly. Main sends `call-prompt:exit` and closes the window after the animation whether or not it played.
- For both slides main widens the window to the work area's right edge (transparent, so invisible), otherwise the card would be clipped 15px short of the screen edge. It's back at its resting size once the card has landed, so that edge strip (browser scrollbars) stays clickable while the prompt is up. The card has a fixed width so resizing never moves it. Corners are concentric: the logo tile and buttons use card radius − padding (24 − 12 = 12px), encoded as `--radius-inner`. Font: DM Sans (`@fontsource-variable/dm-sans`, this window only).
- **Launch Coach** → `launchFromCallPrompt()`: hides the widget, then opens or focuses the coach window (same path as the tray, mic-permission check included). No session starts; this is the one function to change if the CTA should start one.
- Hide reasons:

| Reason | Trigger | Marks session dismissed |
|---|---|---|
| `launch` | Launch Coach | no |
| `dismiss` | Dismiss button, or the window closed from outside | yes |
| `timeout` | 10s untouched (`CALL_PROMPT_AUTO_HIDE_MS`) | yes |
| `call-ended` | the prompted call ended | no |
| `coach-opened` | `createCoachWindow()` from any path | no |
| `signed-out` | sign-out / session expiry | no; clears all dismissed sessions |
| `disabled` | "Meeting detection" turned off in Settings | no |

**Setting (SAYSO-531):** Settings > General > "Meeting detection" is `coach_settings.call_detection_prompt_enabled` (account-wide, default on). Main reads its cached copy (`callDetectionPromptEnabled` in `electron/store/cacheRegistry.ts`) synchronously, so a call never waits on the network. Unknown, a missing column, or no `coach_settings` row (404, SAYSO-446) all mean on. It gates only the prompt; the monitor keeps running.

`call-ended` clears the session from the dismissed set, so the next call prompts again. Dev: the tray shows a "Test call prompt" row in `npm run dev` builds.

## Go / no-go

Pending the matrix. The provisional debounce is 3s (`CALL_DETECTION_DEBOUNCE_MS`).
