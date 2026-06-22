import { app } from 'electron';
import { spawnSync } from 'child_process';
import fs from 'node:fs';
import path from 'node:path';

// The team ID is read at runtime from the running app's own code signature
// (see getRunningTeamId). When it differs from the stored value, stale TCC
// entries are cleared once so macOS re-prompts cleanly under the new identity.
// No hardcoded constant — the migration self-detects any signing change.
const MIGRATION_FILE = 'permissions-team-id.json';

function getMigrationFilePath(): string {
  return path.join(app.getPath('userData'), MIGRATION_FILE);
}

function getStoredTeamId(): string | null {
  try {
    const raw = fs.readFileSync(getMigrationFilePath(), 'utf-8');
    return (JSON.parse(raw) as { teamId?: string }).teamId ?? null;
  } catch {
    return null;
  }
}

function storeTeamId(teamId: string): void {
  try {
    fs.writeFileSync(getMigrationFilePath(), JSON.stringify({ teamId }));
  } catch (err) {
    console.warn('[Migration] Failed to persist team ID:', err);
  }
}

function getAppBundlePath(): string {
  return app.getPath('exe').split('/Contents/MacOS')[0];
}

function getAppBundleId(): string {
  try {
    const appBundle = getAppBundlePath();
    const result = spawnSync('defaults', ['read', `${appBundle}/Contents/Info.plist`, 'CFBundleIdentifier'], { encoding: 'utf8', timeout: 5000 });
    if (result.error || result.status !== 0) {
      console.warn('[Migration] getAppBundleId failed, using fallback:', result.error ?? `exit ${result.status}`);
      return 'com.asksayso.app';
    }
    return result.stdout.trim() || 'com.asksayso.app';
  } catch {
    return 'com.asksayso.app';
  }
}

// Reads the Apple Team ID from the running app's own code signature.
// Returns null for unsigned / ad-hoc / dev builds (TeamIdentifier absent or "not set"),
// in which case the caller treats the migration as a safe no-op.
function getRunningTeamId(): string | null {
  try {
    const result = spawnSync('codesign', ['-dvvv', getAppBundlePath()], { encoding: 'utf8', timeout: 5000 });
    if (result.error || result.status !== 0) {
      console.warn('[Migration] codesign read failed:', result.error ?? `exit ${result.status}`);
      return null;
    }
    // codesign writes its verbose info to stderr.
    const output = `${result.stderr ?? ''}${result.stdout ?? ''}`;
    const match = output.match(/TeamIdentifier=([^\n]+)/);
    if (!match) return null;
    const teamId = match[1].trim();
    return teamId === 'not set' ? null : teamId;
  } catch (err) {
    console.warn('[Migration] getRunningTeamId threw:', err);
    return null;
  }
}

export function resetPermissionsIfCertChanged(): void {
  if (process.platform !== 'darwin') return;

  // Read the team ID the running binary is actually signed with. If we can't
  // determine it (dev/ad-hoc/unsigned), there's no reliable identity to migrate
  // against — do nothing rather than risk a spurious reset.
  const currentTeamId = getRunningTeamId();
  if (!currentTeamId) {
    console.log('[Migration] No signing team ID readable (dev/ad-hoc/unsigned build) — skipping');
    return;
  }

  const storedTeamId = getStoredTeamId();
  if (storedTeamId === null) {
    const userDataPath = app.getPath('userData');
    // Key off specific pre-migration app files rather than a generic directory scan —
    // Electron writes Cache/GPUCache/etc. before whenReady fires and would falsely
    // classify a true first install as an upgrade.
    const hasPriorInstall =
      fs.existsSync(path.join(userDataPath, 'auth.json')) ||
      fs.existsSync(path.join(userDataPath, 'logs'));
    if (!hasPriorInstall) {
      console.log(`[Migration] First install — recording team ID ${currentTeamId}, no reset`);
      storeTeamId(currentTeamId);
      return;
    }
    // Existing user upgrading from a pre-migration version — fall through to reset
    console.log(`[Migration] Pre-migration install (no stored team ID) — resetting under ${currentTeamId}`);
  }
  if (storedTeamId === currentTeamId) {
    console.log(`[Migration] Team ID unchanged (${currentTeamId}) — no action`);
    return;
  }

  console.log(`[Migration] Team ID change detected (${storedTeamId ?? 'none'} → ${currentTeamId}) — resetting TCC permissions`);
  const bundleId = getAppBundleId();
  const screenResult = spawnSync('tccutil', ['reset', 'ScreenCapture', bundleId]);
  const micResult = spawnSync('tccutil', ['reset', 'Microphone', bundleId]);

  if (screenResult.error || micResult.error || screenResult.status !== 0 || micResult.status !== 0) {
    console.warn('[Migration] Failed to reset TCC permissions:', screenResult.error ?? micResult.error ?? `exit ${screenResult.status ?? micResult.status}`);
    return;
  }

  // Clear the permissions-complete flag so the user goes through the permissions
  // flow again after the TCC reset (they'll need to re-grant mic + screen recording).
  const permissionsFlag = path.join(app.getPath('userData'), 'permissions-complete');
  try {
    if (fs.existsSync(permissionsFlag)) fs.unlinkSync(permissionsFlag);
  } catch (err) {
    console.warn('[Migration] Failed to delete permissions-complete flag:', err);
  }

  storeTeamId(currentTeamId);
  console.log(`[Migration] TCC permissions reset for ${bundleId} (${storedTeamId ?? 'none'} → ${currentTeamId})`);
}
