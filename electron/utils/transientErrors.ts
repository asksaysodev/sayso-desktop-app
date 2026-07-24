/**
 * Transient upstream HTTP statuses that recover on the next attempt and should
 * not be treated as real failures:
 *  - 429             rate limited
 *  - 502 / 503 / 504 gateway / upstream hiccups
 *  - 618             GitHub release-assets CDN "jwt:expired": the signed asset
 *                    URL's short-lived token lapses before the CDN serves it, so
 *                    a fresh check simply gets a new URL. Non-standard
 *                    Fastly/Varnish code; response body reads "618 jwt:expired".
 */
export function isTransientUpstreamStatus(status: number | undefined): boolean {
  return status === 429 || status === 502 || status === 503 || status === 504 || status === 618;
}

function getStatus(err: any): number | undefined {
  return typeof err?.statusCode === 'number' ? err.statusCode : undefined;
}

// Errors that mean the user simply has no connectivity — don't alarm them, and
// don't report them to Sentry.
const OFFLINE_PATTERNS = ['ERR_INTERNET_DISCONNECTED', 'ENOTFOUND', 'ENETUNREACH', 'EAI_AGAIN'];

export type UpdaterErrorKind = 'offline' | 'transient' | 'fatal';

/**
 * Classify an auto-updater error for both Sentry-suppression and the
 * user-facing message:
 *  - 'offline'   → user has no connection; suppress from Sentry, show a network hint
 *  - 'transient' → upstream/CDN hiccup that self-heals; suppress, show "try again shortly"
 *  - 'fatal'     → a real error; report to Sentry and surface the raw message
 *
 * The `jwt:expired` message guard is a belt-and-suspenders fallback for the
 * 618 case in case `statusCode` isn't populated on some code path.
 */
export function classifyUpdaterError(err: any): UpdaterErrorKind {
  const message = `${err?.message ?? ''}`;
  if (OFFLINE_PATTERNS.some(p => message.includes(p))) return 'offline';
  if (isTransientUpstreamStatus(getStatus(err)) || message.includes('jwt:expired')) return 'transient';
  return 'fatal';
}

/** Updater errors we should not report to Sentry (offline or transient upstream). */
export function isSuppressibleUpdaterError(err: any): boolean {
  return classifyUpdaterError(err) !== 'fatal';
}

/**
 * The user-facing text for an updater failure. Lives here, next to the
 * classifier, because electron-updater reports the *same* failure twice — it
 * emits 'error' and rejects the promise — so several call sites race to write
 * `updateState.errorMessage`. They must all produce the same string, or a
 * transient hiccup can end up showing the raw signed-URL/HTML error body
 * depending on which handler happens to run last.
 *
 * Only 'fatal' errors surface their raw message; noise gets a friendly hint.
 */
export function updaterErrorMessage(err: any): string {
  switch (classifyUpdaterError(err)) {
    case 'offline':
      return 'No internet connection. Please check your network and try again.';
    case 'transient':
      return 'Couldn’t reach the update server. Please try again shortly.';
    default:
      return `${err?.message ?? ''}`;
  }
}

/**
 * Whether a network op should be retried after the network settles on
 * wake/resume. When the system wakes, the network/DNS stack may not be ready
 * for a few seconds, so DNS/connection errors here are transient and a retry
 * fixes them. Distinct from the offline set above: this is "not ready yet",
 * not "no internet".
 */
export function isTransientNetworkError(err: any): boolean {
  if (isTransientUpstreamStatus(getStatus(err))) return true;
  const msg = `${err?.message ?? ''} ${err?.code ?? ''}`;
  return /ERR_NAME_NOT_RESOLVED|ENOTFOUND|EAI_AGAIN|fetch failed|ECONNREFUSED|ETIMEDOUT/i.test(msg);
}
