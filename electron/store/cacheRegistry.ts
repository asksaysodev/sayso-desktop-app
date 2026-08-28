import axios from 'axios';

// The declaration of everything we cache in the main process. One entry per
// value, and this file is meant to be readable as documentation: what we keep,
// what shape it has to be, whether it survives on disk, when it's thrown away,
// and who gets told when it changes.
//
// Before this existed, applyFontSize / applyPlaybooksCache /
// applyOpenLastUsedCache were the same function three times, and had already
// drifted — playbooks refuses to overwrite good data with an error on the main
// side but persists an error payload from the renderer side; open-last-used
// rejects a malformed 200; font size had no rule at all. Same question, three
// answers. Declaring it once is the point.

export type CacheSource = 'server' | 'user' | 'disk';
export type ClearEvent = 'sign-out' | 'session-expired' | 'account-change';
export type NotifyTarget = 'all-windows' | 'coach-and-playbook' | 'playbook';

export type FontSize = 's' | 'm' | 'l';
export type PlaybooksCachePayload = { playbooks: unknown[] | null; error: string | null };

export interface CacheValues {
  fontSize: FontSize;
  enabledFeatures: string[];
  playbooks: PlaybooksCachePayload;
  openLastUsed: boolean | null;
}

export type CacheKey = keyof CacheValues;

export interface CacheEntryDef<T> {
  // Used until something real arrives. Every fallback here is also a
  // legitimate server value, which is exactly why the manager tracks
  // "do we know this?" separately from the value itself.
  fallback: T;

  // The single home for this value's shape check. Returns null to reject —
  // a rejected value is never assigned, persisted or broadcast, and a
  // rejected *fetch* result is raised so the retry ladder sees a failure.
  validate: (raw: unknown) => T | null;

  persist: boolean;

  // Narrower than validate: a value can be legitimate in memory but not worth
  // keeping on disk. Only playbooks needs this today — see its entry.
  persistable?: (value: T) => boolean;

  clearOn: ClearEvent[];

  // Whether a failed fetch arms the retry ladder. Only the two values that
  // already had one keep it; turning it on for the others is a one-word
  // change, deliberately not made in the same commit as the migration.
  retry: boolean;

  notify: { channel: string; target: NotifyTarget; payload: (value: T) => unknown } | null;

  // Extracts this value from its endpoint. Returns the raw candidate — the
  // manager runs it through validate(), so shape-checking stays in one place.
  refresh: ((baseUrl: string, token: string) => Promise<unknown>) | null;
}

// Unchanged from the call sites these replace: 5s for the boot-critical
// values, 8s for the two that only gate the playbook window.
const FAST_TIMEOUT_MS = 5000;
const SLOW_TIMEOUT_MS = 8000;

const authHeaders = (token: string) => ({ Authorization: `Bearer ${token}` });

export const CACHE_REGISTRY: { [K in CacheKey]: CacheEntryDef<CacheValues[K]> } = {
  fontSize: {
    fallback: 's',
    validate: (raw) => (raw === 's' || raw === 'm' || raw === 'l' ? raw : null),
    persist: true,
    clearOn: ['account-change'],
    retry: true,
    notify: { channel: 'font-size-changed', target: 'coach-and-playbook', payload: (v) => v },
    refresh: async (baseUrl, token) => {
      const res = await axios.get(`${baseUrl}/sales-coach/settings`, {
        headers: authHeaders(token),
        timeout: FAST_TIMEOUT_MS,
      });
      // A null font_size is legitimate, not a malformed payload — migration
      // 003 only defaulted new coach_settings rows. It means "never chosen".
      return res.data?.coachSettings?.font_size ?? 's';
    },
  },

  enabledFeatures: {
    fallback: [],
    validate: (raw) =>
      Array.isArray(raw) && raw.every((k) => typeof k === 'string' && k.length > 0) ? (raw as string[]) : null,
    persist: true,
    clearOn: ['account-change'],
    retry: true,
    notify: {
      channel: 'enabled-features-changed',
      target: 'all-windows',
      payload: (v) => ({ enabledFeatures: v }),
    },
    refresh: async (baseUrl, token) => {
      const res = await axios.get(`${baseUrl}/features/company`, {
        headers: authHeaders(token),
        timeout: FAST_TIMEOUT_MS,
      });
      const features = res.data?.features;
      // Anything other than an array is a malformed payload. Returning it
      // as-is lets validate() reject it, which the manager raises.
      if (!Array.isArray(features)) return features;
      return features.filter((f: { enabled?: boolean }) => f?.enabled).map((f: { key?: string }) => f?.key);
    },
  },

  playbooks: {
    fallback: { playbooks: null, error: null },
    validate: (raw) => {
      if (!raw || typeof raw !== 'object') return null;
      const p = raw as Partial<PlaybooksCachePayload>;
      const listOk = p.playbooks === null || p.playbooks === undefined || Array.isArray(p.playbooks);
      const errorOk = p.error === null || p.error === undefined || typeof p.error === 'string';
      if (!listOk || !errorOk) return null;
      return { playbooks: p.playbooks ?? null, error: p.error ?? null };
    },
    persist: true,
    // An error payload is a legitimate in-memory state — the playbook window
    // renders it — but it must never overwrite a good cached list on disk.
    // usePlaybookPrefetch sends { playbooks: null, error } on a failed fetch
    // and applyPlaybooksCache persisted it unconditionally, so a single
    // offline open could wipe a good warm start.
    persistable: (v) => v.error === null && v.playbooks !== null,
    // Call content, unlike the three configuration values: it should not sit
    // on a machine whose user has signed out.
    clearOn: ['sign-out', 'session-expired', 'account-change'],
    retry: false,
    notify: { channel: 'playbooks-updated', target: 'playbook', payload: (v) => v },
    refresh: async (baseUrl, token) => {
      const res = await axios.get(`${baseUrl}/playbooks`, {
        headers: authHeaders(token),
        timeout: SLOW_TIMEOUT_MS,
      });
      return { playbooks: res.data?.playbooks ?? null, error: null };
    },
  },

  openLastUsed: {
    // null means "not known yet"; the IPC getter keeps its `?? true` default,
    // so an unknown still reads as true to the playbook window.
    fallback: null,
    validate: (raw) => (typeof raw === 'boolean' ? raw : null),
    persist: true,
    clearOn: ['account-change'],
    retry: false,
    notify: { channel: 'open-last-used-updated', target: 'playbook', payload: (v) => v },
    refresh: async (baseUrl, token) => {
      const res = await axios.get(`${baseUrl}/sales-coach/settings`, {
        headers: authHeaders(token),
        timeout: SLOW_TIMEOUT_MS,
      });
      return res.data?.coachSettings?.open_last_used;
    },
  },
};

export const CACHE_KEYS = Object.keys(CACHE_REGISTRY) as CacheKey[];
