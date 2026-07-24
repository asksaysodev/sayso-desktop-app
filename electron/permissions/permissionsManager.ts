import { app, ipcMain } from 'electron';
import * as Sentry from '@sentry/electron/main';

import provider from './index';
import type { PermissionsStatus } from './IPermissionsProvider';

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

/** Register the permissions-* IPC handlers. Call once during app init. */
export function registerPermissionsIpc(): void {
  // Check current mic + screen status (non-interactive)
  ipcMain.handle('permissions-check', async () => {
    try {
      const result = await provider.checkGranted();
      if (isDev) console.log('[Permissions] check:', result);
      return { mic: result.mic, screen: result.screen };
    } catch (e: any) {
      console.error('[MAIN] [Permissions] Error checking permissions:', e);
      Sentry.captureException(e);
      return { mic: false, screen: false, error: e.message };
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

  // Write permissions-complete flag then relaunch. Only relaunch if the write succeeded —
  // otherwise the next boot would route back to permissions (potential loop).
  ipcMain.handle('permissions-complete', () => {
    try {
      provider.markComplete();
      console.log('[Permissions] permissions-complete flag written');
    } catch (e: any) {
      console.error('[MAIN] [Permissions] Failed to write permissions-complete flag:', e);
      Sentry.captureException(e);
      return { error: e.message };
    }
    app.relaunch();
    app.quit();
  });

  // Returns whether the permissions-complete flag is set (for renderer routing)
  ipcMain.handle('permissions-get-flag', () => isPermissionsComplete());
}
