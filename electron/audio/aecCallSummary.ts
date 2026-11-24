// Echo cancellation over one Cue call, built from the native `aec_*` lifecycle events.
// Pure, no electron or Sentry, so test/aecCallSummary.test.mts can replay a call's events.
//
// Two jobs:
// - Sentry dedupe: each echo-cancellation failure type is reported at most once per call.
// - One summary per call (enabled / active, delay estimate, fallbacks and their reasons), for the
//   app log and the call's Sentry context. The 60-second `aec_stats` lines stay in the log only.

/** Native counters from `aec_stats` / `aec_stopped` lines, e.g. `aec_frames=1234 delay_ms=8`. */
function parseFields(event: string): Record<string, string> {
  const fields: Record<string, string> = {};
  for (const token of event.split(' ').slice(1)) {
    const eq = token.indexOf('=');
    if (eq > 0) fields[token.slice(0, eq)] = token.slice(eq + 1);
  }
  return fields;
}

function eventName(event: string): string {
  const space = event.indexOf(' ');
  return space < 0 ? event : event.slice(0, space);
}

/** `aec_failed reason=process_error code=-11` -> `aec_failed:process_error`. */
function eventKey(event: string): string {
  const reason = parseFields(event).reason;
  return reason ? `${eventName(event)}:${reason}` : eventName(event);
}

function numberField(fields: Record<string, string>, key: string): number | null {
  if (!(key in fields)) return null;
  const value = Number(fields[key]);
  return Number.isFinite(value) ? value : null;
}

/** Events where the mic went out without echo cancellation, for a reason worth counting. */
const FALLBACK_EVENTS = new Set([
  'aec_failed',              // engine or worker failed: mic passed through
  'aec_engine_unavailable',  // module built without the WebRTC library
  'aec_reference_missing',   // no system audio to cancel against (also normal before the prospect speaks)
  'aec_backlog_dropped',     // fell behind: frames skipped cancellation
  'aec_queue_full',          // worker stalled: capture thread delivered directly
]);

/** Fallbacks that mean something went wrong, rather than "nothing to cancel yet". */
const PROBLEM_EVENTS = new Set(['aec_failed', 'aec_engine_unavailable', 'aec_backlog_dropped', 'aec_queue_full']);

export interface AecCallSummary {
  /** Switched on at any point in the call. */
  enabled: boolean;
  /** Delivered cleaned audio at any point in the call. */
  active: boolean;
  /** Share of mic frames that were echo-cancelled, 0–100; null before any stats arrived. */
  activePercent: number | null;
  /** Engine's last echo delay estimate in ms; null = unknown. */
  delayMs: number | null;
  /** Average mic latency the canceller added, ms (last mic session in the call). */
  addedLatencyMs: number | null;
  /** Echo removed over the last mic session, dB. */
  reductionDb: number | null;
  engine: string | null;
  /** `event:reason` -> how many times, e.g. `aec_reference_missing:system_audio_not_started: 1`. */
  fallbacks: Record<string, number>;
  lastError: string | null;
  /** A failure or dropped audio happened (not just a missing reference). */
  hadProblem: boolean;
}

export function createAecCallTracker() {
  let enabled = false;
  let sawAnyEvent = false;
  let engine: string | null = null;
  let captureFrames = 0;
  let aecFrames = 0;
  let delayMs: number | null = null;
  let addedLatencyMs: number | null = null;
  let reductionDb: number | null = null;
  let lastError: string | null = null;
  let hadProblem = false;
  let fallbacks: Record<string, number> = {};
  let reported = new Set<string>();

  return {
    /** Call at the start of each call. `enabledNow` = the switch's state as the call starts. */
    reset(enabledNow: boolean): void {
      enabled = enabledNow;
      sawAnyEvent = false;
      engine = null;
      captureFrames = 0;
      aecFrames = 0;
      delayMs = null;
      addedLatencyMs = null;
      reductionDb = null;
      lastError = null;
      hadProblem = false;
      fallbacks = {};
      reported = new Set<string>();
    },

    /** Feed every lifecycle event; non-`aec_` events are ignored. */
    onEvent(event: string): void {
      if (!event.startsWith('aec_')) return;
      sawAnyEvent = true;
      const name = eventName(event);
      const fields = parseFields(event);

      if (event === 'aec_switched on') enabled = true;
      if (name === 'aec_engine_ready' && fields.engine) engine = fields.engine;

      if (FALLBACK_EVENTS.has(name)) {
        const key = eventKey(event);
        fallbacks[key] = (fallbacks[key] ?? 0) + 1;
        if (PROBLEM_EVENTS.has(name)) hadProblem = true;
        if (name === 'aec_failed' || name === 'aec_engine_unavailable') lastError = event;
      }

      if (name === 'aec_stats' || name === 'aec_stopped') {
        const delay = numberField(fields, 'delay_ms');
        if (delay !== null && delay >= 0) delayMs = delay;
        if (fields.enabled === '1') enabled = true;
      }
      if (name === 'aec_stopped') {
        // Session totals for one mic session; a call can have several (mic restarts).
        captureFrames += numberField(fields, 'capture_frames') ?? 0;
        aecFrames += numberField(fields, 'aec_frames') ?? 0;
        addedLatencyMs = numberField(fields, 'latency_ms_avg') ?? addedLatencyMs;
        reductionDb = numberField(fields, 'reduction_db') ?? reductionDb;
      }
    },

    /**
     * True the first time this call sees this failure type (event + reason), false after that, so a
     * repeating failure reaches Sentry once per call.
     */
    shouldReport(event: string): boolean {
      const key = eventKey(event);
      if (reported.has(key)) return false;
      reported.add(key);
      return true;
    },

    /** Null when echo cancellation was never switched on and emitted nothing in this call. */
    summary(): AecCallSummary | null {
      if (!enabled && !sawAnyEvent) return null;
      return {
        enabled,
        active: aecFrames > 0,
        activePercent: captureFrames > 0 ? Math.round((aecFrames / captureFrames) * 1000) / 10 : null,
        delayMs,
        addedLatencyMs,
        reductionDb,
        engine,
        fallbacks: { ...fallbacks },
        lastError,
        hadProblem,
      };
    },
  };
}

/** Single line for the packaged log file. */
export function formatAecCallSummary(s: AecCallSummary): string {
  const fallbacks = Object.entries(s.fallbacks).map(([key, count]) => `${key}x${count}`).join(',') || 'none';
  return (
    `enabled=${s.enabled} active=${s.active} active_pct=${s.activePercent ?? '?'} delay_ms=${s.delayMs ?? '?'}` +
    ` added_latency_ms=${s.addedLatencyMs ?? '?'} reduction_db=${s.reductionDb ?? '?'} engine=${s.engine ?? 'none'}` +
    ` fallbacks=${fallbacks} last_error=${s.lastError ? `"${s.lastError}"` : 'none'}`
  );
}

/** Sentry tags for the call: short, searchable values. */
export function aecCallSummaryTags(s: AecCallSummary): Record<string, string> {
  return {
    aec_enabled: String(s.enabled),
    aec_active: String(s.active),
    aec_fallback: s.hadProblem ? 'problem' : Object.keys(s.fallbacks).length > 0 ? 'no_reference' : 'none',
  };
}
