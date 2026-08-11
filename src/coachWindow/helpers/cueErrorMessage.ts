import { CueStartError, isCuePermissionsDeniedError } from '@/utils/errorReporting';

/**
 * User-facing copy for a failed cue start/stop.
 *
 * Kept out of errorReporting.ts on purpose: that module decides what reaches
 * Sentry, this one decides what reaches the user. The two answers differ —
 * permissions_denied is silent in both, but a transient network failure is
 * unreportable *and* worth showing.
 */

export const CUE_CONNECTIVITY_MESSAGE =
    "Couldn't connect to Sayso. Check your network and try again.";

const CUE_GENERIC_START_MESSAGE = "We couldn't start Cue. Please try again.";

export const CUE_STOP_MESSAGE =
    "Your session stopped, but we couldn't save it cleanly.";

/**
 * `CueStartError.code` is not yet a stable code — main returns `error.message`
 * verbatim (electron/audio/audioManager.ts:473), so everything except
 * `permissions_denied` (audioManager.ts:323) arrives as free-form English or a
 * raw Node error string. Matching on substrings is the cost of that; SAYSO-347
 * is where real codes would come from, and this table takes them unchanged the
 * day they exist because an exact code still matches its own pattern.
 */
const MESSAGE_PATTERNS: ReadonlyArray<[RegExp, string]> = [
    [
        /failed to start microphone capture|startmicrophonecapture/i,
        "Sayso couldn't access your microphone. Another app may be using it.",
    ],
    [
        /native audio module not loaded|native module not found|startprospectstreaming method not available|native module may need to be rebuilt/i,
        "Sayso's audio engine isn't ready. Restart the app and try again.",
    ],
    [
        /enotfound|eai_again|econnrefused|etimedout|econnreset|connection timeout|socket hang up|getaddrinfo|network|fetch failed/i,
        CUE_CONNECTIVITY_MESSAGE,
    ],
];

/**
 * Maps a start failure to banner copy, or null when the user should see nothing.
 *
 * Null means permissions_denied: main has already surfaced the splash window and
 * PostAuthRedirect routes to /permissions (audioManager.ts:320-323), so a banner
 * would be a second, competing explanation of the same thing.
 */
export const cueStartErrorMessage = (error: unknown): string | null => {
    if (isCuePermissionsDeniedError(error)) return null;

    const code = error instanceof CueStartError ? error.code : undefined;
    if (!code) return CUE_GENERIC_START_MESSAGE;

    const match = MESSAGE_PATTERNS.find(([pattern]) => pattern.test(code));
    return match ? match[1] : CUE_GENERIC_START_MESSAGE;
};
