# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

The workspace-level `~/Desktop/Sayso/CLAUDE.md` covers the product as a whole and the sibling repos. This file covers only `sayso-desktop-app`. Source lives at the repo **root** (`src/`, `electron/`) — there is no `client/` prefix.

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
- `src/helpWindow/` exists but both files are **0 bytes** — an unstarted placeholder, no HTML entry, not in the Vite config.
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
- The Supabase JS SDK is no longer part of auth. `src/config/supabase.ts`, `src/utils/tokenEncryption.ts`, `src/utils/supabaseClient.ts`, `src/utils/debugStorage.ts` are leftovers. `src/services/mfaServices.ts` still calls the old SDK, so **MFA *enrollment* is known-broken**; MFA *verification* at login goes through `AuthManager` and works.

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

- The client does **all** transcoding. The server transcodes nothing and spawns no processes.
- Buffer first, convert after — the converter uses the `format` the native module reports, so a native module must describe its buffers **truthfully** rather than match a fixed rate. `docs/NATIVE_AUDIO_CONTRACT.md` is the frozen 11-method NAN surface; `electron/native-audio/index.js` is the authority on how those methods are called — implement to match it, don't change it to fit a new native shape.
- The native addon must never call into V8 from the audio thread; it hops to the JS thread via `uv_async_send`.
- Orchestration lives in `electron/audio/audioManager.ts` (`start-cue` / `stop-cue` handlers, low-audio watchdog, teardown dedup). It requires the provider lazily so a missing `.node` build doesn't crash app start.
- The native module is **per-architecture**. Switching arch or Electron version requires `npm run rebuild-native*`; `electron-builder` hooks (`scripts/rebuild-native-for-arch.js`, `verify-and-fix-native-module.js`, `sign-native-module.js`) handle this during packaging, and the module is `asarUnpack`ed.

## Cross-platform conventions

Only `darwin` and `win32` are targets. `docs/IPC_CONTRACT.md` classifies every channel.

- **Never branch on `process.platform` inline inside an IPC handler.** Per-OS behavior goes behind a provider interface — `electron/audio/` (`IAudioProvider`) and `electron/permissions/` (`IPermissionsProvider`) — dispatched in that module's `index.ts`. Handlers stay thin.
- For small presentational differences use the flags in `electron/utils/platform.ts` (`IS_MAC`, `IS_WINDOWS`, `ALLOW_VIBRANCY`), never a raw `'darwin'`/`'win32'` literal.
- Known macOS-only seams in `main.ts`: dock badge reset, the `window-all-closed` quit rule, tray positioning, vibrancy, and the ShipIt `launchctl kickstart` watchdog on update install.

## Permissions

Mic + screen recording are collected during **first-run onboarding in the splash window**, not lazily at coach-open time; a runtime guard re-surfaces the screen if they're missing. Reads use non-prompting `CGPreflightScreenCaptureAccess`; prompting happens only on explicit user action. macOS binds screen-recording permission at process launch, so granting it mid-session requires a quit-and-reopen — that's why the flow ends in "Quit and Reopen". `isPermissionsComplete()` requires the persisted flag **and** a live mic grant **and** a live screen grant. See `docs/PERMISSIONS_FLOW.md`.

## Network state

The OS is the source of truth. `src/services/networkReporter.ts` is a side-effect module that forwards `online`/`offline` events to main, which mirrors them in `global.networkState` (`'online' | 'reconnecting'`) and broadcasts `network:state-changed`. While reconnecting, `AuthManager` pauses both its expiry timer and its retry timer — no polling, no backoff spam — and several IPC handlers early-return. **Every renderer entry point must import `networkReporter`**; the tray is the critical one because it's often the only window open. Network errors never sign the user out — only a definitive Supabase `invalid_grant` does. See `docs/NETWORK_STATE_FLOW.md`.

## Updates and releases

`autoDownload = false`; the user opts in. Splash window is the interruption surface, App Settings → Software Update the self-service one. An update detected during an active coach session does **not** show the splash — it rechecks every 10 min and opens once the session ends. Download-complete auto-restarts via `quitAndInstall()`. Main is the single source of update state, pushed as `update:state-changed`. See `docs/UPDATE_FLOW.md`.

Release flow (`docs/VERSIONING.md`): there is no `main` branch. `feature/* → development → npm version <patch|minor|major> → staging`. **Bump the version on `development`, never on `staging`** — doing it backwards is what shipped a mislabeled 1.2.5. Both staging and production artifacts build from `staging`; `fresh-export` aborts if local `package.json` disagrees with `origin/staging`. Run `/changelog` after bumping to generate release notes.

## Dead code — do not build on these

- `src/components/Sidebar.tsx` and `src/components/AdminPanel/` — leftovers from when this app had a dashboard. Unreferenced. `src/hooks/useAdmin.tsx` / `src/store/adminStore.ts` exist only to serve `AdminPanel`.
- `Account.role?: UserRole` in `src/types/user.ts` is declared but never read. **The app has no role gates.**
- The Prospects *entity* (`useProspects`, `ProspectsContext`, `ProspectCard`, `ProspectDetail`, `FormLine`) is deprecated product-wide — see the workspace CLAUDE.md. Still present, zero live consumers. Note that "prospect" in the **audio** path (`/cue/stream/prospect`, prospect-side transcripts) is core product and fully active.
- `src/context/AppContext.tsx`, `SalesCoachContext.tsx`, `SubscriptionAlertContext.tsx` are mounted nowhere. `AuthProvider` is mounted only in the splash window; `ToastProvider` only by the dead components above.
- `src/coachWindow/hooks/useCue.tsx` is superseded by `src/store/coachWindowStore.ts` (a comment in the file says so). The Zustand store is the live path.
- Orphaned preload bridges with no `ipcMain` handler: `upload-file`, `upload-both-files`, and the `autoUpdater.on*` subscriptions (`update-available` / `download-progress` / `update-downloaded`).

Before deleting any of the above, grep for consumers — this list is a strong default, not a guarantee.

## Conventions

- State: Zustand (`src/store/`) for coach-session state, React Context for the splash's auth tree, React Query with a 2-minute `staleTime` for async data.
- UI: Radix primitives + shadcn (`src/components/ui/`, config in `components.json`) + Tailwind, alongside a lot of older plain CSS in `src/styles/` and per-window `styles/` folders.
- IPC additions go in `electron/preload.ts`, are typed in `src/types/electron.d.ts`, and should be recorded in `docs/IPC_CONTRACT.md` with a platform classification.
- Main→renderer fan-out uses the `broadcastToAllWindows(channel, payload)` / `sendToOnboardingWindow(channel)` helpers in `main.ts`, not literal `.send()` at each call site.
- Branches: `feature/`, `bugfix/`, `refactor/`, `chore/`, `docs/`, `test/`, `perf/`, `release/`. PRs target `development` and use `.github/pull_request_template.md`. See `PR_GUIDELINES.md`.
- Strip `console.log` and debug code before opening a PR.
