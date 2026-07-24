export interface AudioCaptureResult {
  success: boolean;
  error?: string;
}

export interface AudioFormat {
  sampleRate: number;
  channels: number;
  bitDepth: number;
  isFloat: boolean;
}

export type StreamingCallback = (buffer: Buffer, format: AudioFormat) => void;

export interface IAudioProvider {
  initialize(): Promise<void>;

  // Permissions — macOS requires explicit grants; other platforms return safe defaults
  checkScreenRecordingGranted(): boolean;
  requestScreenRecordingPermission(): Promise<void>;

  // System audio capture (ScreenCaptureKit on Mac, WASAPI loopback on Windows).
  // The public entry point is startProspectStreaming (below); the provider owns
  // starting capture internally. These manage teardown and status only.
  stopSystemAudioCapture(): Promise<AudioCaptureResult>;
  isSystemAudioCaptureActive(): Promise<boolean>;

  // Microphone capture
  startMicrophoneCapture(options?: { streamingCallback?: StreamingCallback; [key: string]: unknown }): Promise<boolean>;
  stopMicrophoneCapture(): Promise<boolean>;
  isMicrophoneCaptureActive(): Promise<boolean>;

  // Streaming
  setStreamingCallback(callback: StreamingCallback | null): void;
  startProspectStreaming(options: { streamingCallback: StreamingCallback }): Promise<AudioCaptureResult>;
}
