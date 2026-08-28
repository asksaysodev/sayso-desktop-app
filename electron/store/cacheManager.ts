import * as Sentry from '@sentry/electron/main';
import { isTransientNetworkError } from '../utils/transientErrors';
import { readStoreFile, writeStoreFile, clearStoreFile } from './persistentStore';
import {
  CACHE_KEYS,
  CACHE_REGISTRY,
  type CacheEntryDef,
  type CacheKey,
  type CacheSource,
  type CacheValues,
  type ClearEvent,
  type NotifyTarget,
} from './cacheRegistry';

// Runtime for the declared caches: in-memory values, the one writer everything
// goes through, hydration from disk, clearing, and the retry ladder.
//
// Deliberately knows nothing about main.ts — it's injected with what it needs,
// so main can import it without a cycle and so it can be exercised without an
// Electron app running.

export interface CacheManagerDeps {
  backendBaseUrl: () => string;
  getAccessToken: () => Promise<string | null>;
  isAuthenticated: () => boolean;
  getAccountId: () => string | null;
  // main owns the fan-out helpers, so sending stays there rather than this
  // module growing its own BrowserWindow knowledge.
  broadcast: (target: NotifyTarget, channel: string, payload: unknown) => void;
}

let deps: CacheManagerDeps | null = null;

// Same ladder as the one this replaces (SAYSO-376): ~52s of attempts.
const RETRY_DELAYS_MS = [2000, 5000, 15000, 30000];

const values: { [K in CacheKey]: CacheValues[K] } = {
  fontSize: CACHE_REGISTRY.fontSize.fallback,
  enabledFeatures: CACHE_REGISTRY.enabledFeatures.fallback,
  playbooks: CACHE_REGISTRY.playbooks.fallback,
  openLastUsed: CACHE_REGISTRY.openLastUsed.fallback,
};

// Two different questions, and conflating them is what the old code got wrong.
// `known` — we hold a real value from somewhere, including disk; it may be
// persisted and served. `loaded` — the server (or the user) has confirmed it
// during THIS session; only this stands the retry ladder down. A hydrated disk
// value is known but not loaded, so a warm start still gets reconciled.
const known: Record<CacheKey, boolean> = { fontSize: false, enabledFeatures: false, playbooks: false, openLastUsed: false };
const loaded: Record<CacheKey, boolean> = { fontSize: false, enabledFeatures: false, playbooks: false, openLastUsed: false };

const retryTimers: Record<CacheKey, NodeJS.Timeout | null> = { fontSize: null, enabledFeatures: null, playbooks: null, openLastUsed: null };
const retryInFlight: Record<CacheKey, boolean> = { fontSize: false, enabledFeatures: false, playbooks: false, openLastUsed: false };

// Bumped whenever the world a fetch was issued into stops being the world its
// result would land in: the account changed, or the user set the value
// themselves. Every fetch captures it before awaiting and drops its result if
// it moved. Cancelling timers cannot do this on its own — a callback already
// parked on an await is past every timer check, and its response still lands.
let generation = 0;

// Whose values we currently hold. Set by setAccount() at boot and on sign-in.
let ownerAccountId: string | null = null;

export function initCacheManager(injected: CacheManagerDeps): void {
  deps = injected;
}

function requireDeps(): CacheManagerDeps {
  if (!deps) throw new Error('[cache] used before initCacheManager()');
  return deps;
}

function defOf<K extends CacheKey>(key: K): CacheEntryDef<CacheValues[K]> {
  return CACHE_REGISTRY[key] as CacheEntryDef<CacheValues[K]>;
}

export function currentGeneration(): number {
  return generation;
}

export function isGenerationCurrent(gen: number): boolean {
  return gen === generation;
}

export function getValue<K extends CacheKey>(key: K): CacheValues[K] {
  return values[key];
}

export function isLoaded(key: CacheKey): boolean {
  return loaded[key];
}

function notify<K extends CacheKey>(key: K, value: CacheValues[K]): void {
  const def = defOf(key);
  if (!def.notify) return;
  const { channel, target, payload } = def.notify;
  requireDeps().broadcast(target, channel, payload(value));
}

function persist(): void {
  const accountId = requireDeps().getAccountId();
  // Never write data we cannot attribute — an untagged file could be read
  // into the wrong session on the next boot.
  if (!accountId) return;

  const entries: Record<string, unknown> = {};
  for (const key of CACHE_KEYS) {
    const def = defOf(key);
    if (!def.persist || !known[key]) continue;
    const value = values[key];
    if (def.persistable && !def.persistable(value as never)) continue;
    entries[key] = value;
  }

  if (Object.keys(entries).length === 0) {
    clearStoreFile();
    return;
  }
  writeStoreFile(accountId, entries);
}

/**
 * The single writer. Every value that reaches memory, disk or a window goes
 * through here, whatever produced it.
 *
 * Returns false if the value was rejected by the key's validate().
 */
export function setValue<K extends CacheKey>(key: K, raw: unknown, source: CacheSource): boolean {
  const def = defOf(key);
  const value = def.validate(raw);
  if (value === null) {
    console.warn(`[cache] ${key}: rejected ${source} value of unexpected shape`);
    return false;
  }

  values[key] = value;
  known[key] = true;

  // A disk value is a warm start, not a confirmation — leaving `loaded` false
  // keeps the normal fetch (and its ladder) running so the server still gets
  // the last word.
  if (source !== 'disk') loaded[key] = true;

  // The user's own choice outranks anything already in flight for this key.
  if (source === 'user') {
    generation += 1;
    cancelRetry(key);
  }

  if (source !== 'disk') persist();
  notify(key, value);
  return true;
}

/**
 * Fetch one key and store the result. The single fetch path — the boot batch
 * and the retry ladder both come through here, so the generation guard and the
 * shape check can't be forgotten at a call site.
 *
 * Throws on a failed request or a rejected payload, so callers can arm the
 * ladder from one rejection branch.
 */
export async function refreshKey(key: CacheKey, token: string): Promise<void> {
  const def = defOf(key);
  if (!def.refresh) return;

  const gen = generation;
  const raw = await def.refresh(requireDeps().backendBaseUrl(), token);

  if (!isGenerationCurrent(gen)) {
    console.log(`[cache] ${key}: dropping a result from a previous generation`);
    return;
  }
  if (!setValue(key, raw, 'server')) {
    throw new Error(`[cache] ${key}: unexpected payload shape`);
  }
}

export function cancelRetry(key: CacheKey): void {
  if (retryTimers[key]) clearTimeout(retryTimers[key]!);
  retryTimers[key] = null;
  retryInFlight[key] = false;
}

export function cancelAllRetries(): void {
  CACHE_KEYS.forEach(cancelRetry);
}

export function scheduleRetry(key: CacheKey, attempt = 0): void {
  const def = defOf(key);
  if (!def.retry || !def.refresh) return;

  if (attempt >= RETRY_DELAYS_MS.length) {
    retryInFlight[key] = false;
    return;
  }
  if (attempt === 0 && retryInFlight[key]) {
    console.log(`[cache] ${key}: retry already in flight — not starting a second chain`);
    return;
  }

  retryInFlight[key] = true;
  retryTimers[key] = setTimeout(async () => {
    retryTimers[key] = null;

    if (loaded[key] || !requireDeps().isAuthenticated()) {
      retryInFlight[key] = false;
      return;
    }

    const gen = generation;
    const token = await requireDeps().getAccessToken().catch(() => null);
    if (!isGenerationCurrent(gen)) {
      retryInFlight[key] = false;
      return;
    }
    if (!token) {
      scheduleRetry(key, attempt + 1);
      return;
    }

    try {
      await refreshKey(key, token);
      retryInFlight[key] = false;
      console.log(`[cache] ${key}: retry succeeded`);
    } catch (err) {
      console.warn(`[cache] ${key}: retry ${attempt + 1}/${RETRY_DELAYS_MS.length} failed:`, (err as Error)?.message);
      scheduleRetry(key, attempt + 1);
    }
  }, RETRY_DELAYS_MS[attempt]);
}

/** Arms the ladder for every key still unconfirmed — used on reconnect. */
export function scheduleRetriesForUnloaded(): void {
  CACHE_KEYS.forEach((key) => {
    if (!loaded[key]) scheduleRetry(key);
  });
}

/**
 * Reads the disk cache into memory. Must run before the first window opens,
 * and only once the session is confirmed: on a transient init() failure
 * getAccountId() is null even though a valid session exists, and comparing
 * against that null would read as a mismatch and delete a good cache.
 */
export function hydrate(): void {
  const accountId = requireDeps().getAccountId();
  if (!accountId) return;

  // At boot there is nothing in memory to invalidate, so claim ownership
  // directly rather than going through setAccount().
  ownerAccountId = accountId;

  const file = readStoreFile();
  if (!file) return;

  if (file.accountId !== accountId) {
    console.log('[cache] disk cache belongs to another account — discarding');
    clearStoreFile();
    return;
  }

  let count = 0;
  for (const key of CACHE_KEYS) {
    if (!(key in file.entries)) continue;
    if (setValue(key, file.entries[key], 'disk')) count += 1;
  }
  console.log(`[cache] hydrated ${count} value(s) from disk`);
}

/**
 * Applies each key's clearOn policy. Bumps the generation, so any fetch already
 * in flight discards its result rather than writing into the new state.
 */
export function clearFor(event: ClearEvent): void {
  generation += 1;

  const cleared: CacheKey[] = [];
  for (const key of CACHE_KEYS) {
    if (!defOf(key).clearOn.includes(event)) continue;
    // Nothing held means nothing to clear and nothing to announce — otherwise
    // a key the store doesn't own yet would still broadcast an empty value.
    if (!known[key] && !loaded[key]) continue;
    values[key] = CACHE_REGISTRY[key].fallback as never;
    known[key] = false;
    loaded[key] = false;
    cancelRetry(key);
    cleared.push(key);
  }

  if (cleared.length === 0) return;
  persist();
  cleared.forEach((key) => notify(key, values[key]));
  console.log(`[cache] cleared on ${event}: ${cleared.join(', ')}`);
}

/**
 * Declares whose values we are holding, and drops them if that changed.
 *
 * Called at boot and again on every sign-in: once configuration survives
 * sign-out, an account switch is the only thing left that must invalidate it,
 * and sign-out no longer wipes everything for us.
 *
 * The owner is tracked here rather than read back off disk, because memory can
 * hold values that never reached disk — persist() skips writes it can't
 * attribute — and those need invalidating just the same.
 */
export function setAccount(accountId: string | null): void {
  if (!accountId) return;
  if (ownerAccountId !== null && ownerAccountId !== accountId) {
    console.log('[cache] account changed — dropping cached values');
    clearFor('account-change');
  }
  ownerAccountId = accountId;
}

/** Reports a cache fetch failure with the same transient/fatal split as main. */
export function reportCacheFailure(key: CacheKey, err: unknown): void {
  console.warn(`[cache] ${key}: fetch failed —`, (err as Error)?.message);
  if (!isTransientNetworkError(err)) Sentry.captureException(err);
}
