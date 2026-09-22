// Windows writes the Launch at Login entry to HKCU\…\CurrentVersion\Run under a
// value name that defaults to the app's AppUserModelId. main.ts now pins that to
// build.appId; before it did, Electron derived "electron.app.<app.name>". Flipping
// the AUMID therefore strands any entry a user already enabled: the old value keeps
// pointing at the exe (so Windows keeps launching it) while the toggle, which reads
// and writes under the new name, reports Disabled. This migration closes that gap.
import { app } from 'electron';
import { IS_WINDOWS } from './platform';

/**
 * Re-registers a Windows login item that was written under a pre-AUMID value name.
 *
 * Deliberately name-agnostic: rather than hardcoding the legacy name, it matches
 * launchItems on the executable path, so it also catches entries left by any other
 * name the app has ever registered under. No-op off Windows, and a no-op on Windows
 * once the user's entry is already named after the current AUMID.
 *
 * Call once at startup, after the single-instance lock and before any window.
 */
export function migrateLegacyWindowsLoginItem(appId: string): void {
  if (!IS_WINDOWS) return;

  // Mirrors the AUMID gate in main.ts: dev runs keep Electron's derived name, so
  // there is nothing to migrate to and `appId` is not the name this process would
  // write under. Without this, a dev launch would delete the dev toggle's own Run
  // value (its name differs from appId) on every start.
  if (!app.isPackaged) return;

  try {
    const settings = app.getLoginItemSettings();
    const exePath = process.execPath.toLowerCase();

    // Scope 'machine' entries live under HKLM and were not written by this app —
    // a per-user install cannot have created them, and removing them would need
    // elevation we do not have.
    //
    // Registry value names are case-insensitive, so compare them that way — an entry
    // differing from appId only in case is the SAME entry, not a stale one, and
    // deleting it would drop a login item Electron had reported as present.
    const appIdLower = appId.toLowerCase();
    const stale = settings.launchItems.filter(
      item =>
        item.scope === 'user' &&
        item.name.toLowerCase() !== appIdLower &&
        item.path.toLowerCase() === exePath
    );
    if (stale.length === 0) return;

    // Preserve the Task Manager "Enabled/Disabled" state the user may have set:
    // if they disabled the old entry there, the re-registered one stays disabled.
    const wasEnabled = stale.some(item => item.enabled);

    // Register BEFORE removing, never after. If the write fails (policy, roaming
    // profile, locked registry) the catch below swallows it, and having deleted first
    // would leave the user with no startup entry at all — silently worse than the
    // stranded one being migrated. This order fails safe: the worst case is a
    // duplicate pointing at the same exe, and the single-instance lock absorbs that.
    //
    // openAtLogin reads the current-AUMID value, so `true` means an entry under the
    // new name already exists and the removal below is just dropping the duplicate.
    if (!settings.openAtLogin) {
      app.setLoginItemSettings({ openAtLogin: true, enabled: wasEnabled });
    }

    for (const item of stale) {
      app.setLoginItemSettings({ openAtLogin: false, name: item.name });
    }

    console.log(
      `[launch-at-login] migrated ${stale.length} legacy Run value(s) to "${appId}" (enabled: ${wasEnabled})`
    );
  } catch (error) {
    // Registry access is not worth failing startup over — the user can always
    // re-toggle the setting by hand.
    console.warn('[launch-at-login] legacy Run value migration failed:', error);
  }
}
