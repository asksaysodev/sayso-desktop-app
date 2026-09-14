export interface PermissionsStatus {
  granted: boolean;
  mic: boolean;
  screen: boolean;
}

export type RequestMicAction = 'already-granted' | 'asked' | 'open-settings' | 'error';

export interface RequestMicResult {
  mic: boolean;
  action: RequestMicAction;
  error?: string;
}

// What the running OS actually gates. Static per platform; the renderer reads it
// from `permissions-check` so it never has to branch on process.platform.
export interface PermissionRequirements {
  // Is system-audio capture behind an OS permission? macOS: Screen & System Audio
  // Recording (ScreenCaptureKit). Windows: no — WASAPI loopback has no gate.
  screen: boolean;
  // Must the app relaunch after the permissions step? macOS binds the
  // screen-recording grant at process launch, so yes. Windows: no.
  relaunchOnComplete: boolean;
}

export interface IPermissionsProvider {
  readonly requirements: PermissionRequirements;

  // Live mic + screen grant status (non-prompting). Used for polling + the cue pre-flight.
  checkGranted(): Promise<PermissionsStatus>;

  // Live microphone grant only (non-prompting). Cheaper than checkGranted() for
  // mic-only gates — skips the screen-recording preflight (and its diagnostic).
  checkMic(): Promise<boolean>;

  // Are all OS permissions required to run granted (and, where one exists, the
  // onboarding flag set)? For startup routing. macOS self-heals the completion
  // flag when live grants are present; Windows has no flag and reads live only.
  isComplete(): boolean;

  // Request microphone access. May prompt or open System Settings. Never restarts the app.
  requestMic(): Promise<RequestMicResult>;

  // Ask the OS to surface the Screen Recording prompt / settings row (fire-and-forget).
  requestScreen(): void;

  // Open the Screen Recording privacy pane directly.
  openScreenSettings(): Promise<void>;

  // Persist the onboarding "permissions complete" flag. Throws on write failure.
  // No-op on platforms where completion is derived from live OS state.
  markComplete(): void;
}
