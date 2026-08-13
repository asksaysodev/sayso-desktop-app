import { app, ipcMain } from 'electron';
import * as Sentry from '@sentry/electron/main';

import type { IAudioProvider, AudioFormat } from './IAudioProvider';

// recorder + audioStreamer are CommonJS modules. Require them LAZILY (not at
// module top): recorder.ts eagerly pulls in the native audio chain
// (require('./audio') → MacAudioProvider → native-audio, which throws if the
// native binary is missing / ABI-mismatched). This module is imported by main.ts
// BEFORE Sentry.init() + setupLogging() run, so a top-level require here would
// turn a bad native load into a silent, unreported pre-Sentry crash. Deferring to
// first cue-start keeps that load inside initAudioProvider()'s guarded path (and
// well after logging/Sentry are configured).
function getRecorder(): { startUserStreaming: Function; stopUserStreaming: Function } {
  return require('../recorder');
}
function getAudioStreamerCtor(): any {
  return require('../streaming/audioStreamer').AudioStreamer;
}

// Match main.ts: app.isPackaged is reliable at module-load time; NODE_ENV is not
// yet set when this module is imported, so keying off it would leak dev logging
// into packaged production builds.
const isDev = !app.isPackaged;

const CUE_MIC_JS_WARMUP_MS = 500;
const CUE_MIC_JS_RESTART_WAIT_MS = 700;

// Platform audio provider (ScreenCaptureKit on macOS; stubbed on Windows).
// Loaded via initAudioProvider() (called from main after logging/Sentry are set
// up) so the native module's first load happens inside a guarded, reported path
// — see the lazy getRecorder/getAudioStreamerCtor note above.
let provider: IAudioProvider | null = null;

// Active Cue streamer (2 audio websockets: user + prospect). Null when idle.
let cueAudioStreamer: any = null;

/** Serialize stop-cue: overlapping IPC invokes await the same teardown (no double native stop). */
let cueStopInFlight: Promise<{ success: boolean; error?: string; deduped?: boolean }> | null = null;

/** Serialize start-cue: overlapping invokes (double-click, renderer retry) dedupe onto the same
 *  in-flight start instead of interleaving through its multi-await body (SAYSO-355). */
let cueStartInFlight: Promise<{ success: boolean; error?: string; sessionId?: string; mic?: boolean; screen?: boolean }> | null = null;

/** Per-session telemetry: chunks + bytes forwarded from native callbacks into AudioStreamer */
let cueCaptureStats = {
  userChunks: 0,
  prospectChunks: 0,
  userBytes: 0,
  prospectBytes: 0
};

let cueLowAudioTimer: ReturnType<typeof setTimeout> | null = null;

/** SAYSO-353: mid-session mic-stall watchdog — see startCueMicStallWatchdog. */
const CUE_MIC_STALL_CHECK_MS = 8000;
let cueMicStallTimer: ReturnType<typeof setInterval> | null = null;
let cueMicStallLastUserChunks = 0;
let cueMicStallRecovering = false;

/** Onboarding-window notifier, injected via registerCueIpc. Used by stopCue(). */
let notifyOnboarding: (channel: string) => void = () => {};

/** Dependencies injected by main so this module stays free of window/permission internals. */
export interface CueIpcDeps {
  checkOSPermissionsGranted: () => Promise<{ granted: boolean; mic: boolean; screen: boolean }>;
  createSplashWindow: () => void;
  sendToOnboardingWindow: (channel: string) => void;
}

// ─── Provider lifecycle ───────────────────────────────────────────────────────

/** Load the platform audio provider. Call once, after logging is configured. */
export function initAudioProvider(): void {
  try {
    provider = require('./index').default;
    // Native lifecycle diagnostics (SAYSO-355): route to console so the prod file logger captures
    // them, and escalate `_failed` events (e.g. an orphaned SCK stream that would not stop — a
    // zombie capture holding the screen-recording indicator) to Sentry.
    if (provider && typeof provider.setLifecycleEventCallback === 'function') {
      provider.setLifecycleEventCallback((event: string) => {
        console.warn(`[NativeAudio] lifecycle: ${event}`);

        if (event.startsWith('mic_route_recovery_failed')) {
          // SAYSO-353: terminal mid-session mic-recovery failure (native backoff exhausted its
          // ~30s ceiling). Escalates harder than the generic `_failed` path below — a real Error
          // for better Sentry grouping/stack, plus a user-visible notice, since this is silent
          // audio loss for the rest of the call otherwise.
          Sentry.captureException(new Error(`[native-audio] ${event}`));
          try {
            if (global.coachWindow && !global.coachWindow.isDestroyed()) {
              global.coachWindow.webContents.send('cue-mic-recovery-failed');
            }
          } catch {
            /* ignore */
          }
          return;
        }

        if (event.includes('_failed')) {
          Sentry.captureMessage(`[native-audio] ${event}`, 'warning');
        }
      });
    }
  } catch (error) {
    Sentry.captureException(error);
  }
}

/**
 * Non-prompting screen-recording status (macOS). False when the provider is unavailable.
 * Pass warnIfUnavailable for status-reporting callers that want a diagnostic when the
 * provider can't answer; the onboarding-gate caller leaves it off (silent).
 */
export function isScreenRecordingGranted(opts?: { warnIfUnavailable?: boolean }): boolean {
  if (!provider || typeof provider.checkScreenRecordingGranted !== 'function') {
    if (opts?.warnIfUnavailable) {
      console.warn('[Permissions] checkScreenRecordingGranted not available on nativeAudio');
    }
    return false;
  }
  return !!provider.checkScreenRecordingGranted();
}

/** Ask the OS to surface the Screen Recording prompt/settings (macOS). No-op otherwise. */
export function requestScreenRecordingPermission(): void {
  if (!provider || typeof provider.requestScreenRecordingPermission !== 'function') return;
  try { provider.requestScreenRecordingPermission(); } catch { /* ignore */ }
}

// ─── Cue lifecycle accessors (used by auth handlers in main) ──────────────────

/** True while a Cue session is active. */
export function isCueActive(): boolean {
  return cueAudioStreamer !== null;
}

/** Session id of the active Cue session, or null when idle. */
export function getActiveCueSessionId(): string | null {
  return cueAudioStreamer?.sessionId ?? null;
}

/** Keep the active Cue websocket token current across auth refreshes. */
export function updateCueToken(token: string): void {
  if (cueAudioStreamer) cueAudioStreamer.updateToken(token);
}

/** Stop Cue reconnect loops on session expiry (no valid token to reconnect with). */
export function stopCueForSessionExpired(): void {
  if (cueAudioStreamer) {
    cueAudioStreamer.shouldReconnect = false;
    cueAudioStreamer.stop(false).catch(() => {});
  }
}

// ─── Low-audio watchdog ───────────────────────────────────────────────────────

function clearCueLowAudioTimer() {
  if (cueLowAudioTimer) {
    clearTimeout(cueLowAudioTimer);
    cueLowAudioTimer = null;
  }
}

function scheduleCueLowAudioCheck(sessionId: string) {
  clearCueLowAudioTimer();
  cueLowAudioTimer = setTimeout(() => {
    cueLowAudioTimer = null;
    if (!cueAudioStreamer || cueAudioStreamer.sessionId !== sessionId) return;
    if (cueCaptureStats.userChunks > 0) return;
    try {
      if (global.coachWindow && !global.coachWindow.isDestroyed()) {
        global.coachWindow.webContents.send('cue-low-user-audio', { sessionId });
      }
    } catch {
      /* ignore */
    }
  }, 4000);
}

// ─── Mid-session mic-stall watchdog (SAYSO-353) ───────────────────────────────

function clearCueMicStallWatchdog() {
  if (cueMicStallTimer) {
    clearInterval(cueMicStallTimer);
    cueMicStallTimer = null;
  }
  cueMicStallLastUserChunks = 0;
  cueMicStallRecovering = false;
}

/**
 * Continuous check, distinct from the one-shot low-audio/warmup checks above: if userChunks
 * stops advancing for ~CUE_MIC_STALL_CHECK_MS while a session is active, attempt one
 * stop/start cycle. This is a JS-side safety net and cross-check on top of the native backoff
 * recovery (SaysoScheduleMicRouteRecoveryRetry in audio_device_manager.mm), not the primary
 * recovery path — the native side already retries for up to ~30s on its own before this would
 * even see a stall long enough to trigger.
 */
function startCueMicStallWatchdog(sessionId: string, streamingCallback: (buffer: Buffer, format: unknown) => void) {
  clearCueMicStallWatchdog();
  cueMicStallLastUserChunks = cueCaptureStats.userChunks;

  cueMicStallTimer = setInterval(async () => {
    if (!cueAudioStreamer || cueAudioStreamer.sessionId !== sessionId) {
      clearCueMicStallWatchdog();
      return;
    }
    if (cueMicStallRecovering) return;

    const current = cueCaptureStats.userChunks;
    if (current > cueMicStallLastUserChunks) {
      cueMicStallLastUserChunks = current;
      return;
    }

    cueMicStallRecovering = true;
    console.warn('[Cue] User audio stalled — no new chunks in', CUE_MIC_STALL_CHECK_MS, 'ms, attempting one restart', {
      sessionId,
    });
    Sentry.captureMessage('[Cue] User audio stall detected mid-session', 'warning');
    try {
      await getRecorder().stopUserStreaming();
      if (!cueAudioStreamer || cueAudioStreamer.sessionId !== sessionId) return;
      await getRecorder().startUserStreaming({ streamingCallback });
    } catch (error) {
      console.error('[Cue] Error restarting user streaming after stall:', error);
      Sentry.captureException(error);
    } finally {
      cueMicStallLastUserChunks = cueCaptureStats.userChunks;
      cueMicStallRecovering = false;
    }
  }, CUE_MIC_STALL_CHECK_MS);
}

// ─── Mic-delivery watchdog ────────────────────────────────────────────────────

async function waitForCueUserChunks(sessionId: string, maxMs: number): Promise<boolean> {
  const deadline = Date.now() + maxMs;
  while (Date.now() < deadline) {
    if (cueCaptureStats.userChunks > 0) return true;
    if (!cueAudioStreamer || cueAudioStreamer.sessionId !== sessionId) return false;
    await new Promise<void>((resolve) => setImmediate(resolve));
  }
  return cueCaptureStats.userChunks > 0;
}

/** If Node never receives mic buffers after native start, stop/start mic once (belt-and-suspenders). */
async function ensureCueUserMicDeliversJsChunks(
  sessionId: string,
  streamingCallback: (buffer: Buffer, format: unknown) => void
): Promise<void> {
  if (await waitForCueUserChunks(sessionId, CUE_MIC_JS_WARMUP_MS)) return;
  if (!cueAudioStreamer || cueAudioStreamer.sessionId !== sessionId) return;
  console.warn('[Cue] No user chunks on JS side after native mic start — restarting user streaming once', {
    sessionId,
  });
  await getRecorder().stopUserStreaming();
  if (!cueAudioStreamer || cueAudioStreamer.sessionId !== sessionId) return;
  await getRecorder().startUserStreaming({ streamingCallback });
  await waitForCueUserChunks(sessionId, CUE_MIC_JS_RESTART_WAIT_MS);
}

// ─── Teardown / cleanup ───────────────────────────────────────────────────────

/**
 * Stop Cue mic + SCK + websocket streamer (shared by stop-cue and defensive start-cue).
 * Does not touch cueStopInFlight mutex.
 */
async function teardownCueStreamsAndNative(): Promise<void> {
  clearCueLowAudioTimer();
  clearCueMicStallWatchdog();
  if (cueAudioStreamer) {
    await cueAudioStreamer.stop(false);
    cueAudioStreamer = null;
  }
  await getRecorder().stopUserStreaming();
  if (provider) {
    await provider.stopSystemAudioCapture();
    provider.setStreamingCallback(null);
  }
}

/**
 * Cleanup all audio capture resources (screen capture, microphone, streams).
 * Called when the coach window closes or the app quits to release permissions.
 */
export async function cleanupAllAudioCapture(): Promise<void> {
  if (isDev) {
    console.log('[Cleanup] Starting audio capture cleanup...');
  }

  try {
    if (cueStopInFlight) {
      await cueStopInFlight;
    }

    // 1. Stop user streaming (handles global.userStreamingProcess)
    await getRecorder().stopUserStreaming();

    // 2. Stop system audio capture (screen recording) via native module
    if (provider) {
      await provider.stopSystemAudioCapture();
      provider.setStreamingCallback(null);
    }

    // 3. Stop Cue audio streamer
    if (cueAudioStreamer) {
      try {
        await cueAudioStreamer.stop(false);
        cueAudioStreamer = null;
      } catch (error) {
        console.error('[Cleanup] Error stopping cue audio streamer:', error);
        Sentry.captureException(error);
        cueAudioStreamer = null;
      }
    }

    if (isDev) {
      console.log('[Cleanup] All audio capture cleaned up');
    }
  } catch (error) {
    console.error('[Cleanup] Error during audio cleanup:', error);
    Sentry.captureException(error);
  }
}

// ─── IPC registration ─────────────────────────────────────────────────────────

// IPC classification: start-cue / stop-cue are PLATFORM-DISPATCHED. All native
// audio capture goes through the active IAudioProvider (mac = ScreenCaptureKit +
// AVAudioEngine; win32 = the WASAPI native module). See docs/IPC_CONTRACT.md.

/**
 * Stop the Cue capture stack behind the cueStopInFlight mutex, so overlapping
 * callers — the stop-cue IPC and the pre-logout persist in main — await the same
 * teardown instead of racing two native stops. Idempotent: dedupes via
 * cueStopInFlight and clears the streamer, so a second call returns immediately.
 */
export async function stopCue(): Promise<{ success: boolean; error?: string; deduped?: boolean }> {
  if (cueStopInFlight) {
    const result = await cueStopInFlight;
    return { ...result, deduped: true };
  }

  const stopWork = (async (): Promise<{ success: boolean; error?: string }> => {
    const statsAtStop = { ...cueCaptureStats };
    try {
      await teardownCueStreamsAndNative();
      notifyOnboarding('onboarding:session-stopped');
      console.log(
        `[Cue] Session teardown complete — capture stats: userChunks=${statsAtStop.userChunks} prospectChunks=${statsAtStop.prospectChunks} userBytes=${statsAtStop.userBytes} prospectBytes=${statsAtStop.prospectBytes}`
      );
      cueCaptureStats = { userChunks: 0, prospectChunks: 0, userBytes: 0, prospectBytes: 0 };
      return { success: true };
    } catch (error: any) {
      console.error('[MAIN] Error stopping Cue:', error);
      Sentry.captureException(error);
      cueAudioStreamer = null;
      cueCaptureStats = { userChunks: 0, prospectChunks: 0, userBytes: 0, prospectBytes: 0 };
      return { success: false, error: error.message };
    } finally {
      cueStopInFlight = null;
    }
  })();

  cueStopInFlight = stopWork;
  return stopWork;
}

/** Register the start-cue / stop-cue IPC handlers. Call once during app init. */
export function registerCueIpc(deps: CueIpcDeps): void {
  const { checkOSPermissionsGranted, createSplashWindow, sendToOnboardingWindow } = deps;
  notifyOnboarding = sendToOnboardingWindow;

  // Start Cue (handles 2 audio websockets: user + prospect)
  ipcMain.handle('start-cue', async (event: Electron.IpcMainInvokeEvent, { sessionId, token }: { sessionId: string, token: string }) => {
    if (cueStartInFlight) {
      console.warn('[Cue] start-cue invoked while a start is already in flight — deduping onto it');
      const result = await cueStartInFlight;
      return { ...result, deduped: true };
    }
    const startWork = (async (): Promise<{ success: boolean; error?: string; sessionId?: string; mic?: boolean; screen?: boolean }> => {
    try {
      if (!token) {
        throw new Error('Token is required');
      }

      if (!sessionId) {
        throw new Error('SessionId is required');
      }

      if (!provider) {
        throw new Error('Native audio module not loaded. Please wait for app initialization.');
      }

      const permsCheck = await checkOSPermissionsGranted();
      if (!permsCheck.granted) {
        console.warn('[Cue] OS permissions not granted — mic:', permsCheck.mic, 'screen:', permsCheck.screen);
        // Surface the splash window; PostAuthRedirect sees permissions are incomplete and routes to /permissions.
        createSplashWindow();
        return { success: false, error: 'permissions_denied', mic: permsCheck.mic, screen: permsCheck.screen };
      }

      if (cueStopInFlight) {
        await cueStopInFlight;
      }

      let systemCaptureActive = false;
      let micCaptureActive = false;
      try {
        if (typeof provider.isSystemAudioCaptureActive === 'function') {
          systemCaptureActive = !!(await provider.isSystemAudioCaptureActive());
        }
        if (typeof provider.isMicrophoneCaptureActive === 'function') {
          micCaptureActive = !!(await provider.isMicrophoneCaptureActive());
        }
      } catch (probeErr) {
        console.warn('[Cue] Could not probe native capture state:', probeErr);
      }

      if (cueAudioStreamer || systemCaptureActive || micCaptureActive) {
        console.warn('[Cue] Guard: leftover streamer or native capture — running teardown before start', {
          hadStreamer: !!cueAudioStreamer,
          systemCaptureActive,
          micCaptureActive
        });
        await teardownCueStreamsAndNative();
      }

      cueCaptureStats = { userChunks: 0, prospectChunks: 0, userBytes: 0, prospectBytes: 0 };

      // Create AudioStreamer for 2 audio websockets (user + prospect)
      const AudioStreamer = getAudioStreamerCtor();
      cueAudioStreamer = new AudioStreamer({
        sessionId: sessionId, // Use provided sessionId from backend
        onUserConnected: () => {
          try {
            if (!event.sender.isDestroyed()) {
              event.sender.send('cue-status', { user: 'connected' });
            }
          } catch (error) {
            console.error('[Cue] Error sending user connected status:', error);
          }
        },
        onProspectConnected: () => {
          try {
            if (!event.sender.isDestroyed()) {
              event.sender.send('cue-status', { prospect: 'connected' });
            }
          } catch (error) {
            console.error('[Cue] Error sending prospect connected status:', error);
          }
        },
        onError: (stream: string, error: Error) => {
          console.error(`[Cue] ${stream} stream error:`, error);
          try {
            if (!event.sender.isDestroyed()) {
              event.sender.send('cue-error', { stream, error: error.message });
            }
          } catch (err) {
            console.error('[Cue] Error sending error status:', err);
          }
        },
        onMessage: (message: any) => {
          try {
            // Forward insight messages to renderer process
            if (message && message.type === 'insight' && message.data) {
              // Forward to coach window if it exists
              if (global.coachWindow && !global.coachWindow.isDestroyed()) {
                global.coachWindow.webContents.send('cue-insight', message.data);
              }
            }

            if (message && message.type === 'smart_capture' && message.data) {
              if (global.coachWindow && !global.coachWindow.isDestroyed()) {
                global.coachWindow.webContents.send('cue-smart-capture', message.data);
              }
            }

            if (message && message.type === 'auto_stop') {
              if (global.coachWindow && !global.coachWindow.isDestroyed()) {
                global.coachWindow.webContents.send('cue-auto-stop');

                if (process.platform === 'darwin') {
                  app.setBadgeCount(app.getBadgeCount() + 1);
                  if (app.dock) {
                    app.dock.bounce('critical');
                  }
                }
              }
            }
          } catch (error) {
            console.error('[Cue] Error handling message:', error);
          }
        }
      });

      // Start audio streaming (2 websockets)
      await cueAudioStreamer.start(token);

      // Set up audio capture callbacks (streaming only - no file saving)
      const cueUserStreamingCallback = (buffer: Buffer, format: unknown) => {
        if (cueAudioStreamer) {
          cueCaptureStats.userChunks += 1;
          cueCaptureStats.userBytes += buffer?.length ?? 0;
          cueAudioStreamer.addUserAudio(buffer, format);
        }
      };
      await getRecorder().startUserStreaming({ streamingCallback: cueUserStreamingCallback });
      await ensureCueUserMicDeliversJsChunks(sessionId, cueUserStreamingCallback);

      if (!provider) {
        throw new Error('Native audio module not loaded. Please wait for app initialization.');
      }

      if (typeof provider.startProspectStreaming !== 'function') {
        throw new Error('startProspectStreaming method not available. Native module may need to be rebuilt.');
      }

      await provider.startProspectStreaming({
        streamingCallback: (buffer: Buffer, format: AudioFormat) => {
          if (cueAudioStreamer) {
            cueCaptureStats.prospectChunks += 1;
            cueCaptureStats.prospectBytes += buffer?.length ?? 0;
            cueAudioStreamer.addProspectAudio(buffer, format);
          }
        }
      });

      scheduleCueLowAudioCheck(sessionId);
      startCueMicStallWatchdog(sessionId, cueUserStreamingCallback);

      if (isDev) {
        console.log('[Cue] Started session', sessionId, '— stats reset; low-audio check in 4s if no user chunks');
      }

      sendToOnboardingWindow('onboarding:session-started');
      return {
        success: true,
        sessionId: sessionId
      };
    } catch (error: any) {
      console.error('[MAIN] Error starting Cue:', error);
      Sentry.captureException(error);
      clearCueLowAudioTimer();
      clearCueMicStallWatchdog();
      try {
        await teardownCueStreamsAndNative();
      } catch (teardownErr: any) {
        console.error('[Cue] Error tearing down after failed start:', teardownErr);
        cueAudioStreamer = null;
      }
      return { success: false, error: error.message };
    } finally {
      cueStartInFlight = null;
    }
    })();
    cueStartInFlight = startWork;
    return startWork;
  });

  // Stop Cue (closes all websockets)
  ipcMain.handle('stop-cue', async (_event: Electron.IpcMainInvokeEvent) => stopCue());
}
