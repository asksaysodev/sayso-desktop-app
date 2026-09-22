import { app } from 'electron';
import fs from 'node:fs';
import path from 'node:path';

/**
 * Log retention and the one-time purge of pre-redaction logs (SAYSO-460).
 *
 * Before the redaction work landed, every axios failure wrote a live
 * `Authorization: Bearer …` into the daily log file, and nothing ever deleted
 * those files. Redacting future writes does not remove what is already on disk,
 * which is why cleanup ships in the same change rather than as a follow-up.
 *
 * AGE-PRUNING ALONE IS NOT ENOUGH. The file holding a *live* token is
 * `sayso-<today>.log` — zero days old. Any age policy leaves it in place for the
 * length of the retention window after the user installs the fix. So the first
 * launch of a redacting build purges every log unconditionally, and only then
 * does the age policy take over.
 *
 * Scrubbing the existing files in place was considered and rejected: it means
 * rewriting arbitrary-size user files during boot, it can fail halfway and leave
 * a file that *looks* clean, and it cannot be verified after the fact. Deletion
 * is atomic and total, and Sentry already holds the events that matter.
 */

/**
 * Only the daily files written by setupLogging(). Anchored, so it can never
 * match anything else that lands in the directory.
 */
export const LOG_FILE_PATTERN = /^sayso-(\d{4})-(\d{2})-(\d{2})\.log$/;

export const RETENTION_DAYS = 7;

/**
 * Bump to force another full purge on every install's next launch — e.g. if the
 * key list widens and older logs turn out to hold something the new rules would
 * have caught. An integer rather than a boolean for exactly that reason; same
 * reasoning as STORE_VERSION in electron/store/persistentStore.ts.
 */
export const REDACTION_EPOCH = 1;

/**
 * Lives in userData, NOT in logs/ — so a purge can never delete its own marker.
 * Mirrors permissions-team-id.json (electron/utils/permissionsMigration.ts).
 */
const MARKER_FILE = 'log-redaction.json';

function markerPath(): string {
  return path.join(app.getPath('userData'), MARKER_FILE);
}

function readEpoch(): number {
  try {
    const raw = fs.readFileSync(markerPath(), 'utf-8');
    const parsed = (JSON.parse(raw) as { epoch?: unknown }).epoch;
    return typeof parsed === 'number' ? parsed : 0;
  } catch {
    // Missing, unreadable or corrupt — treat as "never purged" and purge.
    return 0;
  }
}

function writeEpoch(): void {
  try {
    fs.writeFileSync(markerPath(), JSON.stringify({ epoch: REDACTION_EPOCH }));
  } catch (err) {
    // Not fatal: the next launch simply purges again. Better than throwing
    // inside whenReady, and better than skipping the purge entirely.
    console.warn('[LogRetention] Could not persist purge marker:', (err as Error)?.message);
  }
}

/** Age from the date in the FILENAME, not mtime — backup/restore tools and
 *  Time Machine rewrite mtimes, and the filename is what setupLogging() chose. */
function ageInDays(match: RegExpMatchArray, now: number): number {
  const stamped = Date.UTC(Number(match[1]), Number(match[2]) - 1, Number(match[3]));
  return (now - stamped) / 86_400_000;
}

/**
 * Prune `<userData>/logs`. Synchronous and total: it never throws, and a failure
 * degrades to "no prune", never to a failed boot — this runs inside whenReady,
 * before any window exists, where a throw is a black-screen launch.
 *
 * DELETES INDIVIDUAL FILES ONLY. Never rmdir, never `recursive: true`.
 * electron/utils/permissionsMigration.ts:93 reads the *existence* of this
 * directory as the macOS "this user had a prior install" signal. Removing it
 * would classify every upgrading user as a first install — and, in the inverse
 * case, fire a spurious `tccutil reset` of Microphone and Screen Recording.
 *
 * Does not touch `main.log`: that is electron-log's file (wired for the updater
 * in main.ts), it holds an open write stream, and it runs its own 1 MB rotation.
 * Unlinking underneath it is EBUSY on Windows, and on POSIX it succeeds while
 * the updater's writes vanish into a dangling inode — a failure you would only
 * discover during an update incident. It carries no Authorization headers
 * anyway. `debug-startup.log` and `shipit-watchdog.log` are likewise not ours.
 */
export function enforceLogRetention(logDir: string): void {
  try {
    const storedEpoch = readEpoch();
    const purgeEverything = storedEpoch < REDACTION_EPOCH;
    const now = Date.now();

    let entries: string[];
    try {
      entries = fs.readdirSync(logDir);
    } catch {
      // First install, or the user deleted the directory. Nothing to do.
      return;
    }

    let removed = 0;
    let failures = 0;

    for (const name of entries) {
      const match = name.match(LOG_FILE_PATTERN);
      if (!match) continue;
      if (!purgeEverything && ageInDays(match, now) <= RETENTION_DAYS) continue;

      try {
        fs.rmSync(path.join(logDir, name), { force: true });
        removed++;
      } catch (err) {
        // One locked file must not abort the rest — same shape as
        // removeLegacyCacheFiles() in electron/store/persistentStore.ts.
        failures++;
        console.warn(`[LogRetention] Could not remove ${name}:`, (err as Error)?.message);
      }
    }

    if (purgeEverything) {
      // Only mark the epoch once everything actually went. If a file was locked,
      // leaving the marker unwritten means the next launch retries it rather
      // than stranding a contaminated file on disk forever.
      if (failures === 0) writeEpoch();
      console.log(
        `[LogRetention] Purged ${removed} pre-redaction log file(s)` +
          (failures > 0 ? ` — ${failures} could not be removed, will retry next launch` : ''),
      );
    } else if (removed > 0) {
      console.log(`[LogRetention] Removed ${removed} log file(s) older than ${RETENTION_DAYS} days`);
    }
  } catch (err) {
    console.warn('[LogRetention] Retention pass failed:', (err as Error)?.message);
  }
}
