import type { AxiosError } from 'axios';
import type { CoachError } from '@/types/store/coachWindowStore';
import type { Account } from '@/types/user';
import { canManageBilling } from '@/utils/canManageBilling';
import { CueStartError, isBillingRefusalError, isCuePermissionsDeniedError, isCueStartCancelledError } from '@/utils/errorReporting';

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
 * would be a second, competing explanation of the same thing. Also null for
 * cue_start_cancelled (SAYSO-383): the user stopped it themselves.
 */
export const cueStartErrorMessage = (error: unknown): string | null => {
    if (isCuePermissionsDeniedError(error) || isCueStartCancelledError(error)) return null;

    const code = error instanceof CueStartError ? error.code : undefined;
    if (!code) return CUE_GENERIC_START_MESSAGE;

    const match = MESSAGE_PATTERNS.find(([pattern]) => pattern.test(code));
    return match ? match[1] : CUE_GENERIC_START_MESSAGE;
};

/**
 * Keyed by the `code` the server puts on a 402 from `POST /cue/session/new`
 * (server middleware/hasMinutes.js, hasTeamHours.js). The server's `error`
 * string is written for logs, not for a sales rep mid-call.
 */
const BILLING_REFUSAL_MESSAGES: Readonly<Record<string, string>> = {
    NO_MINUTES_REMAINING: 'Your team is out of coaching minutes. Let your team leader know.',
    TEAM_CAP_REACHED: 'Your team has reached its coaching hour limit. Let your team leader know.',
};

const BILLING_REFUSAL_FALLBACK =
    "We can't start a session right now. Please try again later.";

interface BillingRefusalBody {
    code?: string;
    /** SUBSCRIPTION_PAUSED only: ISO date, null when the server has none. */
    resumesAt?: string | null;
}

const billingRefusalBody = (error: unknown): BillingRefusalBody | undefined => (
    (error as AxiosError<BillingRefusalBody>).response?.data
);

/** Refusals whose copy depends on whether the user can manage billing. */
const SUBSCRIPTION_REFUSALS: ReadonlySet<string | undefined> = new Set(['SUBSCRIPTION_PAUSED', 'SUBSCRIPTION_INACTIVE']);

export const isSubscriptionRefusal = (error: unknown): boolean => (
    isBillingRefusalError(error) && SUBSCRIPTION_REFUSALS.has(billingRefusalBody(error)?.code)
);

/** Billing dates are UTC calendar days, as in the billing emails and web app; local time could shift them by one. */
const formatBillingDate = (iso: string | null | undefined): string | null => {
    const date = iso ? new Date(iso) : null;
    if (!date || Number.isNaN(date.getTime())) return null;
    return date.toLocaleDateString('en-US', { month: 'short', day: 'numeric', year: 'numeric', timeZone: 'UTC' });
};

/** No account means the profile hasn't loaded: neutral copy, as either hint could be wrong. */
const subscriptionPausedError = (resumesAt: string | null | undefined, account: Account | null): CoachError => {
    const date = formatBillingDate(resumesAt);
    const base = { title: 'Subscription paused', severity: 'warning', icon: 'pause' } as const;
    const until = date ? `until ${date}` : null;
    // Without a date the title is a sentence of its own
    const undated = { ...base, title: 'Subscription paused.' };

    if (!account) return until ? { ...base, message: until } : { ...undated, message: "Cue isn't available for now." };
    if (!canManageBilling(account)) {
        const hint = 'Let your team leader know.';
        return until ? { ...base, message: `${until}. ${hint}` } : { ...undated, message: hint };
    }
    return {
        ...(until ? { ...base, message: until } : { ...undated, message: 'Resume anytime from Subscription.' }),
        action: 'open-subscription',
    };
};

/** Canceled, past due, unpaid, …: the copy fits all of them. No account, as above. */
const subscriptionInactiveError = (account: Account | null): CoachError => {
    if (account && !canManageBilling(account)) {
        return { message: "Your team's Sayso subscription isn't active. Let your team leader know.", severity: 'warning' };
    }
    return {
        message: "Your Sayso subscription isn't active.",
        severity: 'warning',
        ...(account && { action: 'open-subscription' as const }),
    };
};

/**
 * Banner for a 402 from session creation, or null for any other failure.
 * The fallback covers a code this build predates. `account` only matters for
 * the subscription refusals (see isSubscriptionRefusal).
 */
export const cueBillingRefusal = (error: unknown, account: Account | null): CoachError | null => {
    if (!isBillingRefusalError(error)) return null;
    const body = billingRefusalBody(error);
    if (body?.code === 'SUBSCRIPTION_PAUSED') return subscriptionPausedError(body.resumesAt, account);
    if (body?.code === 'SUBSCRIPTION_INACTIVE') return subscriptionInactiveError(account);
    return {
        message: (body?.code && BILLING_REFUSAL_MESSAGES[body.code]) || BILLING_REFUSAL_FALLBACK,
        severity: 'warning',
    };
};
