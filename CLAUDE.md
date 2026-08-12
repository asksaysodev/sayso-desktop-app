# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository. It is self-contained: each repo under `~/Desktop/Sayso/` carries its own `CLAUDE.md`, and there is no workspace-level one.

## What Sayso is, and where this repo sits

Sayso is an AI-powered sales coaching desktop app. It captures both sides of a live sales call, transcribes audio in real time via AssemblyAI, detects buying signals and objections by embedding similarity, and delivers coaching insights through the OpenAI Assistants API into a floating overlay. **This repo is the desktop app** — the capture, streaming and overlay client. Everything server-side (embedding, signal matching, insight generation) lives in `server/`.

`~/Desktop/Sayso/` is a plain folder holding several independent repos side by side — **not** a monorepo. No shared build, no shared `node_modules`, no cross-repo paths. Resolve every path against the repo you were invoked in:

| Folder | Repo (`asksaysodev/…`) | What it is |
|---|---|---|
| `sayso-desktop-app/` | `sayso-desktop-app` | **This repo.** Current Electron desktop app. |
| `server/` | `sayso-backend` | Express API + WebSocket ingest. |
| `sayso-web-app/` | `sayso-web-app` | Web app — dashboard, account, admin, checkout. |
| `sayso-landing/` | `sayso-landing` | Marketing site. |
| `client/` | `sayso-frontend` | **Superseded** earlier desktop-app repo. Don't edit unless asked. |

Two traps worth knowing: source here lives at the repo **root** (`src/`, `electron/`) — there is no `client/` prefix, even though Linear tickets sometimes still say "Repo: sayso-frontend (client)" out of habit. And "prospect" means two different things — see Audio pipeline below.

## Commands

```bash
npm run dev                 # Vite dev server + tsc for electron + launch Electron
npm run build               # Vite production build (dist/)
npm run build:staging       # Vite build against .env.staging
npm run build:electron      # tsc -p tsconfig.electron.json (electron/*.ts → electron/*.js)
npm run rebuild-native      # node-gyp rebuild of the native audio addon (arm64)
npm run rebuild-native-x64  # same, x64
npm run rebuild-native-both # both arches (what `package` uses)
npm run package             # native + electron tsc + vite build + electron-builder, no publish
npm run package:staging     # same via build.staging.js
npm run fresh-export        # clean + full production build + notarize + GitHub draft release
npm run fresh-export:staging
npm run clean               # rm dist/ release*/ caches electron/native-audio/build
npm run kill-port           # free port 5173
```

**No test suite.** `npm test` is a stub that exits 1. There is no lint script either — TypeScript is the only static check (`npm run build:electron` for main/preload; `tsc --noEmit` for the renderer, since `tsconfig.json` sets `noEmit`).

`tsc --noEmit` is **not** at zero: one known error survives — `AudioSettings.tsx:85`, an `ArrayBufferLike`/`ArrayBuffer` variance complaint. Compare against that baseline before assuming a change introduced an error.

**Cue is currently the only feature.** `coachFeature` has no second value, so any `coachFeature === 'recall'`-style comparison is unreachable and TypeScript will flag it. Don't add such branches speculatively.

After changing anything in `electron/*.ts`, run `npm run build:electron` — Electron loads the compiled `electron/main.js`, not the `.ts`.

## Build/runtime layout you need to know

- `package.json` `main` is `electron/main.js` — a **build artifact**. All `electron/**/*.js` and `.js.map` next to a `.ts` file are gitignored compiler output. Never edit them.
- Exceptions — these are **hand-written JS with no `.ts` source**, and are tracked in git: `electron/streaming/*.js`, `electron/native-audio/index.js`, `electron/audioQueue.js`.
- The renderer is a **multi-entry Vite build**. Each window has its own HTML file at the repo root, listed in `vite.config.ts` `rollupOptions.input`, mapped to a folder under `src/`:

  | HTML entry | `src/` folder | Window |
  |---|---|---|
  | `splash-window.html` | `src/splashWindow/` | Login / MFA / permissions / update prompt |
  | `tray-menu.html` | `src/trayMenu/` | Custom tray popover (the app's primary UI) |
  | `coach-window.html` | `src/coachWindow/` | Floating always-on-top coaching overlay |
  | `playbook-window.html` | `src/playbookWindow/` | Playbook panel, docks right of the coach |
  | `app-settings-window.html` | `src/appSettingsWindow/` | Settings incl. Software Update tab |
  | `onboarding-window.html` | `src/onboardingWindow/` | First-run onboarding |

  Adding a window means: new root HTML + new `src/<name>Window/` + a `rollupOptions.input` entry + a `create…Window()` in `electron/main.ts`.
- There is **no `src/views/`**. Dashboard, account, admin and checkout UI live in the `sayso-web-app` repo, not here. The tray's "My Account" just opens `app.asksayso.com` with a token in the URL fragment.
- Path alias `@/*` → `./src/*` (renderer only; `electron/` uses relative requires).
- Each window entry point (`src/*/index.tsx`) mounts its own React root, its own `QueryClient`, calls `Sentry.init`, and **must** `import '@/services/networkReporter'` — see Network state below.

## Auth — main process owns the session

`docs/AUTH_ARCHITECTURE.md` is the full story; the short version:

- `electron/auth/AuthManager.ts` is the single source of truth. It holds the access token in memory, persists **only** the refresh token via `safeStorage` (`electron/utils/tokenStore.ts`, keychain item `Sayso Safe Storage`), decodes `exp` from the JWT, and schedules a proactive refresh 60s before expiry.
- Concurrency is handled by a single `refreshPromise` mutex field — all concurrent callers await the same in-flight refresh.
- Renderers are **read-only consumers**. The only auth surface is five IPC handlers: `auth:sign-in`, `auth:verify-mfa`, `auth:sign-out`, `auth:get-token`, `auth:get-state`, plus broadcasts `auth:state`, `auth:token-refreshed`, `auth:session-expired` (with `auth-tokens-refreshed` / `auth-session-expired` legacy aliases).
- **Never re-introduce a renderer → main "push tokens" channel.** A renderer writing stale tokens back to main was the exact bug (Supabase's compromised-refresh-token detection then revoked the session mid-call). Anything needing a JWT calls `auth:get-token`; don't cache it in React/Zustand state beyond one async call.
- Any new long-lived connection using the JWT must be wired to `AuthManager`'s `token-refreshed` event (this is what `AudioStreamer.updateToken()` is for) and torn down with `shouldReconnect = false` before `stop()` on `session-expired`.
- `AuthManager` reads `VITE_SUPABASE_URL` / `VITE_SUPABASE_ANON_KEY` through **lazy getters**. Main constructs it at module load but `loadEnvironmentVariables()` runs much later in `app.whenReady()` — anything in main that depends on env vars must read them lazily or initialize after that call.
- The Supabase JS SDK is no longer part of auth, and its leftovers (`src/config/supabase.ts`, `src/utils/tokenEncryption.ts`, `src/utils/supabaseClient.ts`, `src/utils/debugStorage.ts`, `src/services/mfaServices.ts`) were deleted in SAYSO-354. Only an `import type { Factor }` in `AuthContext.tsx` remains. **There is no MFA *enrollment* in this app** — users enroll via the web app; MFA *verification* at login goes through `AuthManager.verifyMFA()` and works.

## Audio pipeline (Cue)

```
native addon (ScreenCaptureKit / AVAudioEngine)
  → two independent callbacks: system audio ("prospect") and mic ("user")
  → electron/streaming/audioBuffer.js   accumulate 50–1000ms chunks
  → electron/streaming/audioConverter.js  → PCM16 / 16kHz / mono
  → electron/streaming/websocketClient.js → wss://…/cue/stream/{user,prospect}?token=…
  → server forwards bytes untouched to AssemblyAI
  ← insights arrive back over WS → main → `cue-insight` broadcast → coach window
```

- **"Prospect" means two things.** Here it means *the other side of the live call* — `/cue/stream/prospect`, prospect-side transcripts and embeddings. That is core product and fully active. The separate **Prospects *entity*** (CRUD on prospect records, a `prospects` table, `/prospects/*` routes) is deprecated product-wide and was deleted from this repo in SAYSO-354; sibling repos may still have leftovers. Never conflate the two.
- The client does **all** transcoding. The server transcodes nothing and spawns no processes.
- Buffer first, convert after — the converter uses the `format` the native module reports, so a native module must describe its buffers **truthfully** rather than match a fixed rate. `docs/NATIVE_AUDIO_CONTRACT.md` is the frozen 11-method NAN surface; `electron/native-audio/index.js` is the authority on how those methods are called — implement to match it, don't change it to fit a new native shape.
- The native addon must never call into V8 from the audio thread; it hops to the JS thread via `uv_async_send`.
- Orchestration lives in `electron/audio/audioManager.ts` (`start-cue` / `stop-cue` handlers, low-audio watchdog, teardown dedup). It requires the provider lazily so a missing `.node` build doesn't crash app start.
- The native module is **per-architecture**. Switching arch or Electron version requires `npm run rebuild-native*`; `electron-builder` hooks (`scripts/rebuild-native-for-arch.js`, `verify-and-fix-native-module.js`, `sign-native-module.js`) handle this during packaging, and the module is `asarUnpack`ed.

## Cross-platform conventions

Only `darwin` and `win32` are targets. `docs/IPC_CONTRACT.md` classifies every channel.

- **Never branch on `process.platform` inline inside an IPC handler.** Per-OS behavior goes behind a provider interface — `electron/audio/` (`IAudioProvider`) and `electron/permissions/` (`IPermissionsProvider`) — dispatched in that module's `index.ts`. Handlers stay thin.
- For small presentational differences use the flags in `electron/utils/platform.ts` (`IS_MAC`, `IS_WINDOWS`, `ALLOW_VIBRANCY`), never a raw `'darwin'`/`'win32'` literal.
- Known macOS-only seams in `main.ts`: dock badge reset, the `window-all-closed` quit rule, tray positioning, vibrancy, the ShipIt `launchctl kickstart` watchdog on update install, and the `/Applications` launch guard (`electron/utils/applicationsFolder.ts`, called first thing in `whenReady()`).

## Permissions

Mic + screen recording are collected during **first-run onboarding in the splash window**, not lazily at coach-open time; a runtime guard re-surfaces the screen if they're missing. Reads use non-prompting `CGPreflightScreenCaptureAccess`; prompting happens only on explicit user action. macOS binds screen-recording permission at process launch, so granting it mid-session requires a quit-and-reopen — that's why the flow ends in "Quit and Reopen". `isPermissionsComplete()` requires the persisted flag **and** a live mic grant **and** a live screen grant. See `docs/PERMISSIONS_FLOW.md`.

## Network state

The OS is the source of truth. `src/services/networkReporter.ts` is a side-effect module that forwards `online`/`offline` events to main, which mirrors them in `global.networkState` (`'online' | 'reconnecting'`) and broadcasts `network:state-changed`. While reconnecting, `AuthManager` pauses both its expiry timer and its retry timer — no polling, no backoff spam — and several IPC handlers early-return. **Every renderer entry point must import `networkReporter`**; the tray is the critical one because it's often the only window open. Network errors never sign the user out — only a definitive Supabase `invalid_grant` does. See `docs/NETWORK_STATE_FLOW.md`.

## Updates and releases

`autoDownload = false`; the user opts in. Splash window is the interruption surface, App Settings → Software Update the self-service one. An update detected during an active coach session does **not** show the splash — it rechecks every 10 min and opens once the session ends. Download-complete auto-restarts via `quitAndInstall()`. Main is the single source of update state, pushed as `update:state-changed`. See `docs/UPDATE_FLOW.md`.

Release flow (`docs/VERSIONING.md`): there is no `main` branch. `feature/* → development → npm version <patch|minor|major> → staging`. **Bump the version on `development`, never on `staging`** — doing it backwards is what shipped a mislabeled 1.2.5. Both staging and production artifacts build from `staging`; `fresh-export` aborts if local `package.json` disagrees with `origin/staging`. Run `/changelog` after bumping to generate release notes.

## Dead code

The dashboard-era leftovers this section used to list were deleted in SAYSO-354
(August 2026): the Prospects *entity* and its file-upload components, `Sidebar`,
`AdminPanel` + `useAdmin`/`adminStore`, the unmounted `AppContext` /
`SalesCoachContext` / `SubscriptionAlertContext` / `ToastContext`, the superseded
`coachWindow/hooks/useCue.tsx` and the older `useCoach`/`useInsightScheduler`/
`recallService` coach implementation, the auth guards, the Supabase-SDK
leftovers, and ~30 orphaned CSS files. The `upload-file` / `upload-both-files` /
`demo-insight` IPC bridges and the legacy `autoUpdater.on*` subscriptions went
with them.

**Everything under `src/` is now reachable from one of the six window entry
points**, with two deliberate exceptions:

- `src/components/ui/` — shadcn primitives, kept as an installed library. Only
  `spinner` is currently used; the rest are there for new UI to pull from.
- `src/types/electron.d.ts` and `src/vite-env.d.ts` — ambient declarations, never
  imported by design.

`Account.role?: UserRole` in `src/types/user.ts` is still declared and still never
read: **the app has no role gates.** It's kept deliberately — unreferenced *type
declarations* are cheap and are not deletion targets; unreferenced
*implementation* is.

To re-check reachability after a refactor, walk the import graph from the six
root HTML entries; anything under `src/` that it doesn't reach is a deletion
candidate. Grep for consumers before acting on it.

## Conventions

- State: Zustand (`src/store/`) for coach-session state, React Context for the splash's auth tree, React Query with a 2-minute `staleTime` for async data.
- UI: Radix primitives + shadcn (`src/components/ui/`, config in `components.json`) + Tailwind, alongside a lot of older plain CSS in `src/styles/` and per-window `styles/` folders.
- IPC additions go in `electron/preload.ts`, are typed in `src/types/electron.d.ts`, and should be recorded in `docs/IPC_CONTRACT.md` with a platform classification.
- Main→renderer fan-out uses the `broadcastToAllWindows(channel, payload)` / `sendToOnboardingWindow(channel)` helpers in `main.ts`, not literal `.send()` at each call site.
- Branches: `feature/`, `bugfix/`, `refactor/`, `chore/`, `docs/`, `test/`, `perf/`, `release/`. PRs target `development` and use `.github/pull_request_template.md`. See `PR_GUIDELINES.md`.
- Strip `console.log` and debug code before opening a PR.
