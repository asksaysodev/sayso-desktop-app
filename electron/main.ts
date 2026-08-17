/// <reference path="./globals.d.ts" />
import type { 
  BrowserWindow as BrowserWindowType, 
  DevicePermissionHandlerHandlerDetails, 
  Tray as TrayType,
  WebContents
} from 'electron';
import type { Event } from 'electron';
import type {
  AuthUser
} from './globals';

import { app, BrowserWindow, ipcMain, screen as electronScreen, shell, globalShortcut, dialog, Tray, Menu, nativeTheme, powerMonitor } from 'electron';
import path from 'node:path';
import fs from 'node:fs';
import { spawn } from 'node:child_process';
import { nativeImage } from 'electron/common';
import * as Sentry from '@sentry/electron/main';
import sentryConfig from './sentry.config';
import { WindowManager } from './utils/windowManager';
import { loadRefreshToken, saveRefreshToken } from './utils/tokenStore';
import { resetPermissionsIfCertChanged } from './utils/permissionsMigration';
import { IS_MAC, ALLOW_VIBRANCY } from './utils/platform';
import { classifyUpdaterError, isTransientNetworkError, updaterErrorMessage, READ_ONLY_VOLUME_MESSAGE } from './utils/transientErrors';
import { enforceApplicationsFolderLocation, isOutsideApplicationsFolder } from './utils/applicationsFolder';
import { enforceMinimumMacOSVersion } from './utils/osVersion';
import type { UpdateState } from './shared/update';
import { AuthManager } from './auth/AuthManager';
import type { AuthState } from './auth/AuthManager';
import * as audioManager from './audio/audioManager';
import * as permissions from './permissions/permissionsManager';

Sentry.init(sentryConfig);

// Read lazily, never captured at module scope: loadEnvironmentVariables() runs
// ~1100 lines below this point, so anything evaluated here sees an unset
// VITE_BACKEND_BASE_URL and would pin every main-process call to localhost in
// packaged builds. A getter keeps the DRY win without the ordering dependency.
const backendBaseUrl = () => process.env.VITE_BACKEND_BASE_URL || 'http://localhost:4000';
const IS_STAGING = (require('../package.json') as { build_env?: string }).build_env === 'staging';

if (IS_STAGING) {
  // Give staging its own safeStorage keychain entry so it doesn't conflict
  // with production's "Sayso Safe Storage" item (different binary, same entry name = prompt every launch)
  app.setName('sayso-app-staging');
  app.setPath('userData', path.join(app.getPath('appData'), 'sayso-app-staging'));
} else if (!app.isPackaged) {
  // Dev runs (`npm run dev`) are unpackaged and carry no build_env, so without this
  // they fall through to the default "sayso-app" userData — the SAME directory the
  // installed production app uses — and pollute its auth/permissions state (e.g.
  // writing permissions-team-id.json, which then suppresses the prod cert migration).
  // Isolate dev into its own directory. (app.isPackaged is reliable here; NODE_ENV is not.)
  app.setName('sayso-app-dev');
  app.setPath('userData', path.join(app.getPath('appData'), 'sayso-app-dev'));
} else {
  // Production. Align the Electron app name with the display name ("Sayso") so the
  // safeStorage keychain item is named "Sayso Safe Storage" (matching the convention
  // other apps use) instead of "sayso-app Safe Storage". The default app name comes
  // from package.json ("sayso-app"); npm names must be lowercase, so we rename at
  // runtime here rather than in package.json.
  //
  // app.getName() also drives userData, so PIN it to the existing "sayso-app" dir —
  // otherwise renaming would relocate auth.json / permissions flags / logs and treat
  // every user as a fresh install. Bundle ID, code signature and Team ID come from
  // Info.plist and are unchanged, so the cert/TCC migration is unaffected.
  //
  // NOTE: this changes the keychain service name, so the new "Sayso Safe Storage"
  // item is created fresh (silently) on first login and the old "sayso-app Safe
  // Storage" item is left orphaned. Every existing user re-logs in once — intended.
  app.setName('Sayso');
  app.setPath('userData', path.join(app.getPath('appData'), 'sayso-app'));
}

// ─── Auth: single source of truth ────────────────────────────────────────────
// Owns all token state for the app's lifetime. Renderers ask main via IPC.
export const authManager = new AuthManager();

// True when init() failed transiently at boot (offline at startup).
// The token-refreshed handler checks this to run the deferred profile/features fetch.
let startupOfflinePending = false;

function broadcastToAllWindows(channel: string, data?: unknown): void {
  BrowserWindow.getAllWindows().forEach((win: BrowserWindowType) => {
    if (!win.isDestroyed()) win.webContents.send(channel, data);
  });
}

function setAuthUser(user: AuthUser | null): void {
  global.authUser = user;
  if (trayMenuWindow && !trayMenuWindow.isDestroyed()) {
    trayMenuWindow.webContents.send('user-auth', { authUser: user });
  }
  if (global.mainWindow && !global.mainWindow.isDestroyed()) {
    global.mainWindow.webContents.send('user-auth', { authUser: user });
  }
}

/**
 * Whether the onboarding window should be opened for the current account.
 *
 * SAYSO-338: a missing profile reads as `undefined`, which is NOT the same as
 * "onboarding is pending". Treating them alike meant any moment where main had
 * no profile forced onboarding open — including the reopen loop this issue is
 * about. Only a profile we actually hold gets to decide; a genuinely new user
 * carries an explicit `null` status and still gets onboarding.
 */
function shouldOpenOnboarding(): boolean {
  if (onboardingStatusThisSession) return false;
  const profile = global.authUser || undefined;
  if (!profile) return false;
  const status = profile.onboarding_status;
  return status !== 'complete' && status !== 'dismissed';
}

// Onboarding status (SAYSO-338)
type OnboardingStatus = 'complete' | 'dismissed';

let onboardingStatusThisSession: OnboardingStatus | null = null;

const ONBOARDING_PUT_RETRY_DELAYS_MS = [1000, 3000, 10000, 30000];

const pendingOnboardingStatusPath = () =>
  path.join(app.getPath('userData'), 'pending-onboarding-status.json');

function writePendingOnboardingStatus(email: string, status: OnboardingStatus): void {
  try {
    fs.writeFileSync(pendingOnboardingStatusPath(), JSON.stringify({ email, status }));
  } catch (err) {
    console.warn('[onboarding] could not persist pending status:', (err as Error)?.message);
  }
}

function clearPendingOnboardingStatus(): void {
  try {
    fs.rmSync(pendingOnboardingStatusPath(), { force: true });
  } catch {}
}

// Bumped by every setOnboardingStatus() call. A retry chain carries the
// generation it was started with and stands down as soon as a newer write
// exists, so a slow 'dismissed' chain can't land after — and overwrite — a
// 'complete' that was decided later.
let onboardingStatusGeneration = 0;

async function pushOnboardingStatus(
  email: string,
  status: OnboardingStatus,
  generation: number,
  attempt = 0,
): Promise<void> {
  if (authManager.getState().user?.email !== email) {
    return;
  }
  if (generation !== onboardingStatusGeneration) {
    console.log(`[onboarding] dropping superseded '${status}' write`);
    return;
  }

  const token = await authManager.getAccessToken().catch(() => null);

  if (token) {
    // Re-check after the await — a newer status may have been set while the
    // token request was in flight.
    if (generation !== onboardingStatusGeneration) {
      console.log(`[onboarding] dropping superseded '${status}' write`);
      return;
    }
    try {
      await axios.put(
        `${backendBaseUrl()}/accounts/update-account`,
        { updateData: { onboarding_status: status } },
        { headers: { Authorization: `Bearer ${token}` }, timeout: 10000 },
      );
      console.log(`[onboarding] status '${status}' persisted`);
      if (generation === onboardingStatusGeneration) clearPendingOnboardingStatus();
      return;
    } catch (err) {
      console.warn(`[onboarding] persisting '${status}' failed (attempt ${attempt + 1}):`, (err as Error)?.message);
      if (attempt >= ONBOARDING_PUT_RETRY_DELAYS_MS.length && !isTransientNetworkError(err)) {
        Sentry.captureException(err);
      }
    }
  }

  if (attempt >= ONBOARDING_PUT_RETRY_DELAYS_MS.length) {
    console.warn('[onboarding] giving up for now — the write will replay on next launch');
    return;
  }
  setTimeout(() => {
    void pushOnboardingStatus(email, status, generation, attempt + 1);
  }, ONBOARDING_PUT_RETRY_DELAYS_MS[attempt]);
}

function setOnboardingStatus(status: OnboardingStatus): void {
  if (onboardingStatusThisSession === 'complete' && status === 'dismissed') {
    console.log("[onboarding] ignoring 'dismissed' — already completed this session");
    return;
  }

  console.log(`[onboarding] status set to '${status}'`);
  onboardingStatusThisSession = status;

  const profile = global.authUser || undefined;
  if (profile) setAuthUser({ ...profile, onboarding_status: status });

  const email = authManager.getState().user?.email;
  if (!email) return;
  const generation = ++onboardingStatusGeneration;
  writePendingOnboardingStatus(email, status);
  void pushOnboardingStatus(email, status, generation);
}

function replayPendingOnboardingStatus(): void {
  let pending: { email?: string; status?: OnboardingStatus } | null = null;
  try {
    pending = JSON.parse(fs.readFileSync(pendingOnboardingStatusPath(), 'utf8'));
  } catch {
    return;
  }
  if (pending?.status !== 'complete' && pending?.status !== 'dismissed') {
    clearPendingOnboardingStatus();
    return;
  }
  if (!pending.email || pending.email !== authManager.getState().user?.email) {
    clearPendingOnboardingStatus();
    return;
  }
  console.log(`[onboarding] replaying pending status '${pending.status}' from a previous session`);
  onboardingStatusThisSession = pending.status;
  const profile = global.authUser || undefined;
  if (profile) setAuthUser({ ...profile, onboarding_status: pending.status });
  void pushOnboardingStatus(pending.email, pending.status, ++onboardingStatusGeneration);
}

// Backoff schedule for re-fetching the account profile after a failed attempt.
// The profile drives the tray's subscription-gated rows and the onboarding gate,
// and a single failed shot at boot used to leave it missing for the entire
// session — no retry, no re-broadcast, recoverable only by restarting.
const PROFILE_RETRY_DELAYS_MS = [2000, 5000, 15000, 30000];

// The boot fetch and the startup-offline recovery both call scheduleProfileRetry
// independently. The `global.authUser` guard keeps two chains correct, but they
// double the request volume on exactly the flaky network that triggered them.
let profileRetryInFlight = false;

function scheduleProfileRetry(email: string | undefined, attempt = 0): void {
  // Only exhaustion releases the guard. A no-op call with no email must not,
  // or scheduleProfileRetry(state.user?.email) with an undefined email would
  // clear the flag out from under a chain that is still running and let the
  // next attempt-0 call start a second one — the exact duplication the flag
  // exists to prevent.
  if (attempt >= PROFILE_RETRY_DELAYS_MS.length) {
    profileRetryInFlight = false;
    return;
  }
  if (!email) return;
  if (attempt === 0 && profileRetryInFlight) {
    console.log('[MAIN] Profile retry already in flight — not starting a second chain');
    return;
  }
  profileRetryInFlight = true;
  setTimeout(async () => {
    if (global.authUser) {
      profileRetryInFlight = false;
      return;
    }
    if (authManager.getState().user?.email !== email) {
      profileRetryInFlight = false;
      return;
    }

    const token = await authManager.getAccessToken().catch(() => null);
    if (!token) {
      scheduleProfileRetry(email, attempt + 1);
      return;
    }
    try {
      const res = await axios.get(`${backendBaseUrl()}/accounts/${email}`, {
        headers: { Authorization: `Bearer ${token}` },
        timeout: 5000,
      });
      profileRetryInFlight = false;
      setAuthUser(res.data.data);
      console.log('[MAIN] Profile retry succeeded — tray and onboarding gate now have the account');
      // The gate is otherwise consulted once, at whenReady or splash-login-success.
      // Without this a boot fetch that only lands on retry leaves the user with no
      // onboarding for the entire session, even though their profile says pending.
      maybeOpenOnboardingLate();
    } catch (err) {
      console.warn(`[MAIN] Profile retry ${attempt + 1}/${PROFILE_RETRY_DELAYS_MS.length} failed:`, (err as Error)?.message);
      scheduleProfileRetry(email, attempt + 1);
    }
  }, PROFILE_RETRY_DELAYS_MS[attempt]);
}

/**
 * Re-evaluates the onboarding gate after a late-arriving profile. Deliberately
 * conservative: a profile can land up to ~52 s after boot, or later still after
 * a reconnect, so it never pops the tour over a splash, a tour already showing,
 * or a coach/playbook window the user is actively working in.
 */
function maybeOpenOnboardingLate(): void {
  if (splashWindowInstance && !splashWindowInstance.isDestroyed()) return;
  if (onboardingWindowInstance && !onboardingWindowInstance.isDestroyed()) return;
  if (isCoachWindowOpen() || isPlaybookWindowOpen()) return;
  if (!shouldOpenOnboarding()) return;
  console.log('[MAIN] Profile arrived late and onboarding is pending — opening onboarding window');
  createOnboardingWindow();
}

let authUserReady: Promise<void> = Promise.resolve();

// Fetches the full account profile into global.authUser (main's source of truth
// for subscription/onboarding state and the tray's logged-in display). Mirrors
// the silent-restore fetch at startup. Never throws — on failure the tray falls
// back to its logged-out state, same as the restore path.
async function loadAuthUserProfile(accessToken: string, email: string | undefined): Promise<void> {
  if (!email) return;
  try {
    const res = await axios.get(`${backendBaseUrl()}/accounts/${email}`, {
      headers: { Authorization: `Bearer ${accessToken}` },
      timeout: 5000,
    });
    setAuthUser(res.data.data);
	maybeReportAppVersion(backendBaseUrl(), accessToken, res.data.data);
  } catch (err) {
    console.warn('[MAIN] sign-in: profile fetch failed — retrying in background', (err as Error)?.message);
    if (!isTransientNetworkError(err)) Sentry.captureException(err);
    scheduleProfileRetry(email);
  }
}

authManager.on('signed-in', (state: AuthState) => {
  console.log('[AuthManager] signed-in:', state.user?.email);
  global.authAccessToken = state.accessToken;
  broadcastToAllWindows('auth:state', { user: state.user, isAuthenticated: state.isAuthenticated, accessToken: state.accessToken });
  if (state.accessToken) {
    // Load the account profile into global.authUser now, so the tray shows the
    // logged-in state and the onboarding gate sees the real onboarding_status.
    authUserReady = loadAuthUserProfile(state.accessToken, state.user?.email);
    fetchAndCacheEnabledFeatures(backendBaseUrl(), state.accessToken).catch((err) => {
      console.warn('[AuthManager] signed-in: features fetch failed', err?.message);
      if (!isTransientNetworkError(err)) Sentry.captureException(err);
    });
  }
});

authManager.on('signed-out', () => {
  console.log('[AuthManager] signed-out');
  global.authAccessToken = null;
  global.authRefreshToken = null;
  setAuthUser(null);
  onboardingStatusThisSession = null;
  cachedEnabledFeatures = [];
  global.playbooksCache = null;
  broadcastEnabledFeatures();
  broadcastToAllWindows('auth:state', { user: null, isAuthenticated: false, accessToken: null });
  broadcastToAllWindows('auth-session-expired');   // backward-compat for unmigrated windows
  broadcastToAllWindows('auth:session-expired');
  tearDownSignedInWindows();
});

authManager.on('token-refreshed', async (state: AuthState) => {
  console.log('[AuthManager] token-refreshed');
  global.authAccessToken = state.accessToken;
  broadcastToAllWindows('auth:state', { user: state.user, isAuthenticated: state.isAuthenticated, accessToken: state.accessToken });
  broadcastToAllWindows('auth:token-refreshed');
  // Also broadcast old event so any remaining unmigrated axios listeners stay warm
  broadcastToAllWindows('auth-tokens-refreshed', { accessToken: state.accessToken, refreshToken: '' });
  // Keep the active audio WebSocket's token current so reconnects after a refresh
  // don't fail with an expired JWT. updateToken() stores the value and the next
  // _connect() call will embed it in the WebSocket URL query string.
  if (state.accessToken) {
    audioManager.updateCueToken(state.accessToken);
  }

  // Startup-offline recovery: the first successful refresh after init() failed
  // transiently at boot. Run the deferred profile/features/font-size fetch now.
  if (startupOfflinePending && state.isAuthenticated) {
    startupOfflinePending = false;
    console.log('[MAIN] Startup-offline recovery — fetching profile and features');
    const headers = { Authorization: `Bearer ${state.accessToken}` };
    const [profileResult, fontSizeResult, featuresResult] = await Promise.allSettled([
      axios.get(`${backendBaseUrl()}/accounts/${state.user?.email}`, { headers, timeout: 5000 }),
      fetchAndCacheFontSize(backendBaseUrl(), state.accessToken!),
      fetchAndCacheEnabledFeatures(backendBaseUrl(), state.accessToken!),
    ]);
    if (profileResult.status === 'fulfilled') {
      setAuthUser(profileResult.value.data.data);
	  maybeReportAppVersion(backendBaseUrl(), state.accessToken!, profileResult.value.data.data);
      // This was the one profile-success path that skipped the gate — its own
      // failure branch reaches it via scheduleProfileRetry, so launching offline
      // and recovering left the user with no onboarding for the whole session.
      replayPendingOnboardingStatus();
      maybeOpenOnboardingLate();
    } else {
      console.warn('[MAIN] Startup-offline recovery: profile fetch failed — retrying in background', profileResult.reason);
      if (!isTransientNetworkError(profileResult.reason)) Sentry.captureException(profileResult.reason);
      scheduleProfileRetry(state.user?.email);
    }
    if (fontSizeResult.status === 'rejected') {
      console.warn('[MAIN] Startup-offline recovery: font_size fetch failed', fontSizeResult.reason);
    }
    if (featuresResult.status === 'rejected') {
      console.warn('[MAIN] Startup-offline recovery: features fetch failed', featuresResult.reason);
    }
  }
});

authManager.on('session-expired', () => {
  console.log('[AuthManager] session-expired');
  global.authAccessToken = null;
  global.authRefreshToken = null;
  setAuthUser(null);
  onboardingStatusThisSession = null;
  cachedEnabledFeatures = [];
  global.playbooksCache = null;
  broadcastEnabledFeatures();
  broadcastToAllWindows('auth:state', { user: null, isAuthenticated: false, accessToken: null });
  broadcastToAllWindows('auth-session-expired');   // backward-compat
  broadcastToAllWindows('auth:session-expired');
  // Stop reconnect loops that have no valid token, and close the secondary
  // windows so the user isn't left clicking around with broken auth
  tearDownSignedInWindows();
  createSplashWindow({ reason: 'session-expired' });
});

let autoUpdater: import('electron-updater').AppUpdater | null = null;

function semverGt(a: string, b: string): boolean {
  // Strip pre-release suffix (e.g. "1.2.0-beta.1" → "1.2.0") before comparing
  const parse = (v: string) => v.replace(/^v/, '').split('-')[0].split('.').map(Number);
  const [aMaj, aMin, aPat] = parse(a);
  const [bMaj, bMin, bPat] = parse(b);
  if (aMaj !== bMaj) return aMaj > bMaj;
  if (aMin !== bMin) return aMin > bMin;
  return aPat > bPat;
}

// ─── Update State Machine ─────────────────────────────────────────────────────
let updateState: UpdateState = {
  phase: 'idle',
  currentVersion: app.getVersion(),
  newVersion: null,
  progressPercent: 0,
  errorMessage: null,
};

let updateDeferralTimer: ReturnType<typeof setTimeout> | null = null;

function setUpdateState(partial: Partial<UpdateState>): void {
  updateState = { ...updateState, ...partial };
  broadcastUpdateState();
}

function broadcastUpdateState(): void {
  const targets: Array<BrowserWindowType | null | undefined> = [
    splashWindowInstance,
    trayMenuWindow,
    global.appSettingsWindow as BrowserWindowType | null,
  ];
  for (const win of targets) {
    if (win && !win.isDestroyed()) {
      win.webContents.send('update:state-changed', updateState);
    }
  }
}

/**
 * Single funnel for every updater failure. electron-updater reports the same
 * failure twice (it emits 'error' *and* rejects the promise), so all call sites
 * race to write this state — routing them through one function is what keeps
 * the phase and the message consistent no matter which one lands last.
 *
 * 'read-only-volume' gets its own phase rather than 'error': the update didn't
 * fail, the app is in the wrong place, and no amount of retrying fixes that.
 */
function setUpdateError(err: any): void {
  setUpdateState({
    phase: classifyUpdaterError(err) === 'read-only-volume' ? 'blocked' : 'error',
    errorMessage: updaterErrorMessage(err),
  });
}

/**
 * Pre-flight gate for both update entry points. Squirrel can't write to a
 * bundle running from a .dmg or an App Translocation mount, so calling into
 * electron-updater from there only produces a raw Squirrel error — answer with
 * the actionable message instead of letting the call through.
 *
 * Defence in depth: enforceApplicationsFolderLocation() in whenReady() means
 * no user, staging or production, should ever be here under normal operation —
 * this only fires if that check itself failed (see the fail-closed handling
 * in isOutsideApplicationsFolder()).
 */
function isUpdateBlockedByLocation(): boolean {
  if (!isOutsideApplicationsFolder()) return false;
  setUpdateState({ phase: 'blocked', errorMessage: READ_ONLY_VOLUME_MESSAGE });
  return true;
}

function isCoachSessionActive(): boolean {
  return audioManager.isCueActive();
}

function openSplashForUpdate(): void {
  if (splashWindowInstance && !splashWindowInstance.isDestroyed()) {
    splashWindowInstance.webContents.send('update:state-changed', updateState);
    splashWindowInstance.focus();
  } else {
    try {
      createSplashWindow();
    } catch (err) {
      Sentry.captureException(err);
    }
  }
}

function scheduleUpdateDeferralRecheck(): void {
  if (updateDeferralTimer) clearTimeout(updateDeferralTimer);
  updateDeferralTimer = setTimeout(() => {
    updateDeferralTimer = null;
    if (updateState.phase !== 'available') return;
    if (!isCoachSessionActive()) {
      openSplashForUpdate();
    } else {
      scheduleUpdateDeferralRecheck();
    }
  }, 10 * 60 * 1000);
}
// ─────────────────────────────────────────────────────────────────────────────

// Global error handler to prevent app crashes from unhandled exc eptions
// (e.g. native module failures on unsupported hardware)
process.on('uncaughtException', (error) => {
  console.error('[MAIN] Uncaught Exception:', error);
  Sentry.captureException(error);
  // Do NOT exit the process. This allows the app to stay alive
  // so the auto-updater can still run or the user can see an error UI.
});

if (app.isPackaged) {
  const { autoUpdater: updater } = require('electron-updater');
  const log = require('electron-log');

  updater.logger = log;
  updater.logger.transports.file.level = 'info';
  log.info('Auto-updater initialized');

  updater.autoDownload = false;
  updater.autoInstallOnAppQuit = false;
  updater.allowDowngrade = false;
  if (IS_STAGING) {
    updater.allowPrerelease = true;
    updater.channel = 'staging';
  }

  updater.on('checking-for-update', () => {
    log.info('Checking for updates...');
    setUpdateState({ phase: 'checking', errorMessage: null });
  });

  updater.on('update-available', (info: { version: string }) => {
    log.info('Update available:', info.version);
    const current = app.getVersion();
    if (!semverGt(info.version, current)) {
      log.warn(`[Updater] Ignoring update to ${info.version} — not newer than current ${current}`);
      setUpdateState({ phase: 'idle', newVersion: null });
      if (splashWindowInstance && !splashWindowInstance.isDestroyed()) {
        splashWindowInstance.webContents.send('update-check-complete');
      }
      return;
    }
    setUpdateState({ phase: 'available', newVersion: info.version });

    if (isCoachSessionActive()) {
      // Don't interrupt an active call — defer splash; tray entry still shows via broadcastUpdateState
      scheduleUpdateDeferralRecheck();
    } else {
      openSplashForUpdate();
    }
  });

  updater.on('update-not-available', (info: { version: string }) => {
    log.info('Update not available. Current version:', info.version);
    setUpdateState({ phase: 'idle', newVersion: null });
    // Signal splash to stop showing the loader (no update found)
    if (splashWindowInstance && !splashWindowInstance.isDestroyed()) {
      splashWindowInstance.webContents.send('update-check-complete');
    }
  });

  updater.on('error', (err: Error & { statusCode?: number }) => {
    log.error('Error in auto-updater:', err);
    if (classifyUpdaterError(err) === 'fatal') Sentry.captureException(err);
    setUpdateError(err);
  });

  updater.on('download-progress', (progressObj: { percent?: number; transferred?: number; total?: number; bytesPerSecond?: number }) => {
    const percent = progressObj.percent ?? 0;
    const speed = progressObj.bytesPerSecond ?? 0;
    log.info(`Download progress: ${percent.toFixed(1)}% - Speed: ${speed} bytes/sec`);
    setUpdateState({ phase: 'downloading', progressPercent: percent });
  });

  updater.on('update-downloaded', (info: { version: string }) => {
    log.info('Update downloaded:', info.version);
    setUpdateState({ phase: 'downloaded' });
    setImmediate(() => {
      try {
        // macOS: launchd sometimes pends Squirrel's submitted ShipIt job
        // instead of spawning it ("pended nondemand spawn = semaphore" in
        // `launchctl print`, `runs = 0`) — typically every update after the
        // first successful one since boot. The app then quits but never
        // installs or relaunches, with no error surfaced anywhere. Spawn a
        // detached watcher that outlives this process: once we exit, if the
        // ShipIt job was submitted but never ran, kick it manually.
        // `launchctl kickstart` on the pended job reliably completes the
        // install + relaunch; if Squirrel's own spawn worked (runs >= 1) the
        // watcher is a no-op, so it can never double-install.
        // The watchdog is best-effort: its own try/catch guarantees a spawn
        // failure degrades to "no watchdog", never to "no install".
        if (process.platform === 'darwin') {
          try {
            const shipItLabel = `${IS_STAGING ? 'com.asksayso.app.staging' : 'com.asksayso.app'}.ShipIt`;
            const watchdogLog = path.join(app.getPath('logs'), 'shipit-watchdog.log');
            // Ensure the log dir exists — a failed `exec >>` redirect would
            // abort the whole watchdog script.
            fs.mkdirSync(path.dirname(watchdogLog), { recursive: true });
            const watcherScript = [
              // Everything below lands in a log file so the outcome (no-op,
              // kickstart, timeout) is diagnosable after the fact.
              `exec >> "${watchdogLog}" 2>&1`,
              `echo "[$(date '+%Y-%m-%d %H:%M:%S')] armed for ${shipItLabel}; waiting on pid ${process.pid}"`,
              `i=0`,
              // Wait (max ~10 min) for the app process to exit.
              `while kill -0 ${process.pid} 2>/dev/null; do i=$((i+1)); [ $i -gt 600 ] && { echo "app never exited — giving up"; exit 0; }; sleep 1; done`,
              // Grace period for Squirrel's own ShipIt spawn to do its job.
              `sleep 6`,
              // Only intervene if the job is loaded but never ran. NOTE: this
              // parses `launchctl print` output ("runs = 0"), a private format
              // with no stability guarantee — if a macOS release changes it,
              // the watchdog degrades to a logged no-op, never a false kick.
              `if launchctl print "gui/$(id -u)/${shipItLabel}" 2>/dev/null | grep -q "runs = 0"; then`,
              `  echo "[$(date '+%Y-%m-%d %H:%M:%S')] ShipIt job never ran — kickstarting"`,
              `  launchctl kickstart "gui/$(id -u)/${shipItLabel}" && echo "kickstart ok" || echo "kickstart failed (exit $?)"`,
              `else`,
              `  echo "[$(date '+%Y-%m-%d %H:%M:%S')] ShipIt ran normally — no-op"`,
              `fi`,
            ].join('\n');
            spawn('/bin/sh', ['-c', watcherScript], { detached: true, stdio: 'ignore' }).unref();
            log.info(`[Updater] Spawned ShipIt watchdog for ${shipItLabel} (outcome log: ${watchdogLog})`);
          } catch (watchdogErr) {
            log.warn('[Updater] Failed to spawn ShipIt watchdog — continuing with install', watchdogErr);
          }
        }
        app.removeAllListeners('window-all-closed');
        updater.quitAndInstall(false, true);
      } catch (err) {
        Sentry.captureException(err);
        setUpdateState({
          phase: 'error',
          errorMessage: 'Failed to install update. Please restart the app manually.',
        });
      }
    });
  });

  autoUpdater = updater;
}

// Add file logging for production
function setupLogging() {
  // Always create a debug file to see what's happening
  const debugPath = path.join(app.getPath('userData'), 'debug-startup.log');
  
  try {
    const debugInfo = {
      timestamp: new Date().toISOString(),
      appName: app.getName(),
      nodeEnv: process.env.NODE_ENV,
      userDataPath: app.getPath('userData'),
      processType: process.type
    };

    fs.writeFileSync(debugPath, JSON.stringify(debugInfo, null, 2));
  } catch (error) {
    console.error('[DEBUG] Error creating debug file:', error);
    Sentry.captureException(error);
  }
  
  // Then proceed with normal logging setup
  if (process.env.NODE_ENV === 'production') {
    const logDir = path.join(app.getPath('userData'), 'logs');
    if (!fs.existsSync(logDir)) {
      fs.mkdirSync(logDir, { recursive: true });
    }
    
    const logFile = path.join(logDir, `sayso-${new Date().toISOString().split('T')[0]}.log`);
    
    // Redirect console.log to both console and file
    const originalLog = console.log;
    const originalError = console.error;
    const originalWarn = console.warn;
    
    function writeToFile(level: string, ...args: any[]) {
      const timestamp = new Date().toISOString();
      const message = args.map(arg => 
        typeof arg === 'object' ? JSON.stringify(arg, null, 2) : String(arg)
      ).join(' ');
      
      const logEntry = `[${timestamp}] [${level}] ${message}\n`;
      
      try {
        fs.appendFileSync(logFile, logEntry);
      } catch (err: any) {
        // Fallback to original console if file writing fails
        originalError(`Failed to write to log file: ${err.message}`);
      }
    }
    
    console.log = (...args) => {
      originalLog(...args);
      writeToFile('INFO', ...args);
    };
    
    console.error = (...args) => {
      originalError(...args);
      writeToFile('ERROR', ...args);
    };
    
    console.warn = (...args) => {
      originalWarn(...args);
      writeToFile('WARN', ...args);
    };
    
    console.log(`[MAIN] Logging to file: ${logFile}`);
  }
}

// ===== ENABLED FEATURES CACHE =====
let cachedEnabledFeatures: string[] = [];

async function fetchAndCacheEnabledFeatures(baseUrl: string, accessToken: string): Promise<void> {
  const res = await axios.get(`${baseUrl}/features/company`, {
    headers: { Authorization: `Bearer ${accessToken}` },
    timeout: 5000,
  });
  const features = res.data?.features;
  if (Array.isArray(features)) {
    cachedEnabledFeatures = features.filter((f: { enabled: boolean }) => f.enabled).map((f: { key: string }) => f.key);
    broadcastEnabledFeatures();
  }
}

function broadcastEnabledFeatures(): void {
  const payload = { enabledFeatures: cachedEnabledFeatures };
  BrowserWindow.getAllWindows().forEach((win: BrowserWindowType) => {
    if (!win.isDestroyed()) win.webContents.send('enabled-features-changed', payload);
  });
}

// ===== FONT SIZE CACHE =====
let cachedFontSize: string = 's';

const VALID_FONT_SIZES = new Set(['s', 'm', 'l']);

function applyFontSize(size: string) {
  if (!VALID_FONT_SIZES.has(size)) return;
  cachedFontSize = size;
  if (isCoachWindowOpen()) {
    global.coachWindow!.webContents.send('font-size-changed', size);
  }
  if (isPlaybookWindowOpen()) {
    global.playbookWindow!.webContents.send('font-size-changed', size);
  }
}

async function fetchAndCacheFontSize(baseUrl: string, accessToken: string): Promise<void> {
  const res = await axios.get(`${baseUrl}/sales-coach/settings`, {
    headers: { Authorization: `Bearer ${accessToken}` },
    timeout: 5000,
  });
  const size = res.data?.coachSettings?.font_size;
  if (size && VALID_FONT_SIZES.has(size)) {
    cachedFontSize = size;
  }
}

async function reportAppVersionIfChanged(baseUrl: string, accessToken: string, storedVersion: string | null | undefined): Promise<void> {
	const runningVersion = app.getVersion();
	if (runningVersion === storedVersion) return;
	const osLabel = process.platform === 'darwin' ? 'macOS' : process.platform === 'win32' ? 'Windows' : process.platform;
	const osVersion = process.getSystemVersion();
	await axios.put(
		`${baseUrl}/accounts/update-account`,
		{
		updateData: {
			desktop_app_latest_version: runningVersion,
			desktop_app_os: `${osLabel} ${osVersion}`,
			desktop_app_updated_at: new Date().toISOString(),
		},
		},
		{ headers: { Authorization: `Bearer ${accessToken}` }, timeout: 5000 }
	);
}

function maybeReportAppVersion(baseUrl: string, accessToken: string, user: AuthUser | false | null): void {
	if (!app.isPackaged || IS_STAGING || !user) return;
	reportAppVersionIfChanged(baseUrl, accessToken, user.desktop_app_latest_version as string | null | undefined).catch((err) => {
		console.warn('[MAIN] Failed to report app version:', err?.message);
		if (!isTransientNetworkError(err)) Sentry.captureException(err);
	});
}

// ===== HELPER FUNCTIONS =====
function isCoachWindowOpen() {
  return global.coachWindow && !global.coachWindow.isDestroyed();
}
function isAppSettingsWindowOpen() {
  return global.appSettingsWindow && !global.appSettingsWindow.isDestroyed();
}
function isPlaybookWindowOpen() {
  return global.playbookWindow && !global.playbookWindow.isDestroyed();
}

/**
 * Releases everything that only makes sense while a user is signed in: the Cue
 * websocket reconnect loop, which has no valid token to reconnect with, and the
 * secondary windows that assume an authenticated session.
 *
 * Shared by both ways a session ends — an explicit logout and an expired session.
 * Those two paths had drifted: only the expiry path closed the windows, so
 * logging out left the coach window floating on screen after the user was
 * already back at the login form. Keep them calling this, not their own copies.
 *
 * Idempotent: stop() early-returns when not streaming, and the window checks
 * guard against destroyed handles.
 */
function tearDownSignedInWindows(): void {
  // Stop the Cue reconnect loop (no valid token to reconnect with). The streamer
  // lives in audioManager now; this is the same shouldReconnect=false + stop(false).
  audioManager.stopCueForSessionExpired();
  if (isCoachWindowOpen()) global.coachWindow!.close();
  if (isPlaybookWindowOpen()) global.playbookWindow!.close();
  closeOnboardingWindowForSignOut();
}
function windowSourceFromEvent(event: Electron.IpcMainEvent): 'coach' | 'independent' {
  const coach = global.coachWindow;
  if (coach && !coach.isDestroyed() && event.sender === coach.webContents) {
    return 'coach';
  }
  return 'independent';
}
function broadcastAppSettingsWindowState(isOpen: boolean) {
  const payload = { isOpen };
  if (global.coachWindow && !global.coachWindow.isDestroyed()) {
    global.coachWindow.webContents.send('app-settings-window-state', payload);
  }
  if (trayMenuWindow && !trayMenuWindow.isDestroyed()) {
    trayMenuWindow.webContents.send('app-settings-window-state', payload);
  }
}
function broadcastPlaybookWindowState(isOpen: boolean) {
  const payload = { isOpen };
  if (global.coachWindow && !global.coachWindow.isDestroyed()) {
    global.coachWindow.webContents.send('playbook-window-state', payload);
  }
  if (trayMenuWindow && !trayMenuWindow.isDestroyed()) {
    trayMenuWindow.webContents.send('playbook-window-state', payload);
  }
}

// ===== CUSTOM TRAY MENU WINDOW =====
let tray: TrayType | null = null;
let trayMenuWindow: BrowserWindowType | null = null;
let trayMenuShownAt = 0;
const TRAY_BLUR_GRACE_MS = 250;
let onboardingWindowInstance: BrowserWindowType | null = null;
let onboardingClosedIntentionally = false;
let isAppQuitting = false;

// Ensure isAppQuitting is set before any window close events fire, regardless
// of whether the process is terminated via Cmd+Q, SIGTERM, or SIGINT.
process.on('SIGTERM', () => { isAppQuitting = true; app.quit(); });
process.on('SIGINT',  () => { isAppQuitting = true; app.quit(); });

function sendToOnboardingWindow(channel: string) {
  if (onboardingWindowInstance && !onboardingWindowInstance.isDestroyed()) {
    onboardingWindowInstance.webContents.send(channel);
  }
}

function closeOnboardingWindowForSignOut(): void {
  if (!onboardingWindowInstance || onboardingWindowInstance.isDestroyed()) return;
  console.log('[onboarding] closing — session ended');
  onboardingClosedIntentionally = true;
  onboardingWindowInstance.close();
}
/**
 * Creates and positions the custom tray menu window near the tray icon
 */
const TRAY_MENU_WIDTH = 230; 
function createTrayMenuWindow() {
  if (trayMenuWindow && !trayMenuWindow.isDestroyed()) {
    if (trayMenuWindow.isVisible()) {
      hideTrayMenu();
    } else {
      showTrayMenu();
    }
    return;
  }

  const preloadScriptPath = path.join(__dirname, 'preload.js');

  // Create a frameless, always-on-top window
  trayMenuWindow = new BrowserWindow({
    width: TRAY_MENU_WIDTH,
    height: 172,
    show: false,
    frame: false,
    transparent: true,
    resizable: false,
    movable: false,
    minimizable: false,
    maximizable: false,
    alwaysOnTop: true,
    skipTaskbar: true,
    hasShadow: true,
    vibrancy: ALLOW_VIBRANCY ? 'menu' : undefined,
    visualEffectState: ALLOW_VIBRANCY ? 'active' : undefined,
    backgroundColor: ALLOW_VIBRANCY ? '#00000000' : (nativeTheme.shouldUseDarkColors ? '#1f2937' : '#F9FAFB'),
    webPreferences: {
      preload: preloadScriptPath,
      contextIsolation: true,
      nodeIntegration: false,
      webSecurity: true,
    },
  });

  const trayMenuUrl = isDev
    ? 'http://localhost:5173/tray-menu.html'
    : `file://${path.join(__dirname, '../dist/tray-menu.html')}`;

  if (trayMenuWindow) {
    trayMenuWindow.loadURL(trayMenuUrl);
    
    trayMenuWindow.on('closed', () => {
        trayMenuWindow = null;
    });

    trayMenuWindow.on('blur', () => {
        // Ignore blur events that fire during/right after show — on macOS,
        // Space transitions (opening the menu from a fullscreen Space) cause
        // focus to flicker, which would otherwise hide the menu immediately.
        if (Date.now() - trayMenuShownAt < TRAY_BLUR_GRACE_MS) return;
        hideTrayMenu();
    });
    
    if (!ALLOW_VIBRANCY) {
      nativeTheme.on('updated', () => {
        if (trayMenuWindow && !trayMenuWindow.isDestroyed()) {
          trayMenuWindow.setBackgroundColor(nativeTheme.shouldUseDarkColors ? '#1f2937' : '#F9FAFB');
        }
      });
    }
  }
}

/**
 * Shows the tray menu window positioned near the tray icon
 */
function showTrayMenu() {
  const reveal = () => {
    if (!trayMenuWindow || trayMenuWindow.isDestroyed()) return;
    // Pin to all workspaces (incl. fullscreen) while visible so macOS doesn't
    // slide the user to the window's "home" Space. Revoked on hide.
    trayMenuWindow.setVisibleOnAllWorkspaces(true, { visibleOnFullScreen: true });
    positionTrayMenu();
    trayMenuShownAt = Date.now();
    trayMenuWindow.show();
    trayMenuWindow.focus();
    trayMenuWindow.webContents.send('coach-window-state', {
      isOpen: isCoachWindowOpen()
    });
    trayMenuWindow.webContents.send('playbook-window-state', {
      isOpen: isPlaybookWindowOpen()
    });
    trayMenuWindow.webContents.send('enabled-features-changed', {
      enabledFeatures: cachedEnabledFeatures
    });
  };

  if (!trayMenuWindow || trayMenuWindow.isDestroyed()) {
    createTrayMenuWindow();
    setTimeout(reveal, 100);
  } else {
    reveal();
  }
}

/**
 * Hides the tray menu window
 */
function hideTrayMenu() {
  if (trayMenuWindow && !trayMenuWindow.isDestroyed()) {
    trayMenuWindow.hide();
    // Revoke the all-workspaces pin set during show so the window doesn't
    // linger on other Spaces (incl. fullscreen) while hidden.
    trayMenuWindow.setVisibleOnAllWorkspaces(false, { visibleOnFullScreen: true });
  }
}

/**
 * Positions the tray menu window near the tray icon
 * macOS: positions below the menu bar on the right side
 */
function positionTrayMenu() {
  if (!trayMenuWindow || trayMenuWindow.isDestroyed() || !tray) return;

  const trayBounds = tray.getBounds();
  const windowBounds = trayMenuWindow.getBounds();

  // Use cursor position to identify which display the user clicked on.
  // tray.getBounds() can return coordinates for the primary display on macOS
  // even when the tray icon was clicked on a secondary display's menu bar.
  const cursorPoint = electronScreen.getCursorScreenPoint();
  const display = electronScreen.getDisplayNearestPoint(cursorPoint);
  const workArea = display.workArea;

  let x, y;

  if (process.platform === 'darwin') {
    // Center horizontally around the cursor (where the icon was clicked),
    // and place just below this display's menu bar.
    x = Math.round(cursorPoint.x - windowBounds.width / 2);
    y = Math.round(workArea.y + 5);

    // Clamp to this display's bounds
    if (x + windowBounds.width > workArea.x + workArea.width) {
      x = workArea.x + workArea.width - windowBounds.width - 5;
    }
    if (x < workArea.x) {
      x = workArea.x + 5;
    }
  } else if (process.platform === 'win32') {
    // Windows: Position above taskbar, aligned with tray icon
    x = Math.round(trayBounds.x + (trayBounds.width / 2) - (windowBounds.width / 2));
    y = Math.round(trayBounds.y - windowBounds.height - 5);
  } else {
    // Linux: Position below tray icon
    x = Math.round(trayBounds.x + (trayBounds.width / 2) - (windowBounds.width / 2));
    y = Math.round(trayBounds.y + trayBounds.height + 5);
  }

  trayMenuWindow.setPosition(x, y, false);
}

/**
 * Registers the tray icon and sets up click handlers
 */
function registerTrayIconMenu() {
  const trayIconFile = IS_STAGING ? 'staging-tray-icon44Template.png' : 'tray-icon44Template.png';
  const iconPath = path.join(__dirname, `../public/assets/${trayIconFile}`);

  let icon = nativeImage.createFromPath(iconPath);

  if (icon.isEmpty()) {
    console.error('Tray icon failed to load! Icon is empty.');
    Sentry.captureMessage('Tray icon failed to load - icon is empty', 'error');
    return;
  }

  icon = icon.resize({ width: 19, height: 19 });
  icon.setTemplateImage(true);

  tray = new Tray(icon);
  if (tray) {
    tray.setToolTip(IS_STAGING ? 'Sayso Staging' : 'Sayso');

    tray.on('click', () => {
      sendToOnboardingWindow('onboarding:tray-clicked');
      if (trayMenuWindow && !trayMenuWindow.isDestroyed() && trayMenuWindow.isVisible()) {
        hideTrayMenu();
      } else {
        showTrayMenu();
      }
    });

    tray.on('right-click', () => {
      if (trayMenuWindow && !trayMenuWindow.isDestroyed() && trayMenuWindow.isVisible()) {
        hideTrayMenu();
      } else {
        showTrayMenu();
      }
    });
  }
}

/**
 * Updates tray menu state when coach window opens/closes
 */
function updateTrayMenu() {
  if (trayMenuWindow && !trayMenuWindow.isDestroyed()) {
    trayMenuWindow.webContents.send('coach-window-state', {
      isOpen: isCoachWindowOpen()
    });
  }
}

// ===== GLOBAL SHORTCUTS =====
const shortcuts = [
  {
    // Open coach window widget
    fn: () => {
      if (global.networkState === 'reconnecting') return;
      if (!global.authUser || global.authUser?.subscription_plan_id === null) return;

      if (isCoachWindowOpen()) {
        global.coachWindow?.close();
      } else {
        createCoachWindow();
        sendToOnboardingWindow('onboarding:coach-opened');
      }
    },
    keyCombination: 'Control+S'
  },
  {
    // Toggle playbook window
    fn: () => {
      if (global.networkState === 'reconnecting') return;
      if (!global.authUser || global.authUser?.subscription_plan_id === null) return;
      if (!cachedEnabledFeatures.includes('playbooks')) return;

      if (isPlaybookWindowOpen()) {
        global.playbookWindow!.close();
      } else {
        createPlaybookWindow();
      }
    },
    keyCombination: 'Control+B'
  }
];

function setupGlobalShortcut() {
  shortcuts.forEach(({ fn, keyCombination }) => {
    globalShortcut.register(keyCombination, () => {
      fn();
    });
  })
}

function unregisterGlobalShortcuts() {
  shortcuts.forEach(({ keyCombination }) => {
    globalShortcut.unregister(keyCombination);
  })
}

// Load environment variables FIRST, before any other modules
if (!process.env.NODE_ENV) {
  process.env.NODE_ENV = 'production';
}


const CUE_STOP_PERSIST_TIMEOUT_MS = 5000;

/**
 * Ends an active Cue session end-to-end from main: tears down the capture stack
 * (via audioManager.stopCue) and persists the session server-side. `reason` is for
 * logging only.
 *
 * Until this existed, only the renderer could persist a session — POST
 * /cue/session/stop/:id lived solely in the coach window's store, so teardown was
 * unreachable from any main-process lifecycle event. Logging out cleared the tokens
 * first, so the renderer's stop went out with no Authorization header; the 401 meant
 * updateSupabaseSession() never ran and the session was lost when the Redis key
 * expired two hours later. Main owns the streamer and the token, so it does it here.
 *
 * Safe with no active session, and idempotent — audioManager.stopCue() dedupes via
 * its in-flight mutex and clears the streamer, so a second call returns immediately.
 * SAYSO-335.
 */
async function stopAndPersistCueSession(reason: string): Promise<void> {
  const sessionId = audioManager.getActiveCueSessionId();
  if (!sessionId) return;

  // Tear down capture first (local + fast), regardless of connectivity.
  await audioManager.stopCue();

  // Offline: skip the server persist rather than stall logout on a 5s POST that
  // can't succeed. The session reconciles server-side when the Redis key expires.
  if (global.networkState === 'reconnecting') {
    console.warn(`[MAIN] ${reason}: offline — cue session ${sessionId} not persisted`);
    return;
  }

  // Fetch a fresh token (refreshes if within 60s of expiry, shares the in-flight
  // refresh mutex) so the POST can't 401 on a stale cached global. Both callers still
  // hold a refresh token here — both callers run before their authManager.signOut()
  // (tray-logout and the auth:sign-out handler).
  const accessToken = await authManager.getAccessToken();
  if (!accessToken) {
    console.warn(`[MAIN] ${reason}: no access token — cue session ${sessionId} not persisted`);
    return;
  }

  try {
    await axios.post(`${backendBaseUrl()}/cue/session/stop/${sessionId}`, null, {
      headers: { Authorization: `Bearer ${accessToken}` },
      timeout: CUE_STOP_PERSIST_TIMEOUT_MS,
    });
    console.log(`[MAIN] ${reason}: cue session ${sessionId} stopped and persisted`);
  } catch (err: any) {
    console.warn(`[MAIN] ${reason}: cue stop failed —`, err?.message);
    if (!isTransientNetworkError(err)) Sentry.captureException(err);
  }
}
// Function to load environment variables
function loadEnvironmentVariables() {
  const isDev = process.env.NODE_ENV !== 'production';

  if (isDev) {
    require('dotenv').config({ path: path.resolve(__dirname, '../.env') });
    return;
  }

  const basePaths = [
    path.resolve(__dirname, '.env'),
    path.resolve(__dirname, '../.env'),
  ];
  for (const basePath of basePaths) {
    if (fs.existsSync(basePath)) {
      require('dotenv').config({ path: basePath });
      break;
    }
  }

  const envFile = IS_STAGING ? '.env.staging' : '.env.production';
  const possiblePaths = [
    path.resolve(__dirname, envFile),
    path.resolve(__dirname, `../${envFile}`),
    path.resolve(__dirname, `../dist/${envFile}`),
  ];

  let loaded = false;
  for (const envPath of possiblePaths) {
    if (fs.existsSync(envPath)) {
      require('dotenv').config({ path: envPath, override: true });
      loaded = true;
      break;
    }
  }

  if (!loaded) {
    console.warn(`[MAIN] ${envFile} not found in any expected location`);
  }
}

// Load environment variables
loadEnvironmentVariables();

// Now require other modules that depend on environment variables
const axios = require('axios');

// Add command line switches for better camera support
app.commandLine.appendSwitch('enable-features', 'WebRTC,MediaDevices,MediaStream');
app.commandLine.appendSwitch('enable-media-stream');
app.commandLine.appendSwitch('enable-usermedia-screen-capturing');
app.commandLine.appendSwitch('allow-running-insecure-content');
app.commandLine.appendSwitch('disable-web-security');
app.commandLine.appendSwitch('disable-features', 'VizDisplayCompositor');

// <<< Disable hardware acceleration >>>
// This can fix GPU process crashes on some systems
// app.disableHardwareAcceleration();
app.setAsDefaultProtocolClient('sayso');

// Handle creating/removing shortcuts on Windows when installing/uninstalling.
if (require('electron-squirrel-startup')) {
  app.quit();
}

// Enforce a single running instance. Without this lock the 'second-instance'
// event never fires, so a deep link opened while the app is already running
// (Windows/Linux) would spawn a new process instead of routing to the existing
// one. The OS forwards the second instance's argv to the primary via the
// 'second-instance' event.
//
// app.quit() is async and does NOT stop synchronous module execution, so the
// doomed second instance must skip deep-link setup and whenReady (gated on
// gotSingleInstanceLock below) — otherwise it would re-register listeners and
// race window creation before the quit lands.
const gotSingleInstanceLock = app.requestSingleInstanceLock();
if (!gotSingleInstanceLock) {
  app.quit();
}

// Prefer Electron's packaging flag to detect development vs production
const isDev = !app.isPackaged;

// Keep track of window instances
let splashWindowInstance: BrowserWindowType | null = null;

// --- Splash Window (Auth / Loading Screen) ---
const createSplashWindow = (opts: { logout?: boolean; reason?: 'session-expired' } | boolean = {}) => {
  // Support legacy boolean call sites (createSplashWindow(true))
  const { logout = false, reason } = typeof opts === 'boolean' ? { logout: opts, reason: undefined } : opts;

  if (splashWindowInstance && !splashWindowInstance.isDestroyed()) {
    if (reason) splashWindowInstance.webContents.send('splash:show-reason', reason);
    splashWindowInstance.focus();
    return;
  }

  const preloadScriptPath = path.join(__dirname, 'preload.js');

  const splashWindow = new BrowserWindow({
    show: false,
    width: 380,
    height: 560,
    center: true,
    resizable: false,
    maximizable: false,
    fullscreenable: false,
    roundedCorners: true,
    titleBarStyle: 'hiddenInset',
    // Matches the app's dark UI (rgba(2, 25, 47, 0.97)) so there's no white
    // flash when the renderer isn't painted over the native backing yet/anymore
    // (e.g. during the native close animation).
    backgroundColor: '#02192f',
    webPreferences: {
      preload: preloadScriptPath,
      contextIsolation: true,
      nodeIntegration: false,
      webSecurity: true,
    },
  });

  splashWindowInstance = splashWindow;

  const params = new URLSearchParams();
  if (logout) params.set('logout', 'true');
  if (reason) params.set('reason', reason);
  const query = params.toString() ? `?${params.toString()}` : '';
  const splashUrl = isDev
    ? `http://localhost:5173/splash-window.html${query}`
    : `file://${path.join(__dirname, '../dist/splash-window.html')}${query}`;

  splashWindow.once('ready-to-show', () => {
    splashWindow.show();
  });

  splashWindow.loadURL(splashUrl);

  if (isDev) {
    splashWindow.webContents.openDevTools();
  }

  splashWindow.on('closed', () => {
    splashWindowInstance = null;
  });
};

// Handler for opening URLs externally
ipcMain.on('open-external', (event: Electron.IpcMainInvokeEvent, url: string) => {
  try {
    shell.openExternal(url);

    // If it's a Slack OAuth URL, send reset-to-home
    if (url.includes('slack/auth')) {
      BrowserWindow.getAllWindows().forEach((win: BrowserWindowType) => {
        win.webContents.send('reset-to-home', { source: 'open-external-ipc', service: 'slack' });
      });
    }
  } catch (error) {
    console.error('[MAIN] [Electron][open-external] Error opening URL externally:', error);
    Sentry.captureException(error);
  }
});

// Permissions IPC (permissions-*) is registered via permissions.registerPermissionsIpc().

// Set when a `launch-coach` deep link arrives before the app is ready (cold
// launch via protocol). Flushed once `app.whenReady()` resolves — calling
// createCoachWindow() before 'ready' crashes because the `screen` module is
// not yet available (SAYSO-268).
let pendingLaunchCoach = false;

function openCoachFromProtocol() {
  if (!isCoachWindowOpen()) {
    createCoachWindow();
  } else {
    global.coachWindow?.focus();
  }
}

// Restore and focus an existing window. Used when a second instance launches
// (the user re-opened the app while it was already running).
function focusExistingWindow(): void {
  const win = global.coachWindow && !global.coachWindow.isDestroyed()
    ? global.coachWindow
    : (splashWindowInstance && !splashWindowInstance.isDestroyed() ? splashWindowInstance : null);
  if (win) {
    if (win.isMinimized()) win.restore();
    win.focus();
  }
}

// Platform-agnostic deep link router. Called from open-url (macOS) and from the
// process argv on Windows/Linux (cold launch via argv, warm launch via
// second-instance). Parsing is guarded so malformed input can't crash the
// main process.
function handleDeepLink(url: string): void {
  if (isDev) console.log('[Electron] deep link:', url);

  let urlObj: URL;
  try {
    urlObj = new URL(url);
  } catch {
    // Malformed sayso:// is OS/attacker-controlled input, not an app fault.
    // Report as a warning with the URL as context (stable grouping) rather than
    // an exception, so junk input can't flood Sentry with errors.
    console.warn('[Electron] Ignoring malformed deep link:', url);
    Sentry.withScope(scope => {
      scope.setLevel('warning');
      scope.setExtra('url', url);
      Sentry.captureMessage('Malformed deep link');
    });
    return;
  }

  if (urlObj.hostname === 'launch-coach') {
    // A cold launch (notably macOS open-url) can deliver this before whenReady
    // resolves; creating the coach window touches `screen`, which throws before
    // 'ready'. Defer and let the whenReady flush handle it.
    if (!app.isReady()) {
      pendingLaunchCoach = true;
      return;
    }
    openCoachFromProtocol();
  }
}

// Returns the first sayso:// deep link found in a process argv list, if any.
// Windows/Linux pass the protocol URL as a launch argument rather than via
// the macOS-only open-url event.
function findDeepLinkArg(argv: string[]): string | undefined {
  return argv.find(arg => arg.startsWith('sayso://'));
}

// Wires up sayso:// deep-link routing for the current platform. macOS and
// Windows/Linux deliver protocol activations through entirely different
// mechanisms, so each platform gets its own explicit branch.
function setupDeepLinkHandling(): void {
  if (IS_MAC) {
    // macOS delivers the URL through the open-url event — for both a cold
    // launch (fires before whenReady) and while the app is already running.
    app.on('open-url', (event: Event, url: string) => {
      event.preventDefault();
      handleDeepLink(url);
    });
    return;
  }

  // Windows/Linux deliver the URL as a process argument.
  // Warm launch: the OS starts a second process; with the single-instance lock
  // held, its argv is forwarded to the primary via 'second-instance'.
  app.on('second-instance', (_event: Event, argv: string[]) => {
    if (isDev) console.log('Second instance detected, command line:', argv);
    focusExistingWindow();
    const url = findDeepLinkArg(argv);
    if (url) handleDeepLink(url);
  });

  // Cold launch: the URL is in this process's own argv. handleDeepLink defers
  // window creation until ready (pendingLaunchCoach), just like macOS open-url,
  // so the whenReady flush below opens the window.
  const coldLaunchUrl = findDeepLinkArg(process.argv);
  if (coldLaunchUrl) handleDeepLink(coldLaunchUrl);
}

// Only the primary instance routes deep links and boots the app.
if (gotSingleInstanceLock) {
  setupDeepLinkHandling();
}

// When the system wakes from sleep, the network/DNS stack may not be ready for
// a few seconds. Firing network requests immediately produces ERR_NAME_NOT_RESOLVED
// / "fetch failed" errors that get reported to Sentry on every resume. This helper
// defers `fn` by an initial delay and retries with backoff — but only for transient
// DNS/connection errors — so the first attempt lands after the network has settled.
const RESUME_NETWORK_DELAY_MS = 4000;

// Labels currently running under a given dedupe `key`, so that e.g. the
// 'resume' and 'unlock-screen' events firing back-to-back (common when
// closing a laptop lid) don't each spin up their own overlapping retry chain.
const inFlightNetworkSettleKeys = new Set<string>();

function runAfterNetworkSettles(
  label: string,
  fn: () => Promise<unknown>,
  { 
    initialDelay = RESUME_NETWORK_DELAY_MS,
    retries = 3,
    backoff = 3000,
    key, 
    report = true 
  }: { 
    initialDelay?: number; 
    retries?: number; 
    backoff?: number; 
    key?: string; 
    report?: boolean 
  } = {}
): void {
  if (key) {
    if (inFlightNetworkSettleKeys.has(key)) {
      console.log(`[Resume] ${label} skipped — '${key}' already in flight`);
      return;
    }
    inFlightNetworkSettleKeys.add(key);
  }
  const done = () => {
    if (key) inFlightNetworkSettleKeys.delete(key);
  };

  let attempt = 0;
  const tryRun = () => {
    // Re-check on every attempt (not just at schedule time) — the OS can flip
    // networkState between when this was scheduled and when it actually runs,
    // and again between retries.
    if (global.networkState === 'reconnecting') {
      console.log(`[Resume] ${label} skipped — network still reconnecting`);
      done();
      return;
    }
    fn().then(done, (err) => {
      if (isTransientNetworkError(err) && attempt < retries) {
        const wait = backoff * Math.pow(2, attempt);
        attempt++;
        console.warn(`[Resume] ${label} transient failure, retry ${attempt}/${retries} in ${wait}ms`);
        setTimeout(tryRun, wait);
      } else {
        console.warn(`[Resume] ${label} failed:`, err?.message);
        if (report && !isTransientNetworkError(err)) Sentry.captureException(err);
        done();
      }
    });
  };
  setTimeout(tryRun, initialDelay);
}

// This method will be called when Electron has finished
// initialization and is ready to create browser windows.
// Some APIs can only be used after this event occurs.
app.whenReady().then(async () => {
  // Second instance is quitting (lock not acquired) — don't boot/create windows.
  if (!gotSingleInstanceLock) return;

  // Block before anything else can touch macOS-13-only APIs (native ScreenCaptureKit
  // audio capture chief among them — SAYSO-A3 crashed here on macOS 12).
  // LSMinimumSystemVersion is the first line of defense but isn't airtight; this is
  // the backstop and gives a real explanation instead of a hard crash.
  if (!enforceMinimumMacOSVersion()) return;

  // macOS-only seam: a bundle running from a .dmg or an App Translocation mount
  // can never update itself, and any permission granted from there is bound to
  // a path that disappears the moment the user moves the app. Block before ANY
  // window exists — must stay right after the OS-version gate above.
  if (!enforceApplicationsFolderLocation()) return;

  global.appSettingsWindowSource = null;
  global.playbookWindowSource = null;

  setupLogging();
  resetPermissionsIfCertChanged();

  // Run auto-updater check FIRST, before any potential native module crashes.
  // Skipped entirely from a read-only location (staging only in practice — the
  // guard above already sent production users to Applications or to quit):
  // both the immediate check and the hourly one would fail the same way, so
  // arm neither and leave the state on the actionable message.
  if (autoUpdater && !isUpdateBlockedByLocation()) {
    // Check immediately (with small delay to ensure network is ready)
    setTimeout(() => {
      autoUpdater.checkForUpdates().catch(err => {
        console.error('Failed to check for updates:', err);
      });
    }, 1000); // 1 second delay

    // Check every hour. JS timers are driven by wall-clock time, so on any OS
    // a setInterval whose tick elapsed during sleep fires the instant the
    // system wakes — route it through the resume-settle helper so it waits
    // for the network instead of hitting ERR_NAME_NOT_RESOLVED.
    setInterval(() => {
      if (!autoUpdater) return;
      runAfterNetworkSettles('auto-updater check', () => autoUpdater!.checkForUpdates(), { report: false });
    }, 60 * 60 * 1000);
  }

  app.on('browser-window-focus', () => {
    if (process.platform === 'darwin') {
      app.setBadgeCount(0);
    }
  });
  
  // Load native audio module AFTER logging is set up, then register the Cue IPC.
  // ORDERING CONSTRAINT: start-cue/stop-cue and permissions-* are registered here,
  // inside app.whenReady() and BEFORE any renderer window is created below. Keep it
  // that way — a window opened earlier in whenReady() would hit "No handler
  // registered for '<channel>'" for these channels.
  audioManager.initAudioProvider();
  audioManager.registerCueIpc({
    checkOSPermissionsGranted: permissions.checkOSPermissionsGranted,
    createSplashWindow,
    sendToOnboardingWindow,
  });
  permissions.registerPermissionsIpc();

  // Always attempt silent auth via AuthManager first.
  // init() reads the persisted refresh token, exchanges it for a fresh access
  // token, and schedules the proactive refresh timer. If it fails or there is
  // no stored token it returns cleanly and we fall through to the splash.
  await authManager.init();

  // Re-validate auth on system wake and screen unlock so the first API call
  // after a sleep/lock cycle never races a half-connected network.
  //
  // 'resume' fires on macOS, Windows, and Linux. 'unlock-screen' fires on
  // macOS and Windows only — Electron never emits it on Linux. Closing the
  // lid on mac/Windows commonly fires both in quick succession, which is why
  // they share the 'token-refresh' dedupe key below.
  //
  // retries:0 is deliberate — AuthManager arms its own backoff ladder
  // (1/3/10/30/60 s) on any transient refresh failure, so retrying here too
  // would run two independent chains for the same wake. We keep the initial
  // delay, which is the part AuthManager can't do, and let it own the rest.
  powerMonitor.on('resume', () => {
    if (global.networkState === 'reconnecting') return;
    console.log('[PowerMonitor] System resumed — scheduling delayed token refresh');
    runAfterNetworkSettles('token refresh (resume)', () => authManager.forceRefresh(), { key: 'token-refresh', retries: 0 });
  });

  powerMonitor.on('unlock-screen', () => {
    if (global.networkState === 'reconnecting') return;
    console.log('[PowerMonitor] Screen unlocked — scheduling delayed token refresh');
    runAfterNetworkSettles('token refresh (unlock)', () => authManager.forceRefresh(), { key: 'token-refresh', retries: 0 });
  });

  const authState = authManager.getState();
  if (authState.isAuthenticated) {
    if (!permissions.isPermissionsComplete()) {
      // Token restored but permissions flow was never completed — show splash.
      // PostAuthRedirect will see the user is authenticated and route to /permissions.
      console.log('[MAIN] Authenticated but permissions-complete flag missing — showing splash for permissions');
      createSplashWindow();
    } else {
      const headers = { Authorization: `Bearer ${authState.accessToken}` };

      // Fetch profile, font size, and enabled features in parallel before any window opens.
      const [profileResult, fontSizeResult, featuresResult] = await Promise.allSettled([
        axios.get(`${backendBaseUrl()}/accounts/${authState.user?.email}`, { headers, timeout: 5000 }),
        fetchAndCacheFontSize(backendBaseUrl(), authState.accessToken!),
        fetchAndCacheEnabledFeatures(backendBaseUrl(), authState.accessToken!),
      ]);

      if (profileResult.status === 'fulfilled') {
        // Route through setAuthUser so the tray is told, rather than assigning
        // global.authUser directly and leaving every listener stale.
        setAuthUser(profileResult.value.data.data);
      } else {
        console.warn('[MAIN] Silent auth succeeded but profile fetch failed — retrying in background', profileResult.reason);
        if (!isTransientNetworkError(profileResult.reason)) Sentry.captureException(profileResult.reason);
        scheduleProfileRetry(authState.user?.email);
      }

      if (fontSizeResult.status === 'rejected') {
        console.warn('[MAIN] Silent auth: font_size fetch failed — falling back to default S', fontSizeResult.reason);
        if (!isTransientNetworkError(fontSizeResult.reason)) Sentry.captureException(fontSizeResult.reason);
      }

      if (featuresResult.status === 'rejected') {
        console.warn('[MAIN] Silent auth: features fetch failed — no features enabled by default', featuresResult.reason);
        if (!isTransientNetworkError(featuresResult.reason)) Sentry.captureException(featuresResult.reason);
      }

	  maybeReportAppVersion(backendBaseUrl(), authState.accessToken!, global.authUser);

      replayPendingOnboardingStatus();
      // Open onboarding directly if not yet complete — no splash shown.
      if (shouldOpenOnboarding()) {
        console.log('[MAIN] Permissions complete but onboarding not done — opening onboarding window');
        createOnboardingWindow();
      }
    }
  } else if (authManager.isNetworkRetryPending()) {
    // Offline at startup — session exists but network was down during init().
    // Silent: no splash, tray boots in disabled state. Pause auth retries until
    // the renderer reports 'online'; the token-refreshed handler then runs the
    // deferred profile/features fetch.
    global.networkState = 'reconnecting';
    startupOfflinePending = true;
    authManager.pauseRefresh();
    console.log('[MAIN] Started offline — silent tray mode, awaiting network recovery');
  } else {
    createSplashWindow();
  }
  registerTrayIconMenu();
  setupGlobalShortcut();

  // Flush a launch-coach deep link that arrived during a cold launch, before
  // the app was ready — queued by setupDeepLinkHandling on any platform
  // (macOS open-url / Windows-Linux argv). Safe to create windows now (SAYSO-268).
  if (pendingLaunchCoach) {
    pendingLaunchCoach = false;
    openCoachFromProtocol();
  }

  app.on('activate', () => {
    // On macOS it's common to re-create a window in the app when the
    // dock icon is clicked and there are no other windows open.
    if (BrowserWindow.getAllWindows().length === 0) {
      createSplashWindow();
      setupGlobalShortcut();
    } else if (splashWindowInstance && !splashWindowInstance.isDestroyed()) {
      splashWindowInstance.restore();
      splashWindowInstance.focus();
    }
  });
});

// Cleanup audio capture before app quits
app.on('before-quit', async (event: Event) => {
  isAppQuitting = true;
  if (isDev) {
    console.log('App quitting - cleaning up audio capture...');
  }

  // Force cleanup of all audio capture before quitting
  await audioManager.cleanupAllAudioCapture();
  unregisterGlobalShortcuts();
});

// Modify window-all-closed to NOT quit if dashboard is meant to be main interface
app.on('window-all-closed', () => {
  // Standard macOS behavior: quit only if platform is not darwin
  if (process.platform !== 'darwin') {
    app.quit();
  }

  // If you want the app to quit when the dashboard closes even on macOS,
  // you would add app.quit() here.
});

let lastLeaveUrl: string | null = null;
let lastLeaveUrlTime = 0;
let isProcessingPostCall = false;

// Intercept navigation in ALL windows
app.on('web-contents-created', (event: Event, contents: WebContents) => {
  contents.on('will-navigate', (event: Event, url: string) => {
    if (url.includes('post-call')) {
      const now = Date.now();
      if ((url === lastLeaveUrl && now - lastLeaveUrlTime < 3000) || isProcessingPostCall) {
        event.preventDefault();
        return;
      }

      isProcessingPostCall = true;
      lastLeaveUrl = url;
      lastLeaveUrlTime = now;

      setTimeout(() => {
        isProcessingPostCall = false;
      }, 3000);

      event.preventDefault();
      shell.openExternal(url);
      const urlObj = new URL(url);
      const params = new URLSearchParams(urlObj.search);
      const meetingId = params.get('meetingId');
      const prospectId = params.get('prospectId');
      const sessionId = params.get('sessionId');
      BrowserWindow.getAllWindows().forEach((win: BrowserWindowType) => {
        win.webContents.send('reset-to-home', { meetingId, prospectId, sessionId });
      });
    }
  });

  contents.setWindowOpenHandler(({ url }: { url: string }) => {
    if (isDev) {
      console.log('[Electron][setWindowOpenHandler] Attempt to open URL:', url);
    }
    if (url.includes('post-call')) {
      const now = Date.now();
      // Enhanced duplicate prevention
      if ((url === lastLeaveUrl && now - lastLeaveUrlTime < 3000) || isProcessingPostCall) {
        if (isDev) {
          console.log('[Electron][DEBUG] Skipping duplicate post-call open in setWindowOpenHandler:', url);
        }
        return { action: 'deny' };
      }
      
      isProcessingPostCall = true;
      lastLeaveUrl = url;
      lastLeaveUrlTime = now;
      
      // Reset processing flag after a delay
      setTimeout(() => {
        isProcessingPostCall = false;
      }, 3000);
      
      shell.openExternal(url);
      // Extract meetingId and prospectId from query parameters
      const urlObj = new URL(url);
      const params = new URLSearchParams(urlObj.search);
      const meetingId = params.get('meetingId');
      const prospectId = params.get('prospectId');
      const sessionId = params.get('sessionId'); // Added sessionId extraction
      if (isDev) {
        console.log('[Electron][setWindowOpenHandler] Extracted params:', { meetingId, prospectId, sessionId });
      }
      BrowserWindow.getAllWindows().forEach((win: BrowserWindowType) => {
        if (isDev) {
          console.log('[Electron][setWindowOpenHandler] Sending reset-to-home to window:', win.id, { meetingId, prospectId, sessionId });
        }
        win.webContents.send('reset-to-home', { meetingId, prospectId, sessionId });
      });
      return { action: 'deny' };
    }
    return { action: 'allow' };
  });

  contents.session.webRequest.onBeforeRequest({ urls: ['*://*/*'] }, (details: Electron.OnBeforeRequestListenerDetails, callback: (response: { cancel?: boolean; redirectURL?: string }) => void) => {
    callback({});
  });
});


// ════════════════════════════════════════════════════════════════════════════
// IPC HANDLERS
//
// Every handler below is UNIVERSAL (identical behavior on darwin + win32) unless
// tagged otherwise. Platform-specific channels live in their provider modules,
// not here: audio/Cue → electron/audio/audioManager.ts (IAudioProvider);
// permissions → electron/permissions/permissionsManager.ts (IPermissionsProvider).
// Full classification: docs/IPC_CONTRACT.md. Do NOT branch on process.platform
// inside a handler — put OS differences behind a provider interface, or use the
// flags in utils/platform.ts for small presentational branches.
// ════════════════════════════════════════════════════════════════════════════

// Authenticated User
global.authUser = false;
/**
 * Handler for getting user auth state
 * Sends current authentication status to requesting window
 */
ipcMain.on('get-user-auth', (event: Electron.IpcMainInvokeEvent) => {
  event.sender.send('user-auth', {
    authUser: global.authUser
  });
});

ipcMain.handle('get-launch-at-login', () => {
  return app.getLoginItemSettings().openAtLogin;
});

ipcMain.handle('set-launch-at-login', (_event: Electron.IpcMainInvokeEvent, enabled: boolean) => {
  app.setLoginItemSettings({ openAtLogin: enabled, openAsHidden: true });
});

ipcMain.handle('auth:sign-in', async (_event, { email, password }: { email: string; password: string }) => {
  return authManager.signIn(email, password);
});

ipcMain.handle('auth:verify-mfa', async (_event, { factorId, code }: { factorId: string; code: string }) => {
  return authManager.verifyMFA(factorId, code);
});

ipcMain.handle('auth:sign-out', async () => {
  // Persist any in-flight cue session while the token is still valid. SAYSO-335.
  await stopAndPersistCueSession('sign-out');
  await authManager.signOut();
});

/**
 * Returns a valid access token, proactively refreshing if within 60 s of expiry.
 * All windows call this instead of caching a token themselves.
 */
ipcMain.handle('auth:get-token', async () => {
  return authManager.getAccessToken();
});

/**
 * Forces an immediate token refresh, bypassing the 60-second proactive window.
 * Returns a discriminated result so the renderer can distinguish permanent
 * session expiry (invalid_grant) from a transient network failure — without
 * relying on IPC error serialisation which strips typed error fields.
 */
ipcMain.handle('auth:force-refresh-token', async () => {
  try {
    const token = await authManager.forceRefresh();
    if (token === null) return { ok: false, kind: 'invalid_grant' } as const;
    return { ok: true, token } as const;
  } catch {
    return { ok: false, kind: 'transient' } as const;
  }
});

ipcMain.handle('auth:get-state', () => {
  return authManager.getState();
});

// ─── Network state (renderer-driven) ────────────────────────────────────────
// Renderers report online/offline via OS events (window.addEventListener).
// Main mirrors the state globally and broadcasts so all windows stay in sync.
global.networkState = 'online';

ipcMain.on('network:report-status', (_event, status: 'online' | 'offline') => {
  const next = status === 'offline' ? 'reconnecting' : 'online';
  if (global.networkState === next) return;
  console.log('[Network] state changed:', next);
  global.networkState = next;
  broadcastToAllWindows('network:state-changed', next);

  if (next === 'reconnecting') {
    // OS says we're offline — pause both the proactive refresh timer and any
    // pending backoff retry. forceRefresh() on the 'online' event lifts the pause.
    authManager.pauseRefresh();
  } else {
    // OS says we're back online — trigger one immediate refresh instead of
    // waiting for the next scheduled tick.
    authManager.forceRefresh().catch((err) => {
      console.warn('[Network] forceRefresh on reconnect failed:', err?.message);
    });
    // The profile backoff gives up permanently after ~52 s. If we were offline
    // for longer than that at boot the session would stay profile-less until
    // restart, which is the state SAYSO-338 is about. Re-arm it here for free.
    const authState = authManager.getState();
    if (authState.isAuthenticated && !global.authUser) {
      console.log('[Network] Back online without a profile — re-arming the profile retry');
      scheduleProfileRetry(authState.user?.email);
    }
  }
});

ipcMain.handle('network:get-state', () => global.networkState);

// ─────────────────────────────────────────────────────────────────────────────

/**
 * Handler for updating user auth state
 *
 * SAYSO-338: a renderer is never the authority on whether a session exists.
 * The splash's AuthProvider mounts with `user === null` — it has to read the
 * real state from main over async IPC — and an effect reports that initial
 * null up here before the answer arrives. That wiped a perfectly good profile
 * and logged the tray out on a valid session, with no path back short of a
 * restart. Ignore any clearing update while AuthManager still holds a session;
 * genuine clears (sign-out, session-expired) always run after AuthManager has
 * already dropped it, so they still get through.
 */
ipcMain.on('update-user-auth', (_event: Electron.IpcMainInvokeEvent, { userAuthenticated }: { userAuthenticated: AuthUser | null }) => {
  if (!userAuthenticated && authManager.getState().isAuthenticated) {
    console.log('[MAIN] update-user-auth: ignored a null from a renderer — AuthManager still holds a session');
    return;
  }
  setAuthUser(userAuthenticated);
})
// Handle for opening Coach settings window
ipcMain.on('open-app-settings-window', (event: Electron.IpcMainEvent) => {
    createAppSettingsWindow(undefined, windowSourceFromEvent(event));
})
ipcMain.on('close-app-settings-window', () => {
    if (global.appSettingsWindow && !global.appSettingsWindow.isDestroyed()) {
        global.appSettingsWindow.close();
        global.appSettingsWindow = null;
    } else {
        global.appSettingsWindow = null;
    }
})

ipcMain.on('app-settings:session-expired-redirect', () => {
    createSplashWindow({ reason: 'session-expired' });
    if (global.appSettingsWindow && !global.appSettingsWindow.isDestroyed()) {
        global.appSettingsWindow.close();
        global.appSettingsWindow = null;
    } else {
        global.appSettingsWindow = null;
    }
})
ipcMain.on('get-app-settings-window-state', (event: Electron.IpcMainInvokeEvent) => {
    event.sender.send('app-settings-window-state', {
        isOpen: isAppSettingsWindowOpen()
    })
})
ipcMain.handle('get-app-settings-window-open-state', () => {
    return isAppSettingsWindowOpen();
})

// Handler for opening coach window — checks mic permission first; if missing, opens splash for permissions flow.
// Classification: universal (the mic pre-flight gate is platform-dispatched via the permissions provider).
ipcMain.on('open-coach-window', async () => {
  if (global.networkState === 'reconnecting') return;
  // Mic-only gate, routed through the permissions provider (isMicGranted avoids
  // the screen-recording preflight this handler doesn't need). Fail closed: any
  // provider error routes to the splash/permissions flow rather than silently
  // leaving the coach window unopened.
  let micGranted = false;
  try {
    micGranted = await permissions.isMicGranted();
  } catch (e) {
    console.error('[MAIN] open-coach-window: mic permission check failed:', e);
    Sentry.captureException(e);
  }
  if (!micGranted) {
    createSplashWindow();
    return;
  }
  createCoachWindow();
  sendToOnboardingWindow('onboarding:coach-opened');
});
ipcMain.on('close-coach-window', () => {
  if (global.coachWindow && !global.coachWindow.isDestroyed()) {
    global.coachWindow.close();
  }
  global.coachWindow = null;
});
// Handler for getting coach window state
ipcMain.on('get-coach-window-state', (event: Electron.IpcMainInvokeEvent) => {
  event.sender.send('coach-window-state', {
    isOpen: isCoachWindowOpen()
  });
});
// Handler for getting coach window state (async version for invoke)
ipcMain.handle('get-coach-window-open-state', () => {
  return isCoachWindowOpen();
});

ipcMain.on('get-enabled-features', (event: Electron.IpcMainInvokeEvent) => {
  event.sender.send('enabled-features-changed', { enabledFeatures: cachedEnabledFeatures });
});

// ─── Update IPC handlers ──────────────────────────────────────────────────────
ipcMain.handle('update:get-state', () => updateState);

ipcMain.handle('app:get-version', () => app.getVersion());

ipcMain.on('update:start-download', () => {
  if (global.networkState === 'reconnecting') return;
  if (!autoUpdater || updateState.phase !== 'available') return;

  // Close coach window before downloading
  if (global.coachWindow && !global.coachWindow.isDestroyed()) {
    global.coachWindow.close();
    global.coachWindow = null;
  }

  autoUpdater.downloadUpdate().catch((err: Error) => {
    // Reporting is owned by updater.on('error') — see the startup check.
    console.error('[Updater] Download failed:', err);
    setUpdateError(err);
  });
});

ipcMain.on('update:dismiss', () => {
  // State intentionally stays 'available' — tray entry persists so user can update later
});

ipcMain.on('update:check-for-updates', () => {
  if (updateState.phase === 'downloading' || updateState.phase === 'downloaded') return;

  // Before 'checking', so the tab never shows a spinner for a check that can't
  // succeed. Re-probed on every click rather than cached: the user may have
  // moved the app since launch.
  if (isUpdateBlockedByLocation()) return;

  // Flip to 'checking' immediately so the UI reflects the click without depending
  // on autoUpdater's 'checking-for-update' event timing (which can race against
  // 'update-not-available' on fast networks).
  setUpdateState({ phase: 'checking', errorMessage: null });

  if (!autoUpdater) {
    // Dev mode: no real auto-updater is wired up (only initialized when app.isPackaged).
    // Flip back to idle after a short delay so the loading UX is testable locally.
    setTimeout(() => setUpdateState({ phase: 'idle' }), 1500);
    return;
  }

  autoUpdater.checkForUpdates().catch((err: Error) => {
    // Reporting is owned by updater.on('error') — see the startup check.
    console.error('[Updater] Check failed:', err);
    setUpdateError(err);
  });
});

ipcMain.on('app-settings:open-update-tab', () => {
  if (global.networkState === 'reconnecting') return;
  createAppSettingsWindow('software-update');
});
// ─────────────────────────────────────────────────────────────────────────────

ipcMain.on('set-font-size', (_event, size: string) => {
  applyFontSize(size);
});

ipcMain.on('open-onboarding-window', () => {
  if (splashWindowInstance && !splashWindowInstance.isDestroyed()) {
    console.log('[MAIN] open-onboarding-window: blocked — splash still open');
    return;
  }
  createOnboardingWindow();
});

ipcMain.on('onboarding:set-status', (_event, status: OnboardingStatus) => {
  if (status !== 'complete' && status !== 'dismissed') return;
  setOnboardingStatus(status);
});

ipcMain.on('close-onboarding-window', (_event) => {
  onboardingClosedIntentionally = true;
  const win = BrowserWindow.fromWebContents(_event.sender);
  if (win && !win.isDestroyed()) win.close();
});

ipcMain.on('complete-onboarding', (_event) => {
  onboardingClosedIntentionally = true;
  const win = BrowserWindow.fromWebContents(_event.sender);
  if (win && !win.isDestroyed()) win.close();
});

// Handler for the splash window to signal successful login — closes splash, then opens onboarding if needed
ipcMain.on('splash-login-success', async () => {
  // Wait for the sign-in profile fetch so the gate reads a fresh
  // onboarding_status rather than a stale `false` from before login.
  await authUserReady;
  // A status written in a previous session that never reached the server would
  // otherwise only replay on a launch that happens to go through silent auth,
  // leaving the pending file to sit in userData indefinitely. SAYSO-338.
  replayPendingOnboardingStatus();
  const openOnboarding = shouldOpenOnboarding();
  console.log('[MAIN] splash-login-success — profile:', global.authUser ? 'present' : 'missing', '| opening onboarding:', openOnboarding);

  if (splashWindowInstance && !splashWindowInstance.isDestroyed()) {
    splashWindowInstance.once('closed', () => {
      if (openOnboarding) {
        createOnboardingWindow();
      }
    });
    splashWindowInstance.close();
  } else if (openOnboarding) {
    createOnboardingWindow();
  }
});

// Handler for showing the splash window from the tray menu (e.g. Log In)
ipcMain.on('tray-show-window', () => {
  if (!splashWindowInstance || splashWindowInstance.isDestroyed()) {
    createSplashWindow();
  } else {
    if (splashWindowInstance.isMinimized()) splashWindowInstance.restore();
    splashWindowInstance.show();
    splashWindowInstance.focus();
  }
});

// Handler for triggering logout from the tray menu — opens splash window with sign-out flag
ipcMain.on('tray-logout', async () => {
  hideTrayMenu();
  // Must run before the session is torn down: once the refresh token is gone,
  // getAccessToken() returns null and the stop call can only 401. SAYSO-335.
  await stopAndPersistCueSession('tray-logout');
  // End the session through AuthManager rather than clearing the token store
  // behind its back. clearRefreshToken() wiped the persisted token and nothing
  // else — AuthManager kept reporting isAuthenticated, and since the tray now
  // derives its logged-in state from auth:state / auth:get-state (SAYSO-338),
  // it went on rendering a logged-in menu for a dead session with no Log In row
  // to click. The tray window is built once and only hidden on blur, so its
  // one-shot auth:get-state never re-ran and nothing corrected it short of a
  // restart. signOut() clears the session, clears the token and emits
  // 'signed-out', whose handler already does the profile clear, the onboarding
  // flag reset, tearDownSignedInWindows() and the auth:state broadcast — so the
  // splash's LogoutGate round-trip is now a redundant no-op rather than the
  // only thing standing between a logout and a coherent state. SAYSO-335/338.
  await authManager.signOut();
  if (!splashWindowInstance || splashWindowInstance.isDestroyed()) {
    createSplashWindow({ logout: true });
  } else {
    const logoutUrl = isDev
      ? 'http://localhost:5173/splash-window.html?logout=true'
      : `file://${path.join(__dirname, '../dist/splash-window.html')}?logout=true`;
    if (splashWindowInstance.isMinimized()) splashWindowInstance.restore();
    splashWindowInstance.show();
    splashWindowInstance.focus();
    splashWindowInstance.loadURL(logoutUrl);
  }
});

// Handler for resizing the tray menu window (e.g. when items are shown/hidden)
ipcMain.on('set-tray-menu-height', (_event: Electron.IpcMainEvent, height: number) => {
  if (trayMenuWindow && !trayMenuWindow.isDestroyed()) {
    trayMenuWindow.setSize(TRAY_MENU_WIDTH, height, false);
    positionTrayMenu();
  }
});

// Handler for quitting the app
ipcMain.on('quit-app', () => {
  app.quit();
});

// --- Coach Settings Window ---
const createOnboardingWindow = (tab?: string) => {
  if (onboardingWindowInstance && !onboardingWindowInstance.isDestroyed()) {
    onboardingWindowInstance.focus();
    return;
  }

  if (onboardingWindowInstance && onboardingWindowInstance.isDestroyed()) {
    onboardingWindowInstance = null;
  }

  onboardingClosedIntentionally = false;

  const preloadScriptPath = path.join(__dirname, 'preload.js');
  const onboardingWindow = new BrowserWindow({
    width: 720,
    height: 560,
    titleBarStyle: 'hiddenInset',
    resizable: false,
    maximizable: false,
    minimizable: false,
    fullscreenable: false,
	backgroundColor: '#2a3f5f',
	roundedCorners: true,
    webPreferences: {
      preload: preloadScriptPath,
      contextIsolation: true,
      nodeIntegration: false,
      webSecurity: true,
    },
  });

  const onboardingUrl = isDev
    ? 'http://localhost:5173/onboarding-window.html'
    : `file://${path.join(__dirname, '../dist/onboarding-window.html')}`;

  onboardingWindow.loadURL(onboardingUrl);

  onboardingWindowInstance = onboardingWindow;

  onboardingWindow.on('close', (e) => {
    console.log('[onboarding] close event fired — intentionally:', onboardingClosedIntentionally, '| isAppQuitting:', isAppQuitting);
    if (!onboardingClosedIntentionally && !isAppQuitting) {
      setOnboardingStatus('dismissed');
    }
    onboardingClosedIntentionally = false;
  });

  onboardingWindow.on('closed', () => { onboardingWindowInstance = null; });

  if (isDev) {
    onboardingWindow.webContents.openDevTools({ mode: 'detach' });
  }
};

const createAppSettingsWindow = (tab?: string, source: 'coach' | 'independent' = 'independent') => {
    if (global.appSettingsWindow && !global.appSettingsWindow.isDestroyed()) {
        if (isDev) {
            console.log('Coach window already exists and is not destroyed, focusing...');
        }
        global.appSettingsWindowSource = source;
        global.appSettingsWindow.focus();
        if (tab) {
            global.appSettingsWindow.webContents.send('app-settings:navigate-to-update');
        }
        return;
    }

    if (global.appSettingsWindow && global.appSettingsWindow.isDestroyed()) {
        global.appSettingsWindow = null;
    }

    global.appSettingsWindowSource = source;

    const preloadScriptPath = path.join(__dirname, 'preload.js');
    const windowConfig = WindowManager.getAppSettingsWindowConfig();
    const appSettingsWindow = new BrowserWindow({
        ...windowConfig,
        icon: path.join(__dirname, '../public/assets/icon.icns'),
        titleBarStyle: 'hiddenInset',
        titleBarOverlay: {
          color: '#02192f',
          symbolColor: '#FFF',
          height: 30,
        },
        webPreferences: {
            preload: preloadScriptPath,
            contextIsolation: true,
            nodeIntegration: false,
            webSecurity: true,
            enableBlinkFeatures: 'MediaDevices,MediaStream,WebRTC',
            allowRunningInsecureContent: false,
            experimentalFeatures: false
        },
    });

    global.appSettingsWindow = appSettingsWindow;

    const tabParam = tab ? `?tab=${tab}` : '';
    const appSettingsUrl = isDev
      ? `http://localhost:5173/app-settings-window.html${tabParam}`
      : `file://${path.join(__dirname, '../dist/app-settings-window.html')}${tabParam}`;

    appSettingsWindow.loadURL(appSettingsUrl).catch((err: Error) => {
        Sentry.captureException(err);
    });
    broadcastAppSettingsWindowState(true);

    appSettingsWindow.on('closed', () => {
        global.appSettingsWindow = null;
        global.appSettingsWindowSource = null;
        broadcastAppSettingsWindowState(false);
    })
}

// --- Coach Window (Sales Coach Interface) ---
const createCoachWindow = () => {
  if (global.coachWindow && !global.coachWindow.isDestroyed()) {
    if (isDev) {
      console.log('Coach window already exists and is not destroyed, returning...');
    }
    return;
  }
  
  if (global.coachWindow && global.coachWindow.isDestroyed()) {
    global.coachWindow = null;
  }
  
  const preloadScriptPath = path.join(__dirname, 'preload.js');
  
  const windowConfig = WindowManager.getCoachWindowConfig();
  
  const coachWindow = new BrowserWindow({
    ...windowConfig,
    icon: path.join(__dirname, '../public/assets/icon.icns'),
    webPreferences: {
      preload: preloadScriptPath,
      contextIsolation: true,
      nodeIntegration: false,
      webSecurity: true,
      enableBlinkFeatures: 'MediaDevices,MediaStream,WebRTC',
      // permissions: ['media', 'microphone'], 'permissions' does not exist in type 'WebPreferences'.
      allowRunningInsecureContent: false,
      experimentalFeatures: false
    },
  });

  global.coachWindow = coachWindow;

  // dev vs prod URL for the coach window (use the HTML that bootstraps src/coachWindow/index.jsx)
  const coachUrl = isDev
    ? `http://localhost:5173/coach-window.html?fontSize=${cachedFontSize}`
    : `file://${path.join(__dirname, '../dist/coach-window.html')}?fontSize=${cachedFontSize}`;

  coachWindow.loadURL(coachUrl);
  
  if (isDev) {
    // coachWindow.webContents.openDevTools();
  }

  coachWindow.on('closed', async () => {
    if (isDev) {
      console.log('Coach window closed event fired, cleaning up reference');
    }

    // Force cleanup of all audio capture when coach window closes
    await audioManager.cleanupAllAudioCapture();

    global.coachWindow = null;

    // Close Settings/Playbooks windows that were opened from Coach
    if (global.appSettingsWindowSource === 'coach' && isAppSettingsWindowOpen()) {
      global.appSettingsWindow!.close();
    }
    if (global.playbookWindowSource === 'coach' && isPlaybookWindowOpen()) {
      global.playbookWindow!.close();
    }

    updateTrayMenu();
  });

  if (isDev) {
    console.log('Coach window created successfully at position:', { x: windowConfig.x, y: windowConfig.y });
  }

  updateTrayMenu();
};

// Update the resize handler to use WindowManager
ipcMain.on('resize-coach-window', (event: Electron.IpcMainInvokeEvent, width: number, height: number) => {
  if (global.coachWindow) {
    WindowManager.resizeCoachWindow(global.coachWindow, width, height);
  }
});

ipcMain.handle('get-coach-work-area-bottom', () => {
  if (global.coachWindow && !global.coachWindow.isDestroyed()) {
    const { workArea } = electronScreen.getDisplayMatching(global.coachWindow.getBounds());
    return workArea.y + workArea.height;
  }
  return null;
});

// Handler for manual window dragging
ipcMain.handle('get-window-position', () => {
  if (global.coachWindow && !global.coachWindow.isDestroyed()) {
    return global.coachWindow.getPosition();
  }
  return [0, 0];
});

ipcMain.on('set-window-position', (event: Electron.IpcMainInvokeEvent, x: number, y: number) => {
  if (global.coachWindow && !global.coachWindow.isDestroyed()) {
    global.coachWindow.setPosition(Math.round(x), Math.round(y));
  }
});

// --- Playbook Window ---
const createPlaybookWindow = (source: 'coach' | 'independent' = 'independent') => {
  if (global.playbookWindow && !global.playbookWindow.isDestroyed()) {
    global.playbookWindowSource = source;
    return;
  }

  if (global.playbookWindow && global.playbookWindow.isDestroyed()) {
    global.playbookWindow = null;
  }

  global.playbookWindowSource = source;

  const preloadScriptPath = path.join(__dirname, 'preload.js');
  const coachBounds = isCoachWindowOpen() ? global.coachWindow!.getBounds() : undefined;
  const windowConfig = WindowManager.getPlaybookWindowConfig(coachBounds);

  const playbookWindow = new BrowserWindow({
    ...windowConfig,
    icon: path.join(__dirname, '../public/assets/icon.icns'),
    webPreferences: {
      preload: preloadScriptPath,
      contextIsolation: true,
      nodeIntegration: false,
      webSecurity: true,
      allowRunningInsecureContent: false,
      experimentalFeatures: false
    }
  });

  global.playbookWindow = playbookWindow;

  const playbookUrl = isDev
    ? `http://localhost:5173/playbook-window.html?fontSize=${cachedFontSize}`
    : `file://${path.join(__dirname, '../dist/playbook-window.html')}?fontSize=${cachedFontSize}`;

  playbookWindow.loadURL(playbookUrl);
  broadcastPlaybookWindowState(true);

  playbookWindow.on('closed', () => {
    global.playbookWindow = null;
    global.playbookWindowSource = null;
    broadcastPlaybookWindowState(false);
  });
};

ipcMain.on('open-playbook-window', (event: Electron.IpcMainEvent) => {
  if (global.networkState === 'reconnecting') return;
  createPlaybookWindow(windowSourceFromEvent(event));
});

ipcMain.on('close-playbook-window', () => {
  if (isPlaybookWindowOpen()) {
    global.playbookWindow!.close();
  }
});

ipcMain.on('get-playbook-window-state', (event: Electron.IpcMainInvokeEvent) => {
  event.sender.send('playbook-window-state', { isOpen: isPlaybookWindowOpen() });
});

ipcMain.handle('get-playbook-window-position', () => {
  if (isPlaybookWindowOpen()) {
    return global.playbookWindow!.getPosition();
  }
  return [0, 0];
});

ipcMain.on('set-playbook-window-position', (_event: Electron.IpcMainInvokeEvent, x: number, y: number) => {
  if (isPlaybookWindowOpen()) {
    global.playbookWindow!.setPosition(Math.round(x), Math.round(y));
  }
});

// --- Playbooks data cache (prewarmed by coach window, consumed by playbook window) ---
ipcMain.on('set-playbooks-cache', (_event, payload: { playbooks: unknown[] | null; error: string | null }) => {
  global.playbooksCache = payload;
  if (isPlaybookWindowOpen()) {
    global.playbookWindow!.webContents.send('playbooks-updated', payload);
  }
});

ipcMain.handle('get-playbooks-cache', () => {
  return global.playbooksCache ?? { playbooks: null, error: null };
});
