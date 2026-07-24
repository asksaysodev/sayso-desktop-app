"use strict";
var __createBinding = (this && this.__createBinding) || (Object.create ? (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    var desc = Object.getOwnPropertyDescriptor(m, k);
    if (!desc || ("get" in desc ? !m.__esModule : desc.writable || desc.configurable)) {
      desc = { enumerable: true, get: function() { return m[k]; } };
    }
    Object.defineProperty(o, k2, desc);
}) : (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    o[k2] = m[k];
}));
var __setModuleDefault = (this && this.__setModuleDefault) || (Object.create ? (function(o, v) {
    Object.defineProperty(o, "default", { enumerable: true, value: v });
}) : function(o, v) {
    o["default"] = v;
});
var __importStar = (this && this.__importStar) || (function () {
    var ownKeys = function(o) {
        ownKeys = Object.getOwnPropertyNames || function (o) {
            var ar = [];
            for (var k in o) if (Object.prototype.hasOwnProperty.call(o, k)) ar[ar.length] = k;
            return ar;
        };
        return ownKeys(o);
    };
    return function (mod) {
        if (mod && mod.__esModule) return mod;
        var result = {};
        if (mod != null) for (var k = ownKeys(mod), i = 0; i < k.length; i++) if (k[i] !== "default") __createBinding(result, mod, k[i]);
        __setModuleDefault(result, mod);
        return result;
    };
})();
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
const electron_1 = require("electron");
const node_fs_1 = __importDefault(require("node:fs"));
const node_path_1 = __importDefault(require("node:path"));
const audioManager = __importStar(require("../audio/audioManager"));
function completeFlagPath() {
    return node_path_1.default.join(electron_1.app.getPath('userData'), 'permissions-complete');
}
class MacPermissionsProvider {
    async checkGranted() {
        const mic = electron_1.systemPreferences.getMediaAccessStatus('microphone') === 'granted';
        // Use CGPreflightScreenCaptureAccess (non-prompting) to READ status — never triggers the macOS
        // dialog. The dialog is only shown on explicit user action via requestScreen().
        const screen = audioManager.isScreenRecordingGranted({ warnIfUnavailable: true });
        return { granted: mic && screen, mic, screen };
    }
    isComplete() {
        const mic = electron_1.systemPreferences.getMediaAccessStatus('microphone') === 'granted';
        // CGPreflight is accurate for the running process (screen-recording grant is bound at launch).
        const screen = audioManager.isScreenRecordingGranted();
        // Live OS state is authoritative: if both are actually granted for this process, onboarding is
        // complete regardless of the flag. This self-heals the case where the flag is missing but perms
        // work — e.g. after the cert migration deletes the flag and the user re-grants + reopens. We only
        // short-circuit on live grants, so an optimistically-written flag without a real SCK grant
        // (screen === false) still routes back to /permissions.
        if (mic && screen) {
            if (!node_fs_1.default.existsSync(completeFlagPath())) {
                try {
                    node_fs_1.default.writeFileSync(completeFlagPath(), '1');
                }
                catch (e) {
                    console.warn('[Permissions] Failed to self-heal permissions-complete flag:', e);
                }
            }
            return true;
        }
        return false;
    }
    async requestMic() {
        const micStatus = electron_1.systemPreferences.getMediaAccessStatus('microphone');
        if (micStatus === 'granted')
            return { mic: true, action: 'already-granted' };
        if (micStatus === 'not-determined') {
            const granted = await electron_1.systemPreferences.askForMediaAccess('microphone');
            return { mic: granted, action: 'asked' };
        }
        // denied / restricted → open Microphone privacy pane directly
        await electron_1.shell.openExternal('x-apple.systempreferences:com.apple.preference.security?Privacy_Microphone');
        return { mic: false, action: 'open-settings' };
    }
    requestScreen() {
        // Screen Recording is required by ScreenCaptureKit; the prompt lives in the native audio module.
        audioManager.requestScreenRecordingPermission();
    }
    async openScreenSettings() {
        await electron_1.shell.openExternal('x-apple.systempreferences:com.apple.preference.security?Privacy_ScreenCapture');
    }
    markComplete() {
        node_fs_1.default.writeFileSync(completeFlagPath(), '1');
    }
}
exports.default = new MacPermissionsProvider();
//# sourceMappingURL=MacPermissionsProvider.js.map