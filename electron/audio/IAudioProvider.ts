export interface AudioCaptureResult {
  success: boolean;
  error?: string;
}

// SAYSO-347: distinct, greppable reasons for a mic start failure — previously collapsed into a
// bare `false`, indistinguishable in Sentry between a wiring bug, a teardown race, and a genuine
// audio-route problem (e.g. Bluetooth/AirPods).
export type MicStartFailureReason = 'callback_empty' | 'already_active' | 'no_tap_buffers';

export interface MicStartFailure {
  ok: false;
  reason?: MicStartFailureReason;
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

  // Microphone capture. Success resolves plain `true` (unchanged, for backward compatibility with
  // a stale native build); failure resolves { ok: false, reason } — see MicStartFailureReason.
  // Callers must still tolerate a bare `false` on failure (older native builds predate the reason).
  startMicrophoneCapture(options?: { streamingCallback?: StreamingCallback; [key: string]: unknown }): Promise<boolean | MicStartFailure>;
  stopMicrophoneCapture(): Promise<boolean>;
  isMicrophoneCaptureActive(): Promise<boolean>;

  // Streaming
  setStreamingCallback(callback: StreamingCallback | null): void;
  startProspectStreaming(options: { streamingCallback: StreamingCallback }): Promise<AudioCaptureResult>;

  // Native lifecycle diagnostics (SAYSO-355): short snake_case event strings emitted for
  // capture-lifecycle anomalies (watchdog fired, orphaned stream, late callbacks). Optional so a
  // stale native build without the method stays contract-compatible; events ending in `_failed`
  // should be escalated by the consumer.
  setLifecycleEventCallback?(callback: ((event: string) => void) | null): void;
}
