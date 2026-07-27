import type { AxiosError } from 'axios';

const TRANSIENT_AXIOS_CODES = new Set(['ERR_NETWORK', 'ERR_EMPTY_RESPONSE', 'ECONNABORTED']);

/**
 * Renderer-side counterpart to main's `isTransientNetworkError` (electron/main.ts).
 * The two match disjoint error shapes and are deliberately not shared: main sees
 * undici/fetch and electron-updater failures (`ENOTFOUND`, `fetch failed`, …) and
 * reads `err.statusCode`, while the renderer only ever sees Chromium-adapter axios
 * errors, which report `ERR_NETWORK` and carry the status under `err.response`.
 *
 * True means the user is offline or the backend was briefly unreachable — expected
 * state, not an application error worth reporting.
 */
export function isTransientApiError(error: unknown): boolean {
    const err = error as AxiosError | undefined;
    if (err?.code && TRANSIENT_AXIOS_CODES.has(err.code)) return true;
    const status = err?.response?.status;
    return status === 502 || status === 503 || status === 504 || status === 429;
}

/**
 * A 401 that survived the response interceptor. By the time an error reaches a
 * caller, axios has already force-refreshed the token and retried once
 * (src/config/axios.ts), so a remaining 401 means the credentials are genuinely
 * gone — logout, or an expired refresh token. The request could never have
 * succeeded, so it is expected state rather than a failure to report.
 */
export function isAuthTeardownError(error: unknown): boolean {
    return (error as AxiosError | undefined)?.response?.status === 401;
}
