/**
 * The permissions wire contract, shared by both TypeScript programs.
 *
 * Main owns the `permissions-check` payload (permissionsManager.ts builds it
 * from the active IPermissionsProvider), so the definition lives on the main
 * side and `src/types/permissions.ts` re-exports it — same direction and same
 * reasoning as electron/shared/update.ts: tsconfig.electron.json pins
 * `rootDir` to `electron/`, so electron → src imports are a hard TS6059 error,
 * while src → electron works because the renderer program is `noEmit`.
 *
 * TYPES ONLY. This file is pulled into the renderer's program, so it must
 * never import electron or node APIs — declarations here are erased at build
 * time and nothing from it may survive into the bundle.
 */

// What the running OS actually gates. Static per platform, so the renderer
// can lay the permissions screen out from the payload instead of branching on
// process.platform.
export interface PermissionRequirements {
  // Is system-audio capture behind an OS permission? macOS: Screen & System
  // Audio Recording (ScreenCaptureKit). Windows: no — WASAPI loopback has no gate.
  screen: boolean;
  // Must the app relaunch after the permissions step? macOS binds the
  // screen-recording grant at process launch, so yes. Windows: no.
  relaunchOnComplete: boolean;
}

// Payload of the `permissions-check` invoke. Distinct from the provider-side
// PermissionsStatus (`{ granted, mic, screen }`): the handler drops `granted`
// and adds `requirements`, and carries `error` when the read failed.
export interface PermissionsCheckResult {
  mic: boolean;
  screen: boolean;
  requirements: PermissionRequirements;
  error?: string;
}
