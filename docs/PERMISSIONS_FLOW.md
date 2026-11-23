# Permissions Flow Documentation

## Overview

Sayso AI Coach requires **microphone** and, on macOS, **screen recording** permissions to function. Permissions are handled during the **first-run onboarding flow** (in the splash window, after login), not lazily at coach-open time. On macOS a flag is persisted once granted so the permissions screen is skipped on subsequent launches; Windows reads the live mic status instead (see the Windows section below).

A runtime guard additionally blocks a Cue session from starting if permissions are missing and re-surfaces the permissions screen.

Most of this document describes macOS, which has the more involved model. The Windows differences are collected in one section at the end.

---

## Required Permissions

### 1. Microphone Access
- **Why**: Capture the user's voice during calls
- **macOS Setting**: System Settings → Privacy & Security → Microphone
- **Status check**: Reliable via `systemPreferences.getMediaAccessStatus('microphone')`
- **Granting**: Can be granted **inline** (no restart) via `systemPreferences.askForMediaAccess('microphone')` when status is `not-determined`

### 2. Screen & System Audio Recording (ScreenCaptureKit / SCK)
- **Why**: Capture the prospect's voice (system audio) during calls
- **macOS Setting**: System Settings → Privacy & Security → Screen & System Audio Recording
- **Status check (read)**: `CGPreflightScreenCaptureAccess()` — **non-prompting**, accurate for the running process
- **Granting**: Cannot be granted inline. macOS binds screen-recording permission **at process launch** — a mid-session grant is **never** reflected to the running process until it restarts. So after the user enables it, the app must be quit and reopened.

> macOS 14.4+ is moving to Core Audio taps, which use a separate System Audio Recording permission instead of Screen Recording. Expected behavior per case and version: `docs/MACOS_AUDIO_CAPTURE_PERMISSIONS.md`.

> Historical note: SCK detection used to always read `false`. The root cause was the native-audio JS wrapper (`electron/native-audio/index.js`) never exposing the check — not an unreliable API. CGPreflight is reliable once exposed.

---

## Architecture

### Platform-dispatched layer (`electron/permissions/`)
All permission logic goes through `IPermissionsProvider`, dispatched by platform in `electron/permissions/index.ts` (`MacPermissionsProvider` / `WindowsPermissionsProvider`). `permissionsManager.ts` owns the `permissions-*` IPC handlers and the three helpers main uses; handlers never branch on `process.platform`.

- `checkOSPermissionsGranted()` → `provider.checkGranted()` — returns `{ granted, mic, screen }` (live read, non-prompting). Used by the `start-cue` guard.
- `isMicGranted()` → `provider.checkMic()` — mic only. Used by the coach-window gate.
- `isPermissionsComplete()` → `provider.isComplete()` — macOS: `flag && mic && screen`; Windows: live mic only. Used for startup routing.
- `provider.requirements` — `{ screen, relaunchOnComplete, systemAudio }`. Windows `{ false, false, 'none' }`. macOS follows the system-audio backend: ScreenCaptureKit `{ true, true, 'screen-recording' }`, Core Audio tap (14.4+) `{ true, false, 'audio-capture' }`, where `screen` means "System Audio Recording" and no relaunch is needed. Returned by `permissions-check` so the permissions screen can lay itself out from it rather than sniffing the platform.

### The completion flag
- `permissions-complete` file in `{userData}/`, written by the `permissions-complete` IPC when the user finishes the permissions step ("Quit and Reopen").
- `isPermissionsComplete()` requires the flag **and** live mic **and** live screen (CGPreflight). So an optimistically-written flag without a real SCK grant still re-routes the user to the permissions screen on next launch.
- Cleared only by the team-id TCC migration (`electron/utils/permissionsMigration.ts`) when the Apple signing team changes.

### Detection vs. prompting
- **Reads** (mount, poll, guards) use non-prompting `CGPreflightScreenCaptureAccess()` — never shows a dialog.
- **Prompting** happens only on explicit user action: "Open System Settings" calls `CGRequestScreenCaptureAccess()` (the native macOS dialog, which has its own "Open System Settings" button). On a second click it opens the Screen Recording pane directly (covers the previously-denied case where macOS won't re-show the dialog).

---

## First-run flow (splash window)

```
Cold start
    ↓
PostAuthRedirect (default route "/")
    ↓
 ├─ not authenticated → /login → (MFA if required) → "/"
 └─ authenticated
        ↓
     isPermissionsComplete()?  (permissions-get-flag IPC)
        ├─ true  → splash-login-success (close splash; open onboarding if status incomplete)
        └─ false → /permissions
                       ↓
                  Mic step: "Allow Microphone" → inline dialog (askForMediaAccess)
                       ↓
                  Screen step: "Open System Settings" → native SCK dialog → user enables in Settings
                       ↓
                  "Quit and Reopen"  (shown once mic granted + Settings opened)
                       ↓
                  permissions-complete flag written → app relaunches
                       ↓
                  Fresh launch: CGPreflight reads SCK as granted → permissions complete → onboarding/tray
```

Routing detail: after a successful login or MFA the renderer navigates to `/` (not `/permissions`) so `PostAuthRedirect` decides via the flag — a returning user with permissions already complete skips the screen.

### Why "Quit and Reopen" is offered optimistically
Because a mid-session SCK grant can't be detected live, the permissions screen shows "Quit and Reopen" once **mic is granted and the user has opened System Settings** — it doesn't wait for a (impossible) live SCK confirmation. The next launch is the source of truth; if SCK wasn't actually granted, `isPermissionsComplete()` returns false and the user lands back on `/permissions`.

The diagram above is the ScreenCaptureKit backend: macOS 13 to 14.3, or 14.4+ with the `force_sck_audio` company feature. It is unchanged.

### macOS 14.4+ (Core Audio tap backend)

System audio comes from the "System Audio Recording Only" permission (`kTCCServiceAudioCapture`) instead of Screen Recording. Its grant applies live, so there is no relaunch and no "Quit and Reopen". Full policy: `docs/MACOS_AUDIO_CAPTURE_PERMISSIONS.md`.

**New user** (no Screen Recording grant). Title "Grant permissions", Microphone and System Audio rows.

```
"Allow Microphone" → askForMediaAccess        → Microphone icon turns green (check, pop)
"Allow System Audio" → requestSystemAudioPermission → macOS prompt
    ├─ Allow → System Audio icon turns green
    │          → permissions-complete written immediately (no relaunch)
    │          → screen fades out, success view: drawn check, "Permissions granted /
    │            You can start using Sayso now", "Launch Coach"
    │          → Launch Coach: open-coach-window + splash-login-success
    │            (closes the splash, opens the onboarding tour if it's due)
    └─ Don't Allow → "Open System Settings" (System Audio pane); the screen polls and
                     turns green on grant. No fallback: a new user needs the grant.
```

**Existing user upgrading** (Screen Recording already granted, System Audio not yet). `permissions-check` returns `upgrade: true`. Title "We've improved audio capture for more reliable prompting", only the System Audio row (with a "New" tag; the mic row shows only if the mic isn't granted), CTA pinned to the bottom.

```
"Allow System Audio" → macOS prompt
    ├─ Allow → same sequence as above; success view says "Permissions have been updated /
    │          You can close this window now", no button
    └─ Don't Allow → hint "You can turn this on anytime in System Settings. You can close
                     this window now." No button, no settings pane.
                     Calls keep running on ScreenCaptureKit (S-3), and isComplete() treats
                     denied + Screen Recording granted as complete, so the screen doesn't
                     come back on the next launch.
```

Grant animations and the success view run only on the tap backend (they need a live grant). On ScreenCaptureKit and Windows a newly granted Microphone row still gets the green pop; everything else is as before.

---

## Runtime enforcement (Cue session)

`start-cue` (`electron/main.ts`) calls `checkOSPermissionsGranted()` before starting. If not granted:
- Returns `{ success: false, error: 'permissions_denied', mic, screen }`
- Calls `createSplashWindow()` → `PostAuthRedirect` → `/permissions`

The renderer (`src/store/coachWindowStore.ts`, via the `CUE_PERMISSIONS_DENIED` code in `src/utils/errorReporting.ts`) treats the failed result as a thrown error — it logs to console and does **not** show a toast (silent fail by design); the surfaced permissions window is the user-facing signal.

---

## Team-id (certificate) migration

`permissionsMigration.ts` runs at startup before the permission checks. It reads the running app's **actual** team id from its code signature at runtime (`codesign -dvvv` → `TeamIdentifier=`) and compares it to the team id stored from the last launch — there is no hardcoded constant to keep in sync. On Apple-team change (same bundle id, new certificate):
1. `tccutil reset ScreenCapture/Microphone <bundleId>` clears stale old-team TCC entries
2. Deletes the `permissions-complete` flag → user re-runs the permissions flow under the new team
3. Stores the new team id so it only resets once

If the running team id can't be read (unsigned / ad-hoc / dev build), the migration is a safe no-op.

This composes correctly with the permission logic: CGPreflight is scoped to the running binary's signature (new team), and flag deletion forces a clean re-grant.

---

## Testing caveat (dev mode)

Screen-recording (and mic) permission is subject to macOS **responsible-process attribution** — under `npm run dev` the permission is attributed to Electron / the parent (Cursor, Terminal), not to the packaged "Sayso" app. So permission flows **cannot be tested from dev mode**; use the packaged app (`npm run package:staging`).

The tap backend's System Audio permission is attributed the same way, but since its grant applies live the screen itself can be exercised in dev: `tccutil reset AudioCapture com.apple.Terminal` (and `Microphone`) brings the prompts back, and deleting `~/Library/Application Support/sayso-app-dev/permissions-complete` routes back to `/permissions`. Which screen you get depends on Terminal's own Screen Recording grant: granted → the upgrade screen, not granted → the new-user screen.

This is macOS-only. Windows has no per-process attribution — the mic switches apply to every desktop app at once — so the Windows flow *can* be exercised from `npm run dev`.

---

## Windows

`WindowsPermissionsProvider` is deliberately much smaller than the macOS one, because the OS model is:

### Mic is the only gate
- Settings → Privacy & security → Microphone has two switches: **Microphone access** (all apps) and **Let desktop apps access your microphone** (all desktop apps as one group). Both are on by default.
- There is **no per-app switch** for a desktop app and **no consent dialog**. `systemPreferences.askForMediaAccess` is macOS-only. The only thing the app can do is open the Settings page.
- **System audio needs no permission.** The WASAPI native module captures the lead's audio via loopback on the default output device (`AUDCLNT_STREAMFLAGS_LOOPBACK`). No OS gate, no prompt, no restart. `screen` is always `true` on Windows.

### Status read
`systemPreferences.getMediaAccessStatus('microphone')` works on Windows and, for a non-packaged app, reflects both switches:

| Status | Treated as |
|---|---|
| `denied`, `restricted` | **blocked** |
| `granted`, `not-determined`, `unknown` | granted |
| read throws / API missing | granted (logged) |

Only an explicit denial blocks. Windows has no dialog to resolve an unclear status, so blocking on one would leave the user on a screen with nothing to do.

### No flag file, no relaunch
- `isComplete()` is the live mic status. There is no `permissions-complete` file on Windows: a user with the mic granted never sees `/permissions`, and a user who flips the switch off is routed there on the next check. `markComplete()` is a no-op.
- The macOS relaunch exists only because macOS picks up the Screen Recording grant at launch. Windows reflects the switches live, so `requirements.relaunchOnComplete` is `false` and the `permissions-complete` handler returns without calling `app.relaunch()`; the permissions screen navigates to `/` itself once the invoke resolves.

### Requesting
`requestMic()`: already granted → `{ mic: true, action: 'already-granted' }`. Otherwise it opens `ms-settings:privacy-microphone` via `shell.openExternal` and returns `{ mic: false, action: 'open-settings' }`. `requestScreen()` / `openScreenSettings()` are no-ops.

### Routing needs nothing new
The three macOS entry points to `/permissions` all go through the provider, so they work on Windows unchanged: startup (`isPermissionsComplete()` in `main.ts` + `PostAuthRedirect` → `permissions-get-flag`), Cue start (`start-cue` → `checkOSPermissionsGranted()`), and opening the coach window (`isMicGranted()`).

### First-run flow
The screen lays itself out from `requirements`, so on Windows it renders one row, one hint and no relaunch:

```
/permissions  (mic-only layout — requirements.screen === false)
    ↓ "Open Microphone Settings" → ms-settings:privacy-microphone   (clicking again reopens it)
    ↓ polls permissions-check every 1.5s → the Microphone row turns green on its own
    ↓ "Continue" → permissions-complete (no flag, no relaunch) → navigate('/')
    ↓ PostAuthRedirect → permissions-get-flag → splash-login-success
    ↓ main awaits the session load it started at boot, closes the splash, opens onboarding if pending
```

The poll is what replaces the macOS relaunch. Windows has no dialog and no per-app switch, so the only way the row can turn green is the screen noticing the switch flip itself. It is gated on `!requirements.relaunchOnComplete` (macOS keeps its click-again behaviour), runs from mount rather than from the first click, and clears both on grant and on unmount.

`loadBootSession()` in `main.ts` runs on **both** authenticated boot branches. The permissions-splash branch starts it un-awaited and parks the promise in `authUserReady`, so the splash still appears immediately while the profile fetch and the cache reconcile run behind it. `splash-login-success` awaits that same promise before deciding on onboarding.

Starting it at boot rather than on Continue is deliberate: the splash is not the only way out of `/permissions`. A user can close it with the title-bar X, fix the microphone in Settings themselves, and open Cue straight from the tray — `open-coach-window` only gates on `isMicGranted()`, so it never routes back through the splash. Tying the load to Continue left that session with no playbooks, no font size and no profile until an offline→online flap. `replayPendingOnboardingStatus()` is deliberately **not** inside the helper, because each path must replay exactly once.

### Testing
Flip either switch off in Settings → Privacy & security → Microphone and launch: the splash routes to `/permissions`, `start-cue` returns `permissions_denied`, and opening the coach window re-surfaces the splash instead. Flip it back on and relaunch: straight to the tray. Works from `npm run dev`.

---

## Summary
- **Onboarding-time setup**, not lazy at coach-open: handled in the splash `/permissions` screen after login
- **Platform-dispatched** via `IPermissionsProvider`: `checkOSPermissionsGranted` / `isMicGranted` / `isPermissionsComplete`; `requirements` tells the renderer what the OS gates
- **Non-prompting reads** (CGPreflight); native dialog only on explicit user action
- **Persisted flag** = `flag && mic && screen`; cleared by the team-id migration
- **Screen recording binds at launch** → grant requires app restart; "Quit and Reopen" offered optimistically
- **Runtime guard** in `start-cue` blocks the session and re-opens the permissions screen
- **Dev mode can't test permissions on macOS** — package and test the real app
- **Windows**: mic is the only gate (two global switches, no dialog), status read live, no flag, no relaunch; `requestMic` opens `ms-settings:privacy-microphone`; the screen renders mic-only from `requirements`, polls the live status, and continues through `/`
