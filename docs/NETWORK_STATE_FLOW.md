# Network State & Auth Flow

> **Audience:** anyone touching auth, the tray menu, or anything that should be disabled while offline.
> **Scope:** how the app detects online/offline and how `AuthManager` reacts.
> **Last updated:** 2026-05-22 (SAYSO-168).

---

## TL;DR

- The OS is the source of truth for "are we online." The renderer listens to `window.addEventListener('online'/'offline')` and IPCs the state to main.
- Main mirrors it in `global.networkState: 'online' | 'reconnecting'` and broadcasts `network:state-changed` to all windows.
- While offline, `AuthManager` is **paused**: both the proactive expiry timer and the backoff retry timer are cleared, and `_scheduleNetworkRetry()` refuses to set new timers.
- The OS `online` event lifts the pause and triggers exactly one refresh attempt. There's no polling, no blind backoff.

---

## Why this exists (context)

The original auth-refresh code, added in PR #149, kept retrying every 60 s for as long as the app was open. If a user opened the app on Friday evening with no internet and didn't touch it until Monday, that was ~4,300 failed refresh attempts and Sentry events.

A few specific concerns surfaced during review that shaped the final design:

1. **"Are we going to log the user out?"** No. `_handleExpired()` only fires on a definitive Supabase `invalid_grant`. Network errors keep the session — that part was already correct in PR #149.
2. **"Are we going to spam Sentry over a weekend?"** Yes, with the original design. Fixed here by pausing.
3. **"`net.isOnline()` returning `true` is unreliable."** Confirmed by Electron docs — `true` means "some link might be up," not "you can reach the internet." We never rely on `true` from a one-shot check; we react to the OS-pushed event instead.
4. **"`powerMonitor` doesn't notice network changes, right?"** Correct. None of its 11 events are network-related. Wake/unlock are useful as *additional* triggers (force a refresh after sleep) but not as the primary signal.
5. **"My Account was disabled while reconnecting."** It just opens a browser URL — the app itself makes no request, so disabling it was wrong. Removed.
6. **"Coach window kept showing the stale 'token expired' error after coming back online."** Fixed: `clearError()` is called when the network state transitions back to online.
7. **"Opening the app while offline showed the login splash."** Fixed: if `init()` fails transiently and we have a stored refresh token, we boot silently into the tray and defer profile/features fetch until the network returns.

---

## The flow

### Signal source

```
Renderer (src/config/axios.ts)
  window.addEventListener('online')  ─► ipc.send('network:report-status', 'online')
  window.addEventListener('offline') ─► ipc.send('network:report-status', 'offline')
  reportNetworkStatus(navigator.onLine)   // initial report on load

Main (electron/main.ts)
  ipcMain.on('network:report-status', ...)
    ├─ updates global.networkState
    ├─ broadcasts 'network:state-changed' to all windows
    └─ pauses or resumes AuthManager
```

### `global.networkState`

Two values only:

| Value           | Meaning                                                |
| --------------- | ------------------------------------------------------ |
| `'online'`      | OS reports connectivity. Features unlocked.            |
| `'reconnecting'`| OS reports offline OR boot-time auth init failed transiently. Tray buttons and IPC actions are gated. |

The state is read at the start of several IPC handlers and shortcut callbacks to gate sensitive actions (open coach window, start update download, etc.). See the `if (global.networkState === 'reconnecting') return;` checks in `main.ts`.

---

## Online → offline

1. Renderer's `offline` event fires → IPC `network:report-status` with `'offline'`.
2. Main flips `global.networkState` to `'reconnecting'`, broadcasts the change.
3. Main calls `authManager.pauseRefresh()`, which:
   - Sets the internal `paused = true` flag.
   - Clears the proactive `refreshTimer` (would have fired ~60 s before token expiry).
   - Clears any pending `networkRetryTimer`.
4. Tray re-renders with buttons disabled. Coach window shows "No internet connection…" if it was open.

From this point on, **`AuthManager` makes no network calls of its own.** If a renderer happens to call `getAccessToken()` while paused, it'll still attempt — that's an explicit user-initiated request, not auto-scheduled work — but the failure won't restart a retry chain because `_scheduleNetworkRetry()` short-circuits on `paused`.

---

## Offline → online

1. Renderer's `online` event fires → IPC `network:report-status` with `'online'`.
2. Main flips `global.networkState` back to `'online'`, broadcasts the change.
3. Main calls `authManager.forceRefresh()`, which:
   - Clears `paused = false`.
   - Runs `_refresh()` once.
4. On success: `token-refreshed` event fires, `_scheduleRefresh()` re-arms the proactive timer.
5. If `forceRefresh()` fails (OS reported `online` prematurely — captive portal, weak Wi-Fi): a normal backoff retry chain starts at 1 s and caps at 60 s. If the OS goes back to `offline`, the chain is cancelled again. Worst case: ~5 attempts in 2 minutes per false-online cycle.

---

## Startup-offline (the case you asked about)

User is already logged in. They open the app with no internet.

1. `app.whenReady()` calls `await authManager.init()`.
2. `init()` reads the stored refresh token, tries to exchange it with Supabase, `fetch()` throws.
3. `init()` keeps the token in memory and calls `_scheduleNetworkRetry()` (1 s timer).
4. Back in `app.whenReady()`, `authState.isAuthenticated` is `false` and `authManager.isNetworkRetryPending()` is `true` — we enter the startup-offline branch:
   - `global.networkState = 'reconnecting'`
   - `startupOfflinePending = true`
   - `authManager.pauseRefresh()` ← **cancels the 1 s timer that `init()` just set**
5. Tray registers, **no splash window opens**.
6. App is fully idle. Zero auth requests, zero timers running.
7. When the OS fires `online`, the standard offline-to-online flow runs. `token-refreshed` fires, and the handler sees `startupOfflinePending === true` → runs the deferred profile / features / font-size fetch that would normally have happened at boot.

> Note the explicit `pauseRefresh()` call in step 4 — without it, the `network:report-status` handler can't help us at boot because `global.networkState` is already `'reconnecting'` by the time the renderer reports `offline`, so the handler short-circuits on the "no state change" guard.

---

## What we deliberately did NOT do

- **No `net.isOnline()` polling in main.** Per Electron docs the `true` return is inconclusive. We rely on the renderer's `online/offline` events (Chromium's `NetworkChangeNotifier`) instead.
- **No "reconnecting…" splash or banner.** Per team lead's call, silent disabling is the chosen UX. Successful silent auth also doesn't show a splash, so showing one for offline-startup would be inconsistent.
- **No reset of `networkRetryCount` on pause/unpause.** If a refresh-after-resume fails transiently, the backoff continues from where it was, which handles flaky-after-outage networks sensibly.

---

## Files & key methods

| File                                             | What it owns                                                                 |
| ------------------------------------------------ | ---------------------------------------------------------------------------- |
| `src/config/axios.ts`                            | `window.addEventListener('online'/'offline')` → IPC to main.                 |
| `electron/main.ts`                               | `ipcMain.on('network:report-status')`, `ipcMain.handle('network:get-state')`, gating in shortcuts and IPC handlers, the startup-offline branch in `app.whenReady()`. |
| `electron/auth/AuthManager.ts`                   | `pauseRefresh()`, `forceRefresh()`, `isNetworkRetryPending()`, the `paused` flag, the retry-backoff logic. |
| `src/trayMenu/TrayMenuApp.tsx`                   | Subscribes to `network:state-changed`, disables Coach / Playbooks / Settings / Update rows while reconnecting. |
| `src/coachWindow/components/CoachWindowMain.tsx` | Overrides error message text while offline, clears stale errors on reconnect. |

| IPC channel                  | Direction       | Purpose                                                     |
| ---------------------------- | --------------- | ----------------------------------------------------------- |
| `network:report-status`      | renderer → main | Renderer reports current online/offline state.              |
| `network:get-state`          | renderer → main | One-shot query for current state (used on window mount).    |
| `network:state-changed`      | main → renderer | Broadcast when state flips.                                 |

---

## When to revisit this

- If we ever ship a Linux build where `navigator.onLine` semantics differ from macOS/Windows. (Chromium normalizes this but edge cases exist.)
- If we want to *show* the user that we're reconnecting (currently silent — disabled buttons only).
- If we add more main-process subsystems (auto-updater retries, audio WebSocket reconnects) that should also pause while offline. The pattern to follow is the same: gate on `global.networkState !== 'reconnecting'`.
- If Electron adds a native main-process network event (unlikely — none exists as of Electron 36).
