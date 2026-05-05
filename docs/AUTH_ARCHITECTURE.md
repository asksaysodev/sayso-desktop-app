# Authentication Architecture

> **Document type:** Hybrid postmortem + architectural reference.
> **Audience:** Anyone who needs to understand how Sayso authenticates users, why the auth system was redesigned, and how to safely change it in the future.
> **Last major rewrite:** May 2026 (this document covers that rewrite end-to-end).

---

## Table of Contents

1. [TL;DR](#tldr)
2. [The Problem](#the-problem)
3. [Historical Context: The Previous Partial Fix (April 2026)](#historical-context-the-previous-partial-fix-april-2026)
4. [Root Cause Analysis](#root-cause-analysis)
5. [The Fix: Architectural Inversion](#the-fix-architectural-inversion)
6. [Components Reference](#components-reference)
7. [IPC Surface](#ipc-surface)
8. [Auth Flows (Detailed)](#auth-flows-detailed)
9. [Files Reference](#files-reference)
10. [Gotchas & Lessons Learned](#gotchas--lessons-learned)
11. [What to Watch For](#what-to-watch-for)

---

## TL;DR

Sayso's auth used to have **two systems competing for ownership of the user's session**: the Supabase JS SDK in each renderer (reading/writing `localStorage` via `cryptoStorage`) and the Electron main process (holding tokens in `global` vars). When a window mounted or reloaded, it would push a stale token from `localStorage` over the fresh token main had just refreshed. Supabase's "detect compromised refresh tokens" protection then revoked the session and the user was logged out — most reproducibly after going AFK for 10–15 minutes during a coach session.

A **partial** fix in April 2026 (commit `c1fdef5`) centralized the *act* of refreshing, but left ownership split. The May 2026 rewrite **inverts the architecture**: the main process is now the single owner of the session via a new `AuthManager` class, and renderers are read-only consumers via IPC. The Supabase JS SDK is no longer involved in auth at all.

---

## The Problem

### Symptoms

- Users were logged out unexpectedly, often after a period of inactivity (~10–15 minutes was the most reproducible window).
- The most concrete reproducer: **start a coach session, go AFK ~10–15 min, return and try to stop/reset → "token expired" error and forced re-login**.
- Sentry: issue **SAYSO-APP-46**, `Token refresh failed: 400`.

### User impact

The product is a real-time sales coaching tool. Mid-call logouts are catastrophic — they kill the audio stream, drop the AI thread, and require the rep to re-authenticate while a prospect is on the line.

### What we wanted instead

The user explicitly asked for **Slack/Discord-level session persistence**: log in once, never log in again unless they explicitly sign out. The Supabase project settings were verified to support this (Time-box = never, Inactivity timeout = never), so permanent sessions were achievable — the failure was on the client side.

---

## Historical Context: The Previous Partial Fix (April 2026)

This is important context because the same problem had been *partially* addressed before.

**Commit:** `c1fdef5` — `fix(auth): centralize token refresh in main process to prevent multi-window race condition`

### What that fix did

Added a `refresh-auth-tokens` IPC handler in main with a queue:

```ts
let isRefreshingTokens = false;
let pendingRefreshResolvers = [];

ipcMain.handle('refresh-auth-tokens', async () => {
  if (isRefreshingTokens) {
    return new Promise(resolve => pendingRefreshResolvers.push(resolve));
  }
  isRefreshingTokens = true;
  try {
    const tokens = await refreshAuthTokens(global.authRefreshToken);
    // broadcast to all windows...
  } finally {
    isRefreshingTokens = false;
    pendingRefreshResolvers = [];
  }
});
```

This solved the case where multiple windows tried to refresh **at the same time**: only one network call would go to Supabase; the others awaited the same in-flight promise.

### Why it was insufficient

It centralized the *operation* of refresh, but **not the ownership of tokens**. The architecture after that fix:

```
Renderer (Supabase SDK + localStorage)  ←→  Main (global.authAccessToken / global.authRefreshToken)
         ↑                                        ↑
    SOURCE OF TRUTH                          CACHED COPIES
```

- The Supabase JS SDK in each renderer still owned the session — persisted to `localStorage` via the `cryptoStorage` adapter.
- Main only held *copies* of the tokens that the renderer pushed to it via `update-auth-tokens`.
- Whenever a renderer's `AuthContext` initialized, it read `localStorage` (which could contain stale tokens) and pushed those to main, overwriting whatever main had.

So while two **simultaneous** refreshes could no longer happen, two **sequential** sources of truth still could — and they did, every time a window mounted or reloaded.

The April fix attacked a symptom. The May rewrite attacks the architecture.

---

## Root Cause Analysis

### The actors

- **Main process** — owns `global.authAccessToken`, `global.authRefreshToken`. Persists nothing of its own (pre-rewrite).
- **Splash window renderer** — runs `AuthProvider` from `AuthContext.tsx`. Owns a Supabase JS SDK instance with `cryptoStorage` persisting to `localStorage`.
- **Coach window renderer** — separate `BrowserWindow`, separate React root, separate Supabase JS SDK instance, but **shares `localStorage`** with the splash window (same origin).
- **Tray menu renderer** — separate again, also shares `localStorage`.
- **Supabase Auth backend** — the source of truth for refresh-token validity, with these relevant features:
  - **Refresh token rotation**: each refresh call invalidates the previous refresh token and issues a new one.
  - **10-second reuse interval**: a refresh token can be used twice within 10s without triggering protection (so a brief retry doesn't kill the session).
  - **Detect compromised refresh tokens**: if the *same* refresh token is used **twice after the reuse interval**, Supabase assumes it was leaked and revokes the entire session.

### The bug, step-by-step

```
T=0s    Main has refresh token R0. User signs in.
        Splash renderer's Supabase SDK persists R0 to localStorage (via cryptoStorage).
        Splash AuthContext sends R0 to main via `update-auth-tokens`.
        global.authRefreshToken = R0. ✅ Aligned.

T=3590s Token expiry approaching. Some 401 fires (or window does something
        that triggers a refresh).
        Main calls Supabase /token?grant_type=refresh_token with R0.
        Supabase returns access token A1 and refresh token R1, invalidates R0.
        global.authRefreshToken = R1. 
        Main broadcasts `auth-tokens-refreshed` { R1 } to all windows.

        BUT — localStorage still has R0 if the splash window didn't process the
        broadcast yet, or if cryptoStorage hadn't been updated yet, or if a
        different window is in the middle of mounting.

T=3591s Splash window navigates / re-mounts the AuthProvider for any reason
        (route change, hot reload, Supabase SDK INITIAL_SESSION event firing).
        AuthProvider reads localStorage → still sees R0.
        AuthProvider calls `update-auth-tokens` with R0.
        global.authRefreshToken = R0 (overwriting R1!)  ❌ Desync.

T=3601s+ Some axios call needs a token. Main triggers refresh with R0.
        Supabase: "I issued R1 11 seconds ago and now you're sending R0 again
        — that's outside the 10s reuse window. This is a compromised token reuse."
        → Revokes the entire session.
        → Returns 400 invalid_grant.
        → User is logged out mid-call.
```

### Why the "AFK for 10 minutes" reproducer works

The 401 → refresh path was the most common trigger, and the JWT lifetime was ~1 hour. But the real driver was the **window-mount-reads-stale-localStorage** pattern. Anything that caused a renderer to re-initialize its `AuthProvider` could re-introduce the old refresh token. Going AFK and coming back exercised this because the OS would suspend timers, the WebSocket would drop, returning would trigger reconnect attempts, those would trigger token reads, and the misalignment would surface.

### Contributing factors (also fixed)

- **`autoRefreshToken: false` on the Supabase client** — meant the SDK never proactively refreshed. Refreshes only happened reactively on 401, increasing the window where stale tokens lived.
- **WebSocket `updateToken()` was never called** — the `AudioStreamer` class had a method to swap tokens on a live WS, but main never called it on refresh. So WS reconnects could fire with stale tokens.
- **`INITIAL_SESSION` event in Supabase JS v2 was not handled** — only `SIGNED_IN` was. This is part of the silent boot path that re-introduced old tokens.

---

## The Fix: Architectural Inversion

```
BEFORE                                           AFTER
─────────────────────────────────────────       ─────────────────────────────────────────
Renderer = source of truth                       Main = source of truth
    (Supabase SDK + localStorage)                    (AuthManager class)
                ↓                                              ↓
Main = cached copy                               Renderer = read-only consumer
    (global vars)                                    (asks via IPC, never writes)
```

The renderer cannot push tokens to main anymore — that channel was deleted.
The Supabase JS SDK in renderers is no longer involved in auth at all (storage operations that used to grab `session.access_token` from the SDK now ask main via IPC).

### The four pillars

1. **`AuthManager`** — a single `EventEmitter`-based class in main that owns all auth state, refresh logic, MFA, and persistence. Lives in `electron/auth/AuthManager.ts`.
2. **Proactive refresh** — `AuthManager` schedules a `setTimeout` to fire **60 seconds before** the JWT expires. Users almost never see a 401 in normal operation.
3. **Single in-flight refresh promise** — `AuthManager` uses a `refreshPromise: Promise<void> | null` field as a mutex. Concurrent callers all `await` the same promise. No queue needed; the language gives this for free.
4. **IPC as the only auth surface** — five handlers (`auth:sign-in`, `auth:get-token`, `auth:get-state`, `auth:sign-out`, `auth:verify-mfa`) are how renderers interact with auth. Nothing else.

---

## Components Reference

### `electron/auth/AuthManager.ts`

The single source of truth for auth in the main process. ~350 lines.

**State (private fields):**
- `accessToken: string | null` — the current JWT.
- `refreshToken: string | null` — the current refresh token (also persisted to disk).
- `expiresAt: number | null` — Unix timestamp, decoded from the JWT's `exp` claim.
- `user: AuthUser | null` — `{ id, email }` decoded from the JWT.
- `refreshTimer: NodeJS.Timeout | null` — proactive refresh timer.
- `refreshPromise: Promise<void> | null` — mutex for concurrent refresh callers.

**Public API:**
- `init(): Promise<void>` — call once at app startup. Loads the encrypted refresh token from disk, exchanges it for a fresh access token, schedules the next proactive refresh. Silent: if there's no stored token or the exchange fails, the user is left signed out.
- `signIn(email, password): Promise<SignInResult>` — POSTs to `/auth/v1/token?grant_type=password`. Checks for verified TOTP factors via `/auth/v1/user`. Returns `{ success: true, mfaRequired: true, mfaFactors }` if MFA is enrolled and the token is at AAL1, otherwise `{ success: true }`.
- `verifyMFA(factorId, code): Promise<{ success, error? }>` — challenge → verify against Supabase. On success, replaces the AAL1 token pair with the new AAL2 pair and persists the new refresh token.
- `signOut(): Promise<void>` — fire-and-forget POST to `/auth/v1/logout`, then clear in-memory state and the on-disk token. Local sign-out does not wait for the network call.
- `getAccessToken(): Promise<string | null>` — the IPC entry point for renderers. Refreshes if the token is within 60s of expiry, otherwise returns the cached token. All concurrent callers share one in-flight refresh.
- `getState(): AuthState` — synchronous snapshot of `{ user, isAuthenticated, accessToken }`.

**Events:**
- `signed-in`, `signed-out`, `token-refreshed`, `session-expired` — main listens to these and broadcasts to all windows + updates global vars + re-points the WebSockets.

**Persistence:**
- The refresh token is the only thing persisted, and it goes through `safeStorage` (OS keychain — Keychain on macOS, DPAPI on Windows, libsecret on Linux). Implemented in `electron/utils/tokenStore.ts`.
- The access token is in-memory only — it expires anyway, no point persisting it.

### `electron/main.ts` — Wiring

Main creates the `AuthManager` singleton at module load (line ~31) and:

1. **Bridges events to broadcasts** — every `AuthManager` event becomes IPC messages to all windows AND updates the legacy `global.authAccessToken` (still read by some non-auth code) AND updates the WebSocket streamers' tokens AND, on `session-expired`, halts WS reconnect loops.

2. **Calls `authManager.init()` on app ready** — replaces the old manual silent-auth block. If `getState().isAuthenticated`, the splash window is skipped and the tray menu is built immediately. Otherwise the splash opens for sign-in.

3. **Exposes the IPC surface** — five `ipcMain.handle()` registrations that simply delegate to `authManager` methods.

### `src/context/AuthContext.tsx`

Pure consumer. Does **not** import the Supabase JS SDK at all (except for the `Factor` *type*).

- **Bootstrap (`useEffect` on mount):** calls `auth:get-state` IPC. If authenticated, sets local React state. Sets `loading=false`.
- **Listens to `auth:state` IPC broadcasts** — when main says auth changed, AuthContext updates React state.
- **Listens to `auth:session-expired`** via `useSessionExpiry` — closes secondary windows gracefully.
- **`signIn`** — calls `auth:sign-in`, handles the MFA-required branch by setting `mfaRequired` state.
- **`verifyMFA`** — calls `auth:verify-mfa`.
- **`handleSignOut`** — calls `auth:sign-out`.

The `AuthContext` exposes `mfaRequired` and `currentAAL` as React state so UI components like `AuthGuard` and `useLoginForm` can react without making their own network calls.

### `src/config/axios.ts`

Pure consumer. Does **not** maintain its own refresh queue anymore (main has the mutex).

- **Request interceptor:** `await ipcRenderer.invoke('auth:get-token')` and attach as `Bearer`.
- **Response interceptor on 401:** call `auth:get-token` again (main's mutex serializes any in-flight refresh), retry the original request once. If main returns null, dispatch a `auth:session-expired` DOM event so `useSessionExpiry` can react.
- **Network retry logic** (3 attempts with backoff for `ERR_NETWORK` / `ERR_EMPTY_RESPONSE`) was preserved.

### `electron/streaming/audioStreamer.ts` (and `cueAudioStreamer`)

Already had an `updateToken(newToken)` method but **it was never called pre-rewrite**. Main now calls it on every `token-refreshed` event from `AuthManager`, so live WebSocket reconnects always carry a current token.

---

## IPC Surface

These five handlers are the **only** way renderers interact with auth.

| IPC channel        | Direction          | Args                              | Returns                                     | Purpose                                               |
| ------------------ | ------------------ | --------------------------------- | ------------------------------------------- | ----------------------------------------------------- |
| `auth:sign-in`     | renderer → main    | `{ email, password }`             | `{ success, mfaRequired?, mfaFactors?, error? }` | Password sign-in. Returns mfaRequired:true if TOTP enrolled. |
| `auth:verify-mfa`  | renderer → main    | `{ factorId, code }`              | `{ success, error? }`                       | Elevate AAL1 → AAL2 with TOTP code.                   |
| `auth:sign-out`    | renderer → main    | —                                 | `void`                                      | Local sign-out + best-effort server logout.           |
| `auth:get-token`   | renderer → main    | —                                 | `string \| null`                            | Fresh JWT. Refreshes if within 60s of expiry. Mutex'd. |
| `auth:get-state`   | renderer → main    | —                                 | `{ user, isAuthenticated, accessToken }`    | Sync read of auth state.                              |
| `auth:state`       | main → all windows | `{ user, isAuthenticated }`       | (broadcast)                                 | Auth changed (sign-in / sign-out / token refresh).    |
| `auth:token-refreshed` | main → all windows | —                              | (broadcast)                                 | Token was refreshed — useful if any UI cares.         |
| `auth:session-expired` | main → all windows | —                              | (broadcast)                                 | Session is gone. Renderers should close.              |

There are also two backwards-compat aliases (`auth-tokens-refreshed`, `auth-session-expired`) that still fire so any legacy listener keeps working during the transition.

---

## Auth Flows (Detailed)

### Sign-in without MFA

```
User                Renderer                       Main (AuthManager)            Supabase
 │                     │                                  │                          │
 │  enter creds        │                                  │                          │
 ├────────────────────►│                                  │                          │
 │                     │  IPC: auth:sign-in {email,pwd}   │                          │
 │                     ├─────────────────────────────────►│                          │
 │                     │                                  │  POST /auth/v1/token     │
 │                     │                                  ├─────────────────────────►│
 │                     │                                  │◄─────────────── A0 / R0 ─┤
 │                     │                                  │  GET /auth/v1/user       │
 │                     │                                  ├─────────────────────────►│
 │                     │                                  │◄────────── factors: [] ──┤
 │                     │                                  │  saveRefreshToken(R0)    │
 │                     │                                  │  scheduleRefresh(exp-60) │
 │                     │  { success: true }               │                          │
 │                     │◄─────────────────────────────────┤                          │
 │                     │  navigate to dashboard           │                          │
 │◄────────────────────┤                                  │                          │
```

### Sign-in with MFA enrolled

```
User                Renderer                       Main (AuthManager)            Supabase
 │  enter creds        │                                  │                          │
 ├────────────────────►│  IPC: auth:sign-in               │                          │
 │                     ├─────────────────────────────────►│                          │
 │                     │                                  │  POST /auth/v1/token     │
 │                     │                                  ├─────────────────────────►│
 │                     │                                  │◄────── AAL1 token A0/R0 ─┤
 │                     │                                  │  GET /auth/v1/user       │
 │                     │                                  ├─────────────────────────►│
 │                     │                                  │◄──── factors: [TOTP] ────┤
 │                     │  { mfaRequired: true,            │                          │
 │                     │    mfaFactors: [TOTP] }          │                          │
 │                     │◄─────────────────────────────────┤                          │
 │                     │  ⚠️ R0 is NOT persisted yet      │                          │
 │                     │  navigate to /mfa-verify         │                          │
 │  enter TOTP code    │                                  │                          │
 ├────────────────────►│  IPC: auth:verify-mfa            │                          │
 │                     ├─────────────────────────────────►│                          │
 │                     │                                  │  POST /factors/:id/      │
 │                     │                                  │       challenge          │
 │                     │                                  ├─────────────────────────►│
 │                     │                                  │  POST /factors/:id/      │
 │                     │                                  │       verify             │
 │                     │                                  ├─────────────────────────►│
 │                     │                                  │◄────── AAL2 token A1/R1 ─┤
 │                     │                                  │  saveRefreshToken(R1)    │
 │                     │                                  │  scheduleRefresh(exp-60) │
 │                     │  { success: true }               │                          │
 │                     │◄─────────────────────────────────┤                          │
 │                     │  navigate to dashboard           │                          │
```

Note: the AAL1 token from the password step is held **in memory only** until MFA completes. If the user abandons the MFA flow, no refresh token is persisted — so they have to re-enter their password next time.

### Proactive refresh

The `_scheduleRefresh()` method calculates `delayMs = (expiresAt - now - 60) * 1000` and sets a `setTimeout`. The timer is `unref()`-ed so it never blocks app quit.

```
T=now+0s     Sign-in. Token expires at T+3600s.
             scheduleRefresh sets timer for T+3540s.
T=now+3540s  Timer fires. Calls _refresh().
             _refresh sees no refreshPromise in flight, starts one.
             POST /token?grant_type=refresh_token (R0)
             Receives A1, R1.
             saveRefreshToken(R1). emit 'token-refreshed'.
             scheduleRefresh sets next timer for T+7080s.
T=now+3600s  (Old token would have expired here — but it's already replaced.)
```

The user never sees a 401 from token expiry in normal operation.

### Reactive refresh on 401

If something racy happens (clock skew, network glitch right at expiry, etc.) and an HTTP request returns 401:

```
axios request → 401
axios response interceptor:
  marks request _retried = true
  invoke('auth:get-token')
    └─ AuthManager.getAccessToken()
         needsRefresh = true (expiresAt - now < 60)
         if (refreshPromise) await it
         else create one and run _doRefresh()
  if main returns null → dispatch 'auth:session-expired' DOM event
  else → set Authorization header → retry once → return response
```

Concurrent 401s from multiple windows all hit the same `refreshPromise` — main makes one network call, all callers get the same fresh token.

### Session restore on app launch

```
app.whenReady()
  ├─ loadEnvironmentVariables()  ← critical: must run before AuthManager.init()
  └─ await authManager.init()
        ├─ loadRefreshToken()  → reads encrypted blob from disk via safeStorage
        ├─ if no token → return (user signs in)
        ├─ _exchangeRefreshToken(stored)
        │     POST /token?grant_type=refresh_token
        │     receives A1, R1 → updates in-memory state
        ├─ saveRefreshToken(R1)  ← Supabase rotated; persist the new one
        └─ scheduleRefresh()

  if authManager.getState().isAuthenticated:
      build tray menu, skip splash
  else:
      open splash window for sign-in
```

If `_exchangeRefreshToken` throws (e.g. the user signed out from another device, or 90 days passed without use, or the token was revoked), `init()` calls `clearRefreshToken()` and the user is shown the login screen. Silent, non-fatal.

### Sign-out

```
Renderer:  IPC auth:sign-out
Main (AuthManager.signOut):
  if accessToken: fire-and-forget POST /auth/v1/logout (don't await)
  _clearSession()  → null all in-memory tokens, cancel refresh timer
  clearRefreshToken()  → delete encrypted file
  emit 'signed-out'
Main wiring on 'signed-out':
  global.authAccessToken = null
  broadcast auth:state {user:null, isAuthenticated:false} to all windows
  stop WebSocket streamers (shouldReconnect=false, then stop())
```

### Session expiry

Triggered when a refresh fails (Supabase returned 400 — refresh token revoked, expired, or compromised-replay).

```
AuthManager._doRefresh catches the error
  → Sentry.captureException
  → _handleExpired()
       _clearSession()
       clearRefreshToken()
       emit 'session-expired'
Main wiring on 'session-expired':
  global.authAccessToken = null; global.authRefreshToken = null
  broadcast auth:state {user:null, isAuthenticated:false}
  broadcast auth:session-expired
  broadcast auth-session-expired (legacy alias)
  stop WebSocket streamers
```

`useSessionExpiry` in renderers picks up the DOM event (axios bridges the IPC event to a `window.dispatchEvent`) and closes secondary windows. The splash window's `AuthContext` calls `resetUser()` which routes to login.

### WebSocket auth lifecycle

WebSockets are authenticated via a `?token=<jwt>` query param at connect time — Supabase / the Sayso server checks the JWT in the query string.

```
Coach session starts
  → cueAudioStreamer.start(sessionId, token)  ← token from auth:get-token
  → WS opens with ?token=<jwt>

T=connect+3540s
  → AuthManager fires proactive refresh
  → emits 'token-refreshed'
  → main listener calls cueAudioStreamer.updateToken(newToken)
       (this updates the streamer's internal token field; on the next reconnect,
        the WS will use the fresh token; some implementations send an in-band
        auth-update message to avoid reconnecting)

WS drops mid-session
  → reconnect logic kicks in
  → uses streamer.token (now fresh)
  → reconnects successfully

Session expires
  → AuthManager emits 'session-expired'
  → main listener: streamer.shouldReconnect = false; streamer.stop()
       (prevents reconnect loops with a dead token)
```

Pre-rewrite, `updateToken` existed but was never called, so reconnects after a refresh would use the original (stale) token from when the WS first opened. This was a major contributor to the "go AFK and break everything" reproducer.

---

## Files Reference

### Created in May 2026

| File | Purpose |
|---|---|
| `electron/auth/AuthManager.ts` | The single source of truth class. Owns all tokens, handles sign-in / refresh / MFA / sign-out. |
| `client/docs/AUTH_ARCHITECTURE.md` | This document. |

### Significantly rewritten in May 2026

| File | What changed |
|---|---|
| `electron/main.ts` | Instantiates `AuthManager`, wires its events to IPC broadcasts + WS updates + global var sync, replaces old silent-auth block with `authManager.init()`, replaces old IPC handlers with the `auth:*` surface. |
| `src/context/AuthContext.tsx` | No more Supabase SDK. Bootstraps from `auth:get-state`. Listens to `auth:state` broadcast. signIn/verifyMFA/signOut all go through IPC. |
| `src/config/axios.ts` | No more refresh queue (main has the mutex). Request interceptor calls `auth:get-token`. 401 handler retries via the same IPC. |

### Migrated in May 2026 (small edits — token source swap)

These files used to call `supabase.auth.getSession()` to extract a JWT; they now call `auth:get-token` IPC.

| File | What it does |
|---|---|
| `src/coachWindow/services/cueService.ts` | Starts the cue WS streaming session; needs a JWT for auth on the WS URL. |
| `src/coachWindow/services/audioUploadService.ts` | Uploads recorded audio files via IPC to main; passes the JWT through. |
| `src/coachWindow/hooks/useAudioUpload.tsx` | Same as above but as a React hook. |
| `src/coachWindow/hooks/useCue.tsx` | Same as `cueService.ts` but as a hook. |
| `src/trayMenu/TrayMenuApp.tsx` | Builds an auth'd URL when the user clicks "My Account" — opens the web app at `app.asksayso.com/settings#access_token=...`. |
| `src/splashWindow/LogoutGate.tsx` | Triggers sign-out when the splash is reopened with `?logout=true`; now goes through IPC. |
| `src/components/AuthGuard.tsx` | Used to call `getAAL()` (which needed a Supabase SDK session); now reads `mfaRequired` from `AuthContext`. |
| `src/splashWindow/routes/Login/hooks/useLoginForm.tsx` | Used to call `getAAL()` post-signIn; now infers MFA-needed from `signIn()`'s return value (`user: null` ⇒ MFA pending). |

### Deleted in May 2026

In `electron/main.ts`:
- The `update-auth-tokens` IPC handler. **This was the bug** — the channel that let renderers push stale tokens to main.
- The `get-auth-tokens` IPC handler.
- The `refresh-auth-tokens` IPC handler with its `isRefreshingTokens` / `pendingRefreshResolvers` queue (the April 2026 partial fix).
- `const { refreshAuthTokens } = require('./utils/authTokens')` import.

### Dead but not deleted (left in place for now)

These files contain references to the old Supabase JS SDK auth methods but are not imported by any active code path. They can be deleted in a future cleanup PR.

| File | Status |
|---|---|
| `src/config/supabase.ts` | Old Supabase JS client with `cryptoStorage`. Still imported by the dead files below. |
| `src/utils/tokenEncryption.ts` | XOR-with-bundled-key "encryption". Theatrical security; keys live in the bundle. |
| `src/utils/supabaseClient.ts` | Re-export wrapper. Nothing imports it. |
| `src/components/StreamingTest.tsx` | Dev/test component. Nothing imports it. |
| `src/services/mfaServices.ts` | MFA enrollment helpers used by the Account settings page. **NOTE:** if/when MFA enrollment is re-tested, these need to be migrated to IPC the same way auth was — or moved to the main process entirely. |
| `src/utils/debugStorage.ts` | Dev debug helper. |

### Touched but otherwise unchanged

| File | Why it matters |
|---|---|
| `electron/utils/tokenStore.ts` | `safeStorage`-backed persistence for the refresh token. Correct as-is, used by `AuthManager`. |
| `electron/streaming/*Streamer.ts` | `updateToken()` method already existed, now actually gets called by main. |
| `src/hooks/useSessionExpiry.ts` | Still needed — listens to the DOM event that `axios.ts` dispatches when the session ends. |

---

## Gotchas & Lessons Learned

### 1. Module-load-time vs. env-load-time race

**Symptom:** sign-in failed with `Failed to parse URL from /auth/v1/token?grant_type=password`.

**Cause:** `AuthManager` was instantiated at module load (line 31 of `main.ts`) but `loadEnvironmentVariables()` runs ~900 lines later. The constructor cached `process.env.VITE_SUPABASE_URL` (still `undefined`) into `this.supabaseUrl`, so `fetch("" + path)` got a relative URL and Node's fetch can't parse those.

**Fix:** convert `supabaseUrl` and `supabaseAnonKey` to **lazy getters** that read `process.env` at call time. Removes the ordering dependency entirely.

**Lesson:** anything in main that depends on env vars must either be initialized after `loadEnvironmentVariables()` or read env vars lazily. Don't cache env-derived values at module-load time.

### 2. Renderer-owned auth in a multi-window Electron app is structurally fragile

`localStorage` is shared across windows of the same origin, which sounds like it should make multi-window auth easy. But it makes it *worse*: any window can write a stale value at any time, and there's no good way to coordinate "who has the freshest token right now" across renderer processes. The main process is the only place where you can actually serialize state.

If you're building a multi-window Electron app, **own auth in main from day one**. Don't try to share renderer-side stores.

### 3. `autoRefreshToken: false` without proactive refresh elsewhere is a trap

Setting this on the Supabase JS SDK is correct *if and only if* something else is doing proactive refresh. Otherwise you've turned a "refresh in the background" SDK into one that only refreshes reactively on 401, which is the worst of both worlds: you still hit 401s and you have no buffer.

### 4. Refresh token rotation requires careful sequencing

Supabase rotates refresh tokens on every use (with a 10s reuse interval as a safety net). Any code path that holds a copy of the old refresh token after a successful refresh is a session-revocation bomb waiting to go off. The architecture must guarantee that **only one component remembers the current refresh token at a time**, and any other component that needs it asks that component fresh.

### 5. Listen for `INITIAL_SESSION` if you keep the Supabase SDK around

In Supabase JS v2, the SDK fires `INITIAL_SESSION` on construction (regardless of whether a session existed). If you're only listening to `SIGNED_IN`, you'll miss the silent-boot path and your auth state will desync from the SDK's. Pre-rewrite, this contributed to the splash-remount-pushes-stale-token bug.

In the new architecture this doesn't matter (no SDK, no listener), but if MFA enrollment services are ever re-enabled, watch for it.

### 6. WebSocket reconnect loops with a dead token are deadly

If a session expires while a WS is open, and the WS tries to reconnect with the dead token, you get an infinite loop of 4xx-rejected reconnects, each one writing to Sentry. Always `shouldReconnect = false` *before* `stop()` on session-expired.

### 7. The April 2026 fix taught us a real lesson

That fix was logically correct for what it claimed to solve (concurrent refreshes), and it made the symptoms less frequent. But because the underlying architecture wasn't changed, the bug persisted in a slightly different form. **A patch that doesn't change the architecture changes the failure mode, not the failure rate.**

When you can identify a class of bug rather than an instance, fix the class.

---

## What to Watch For

### When changing auth code

1. **Never re-introduce a renderer → main "push tokens" channel.** The whole point of the rewrite is that this direction doesn't exist.
2. **Don't add `setSession()` calls anywhere.** If a piece of code needs a JWT, it asks main via `auth:get-token`. Don't try to set it on the Supabase JS client to "make storage work" — instead, refactor the code to take a token argument.
3. **Don't cache tokens in renderer state for more than the duration of a single async call.** State outlives intent; a cached token in a Zustand store is a future stale-token bug.
4. **If you add a long-running connection (WS, SSE) that uses the JWT, wire it to the `token-refreshed` event on `AuthManager`.** Otherwise it'll silently use a stale token after the next refresh.
5. **If you add new `AuthManager` methods, keep the mutex pattern.** Any operation that touches `accessToken` / `refreshToken` should go through `_refresh()` or be exclusive with it.

### Server-side considerations (out of scope for this rewrite)

- The server middleware (`server/middleware/supabaseauth.js`) currently calls `supabase.auth.getUser(token)` per request, which is an HTTP call to Supabase per request. For better latency under load, this could be replaced with local JWT verification using `jose` against Supabase's JWKS endpoint. This was identified during the audit as a future optimization but was not in scope for the May 2026 rewrite.
- The WebSocket auth path (`server/utils/websocketAuth.js`) has the same property and the same potential optimization.

### MFA enrollment

The MFA enrollment flow on the Account settings page (`mfaServices.ts` + UI) was **not** migrated to the new architecture — it still calls `supabase.auth.mfa.enroll()` etc. on the old SDK. Since the SDK no longer has a session set in the renderer, **MFA enrollment is currently broken**. Either:
- Migrate `mfaServices.ts` to call IPC handlers backed by `AuthManager` methods (recommended), or
- If MFA enrollment is dormant, mark this as known-broken and leave a TODO.

### Session lifetime

The Supabase project is configured with **Time-box: never** and **Inactivity timeout: never**. Combined with `AuthManager.init()` exchanging the persisted refresh token at every app launch, sessions are now effectively permanent — users stay signed in until they explicitly sign out, exactly like Slack/Discord. If those Supabase settings ever change, sessions will start expiring again and `AuthManager.init()` will silently fall back to the login screen — which is correct behavior, but worth knowing.

---

*If this document is out of date, fix it. Future engineers (and future-you) will thank present-you.*
