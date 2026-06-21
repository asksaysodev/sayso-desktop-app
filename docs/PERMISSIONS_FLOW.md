# Permissions Flow Documentation

## Overview

Sayso AI Coach requires **microphone** and **screen recording** permissions to function. Permissions are handled during the **first-run onboarding flow** (in the splash window, after login), not lazily at coach-open time. Once granted, a flag is persisted so the permissions screen is skipped on subsequent launches.

A runtime guard additionally blocks a Cue session from starting if permissions are missing and re-surfaces the permissions screen.

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

### Platform-agnostic layer (`electron/main.ts`)
All permission logic dispatches by platform so non-macOS builds (Windows, later) drop in cleanly:

- `checkOSPermissionsGranted()` → `checkMacOSPermissions()` — returns `{ granted, mic, screen }` (live read, non-prompting). Non-darwin returns all-granted.
- `isPermissionsComplete()` → `isMacOSPermissionsComplete()` — returns `flag && mic && screen`. Non-darwin returns `true`.

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

The renderer (`useCue.tsx`) treats the failed result as a thrown error — it logs to console and does **not** show a toast (silent fail by design); the surfaced permissions window is the user-facing signal.

---

## Team-id (certificate) migration

`permissionsMigration.ts` runs at startup before the permission checks. On Apple-team change (same bundle id, new certificate):
1. `tccutil reset ScreenCapture/Microphone <bundleId>` clears stale old-team TCC entries
2. Deletes the `permissions-complete` flag → user re-runs the permissions flow under the new team
3. Stores the new team id so it only resets once

This composes correctly with the permission logic: CGPreflight is scoped to the running binary's signature (new team), and flag deletion forces a clean re-grant.

---

## Testing caveat (dev mode)

Screen-recording (and mic) permission is subject to macOS **responsible-process attribution** — under `npm run dev` the permission is attributed to Electron / the parent (Cursor, Terminal), not to the packaged "Sayso" app. So permission flows **cannot be tested from dev mode**; use the packaged app (`npm run package:staging`).

---

## Summary
- **Onboarding-time setup**, not lazy at coach-open: handled in the splash `/permissions` screen after login
- **Platform-agnostic dispatchers**: `checkOSPermissionsGranted` / `isPermissionsComplete`
- **Non-prompting reads** (CGPreflight); native dialog only on explicit user action
- **Persisted flag** = `flag && mic && screen`; cleared by the team-id migration
- **Screen recording binds at launch** → grant requires app restart; "Quit and Reopen" offered optimistically
- **Runtime guard** in `start-cue` blocks the session and re-opens the permissions screen
- **Dev mode can't test permissions** — package and test the real app
