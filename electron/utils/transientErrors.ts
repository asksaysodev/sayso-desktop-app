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
