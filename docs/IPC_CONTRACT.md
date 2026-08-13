# IPC Contract & Platform Classification

Reference for every IPC channel between the renderer and the Electron main
process, classified by how it behaves across platforms. Written for the Windows
port: it tells you which handlers "just work" on Windows, which are routed
through a per-OS provider you must implement, and which are macOS-only.

**Scope:** only `darwin` (macOS) and `win32` (Windows) are supported. Linux is
not a target.

## Classification

| Tag | Meaning |
|---|---|
| **universal** | One handler, identical behavior on both platforms. Uses only cross-platform Electron/Node APIs. Nothing to do for Windows. |
| **platform-dispatched** | Handler delegates to a provider interface with a per-OS implementation (`electron/audio/`, `electron/permissions/`). The Windows behavior lives in the `Windows*Provider`, not in the handler. |
| **darwin-only** | Only meaningful on macOS. On Windows it is either a no-op or returns a safe default. |
| **win32-only** | Only meaningful on Windows. (None today — reserved for the Windows mic-privacy gate, see below.) |

## Convention (how to add platform-specific behavior)

1. **Never** branch on `process.platform` inline inside an IPC handler. If a
   channel needs different behavior per OS, put it behind a provider interface
   (mirror `IPermissionsProvider` / `IAudioProvider`) and dispatch in that
   module's `index.ts`.
2. For small, presentational one-off differences (window positioning, quit
   semantics), use the flags in `electron/utils/platform.ts`
   (`IS_MAC`, `IS_WINDOWS`, `IS_LINUX`, `ALLOW_VIBRANCY`) — never a raw
   `'win32'`/`'darwin'` string literal.
3. Keep handlers thin. The handler validates/serializes; the provider does the
   platform work.

---

## Inbound channels (renderer → main)

### Audio / Cue — **platform-dispatched** (`electron/audio/audioManager.ts` → `IAudioProvider`)

| Channel | Kind | Notes |
|---|---|---|
| `start-cue` | invoke | Starts prospect (system-audio) + user (mic) streaming. All native capture goes through the provider; Windows impl = the WASAPI native module. |
| `stop-cue` | invoke | Tears down both streams + native capture via the provider. |

### Permissions — **platform-dispatched** (`electron/permissions/permissionsManager.ts` → `IPermissionsProvider`)

| Channel | Kind | Notes |
|---|---|---|
| `permissions-check` | invoke | Live mic + screen grant status. Windows provider returns safe defaults today. |
| `permissions-request-mic` | invoke | Prompts / opens settings for mic. |
| `permissions-check-screen` | invoke | Screen-recording grant. **darwin-relevant** (ScreenCaptureKit); Windows has no equivalent gate → returns `true`. |
| `permissions-request-screen` | invoke | Surfaces the macOS Screen Recording prompt. **darwin-only meaning**; Windows no-op. |
| `permissions-open-screen-settings` | invoke | Opens macOS Screen Recording privacy pane. **darwin-only meaning**; Windows no-op. |
| `permissions-complete` | invoke | Writes onboarding-complete flag + relaunch. Universal mechanism, provider owns the flag path. |
| `permissions-get-flag` | invoke | Reads the onboarding-complete flag. |

> **win32-only (future):** Windows 10/11 has a per-app microphone privacy gate
> (Settings → Privacy → Microphone) with no macOS analog. When implemented it
> belongs in `WindowsPermissionsProvider.requestMic()/checkGranted()`, behind
> the existing interface — no new channel, no handler change.

### Auth — **universal** (`electron/auth/`)

| Channel | Kind |
|---|---|
| `auth:sign-in` | invoke |
| `auth:verify-mfa` | invoke |
| `auth:sign-out` | invoke |
| `auth:get-token` | invoke |
| `auth:force-refresh-token` | invoke |
| `auth:get-state` | invoke |
| `get-user-auth` | send |
| `update-user-auth` | send |

> `auth:sign-out` triggers `audioManager.cleanupAllAudioCapture()`; the cleanup
> itself is platform-dispatched, but the auth handler is not.

### Network — **universal**

| Channel | Kind |
|---|---|
| `network:get-state` | invoke |
| `network:report-status` | send |

### Windows / UI / navigation — **universal**

| Channel | Kind | Notes |
|---|---|---|
| `open-external` | send | `shell.openExternal` — cross-platform. |
| `open-coach-window` | send | Window creation is universal; the mic pre-flight gate is routed through the permissions provider (platform-dispatched). |
| `close-coach-window` | send | |
| `get-coach-window-state` | send | |
| `get-coach-window-open-state` | invoke | |
| `resize-coach-window` | send | |
| `get-coach-work-area-bottom` | invoke | |
| `get-window-position` / `set-window-position` | invoke / send | |
| `open-app-settings-window` | send | |
| `close-app-settings-window` | send | |
| `app-settings:session-expired-redirect` | send | |
| `get-app-settings-window-state` | send | |
| `get-app-settings-window-open-state` | invoke | |
| `app-settings:open-update-tab` | send | |
| `open-onboarding-window` | send | |
| `close-onboarding-window` | send | |
| `complete-onboarding` | send | |
| `onboarding:set-status` | send | Records `'complete'`/`'dismissed'`. Main owns the API write so it survives the window closing. |
| `splash-login-success` | send | |
| `open-playbook-window` | send | |
| `close-playbook-window` | send | |
| `get-playbook-window-state` | send | |
| `get-playbook-window-position` / `set-playbook-window-position` | invoke / send | |
| `get-playbooks-cache` / `set-playbooks-cache` | invoke / send | |
| `get-enabled-features` | send | |
| `set-font-size` | send | |
| `set-tray-menu-height` | send | |
| `tray-show-window` | send | |
| `tray-logout` | send | |
| `quit-app` | send | |

### Updates — **universal** (Electron autoUpdater abstracts the platform)

| Channel | Kind | Notes |
|---|---|---|
| `update:get-state` | invoke | |
| `update:start-download` | send | |
| `update:dismiss` | send | |
| `update:check-for-updates` | send | |
| `app:get-version` | invoke | |

> The autoUpdater **channels** are universal, but the underlying updater
> **feed** differs per OS (Squirrel.Mac vs. Squirrel.Windows / NSIS) and the
> ShipIt-watchdog install step is darwin-only — see "Non-IPC platform code".

### Launch at login — **universal**

| Channel | Kind | Notes |
|---|---|---|
| `get-launch-at-login` | invoke | `app.getLoginItemSettings()` — Electron maps macOS login items ↔ Windows registry run key. |
| `set-launch-at-login` | invoke | `openAsHidden` is a macOS-only sub-option; harmless on Windows. |

---

## Outbound channels (main → renderer)

All outbound events carry platform-agnostic payloads — **universal**. Many are
fan-out via the `broadcastToAllWindows(channel, payload)` /
`sendToOnboardingWindow(channel)` helpers rather than a literal `.send()` at the
call site.

**Auth (broadcast to all windows)**

| Channel | Purpose |
|---|---|
| `user-auth` | Auth-state push (legacy per-window). |
| `auth:state` | Auth state (user / isAuthenticated / accessToken). |
| `auth:token-refreshed` | Access token was refreshed (signal only). |
| `auth-tokens-refreshed` | Refreshed tokens payload (backward-compat for unmigrated windows). |
| `auth:session-expired` | Session expired. |
| `auth-session-expired` | Session expired (backward-compat alias). |

**Cue**

| Channel | Purpose |
|---|---|
| `cue-status` | Per-stream connected/disconnected. |
| `cue-insight` | Coaching insight. |
| `cue-smart-capture` | Smart-capture items. |
| `cue-error` | Stream error. |
| `cue-auto-stop` | Server-driven auto-stop. |
| `cue-low-user-audio` | Mic-silence warning. |
| `cue-mic-recovery-failed` | Mid-session mic route recovery exhausted its backoff (SAYSO-353) — mic audio is permanently lost for the rest of the session, user must Reset. |

**Network / update / windows / features**

| Channel | Purpose |
|---|---|
| `network:state-changed` | Online/offline/reconnecting push. |
| `update:state-changed` | Update lifecycle state (the current update channel). |
| `update-check-complete` | Manual update-check finished. |
| `coach-window-state` / `app-settings-window-state` / `playbook-window-state` | Window open/closed pushes. |
| `enabled-features-changed` | Feature-flag push. |
| `font-size-changed` | Font-size preference push. |
| `playbooks-updated` | Playbooks cache changed. |
| `reset-to-home` | Route the renderer back to home. |
| `splash:show-reason` | Splash routing reason. |
| `app-settings:navigate-to-update` | Focus the Update tab in app settings. |

**Onboarding (`sendToOnboardingWindow`)**

| Channel | Purpose |
|---|---|
| `onboarding:coach-opened` | Coach window opened. |
| `onboarding:session-started` | Cue session started. |
| `onboarding:session-stopped` | Cue session stopped. |
| `onboarding:tray-clicked` | Tray icon clicked during onboarding. |

> `update-available`, `download-progress`, `update-downloaded` are **not** IPC
> channels — they are electron-updater's own event names, consumed by main in
> `setupAutoUpdater()` and folded into the `update:state-changed` broadcast.
> Don't add `ipcRenderer.on` subscriptions for them; renderers read update
> state through `update:get-state` / `update:state-changed`.

---

## Non-IPC platform code (flagged for the Windows port)

These are not IPC handlers but branch on the platform inline. They already use
`process.platform`/`utils/platform.ts` guards; listed here so the Windows
engineer knows where the OS seams are outside the provider layer.

| Location (`electron/main.ts`) | Platform | What |
|---|---|---|
| `browser-window-focus` → `app.setBadgeCount(0)` (~1279) | darwin-only | Dock badge reset. No Windows equivalent. |
| `window-all-closed` (~1413) | darwin-only quit rule | macOS keeps app alive with no windows; other platforms quit. |
| Tray-menu positioning (~782–803) | per-platform | Explicit `darwin` / `win32` / `linux` branches for tray/taskbar placement. |
| `ALLOW_VIBRANCY` (window vibrancy, ~679–709) | darwin (Apple Silicon) only | NSVisualEffectView blur; `undefined` elsewhere. |
| ShipIt watchdog on update install (~388–431) | darwin-only | `launchctl kickstart` workaround for pended Squirrel.Mac ShipIt jobs. Windows uses its own updater flow. |
| `x-apple.systempreferences:` deep links | darwin-only | Opening privacy panes — lives inside `MacPermissionsProvider`. |

---

## Orphaned bridges (no main handler)

None. `upload-file`, `upload-both-files`, `demo-insight` and the legacy
`autoUpdater.on*` subscriptions were removed in the SAYSO-354 cleanup. Every
channel exposed in `preload.ts` now has a main-side counterpart — keep it that
way, and record new channels in the tables above.
