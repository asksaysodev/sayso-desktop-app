"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
// Safe defaults so the app never blocks on a permissions step that doesn't exist yet.
//
// TODO (fast-follow): Windows 10/11 DOES have a per-app microphone privacy gate, and
// `systemPreferences.getMediaAccessStatus('microphone')` works on Windows. A real
// implementation should report actual mic status and route the user to
// Settings > Privacy > Microphone when denied. There is no Screen Recording permission
// model on Windows, so screen stays true and the screen-settings methods are no-ops.
class WindowsPermissionsProvider {
    async checkGranted() {
        return { granted: true, mic: true, screen: true };
    }
    isComplete() {
        return true;
    }
    async requestMic() {
        return { mic: true, action: 'already-granted' };
    }
    requestScreen() {
        // No Screen Recording permission model on Windows.
    }
    async openScreenSettings() {
        // No Screen Recording privacy pane on Windows.
    }
    markComplete() {
        // No onboarding permissions gate on Windows; nothing to persist.
    }
}
exports.default = new WindowsPermissionsProvider();
//# sourceMappingURL=WindowsPermissionsProvider.js.map