import { shell, systemPreferences } from 'electron';

import type {
  IPermissionsProvider,
  PermissionRequirements,
  PermissionsStatus,
  RequestMicResult,
} from './IPermissionsProvider';

// Mic is the only gate on Windows (two global switches, no dialog, no per-app
// switch), system audio needs no permission, and nothing needs a relaunch —
// see the Windows section of docs/PERMISSIONS_FLOW.md for the full model.
//
// `getMediaAccessStatus('microphone')` on Windows maps WinRT
// DeviceAccessInformation.CurrentStatus: Allowed → 'granted', DeniedByUser →
// 'denied', DeniedBySystem → 'restricted', Unspecified → 'not-determined',
// anything else → 'unknown'. For a non-packaged app it reflects both switches.
class WindowsPermissionsProvider implements IPermissionsProvider {
  readonly requirements: PermissionRequirements = { screen: false, relaunchOnComplete: false };

  // Only an explicit denial blocks. 'not-determined' and 'unknown' count as
  // granted — Windows has no dialog to resolve them, so blocking would leave the
  // user on a screen with nothing to do. Same reasoning if the read throws or
  // the API is missing (unsupported-platform fallback in index.ts): fail open.
  private isMicBlocked(): boolean {
    try {
      const status = systemPreferences.getMediaAccessStatus('microphone');
      if (status === 'denied' || status === 'restricted') return true;
      if (status !== 'granted') {
        console.warn(`[Permissions] Mic access status is '${status}' — treating as granted`);
      }
      return false;
    } catch (e) {
      console.warn('[Permissions] Could not read mic access status; treating as granted:', e);
      return false;
    }
  }

  async checkGranted(): Promise<PermissionsStatus> {
    const mic = !this.isMicBlocked();
    return { granted: mic, mic, screen: true };
  }

  async checkMic(): Promise<boolean> {
    return !this.isMicBlocked();
  }

  isComplete(): boolean {
    // Live mic status only. No flag file: a user with the mic granted must never
    // land on /permissions, and a user who flips the switch off must.
    return !this.isMicBlocked();
  }

  async requestMic(): Promise<RequestMicResult> {
    if (!this.isMicBlocked()) return { mic: true, action: 'already-granted' };
    // No dialog to ask; send the user to the page with the two switches.
    await shell.openExternal('ms-settings:privacy-microphone');
    return { mic: false, action: 'open-settings' };
  }

  requestScreen(): void {
    // No Screen Recording permission model on Windows.
  }

  async openScreenSettings(): Promise<void> {
    // No Screen Recording privacy pane on Windows.
  }

  markComplete(): void {
    // Completion is the live mic status (see isComplete); nothing to persist.
  }
}

export default new WindowsPermissionsProvider();
