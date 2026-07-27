import type {
  IAudioProvider,
  AudioCaptureResult,
  StreamingCallback,
} from './IAudioProvider';

class WindowsAudioProvider implements IAudioProvider {
  async initialize(): Promise<void> {
    // No-op until Windows audio backend is implemented
  }

  checkScreenRecordingGranted(): boolean {
    // Windows has no screen recording permission gate
    return true;
  }

  async requestScreenRecordingPermission(): Promise<void> {
    // No-op: Windows has no equivalent permission model
  }

  async stopSystemAudioCapture(): Promise<AudioCaptureResult> {
    return { success: true };
  }

  async isSystemAudioCaptureActive(): Promise<boolean> {
    return false;
  }

  async startMicrophoneCapture(_options?: { streamingCallback?: StreamingCallback; [key: string]: unknown }): Promise<boolean> {
    console.warn('[Audio] startMicrophoneCapture not yet implemented on Windows');
    return false;
  }

  async stopMicrophoneCapture(): Promise<boolean> {
    return false;
  }

  async isMicrophoneCaptureActive(): Promise<boolean> {
    return false;
  }

  setStreamingCallback(_callback: StreamingCallback | null): void {
    // No-op until Windows audio capture is implemented
  }

  async startProspectStreaming(_options: { streamingCallback: StreamingCallback }): Promise<AudioCaptureResult> {
    return { success: false, error: 'Prospect streaming not yet supported on Windows' };
  }
}

export default new WindowsAudioProvider();
