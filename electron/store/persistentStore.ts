import { app } from 'electron';
import fs from 'fs';
import path from 'path';

// Bytes only. This module knows how to put a blob of cache entries on disk and
// get it back; it knows nothing about what the entries mean, which account they
// belong to beyond carrying the tag, or who reads them. Everything above it
// (cacheRegistry / cacheManager) owns that.
//
// Nothing here throws. A cache that cannot be persisted must degrade to "we
// have no warm start" — never to a broken boot.

// Bump when the envelope or any entry's serialized shape changes. A file
// written by a different version is discarded rather than migrated: every entry
// is re-fetchable from the API, so the cost of discarding is one cold start,
// and the cost of maintaining migrations is forever.
export const STORE_VERSION = 1;

export interface StoreFile {
  version: number;
  // The account these entries were written for. Compared on hydrate so one
  // user's cache can never be read into another user's session — including
  // after an unclean shutdown that never ran the sign-out cleanup.
  accountId: string | null;
  cachedAt: number;
  entries: Record<string, unknown>;
}

const storePath = () => path.join(app.getPath('userData'), 'sayso-cache.json');

// Written first, then renamed over the real file. rename(2) is atomic within a
// filesystem, so a crash mid-write leaves either the old file or the new one —
// never a truncated one. The old hand-rolled caches wrote in place and could
// leave a half-written JSON that every subsequent boot failed to parse.
const tempPath = () => `${storePath()}.tmp`;

function discard(reason: string): void {
  try {
    fs.rmSync(storePath(), { force: true });
    console.warn(`[store] discarded cache file — ${reason}`);
  } catch (err) {
    console.warn('[store] could not discard cache file:', (err as Error)?.message);
  }
}

export function readStoreFile(): StoreFile | null {
  let raw: string;
  try {
    raw = fs.readFileSync(storePath(), 'utf8');
  } catch (err) {
    if ((err as NodeJS.ErrnoException).code !== 'ENOENT') {
      // Unreadable for a reason other than "not there" — permissions, a
      // directory in its place. Drop it so we're not re-reading a dud forever.
      discard(`unreadable: ${(err as Error)?.message}`);
    }
    return null;
  }

  let parsed: unknown;
  try {
    parsed = JSON.parse(raw);
  } catch {
    discard('unparseable JSON');
    return null;
  }

  const file = parsed as Partial<StoreFile> | null;
  if (!file || typeof file !== 'object' || typeof file.entries !== 'object' || file.entries === null) {
    discard('unexpected shape');
    return null;
  }
  if (file.version !== STORE_VERSION) {
    discard(`version ${String(file.version)} != ${STORE_VERSION}`);
    return null;
  }

  return {
    version: STORE_VERSION,
    accountId: typeof file.accountId === 'string' ? file.accountId : null,
    cachedAt: typeof file.cachedAt === 'number' ? file.cachedAt : 0,
    entries: file.entries as Record<string, unknown>,
  };
}

export function writeStoreFile(accountId: string | null, entries: Record<string, unknown>): void {
  const file: StoreFile = { version: STORE_VERSION, accountId, cachedAt: Date.now(), entries };
  const tmp = tempPath();
  try {
    fs.writeFileSync(tmp, JSON.stringify(file));
    fs.renameSync(tmp, storePath());
  } catch (err) {
    console.warn('[store] could not persist cache file:', (err as Error)?.message);
    try {
      fs.rmSync(tmp, { force: true });
    } catch {
      // Best effort. A stray .tmp is harmless — the next write overwrites it.
    }
  }
}

export function clearStoreFile(): void {
  try {
    fs.rmSync(storePath(), { force: true });
  } catch (err) {
    console.warn('[store] could not clear cache file:', (err as Error)?.message);
  }
}

// The one-file store replaces these two. They are deleted rather than folded in:
// both are re-fetched on every boot anyway, so migrating buys one warm playbook
// window on the upgrade launch and costs a migration path we'd carry forever.
// Safe to delete this function once 1.3.x has been out long enough that no
// install still has the old files.
const LEGACY_CACHE_FILES = ['playbooks-cache.json', 'open-last-used-cache.json'];

export function removeLegacyCacheFiles(): void {
  for (const name of LEGACY_CACHE_FILES) {
    try {
      fs.rmSync(path.join(app.getPath('userData'), name), { force: true });
    } catch (err) {
      console.warn(`[store] could not remove legacy cache ${name}:`, (err as Error)?.message);
    }
  }
}
