import { app } from 'electron';
import { spawnSync } from 'child_process';
import fs from 'node:fs';
import path from 'node:path';

// Update this constant whenever the signing certificate's team ID changes.
// On mismatch, stale TCC entries are cleared once so macOS re-prompts cleanly.
const CURRENT_TEAM_ID = 'AFGHD8M3VK';

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

function getAppBundleId(): string {
  try {
    const appBundle = app.getPath('exe').split('/Contents/MacOS')[0];
    const result = spawnSync('defaults', ['read', `${appBundle}/Contents/Info.plist`, 'CFBundleIdentifier'], { encoding: 'utf8' });
    if (result.error) {
      console.warn('[Migration] getAppBundleId failed, using fallback:', result.error);
      return 'com.asksayso.app';
    }
    return result.stdout.trim() || 'com.asksayso.app';
  } catch {
    return 'com.asksayso.app';
  }
}

export function resetPermissionsIfCertChanged(): void {
  if (process.platform !== 'darwin') return;

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
      storeTeamId(CURRENT_TEAM_ID);
      return;
    }
    // Existing user upgrading from a pre-migration version — fall through to reset
  }
  if (storedTeamId === CURRENT_TEAM_ID) return;

  const bundleId = getAppBundleId();
  const screenResult = spawnSync('tccutil', ['reset', 'ScreenCapture', bundleId]);
  const micResult = spawnSync('tccutil', ['reset', 'Microphone', bundleId]);

  if (screenResult.error || micResult.error || screenResult.status !== 0 || micResult.status !== 0) {
    console.warn('[Migration] Failed to reset TCC permissions:', screenResult.error ?? micResult.error ?? `exit ${screenResult.status ?? micResult.status}`);
    return;
  }

  storeTeamId(CURRENT_TEAM_ID);
  console.log(`[Migration] TCC permissions reset for ${bundleId} (${storedTeamId ?? 'none'} → ${CURRENT_TEAM_ID})`);
}
