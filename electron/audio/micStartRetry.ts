// SAYSO-383: the one retry path for every JS-initiated mic start (user click, delivery check, stall
// watchdog). Pure — no electron, no Sentry — so scripts/mic-start-retry-test.js can drive it with a
// fake clock. Callers own reporting.

/** Waits before retries 1, 2 and 3. Two fresh engines failed inside 2.7s on Aaron's 08-24 start and
 *  a third worked 16s later: elapsed time between attempts decides it, not the length of one wait. */
export const MIC_START_BACKOFF_MS: readonly number[] = [1000, 2000, 4000];

/** Cold start: the first attempt plus one per backoff step. */
export const MIC_START_MAX_ATTEMPTS = MIC_START_BACKOFF_MS.length + 1;

/** Minimum time from the end of one native StartMicrophoneCapture to the next, from any caller.
 *  Each failed native start already builds and releases two engines, and SAYSO-APP-82 crashed
 *  seconds after rapid rebuilds — never pack them tighter than this. */
export const MIC_START_MIN_GAP_MS = 1000;

/** Retrying can't fix these: a second start racing a live one, or a wiring bug. */
const NON_RETRYABLE_REASONS: ReadonlySet<string> = new Set(['already_active', 'callback_empty']);

/** Only typed native failures (recorder sets `micStartFailed`) are retried, never a thrown bug. */
export function isRetryableMicStartError(error: any): boolean {
  if (!error?.micStartFailed) return false;
  return !NON_RETRYABLE_REASONS.has(error.micStartReason);
}

export interface MicStartClock {
  now(): number;
  sleep(ms: number): Promise<void>;
}

const realClock: MicStartClock = {
  now: () => Date.now(),
  sleep: (ms) => new Promise<void>((resolve) => setTimeout(resolve, ms)),
};

export interface MicStartAttemptsResult {
  ok: boolean;
  /** Native starts actually made — what the watchdog charges against its budget. */
  attempts: number;
  /** The session went away (Stop, reset, new session) between attempts; nothing was started for it. */
  abandoned: boolean;
  /** Last failure, when !ok && !abandoned. */
  error?: unknown;
}

export interface MicStartAttemptsOptions {
  start: () => Promise<unknown>;
  isCurrent: () => boolean;
  maxAttempts: number;
  onAttemptFailed?: (attempt: number, error: unknown, retryInMs: number | null) => void;
}

/** One instance per process in production, so the gap holds across all callers. */
export function createMicStarter(clock: MicStartClock = realClock) {
  let lastStartEndedAtMs = 0;

  async function waitForGap(): Promise<void> {
    if (lastStartEndedAtMs === 0) return;
    const waitMs = lastStartEndedAtMs + MIC_START_MIN_GAP_MS - clock.now();
    if (waitMs > 0) await clock.sleep(waitMs);
  }

  async function startWithBackoff(opts: MicStartAttemptsOptions): Promise<MicStartAttemptsResult> {
    let attempts = 0;
    let lastError: unknown;

    while (attempts < opts.maxAttempts) {
      if (attempts > 0) {
        await clock.sleep(MIC_START_BACKOFF_MS[Math.min(attempts - 1, MIC_START_BACKOFF_MS.length - 1)]);
      }
      await waitForGap();
      if (!opts.isCurrent()) return { ok: false, attempts, abandoned: true, error: lastError };

      attempts += 1;
      try {
        await opts.start();
        return { ok: true, attempts, abandoned: false };
      } catch (error) {
        lastError = error;
        const retryable = isRetryableMicStartError(error) && attempts < opts.maxAttempts;
        opts.onAttemptFailed?.(
          attempts,
          error,
          retryable ? MIC_START_BACKOFF_MS[Math.min(attempts - 1, MIC_START_BACKOFF_MS.length - 1)] : null,
        );
        if (!retryable) break;
      } finally {
        lastStartEndedAtMs = clock.now();
      }
    }

    return { ok: false, attempts, abandoned: false, error: lastError };
  }

  return { startWithBackoff };
}
