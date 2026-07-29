import type {
  IAudioProvider,
  AudioCaptureResult,
  StreamingCallback,
} from './IAudioProvider';

/**
 * Windows audio provider — SAFE-DEFAULT placeholder.
 *
 * Today every method returns a not-supported default so the app boots and Cue
 * fails cleanly on Windows (no capture, no crash). Once the native WASAPI module
 * lands (see docs/NATIVE_AUDIO_CONTRACT.md + src/audio_device_manager_win.cpp),
 * wire this provider to it the way MacAudioProvider does:
 *   const native = require('../native-audio');   // loads native_audio.node
 * and forward each method to the matching native function noted in the TODOs
 * below. The native surface is identical on both platforms, so this becomes a
 * thin pass-through just like the Mac provider.
 */
class WindowsAudioProvider implements IAudioProvider {
  async initialize(): Promise<void> {
    // TODO(win-native): await native.initialize()
    // No-op until Windows audio backend is implemented
  }

  checkScreenRecordingGranted(): boolean {
    // Windows has no screen-recording permission gate for loopback capture.
    // TODO(win-native): native.checkScreenRecordingGranted() (expected to also return true)
    return true;
  }

  async requestScreenRecordingPermission(): Promise<void> {
    // TODO(win-native): native.requestScreenRecordingPermission() (likely a no-op on Windows)
    // No-op: Windows has no equivalent permission model
  }

  async stopSystemAudioCapture(): Promise<AudioCaptureResult> {
    // TODO(win-native): await native.stopSystemAudioCapture() → map { success, error }
    return { success: true };
  }

  async isSystemAudioCaptureActive(): Promise<boolean> {
    // TODO(win-native): native.isSystemAudioCaptureActive()
    return false;
  }

  async startMicrophoneCapture(_options?: { streamingCallback?: StreamingCallback; [key: string]: unknown }): Promise<boolean> {
    // TODO(win-native): native.startMicrophoneCapture(options) — mic chunks via native.setMicrophoneStreamingCallback
    console.warn('[Audio] startMicrophoneCapture not yet implemented on Windows');
    return false;
  }

  async stopMicrophoneCapture(): Promise<boolean> {
    // TODO(win-native): native.stopMicrophoneCapture() — must NOT tear down system audio
    return false;
  }

  async isMicrophoneCaptureActive(): Promise<boolean> {
    // TODO(win-native): native.isMicrophoneCaptureActive()
    return false;
  }

  setStreamingCallback(_callback: StreamingCallback | null): void {
    // TODO(win-native): native.setStreamingCallback(callback) — SYSTEM-AUDIO chunks channel
    // No-op until Windows audio capture is implemented
  }

  async startProspectStreaming(_options: { streamingCallback: StreamingCallback }): Promise<AudioCaptureResult> {
    // TODO(win-native): setStreamingCallback(cb) then await native.startSystemAudioCapture({ streamingOnly: true })
    //                   (native.startSystemAudioCapture is internal; startProspectStreaming is the public entry)
    return { success: false, error: 'Prospect streaming not yet supported on Windows' };
  }
}

export default new WindowsAudioProvider();
