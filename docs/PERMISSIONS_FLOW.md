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

> Historical note: SCK detection used to always read `false`. The root cause was the native-audio JS wrapper (`electron/native-audio/index.js`) never exposing the check — not an unreliable API. CGPreflight is reliable once exposed.

---

## Architecture

### Platform-dispatched layer (`electron/permissions/`)
All permission logic goes through `IPermissionsProvider`, dispatched by platform in `electron/permissions/index.ts` (`MacPermissionsProvider` / `WindowsPermissionsProvider`). `permissionsManager.ts` owns the `permissions-*` IPC handlers and the three helpers main uses; handlers never branch on `process.platform`.

- `checkOSPermissionsGranted()` → `provider.checkGranted()` — returns `{ granted, mic, screen }` (live read, non-prompting). Used by the `start-cue` guard.
- `isMicGranted()` → `provider.checkMic()` — mic only. Used by the coach-window gate.
- `isPermissionsComplete()` → `provider.isComplete()` — macOS: `flag && mic && screen`; Windows: live mic only. Used for startup routing.
- `provider.requirements` — `{ screen, relaunchOnComplete }`, static per platform (macOS `{ true, true }`, Windows `{ false, false }`). Returned by `permissions-check` so the permissions screen can lay itself out from it rather than sniffing the platform.

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
    ↓ main runs the session load it deferred at whenReady, closes the splash, opens onboarding if pending
```

The poll is what replaces the macOS relaunch. Windows has no dialog and no per-app switch, so the only way the row can turn green is the screen noticing the switch flip itself. It is gated on `!requirements.relaunchOnComplete` (macOS keeps its click-again behaviour), runs from mount rather than from the first click, and clears both on grant and on unmount.

The deferred session load matters because a boot that lands on `/permissions` skips the profile fetch and cache reconcile in `whenReady` — `loadBootSession()` in `main.ts`. macOS re-runs it on the relaunch; Windows has no relaunch, so `splash-login-success` drains the `bootSessionLoadDeferred` flag and runs it there instead, and the tray comes back with playbooks and the right font size.

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
