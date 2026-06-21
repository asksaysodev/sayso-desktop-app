"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
class WindowsAudioProvider {
    async initialize() {
        // No-op until Windows audio backend is implemented
    }
    checkScreenRecordingGranted() {
        // Windows has no screen recording permission gate
        return true;
    }
    async requestScreenRecordingPermission() {
        // No-op: Windows has no equivalent permission model
    }
    async listOutputDevices() {
        console.warn('[Audio] listOutputDevices not yet implemented on Windows');
        return [];
    }
    async createMultiOutputDevice(_name, _subDevices) {
        console.warn('[Audio] createMultiOutputDevice not yet implemented on Windows');
        return null;
    }
    async deleteMultiOutputDevice(_deviceId) {
        console.warn('[Audio] deleteMultiOutputDevice not yet implemented on Windows');
        return null;
    }
    async startSystemAudioCapture(_options) {
        return { success: false, error: 'System audio capture not yet supported on Windows' };
    }
    async stopSystemAudioCapture() {
        return { success: true, filePath: null, actualStartMs: null };
    }
    async isSystemAudioCaptureActive() {
        return false;
    }
    async startMicrophoneCapture(_options) {
        console.warn('[Audio] startMicrophoneCapture not yet implemented on Windows');
        return false;
    }
    async stopMicrophoneCapture() {
        return false;
    }
    async isMicrophoneCaptureActive() {
        return false;
    }
    setStreamingCallback(_callback) {
        // No-op until Windows audio capture is implemented
    }
    async startProspectStreaming(_options) {
        return { success: false, error: 'Prospect streaming not yet supported on Windows' };
    }
}
exports.default = new WindowsAudioProvider();
//# sourceMappingURL=WindowsAudioProvider.js.map