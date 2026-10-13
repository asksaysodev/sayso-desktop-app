export interface AudioCaptureResult {
  success: boolean;
  error?: string;
}

export type MicStartFailureReason =
  | 'callback_empty'
  | 'already_active'
  | 'no_input_node'
  | 'tap_install_failed'
  | 'engine_start_failed'
  | 'no_tap_buffers';

export interface MicStartFailure {
  ok: false;
  reason?: MicStartFailureReason;
}

export interface AudioFormat {
  sampleRate: number;
  channels: number;
  bitDepth: number;
  isFloat: boolean;
  gainDb?: number;
}

export type StreamingCallback = (buffer: Buffer, format: AudioFormat) => void;

export interface MicDeviceSnapshot {
  id: number | null;
  name: string | null;
  uid: string | null;
  transport: string;
  nominalSampleRate: number | null;
  inputChannels: number | null;
  isAlive: boolean | null;
  isRunningSomewhere: boolean | null;
  hogModePid: number | null;
  inputMuted: boolean | null;
  inputVolume: number | null;
  formFactor?: string | null;
  state?: string | null;
}

export interface MicInputDiagnostics {
  defaultInput: MicDeviceSnapshot | null;
  openedInputId: number | string | null;
  openedInput: MicDeviceSnapshot | null;
  capturing: boolean;
  routeRecovering: boolean;
  engineRunning: boolean;
  engineBuilds: number;
  msSinceEngineBuilt: number | null;
  msSinceDefaultInputChange: number | null;
  msSinceEngineConfigChange: number | null;
  engineConfigChanges: number;
  tap: {
    callbacks: number;
    delivered: number;
    droppedNoHandle: number;
    droppedNoCallback: number;
    droppedEmpty: number;
    droppedLayout: number;
    droppedUnsupported: number;
    msSinceLastCallback: number | null;
    msSinceLastDelivered: number | null;
    lastSampleRate: number | null;
    lastChannels: number | null;
    lastCommonFormat: number | null;
    silentFlagged?: number;
    droppedOverflow?: number;
  };
}

export interface AudioCaptureCapability {
  available: boolean;
  macOS14_4Plus: boolean;
  error: string | null;
}

export interface IAudioProvider {
  initialize(): Promise<void>;

  checkScreenRecordingGranted(): boolean;
  requestScreenRecordingPermission(): Promise<void>;

  checkAudioCaptureCapability?(): Promise<AudioCaptureCapability | null>;

  stopSystemAudioCapture(): Promise<AudioCaptureResult>;
  isSystemAudioCaptureActive(): Promise<boolean>;

  startMicrophoneCapture(options?: { streamingCallback?: StreamingCallback; [key: string]: unknown }): Promise<boolean | MicStartFailure>;
  stopMicrophoneCapture(): Promise<boolean>;
  isMicrophoneCaptureActive(): Promise<boolean>;

  isMicRouteRecovering?(): Promise<boolean>;

  getMicInputDiagnostics?(): Promise<MicInputDiagnostics | null>;

  setStreamingCallback(callback: StreamingCallback | null): void;
  startProspectStreaming(options: { streamingCallback: StreamingCallback }): Promise<AudioCaptureResult>;

  setLifecycleEventCallback?(callback: ((event: string) => void) | null): void;
}
