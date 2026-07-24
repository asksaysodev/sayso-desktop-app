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

// One shared vocabulary of environmental network failures. Both questions this
// module answers — "should the user see a friendly hint instead of the raw
// error?" and "should this be retried once the network settles?" — read from
// these lists, so a code can never be retryable-but-reportable (see the
// invariant on isTransientNetworkError below).
//
// Split in two only to pick the wording; both halves are equally suppressed.

// No usable connection at all → "check your network".
const OFFLINE_PATTERNS = [
  'ERR_INTERNET_DISCONNECTED',
  'ERR_NETWORK_CHANGED', // Wi-Fi hop / VPN toggle mid-request
  'ERR_NETWORK_IO_SUSPENDED', // machine suspended mid-request
  'ERR_ADDRESS_UNREACHABLE',
  'ENOTFOUND',
  'ENETUNREACH',
  'EAI_AGAIN',
];

// There is a network, but this attempt didn't get a fair shot — DNS not warmed
// up yet after a wake, connection dropped, or it timed out → "try again shortly".
const FLAKY_NETWORK_PATTERNS = [
  'ERR_NAME_NOT_RESOLVED',
  'ERR_CONNECTION_RESET',
  'ERR_TIMED_OUT',
  'ERR_CONNECTION_TIMED_OUT',
  'ECONNREFUSED',
  'ETIMEDOUT',
  'fetch failed',
];

/** Scan both `message` and `code` — Node surfaces the code in either place. */
function matchesAny(err: any, patterns: string[]): boolean {
  const haystack = `${err?.message ?? ''} ${err?.code ?? ''}`.toLowerCase();
  return patterns.some(p => haystack.includes(p.toLowerCase()));
}

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
  if (matchesAny(err, OFFLINE_PATTERNS)) return 'offline';
  if (matchesAny(err, FLAKY_NETWORK_PATTERNS)) return 'transient';
  if (isTransientUpstreamStatus(getStatus(err)) || matchesAny(err, ['jwt:expired'])) return 'transient';
  return 'fatal';
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
 * Whether a network op should be retried (and kept out of Sentry) — the machine
 * was offline, changed networks, or woke before its DNS/network stack was ready.
 *
 * INVARIANT: every error this returns true for must classify as 'offline' or
 * 'transient', never 'fatal'. Retries re-run the operation, and each failed
 * attempt re-emits electron-updater's 'error' event — so an error that is
 * retryable *and* reportable gets reported once per attempt. Reading both
 * predicates off the same lists is what keeps that from happening.
 */
export function isTransientNetworkError(err: any): boolean {
  if (isTransientUpstreamStatus(getStatus(err))) return true;
  return matchesAny(err, OFFLINE_PATTERNS) || matchesAny(err, FLAKY_NETWORK_PATTERNS);
}
