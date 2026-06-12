import { app } from 'electron';
import { execSync } from 'child_process';
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
  fs.writeFileSync(getMigrationFilePath(), JSON.stringify({ teamId }));
}

function getAppBundleId(): string {
  try {
    const appBundle = app.getPath('exe').split('/Contents/MacOS')[0];
    return execSync(`defaults read "${appBundle}/Contents/Info.plist" CFBundleIdentifier`, { encoding: 'utf8' }).trim();
  } catch {
    return 'com.asksayso.app';
  }
}

export function resetPermissionsIfCertChanged(): void {
  if (process.platform !== 'darwin') return;

  const storedTeamId = getStoredTeamId();
  if (storedTeamId === CURRENT_TEAM_ID) return;

  const bundleId = getAppBundleId();
  try {
    execSync(`tccutil reset ScreenCapture ${bundleId}`);
    execSync(`tccutil reset Microphone ${bundleId}`);
    console.log(`[Migration] TCC permissions reset for ${bundleId} (${storedTeamId ?? 'none'} → ${CURRENT_TEAM_ID})`);
  } catch (err) {
    console.warn('[Migration] Failed to reset TCC permissions:', err);
  }

  storeTeamId(CURRENT_TEAM_ID);
}
