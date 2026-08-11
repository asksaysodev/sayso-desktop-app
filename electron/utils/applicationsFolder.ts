import { app, dialog, shell } from 'electron';
import * as Sentry from '@sentry/electron/main';
import path from 'node:path';
import { IS_MAC } from './platform';

/**
 * macOS-only: keep the app out of locations Squirrel can't write to.
 *
 * There are exactly two ways a user ends up running Sayso from a read-only
 * volume, and both look identical to the updater:
 *  1. Launching Sayso.app straight out of the mounted .dmg (DMGs mount read-only).
 *  2. App Translocation — the bundle was unzipped/dragged somewhere without
 *     going through Finder's move-to-Applications, so Gatekeeper runs the
 *     quarantined copy from a randomized read-only mount under
 *     /private/var/folders/…/AppTranslocation/….
 *
 * From either one, `autoUpdater` fails with Squirrel's "read-only volume"
 * error and the install is permanently stuck at whatever version was
 * downloaded. Translocation additionally poisons permissions: screen recording
 * is granted against a bundle path that stops existing the moment the user
 * moves the app.
 */

/** The .app bundle, not the executable inside it (…/Sayso.app/Contents/MacOS/Sayso). */
function appBundlePath(): string {
  return path.resolve(app.getPath('exe'), '..', '..', '..');
}

/** App Translocation mounts the bundle under a randomized, read-only path. */
function isTranslocated(): boolean {
  return app.getPath('exe').includes('/AppTranslocation/');
}

/**
 * Whether the running bundle sits somewhere the updater can't write to.
 *
 * Only meaningful for packaged macOS builds: `isInApplicationsFolder()` is
 * darwin-only, and an unpackaged dev run lives in the repo by definition.
 * Answering `false` everywhere else keeps every caller free of its own
 * platform branch.
 *
 * Fails closed: if the native check itself throws, we can't confirm the
 * location is safe, so we treat it as outside rather than silently letting
 * a DMG/translocated launch through uncaught.
 */
export function isOutsideApplicationsFolder(): boolean {
  if (!IS_MAC || !app.isPackaged) return false;
  try {
    return !app.isInApplicationsFolder();
  } catch (err) {
    Sentry.captureException(err);
    return true;
  }
}

function showManualInstructions(): void {
  const bundle = appBundlePath();
  const detail = isTranslocated()
    ? 'macOS is running Sayso from a temporary read-only copy because it was opened without being moved into Applications first.\n\n1. Quit Sayso.\n2. In Finder, drag the Sayso app you downloaded into your Applications folder.\n3. Open Sayso from Applications.'
    : '1. Quit Sayso.\n2. In Finder, drag Sayso.app into your Applications folder.\n3. Eject the Sayso installer.\n4. Open Sayso from Applications.';

  const choice = dialog.showMessageBoxSync({
    type: 'warning',
    buttons: ['Show in Finder', 'Quit'],
    defaultId: 0,
    cancelId: 1,
    noLink: true,
    title: 'Move Sayso to Applications',
    message: 'Sayso couldn’t move itself to your Applications folder.',
    detail,
  });

  if (choice === 0) {
    try {
      shell.showItemInFolder(bundle);
    } catch {}
  }
}

/**
 * Hard-block launch from outside /Applications on packaged production macOS
 * builds. Returns `false` when the caller must stop booting — the app is
 * either quitting or relaunching from its new home.
 *
 * Must run inside `app.whenReady()` *before any window is created*: the point
 * is that a user in this state never reaches onboarding and grants permissions
 * against a path that's about to disappear. There is deliberately no
 * "continue anyway" path.
 *
 * Staging is excluded so staging builds stay runnable from wherever they were
 * unzipped for testing; they still get the update gate in main.ts, which is
 * what actually prevents the doomed Squirrel call.
 */
export function enforceApplicationsFolderLocation(isStagingBuild: boolean): boolean {
  if (isStagingBuild) return true;
  if (!isOutsideApplicationsFolder()) return true;

  const choice = dialog.showMessageBoxSync({
    type: 'warning',
    buttons: ['Move to Applications', 'Quit'],
    defaultId: 0,
    cancelId: 1,
    noLink: true,
    title: 'Move Sayso to Applications',
    message: 'Sayso needs to be in your Applications folder to install updates.',
    detail: 'Sayso is running from a location it can’t update itself from. Moving it to Applications takes a moment and reopens Sayso automatically.',
  });

  if (choice === 0) {
    try {
      if (app.moveToApplicationsFolder()) return false;
    } catch {
      // fall through to showManualInstructions() below
    }
    showManualInstructions();
  }

  app.quit();
  return false;
}
