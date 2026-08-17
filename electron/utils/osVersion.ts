import { app, dialog } from 'electron';
import { IS_MAC } from './platform';

/**
 * macOS-only: system-audio capture (ScreenCaptureKit's `capturesAudio`) does not
 * exist before macOS 13. `LSMinimumSystemVersion` (set via `mac.extendInfo` in
 * package.json, baked into the packaged Info.plist by electron-builder) stops
 * most pre-13 users before they ever launch, but that's a Finder/LaunchServices
 * check — a copy that bypasses it (direct binary launch, some update paths) can
 * still reach here. This is the app-level backstop, matching SAYSO_MIN_MACOS
 * below.
 */
const SAYSO_MIN_MACOS_MAJOR = 13;

function macOSMajorVersion(): number | null {
  const major = Number.parseInt(process.getSystemVersion().split('.')[0] ?? '', 10);
  return Number.isNaN(major) ? null : major;
}

/**
 * Hard-block launch on macOS below SAYSO_MIN_MACOS_MAJOR. Returns `false` when
 * the caller must stop booting — the app is quitting.
 *
 * Must run inside `app.whenReady()` before any window is created, same as
 * enforceApplicationsFolderLocation(): a user here should never reach onboarding
 * or trigger native audio code that assumes macOS 13 APIs exist (SAYSO-A3).
 *
 * An unparseable version string fails open (returns true) rather than blocking
 * launch on a version we simply couldn't read. Skipped for unpackaged (dev)
 * runs so working on an older Mac doesn't block local development.
 */
export function enforceMinimumMacOSVersion(): boolean {
  if (!IS_MAC || !app.isPackaged) return true;

  const major = macOSMajorVersion();
  if (major === null || major >= SAYSO_MIN_MACOS_MAJOR) return true;

  dialog.showMessageBoxSync({
    type: 'warning',
    buttons: ['Quit'],
    defaultId: 0,
    noLink: true,
    title: 'macOS Update Required',
    message: `Sayso requires macOS ${SAYSO_MIN_MACOS_MAJOR} or later.`,
    detail: `You're running macOS ${process.getSystemVersion()}. Please update macOS to continue using Sayso.`,
  });

  app.quit();
  return false;
}
