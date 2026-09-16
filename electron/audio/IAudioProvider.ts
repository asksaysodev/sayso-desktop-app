export interface AudioCaptureResult {
  success: boolean;
  error?: string;
}

// SAYSO-347: distinct, greppable reasons for a mic start failure — previously collapsed into a
// bare `false`, indistinguishable in Sentry between a wiring bug, a teardown race, and the three
// genuinely different ways the audio engine itself can fail to come up (no input node at all, a
// format/route mismatch on tap install, or the engine's own start call erroring — vs. the engine
// starting cleanly but silently delivering nothing).
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
  /** SAYSO-428 (macOS mic only): dB of AGC gain the native side already applied to this block, 0 when
   *  untouched. Absent on providers/builds that predate it — treat as 0. Consumers judging the mic's
   *  real level must subtract it; the boost would otherwise mask a dead mic. */
  gainDb?: number;
}

export type StreamingCallback = (buffer: Buffer, format: AudioFormat) => void;

/** SAYSO-431: one HAL device as native sees it. `null` fields mean the device doesn't implement that
 *  property or the read failed — never a default value. */
export interface MicDeviceSnapshot {
  id: number;
  name: string | null;
  uid: string | null;
  /** built_in / bluetooth / bluetooth_le / usb / virtual / aggregate / continuity_wired / … / other / unknown */
  transport: string;
  nominalSampleRate: number | null;
  inputChannels: number | null;
  isAlive: boolean | null;
  /** Any process (ours included) is doing IO on the device. */
  isRunningSomewhere: boolean | null;
  /** Process holding exclusive access; -1 when nobody does. */
  hogModePid: number | null;
  inputMuted: boolean | null;
  inputVolume: number | null;
}

/** SAYSO-431: snapshot returned by getMicInputDiagnostics. Tap counters cover the current (or last
 *  failed) engine build; `msSinceLast*` on the tap are process-wide. */
export interface MicInputDiagnostics {
  defaultInput: MicDeviceSnapshot | null;
  openedInputId: number | null;
  /** Only set when the device our engine opened is no longer the OS default. */
  openedInput: MicDeviceSnapshot | null;
  capturing: boolean;
  routeRecovering: boolean;
  /** We started the engine and have seen neither a teardown nor a configuration-change stop since. */
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
  };
}

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

  // SAYSO-353: true while native's own backoff recovery loop is retrying a mic route restart.
  // Optional so a stale native build without the method stays contract-compatible — callers
  // should treat its absence as "not recovering" (fail open, don't block on it).
  isMicRouteRecovering?(): Promise<boolean>;

  // SAYSO-431: diagnostics attached to mic stall / start-failure reports. Optional, and resolves null
  // when the native build lacks it or a snapshot is already in flight — callers must never block a
  // restart or a report on it.
  getMicInputDiagnostics?(): Promise<MicInputDiagnostics | null>;

  // Streaming
  setStreamingCallback(callback: StreamingCallback | null): void;
  startProspectStreaming(options: { streamingCallback: StreamingCallback }): Promise<AudioCaptureResult>;

  // Native lifecycle diagnostics (SAYSO-355): short snake_case event strings emitted for
  // capture-lifecycle anomalies (watchdog fired, orphaned stream, late callbacks). Optional so a
  // stale native build without the method stays contract-compatible; events ending in `_failed`
  // should be escalated by the consumer.
  setLifecycleEventCallback?(callback: ((event: string) => void) | null): void;
}
