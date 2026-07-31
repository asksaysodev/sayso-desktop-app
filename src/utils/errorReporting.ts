import * as Sentry from "@sentry/electron/renderer";
import type { AxiosError } from 'axios';

const TRANSIENT_AXIOS_CODES = new Set(['ERR_NETWORK', 'ERR_EMPTY_RESPONSE', 'ECONNABORTED']);

export const CUE_PERMISSIONS_DENIED = 'permissions_denied';

/**
 * Carries the `error` string returned by the main-process `start-cue` handler,
 * so callers can branch on the reason instead of parsing the message.
 */
export class CueStartError extends Error {
    readonly code?: string;

    constructor(code?: string) {
        super(code ? `Failed to start cue streaming: ${code}` : 'Failed to start cue streaming');
        this.name = 'CueStartError';
        this.code = code;
    }
}

/**
 * Flipped true once this window's session has actually ended. Gates
 * isAuthTeardownError so a 401 only counts as expected teardown *after* auth is
 * gone — a 401 while still signed in is an authz regression that must reach Sentry.
 *
 * Two sources dispatch `auth:session-expired` (both wired in src/config/axios.ts):
 *   - in-window, when a forced refresh comes back `invalid_grant` (axios.ts:94).
 *     dispatchEvent is synchronous, so the flag is set before the 401 rejects and
 *     is already true by the time the caller's .catch runs.
 *   - re-dispatched from main's IPC broadcast on signed-out / session-expired
 *     (axios.ts:135/141). This one is async, so it's best-effort: an in-flight
 *     request that 401s could in principle reject before the IPC lands. IPC beats
 *     a network round-trip in practice, but it is not a guarantee.
 *
 * Never reset. That is safe only because both electron/main.ts paths that end a
 * session — the `signed-out` handler (main.ts:146) and the `session-expired`
 * handler (main.ts:203) — call tearDownSignedInWindows(), which destroys the coach
 * window and this module instance with it, so the next sign-in starts fresh at
 * false. If a coach window is ever allowed to survive logout, this needs a reset.
 */
let sessionEnded = false;
if (typeof window !== 'undefined') {
    window.addEventListener('auth:session-expired', () => { sessionEnded = true; });
}

/**
 * Renderer-side counterpart to main's `isTransientNetworkError` (electron/main.ts).
 * The two match disjoint error shapes and are deliberately not shared: main sees
 * undici/fetch and electron-updater failures (`ENOTFOUND`, `fetch failed`, …) and
 * reads `err.statusCode`, while the renderer only ever sees Chromium-adapter axios
 * errors, which report `ERR_NETWORK` and carry the status under `err.response`.
 *
 * True means the user is offline or the backend was briefly unreachable — expected
 * state, not an application error worth reporting.
 *
 * 429 is deliberately absent: unlike a 502/503/504 blip, a rate limit is the server
 * reporting something about our own traffic, so a rate-limit regression should stay
 * visible in Sentry rather than being filtered out as noise.
 */
export function isTransientApiError(error: unknown): boolean {
    const err = error as AxiosError | undefined;
    if (err?.code && TRANSIENT_AXIOS_CODES.has(err.code)) return true;
    const status = err?.response?.status;
    return status === 502 || status === 503 || status === 504;
}

/**
 * A 401 that arrived after the session ended — logout, or an expired refresh token.
 * The credentials are gone, so the request could never have succeeded; that is
 * expected state rather than a failure to report.
 *
 * The `sessionEnded` gate is load-bearing, not defensive. A 401 can also survive
 * the response interceptor while the user is still signed in: on the first 401
 * axios force-refreshes and retries once (src/config/axios.ts:77-103), and if that
 * refresh *succeeds* the retry carries a brand-new token. A 401 on that retry is
 * the most suspicious 401 the app can produce — a scope or RLS regression — and
 * must not be swallowed. Status alone cannot tell the two apart.
 */
export function isAuthTeardownError(error: unknown): boolean {
    if (!sessionEnded) return false;
    return (error as AxiosError | undefined)?.response?.status === 401;
}

/**
 * True when the start failed only because the OS permissions aren't granted.
 * Main already returns early and routes the user to the permissions UI, so this
 * is expected state — not an exception worth reporting.
 */
export const isCuePermissionsDeniedError = (error: unknown): boolean => (
    error instanceof CueStartError && error.code === CUE_PERMISSIONS_DENIED
);

/**
 * Single entry point for coach-window error reporting. Swallows the three classes
 * of failure that are expected state rather than bugs — OS permissions denied,
 * transient network loss, and post-logout auth teardown — and reports the rest.
 */
export const reportCoachError = (error: unknown): void => {
    if (
        isCuePermissionsDeniedError(error) ||
        isTransientApiError(error) ||
        isAuthTeardownError(error)
    ) {
        console.warn('[Coach] expected failure, not reported:', error);
        return;
    }
    Sentry.captureException(error);
};
