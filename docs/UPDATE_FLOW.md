# Update Flow — Design Decisions

_Feature: SAYSO-91 — Apply changes to version update flow_  
_Branch: `feature/sayso-91-apply-changes-to-version-update-flow`_

---

## Overview

When Sayso detects a new version, it shows a "New Update Available" prompt in the splash window. The user can choose to update immediately or dismiss and update later via the tray menu or App Settings.

---

## Key Decisions

### 1. `autoDownload = false`
Previously the updater downloaded in the background automatically. Now it only starts downloading when the user explicitly clicks "Update Now" — giving them control over timing.

### 2. Splash window as the "interruption" surface
We reuse the existing splash window (not a new dedicated window) because:
- It was already wired to the update flow via `UpdateGate`
- Its dimensions match the design mockup exactly (380×560)
- It already follows a phase-based pattern (login → mfa → permissions → updating)
- A new `BrowserWindow` would add unnecessary overhead (preload, HTML entry, Vite input, lifecycle)

### 3. App Settings "Software Update" tab as the "self-service" surface
The tray "Update Available" entry opens App Settings → Software Update tab (not the splash window). This separates two distinct UX moments:
- **Splash**: the first interruption ("hey, there's an update")
- **Settings**: ongoing management ("I'll handle this when I'm ready")

### 4. "Not Now" keeps state as `available`
Dismissing the splash doesn't reset the update state to `idle`. The tray "Update Available" entry persists so the user can always re-trigger the update. State only transitions away from `available` when download starts or the app restarts.

### 5. "Not Now" navigation logic
- If the user was **unauthenticated** when the update prompt appeared → returns to `/login`
- If the user was **authenticated** → closes the splash (nothing to go back to)

### 6. Mid-call deferral
If `cueAudioStreamer !== null` (i.e. a coach session is active) when an update is detected:
- The splash is **not shown** — we don't interrupt an active call
- The tray "Update Available" entry appears immediately (user can act if they want)
- Main schedules a 10-minute recheck loop; once the session ends, the splash opens automatically.

### 7. Auto-restart on download complete
The "Restart Now / Later" prompt has been removed. When `update-downloaded` fires, the app calls `quitAndInstall()` immediately. No user action required.

### 8. Coach window closed on "Update Now"
When the user clicks "Update Now" (from splash or Settings tab), main closes the coach window before starting the download. App Settings window is **not** closed — it's the surface showing download progress.

### 9. Tray menu during update
When `phase === 'downloading'` or `'downloaded'`:
- **Launch Coach**: disabled
- **Settings**: disabled
- A non-interactive "Update in progress…" entry appears

When `phase === 'available'`:
- **Update Available** entry appears (clickable → opens Settings → Software Update tab)
- All other items remain enabled

### 10. macOS: the app must live in /Applications (SAYSO-356)

Squirrel cannot write to a bundle running from a read-only volume, which happens two ways on macOS: launching straight out of the mounted `.dmg`, or **App Translocation** (the bundle was opened from `~/Downloads` without being moved into Applications via Finder, so Gatekeeper runs a quarantined copy from a randomized read-only mount under `/private/var/folders/…/AppTranslocation/…`). Either way the install is permanently stuck, and translocation additionally binds screen-recording permission to a path that vanishes when the user moves the app.

Two layers:

**Launch guard** — `enforceApplicationsFolderLocation()` (`electron/utils/applicationsFolder.ts`), called in `app.whenReady()` immediately after the single-instance-lock check and **before any window is created**. On `app.isPackaged && IS_MAC && !IS_STAGING`, an `app.isInApplicationsFolder()` miss shows a native `dialog.showMessageBoxSync` with **Move to Applications** / **Quit** and no continue-anyway path. A native dialog rather than a window: this runs before any renderer exists, and reusing the splash would kick off an auth refresh for a user who is about to be blocked. On `moveToApplicationsFolder()` throwing (the usual outcome from a translocated or DMG path) a second dialog gives manual Finder instructions plus **Show in Finder**, then quits. A `false` return means the user cancelled macOS's authorization prompt → quit. Staging is excluded so staging builds stay runnable from wherever they were unzipped.

**Update gate** — `isUpdateBlockedByLocation()` in `main.ts` fronts both entry points (the startup check and the `update:check-for-updates` IPC handler) with the same `isInApplicationsFolder()` probe, and sets `phase: 'blocked'` instead of letting the call reach Squirrel. When blocked at startup, neither the immediate check nor the hourly interval is armed. This layer applies to staging too — the launch guard skips staging, so this is what staging testers actually see. It is defence in depth for production: with the guard shipped, no production user should reach it.

If a read-only failure ever does reach `electron-updater`, `classifyUpdaterError()` classifies it as `'read-only-volume'` — non-fatal, so it is not reported to Sentry, and `updaterErrorMessage()` returns the same actionable copy instead of Squirrel's raw text and GitHub link.

### 11. Single source of truth in main process
`updateState` in `main.ts` is the only authoritative update state. All UI surfaces (splash `UpdateGate`, tray `TrayMenuApp`, App Settings `SoftwareUpdateSettings`) subscribe via `update:state-changed` IPC and hydrate via `update:get-state` on mount.

---

## IPC Contract

| Channel | Direction | Payload | Purpose |
|---|---|---|---|
| `update:state-changed` | main → renderer (broadcast) | `UpdateState` | All subscribers re-render |
| `update:get-state` | renderer → main (invoke) | — | Mount-time hydration |
| `update:start-download` | renderer → main | — | User clicked "Update Now" |
| `update:dismiss` | renderer → main | — | User clicked "Not Now" |
| `update:check-for-updates` | renderer → main | — | Manual re-check (Settings tab) |
| `app:get-version` | renderer → main (invoke) | — | Current version string |
| `app-settings:open-update-tab` | renderer → main | — | Tray entry → open Settings on update tab |
| `app-settings:navigate-to-update` | main → settings renderer | — | Switch to Software Update tab when window already open |
| `update-check-complete` | main → splash | — | No update found; stop loader |

---

## Update State Machine

```
idle ──update-available──▶ available ──user clicks Update Now──▶ downloading ──▶ downloaded (auto-quit)
  │                             │                                       │
  │                             │ user clicks Not Now                   │ error
  │                             ▼                                       ▼
  │                        (stays available)                         error
  │
  └──app outside /Applications (macOS)──▶ blocked  (terminal — no check is attempted,
                                                    no retry offered; fixed by moving the app)
```

---

## File Map

| File | Role |
|---|---|
| `electron/main.ts` | State machine, autoUpdater wiring, IPC handlers, deferral logic, location gate |
| `electron/utils/applicationsFolder.ts` | macOS launch guard + `isOutsideApplicationsFolder()` probe |
| `electron/utils/transientErrors.ts` | Updater error classification and user-facing copy |
| `electron/preload.ts` | Exposes `update.*`, `app.*`, `appSettings.*` to renderers |
| `src/types/update.ts` | Shared `UpdateState` / `UpdatePhase` types |
| `src/splashWindow/UpdateGate.tsx` | Intercepts splash routes when update is pending |
| `src/splashWindow/routes/Updater/` | "New Update Available" UI (initial prompt) |
| `src/splashWindow/routes/Updating/` | Download progress UI |
| `src/appSettingsWindow/components/SoftwareUpdateSettings.tsx` | Settings tab — full update lifecycle UI |
| `src/trayMenu/TrayMenuApp.tsx` | Update row, disable states during download |
| `src/splashWindow/styles/Colors.css` | Added `--sayso-silent-blue` |
| `client/docs/UPDATE_FLOW.md` | This document |
