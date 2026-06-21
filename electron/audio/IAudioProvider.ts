export interface AudioCaptureResult {
  success: boolean;
  error?: string;
}

export interface StopCaptureResult {
  success: boolean;
  filePath?: string | null;
  actualStartMs?: number | null;
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

  // Virtual multi-output device management (macOS CoreAudio concept)
  listOutputDevices(): Promise<any[]>;
  createMultiOutputDevice(name: string, subDevices: string[]): Promise<any>;
  deleteMultiOutputDevice(deviceId: any): Promise<any>;

  // System audio capture (ScreenCaptureKit on Mac, WASAPI loopback on Windows)
  startSystemAudioCapture(options?: Record<string, unknown>): Promise<AudioCaptureResult>;
  stopSystemAudioCapture(): Promise<StopCaptureResult>;
  isSystemAudioCaptureActive(): Promise<boolean>;

  // Microphone capture
  startMicrophoneCapture(options?: { streamingCallback?: StreamingCallback; [key: string]: unknown }): Promise<boolean>;
  stopMicrophoneCapture(): Promise<boolean>;
  isMicrophoneCaptureActive(): Promise<boolean>;

  // Streaming
  setStreamingCallback(callback: StreamingCallback | null): void;
  startProspectStreaming(options: { streamingCallback: StreamingCallback }): Promise<AudioCaptureResult>;
}
