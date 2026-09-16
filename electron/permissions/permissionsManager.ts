import { app, ipcMain } from 'electron';
import * as Sentry from '@sentry/electron/main';

import provider from './index';
import type { PermissionsStatus } from './IPermissionsProvider';
import type { PermissionsCheckResult } from '../shared/permissions';

// app.isPackaged is reliable at module-load time; NODE_ENV is not yet set when
// this module is imported. (Same reasoning as audioManager.)
const isDev = !app.isPackaged;

/**
 * Are all OS permissions required to run granted (and onboarding flag set)?
 * Resilient wrapper for startup routing — never throws (returns false on error).
 */
export function isPermissionsComplete(): boolean {
  try {
    return provider.isComplete();
  } catch {
    return false;
  }
}

/** Live mic + screen grant status. Used by the cue pre-flight and permission polling. */
export function checkOSPermissionsGranted(): Promise<PermissionsStatus> {
  return provider.checkGranted();
}

/** Live microphone grant only (non-prompting). Used by the coach-window gate. */
export function isMicGranted(): Promise<boolean> {
  return provider.checkMic();
}

// IPC classification: every permissions-* channel is PLATFORM-DISPATCHED via the
// active IPermissionsProvider (mac = mic + screen-recording gates and a relaunch;
// win32 = the mic privacy switches only, no flag, no relaunch). The provider's
// `requirements` tells the renderer which of those apply, so handlers stay thin —
// no process.platform here. See docs/IPC_CONTRACT.md.

/** Register the permissions-* IPC handlers. Call once during app init. */
export function registerPermissionsIpc(): void {
  // Check current mic + screen status (non-interactive)
  ipcMain.handle('permissions-check', async (): Promise<PermissionsCheckResult> => {
    try {
      const result = await provider.checkGranted();
      if (isDev) console.log('[Permissions] check:', result);
      return { mic: result.mic, screen: result.screen, requirements: provider.requirements };
    } catch (e: any) {
      console.error('[MAIN] [Permissions] Error checking permissions:', e);
      Sentry.captureException(e);
      // `requirements` is static, so the screen can still lay itself out on error.
      return { mic: false, screen: false, requirements: provider.requirements, error: e.message };
    }
  });

  // Request microphone permission only (never triggers app restart)
  ipcMain.handle('permissions-request-mic', async () => {
    try {
      return await provider.requestMic();
    } catch (e: any) {
      console.error('[MAIN] [Permissions] Error requesting mic:', e);
      Sentry.captureException(e);
      return { mic: false, action: 'error', error: e.message };
    }
  });

  // Non-destructive poll: did the user grant screen recording?
  ipcMain.handle('permissions-check-screen', async () => {
    try {
      const result = await provider.checkGranted();
      return result.screen;
    } catch {
      return false;
    }
  });

  // Prompt macOS to surface the Screen Recording row in System Settings (fire-and-forget)
  ipcMain.handle('permissions-request-screen', () => {
    provider.requestScreen();
  });

  // Open Screen Recording privacy pane directly
  ipcMain.handle('permissions-open-screen-settings', async () => {
    try {
      await provider.openScreenSettings();
    } catch (e: any) {
      console.warn('[MAIN] [Permissions] Could not open Screen Recording settings:', e?.message || e);
    }
  });

  // Persist completion, then relaunch only where the OS needs it. On macOS the
  // relaunch lets SCK re-read a freshly granted screen-recording permission; it
  // only happens if the flag write succeeded, otherwise the next boot would route
  // back to permissions (potential loop). On Windows markComplete() is a no-op
  // (completion is the live mic status) and there is nothing to relaunch for —
  // the renderer navigates on its own once this resolves.
  ipcMain.handle('permissions-complete', () => {
    try {
      provider.markComplete();
    } catch (e: any) {
      console.error('[MAIN] [Permissions] Failed to write permissions-complete flag:', e);
      Sentry.captureException(e);
      return { error: e.message };
    }
    if (!provider.requirements.relaunchOnComplete) return;
    app.relaunch();
    app.quit();
  });

  // Returns whether permissions are complete (for renderer routing): flag + live
  // grants on macOS, live mic status on Windows.
  ipcMain.handle('permissions-get-flag', () => isPermissionsComplete());
}
