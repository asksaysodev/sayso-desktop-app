import type {
  IAudioProvider,
  AudioCaptureResult,
  StopCaptureResult,
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

  async listOutputDevices(): Promise<any[]> {
    console.warn('[Audio] listOutputDevices not yet implemented on Windows');
    return [];
  }

  async createMultiOutputDevice(_name: string, _subDevices: string[]): Promise<any> {
    console.warn('[Audio] createMultiOutputDevice not yet implemented on Windows');
    return null;
  }

  async deleteMultiOutputDevice(_deviceId: any): Promise<any> {
    console.warn('[Audio] deleteMultiOutputDevice not yet implemented on Windows');
    return null;
  }

  async startSystemAudioCapture(_options?: Record<string, unknown>): Promise<AudioCaptureResult> {
    return { success: false, error: 'System audio capture not yet supported on Windows' };
  }

  async stopSystemAudioCapture(): Promise<StopCaptureResult> {
    return { success: true, filePath: null, actualStartMs: null };
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
