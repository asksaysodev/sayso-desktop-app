import type { IPermissionsProvider, PermissionsStatus, RequestMicResult } from './IPermissionsProvider';

// Safe defaults so the app never blocks on a permissions step that doesn't exist yet.
//
// TODO (fast-follow): Windows 10/11 DOES have a per-app microphone privacy gate, and
// `systemPreferences.getMediaAccessStatus('microphone')` works on Windows. A real
// implementation should report actual mic status and route the user to
// Settings > Privacy > Microphone when denied. There is no Screen Recording permission
// model on Windows, so screen stays true and the screen-settings methods are no-ops.
class WindowsPermissionsProvider implements IPermissionsProvider {
  async checkGranted(): Promise<PermissionsStatus> {
    return { granted: true, mic: true, screen: true };
  }

  async checkMic(): Promise<boolean> {
    // TODO (fast-follow): report the real Win10/11 per-app mic privacy status.
    return true;
  }

  isComplete(): boolean {
    return true;
  }

  async requestMic(): Promise<RequestMicResult> {
    return { mic: true, action: 'already-granted' };
  }

  requestScreen(): void {
    // No Screen Recording permission model on Windows.
  }

  async openScreenSettings(): Promise<void> {
    // No Screen Recording privacy pane on Windows.
  }

  markComplete(): void {
    // No onboarding permissions gate on Windows; nothing to persist.
  }
}

export default new WindowsPermissionsProvider();
