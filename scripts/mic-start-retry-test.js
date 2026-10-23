// SAYSO-383: checks the shared mic-start backoff (electron/audio/micStartRetry.ts) against a fake
// clock — retry schedule, retryable reasons, abandonment, and the §3 guarantee that no two native
// starts land closer than MIC_START_MIN_GAP_MS apart, from any caller.
//
//   npm run build:electron && node --test scripts/mic-start-retry-test.js

const test = require('node:test');
const assert = require('node:assert/strict');
const {
  createMicStarter,
  MIC_START_BACKOFF_MS,
  MIC_START_MAX_ATTEMPTS,
  MIC_START_MIN_GAP_MS,
} = require('../electron/audio/micStartRetry');

/** Time only moves when someone sleeps or a fake native start "runs". */
function fakeClock() {
  let now = 1_000_000;
  const sleeps = [];
  return {
    sleeps,
    now: () => now,
    sleep: async (ms) => { sleeps.push(ms); now += ms; },
    advance: (ms) => { now += ms; },
  };
}

function micError(reason) {
  const error = new Error(`mic_start_failed reason=${reason}`);
  error.micStartReason = reason;
  error.micStartFailed = true;
  return error;
}

/** A native start that fails with `outcomes[i]` (an Error) or succeeds (null), taking `durationMs`. */
function fakeNative(clock, outcomes, durationMs = 2700) {
  const calls = [];
  let i = 0;
  return {
    calls,
    start: async () => {
      const begin = clock.now();
      clock.advance(durationMs);
      calls.push({ begin, end: clock.now() });
      const outcome = outcomes[Math.min(i++, outcomes.length - 1)];
      if (outcome) throw outcome;
    },
  };
}

function assertGaps(calls) {
  for (let i = 1; i < calls.length; i++) {
    const gap = calls[i].begin - calls[i - 1].end;
    assert.ok(gap >= MIC_START_MIN_GAP_MS, `native starts ${i - 1}→${i} only ${gap}ms apart`);
  }
}

test('happy path: one attempt, no added delay', async () => {
  const clock = fakeClock();
  const native = fakeNative(clock, [null]);
  const result = await createMicStarter(clock).startWithBackoff({ start: native.start, isCurrent: () => true, maxAttempts: MIC_START_MAX_ATTEMPTS });
  assert.deepEqual(result, { ok: true, attempts: 1, abandoned: false });
  assert.deepEqual(clock.sleeps, []);
});

test('no_tap_buffers twice, then success on attempt 3 after 1s and 2s', async () => {
  const clock = fakeClock();
  const native = fakeNative(clock, [micError('no_tap_buffers'), micError('no_tap_buffers'), null]);
  const result = await createMicStarter(clock).startWithBackoff({ start: native.start, isCurrent: () => true, maxAttempts: MIC_START_MAX_ATTEMPTS });
  assert.equal(result.ok, true);
  assert.equal(result.attempts, 3);
  assert.deepEqual(clock.sleeps, [1000, 2000]);
  assertGaps(native.calls);
});

test('all attempts fail: 4 native starts, 1s/2s/4s backoff (7s), last error returned', async () => {
  const clock = fakeClock();
  const last = micError('engine_start_failed');
  const native = fakeNative(clock, [micError('no_tap_buffers'), micError('no_tap_buffers'), micError('no_tap_buffers'), last]);
  const failures = [];
  const result = await createMicStarter(clock).startWithBackoff({
    start: native.start,
    isCurrent: () => true,
    maxAttempts: MIC_START_MAX_ATTEMPTS,
    onAttemptFailed: (attempt, _e, retryInMs) => failures.push([attempt, retryInMs]),
  });
  assert.equal(result.ok, false);
  assert.equal(result.abandoned, false);
  assert.equal(result.attempts, 4);
  assert.equal(result.error, last);
  assert.deepEqual(clock.sleeps, MIC_START_BACKOFF_MS);
  assert.deepEqual(failures, [[1, 1000], [2, 2000], [3, 4000], [4, null]]);
  assertGaps(native.calls);
});

for (const reason of ['already_active', 'callback_empty']) {
  test(`${reason} is not retried`, async () => {
    const clock = fakeClock();
    const native = fakeNative(clock, [micError(reason), null]);
    const result = await createMicStarter(clock).startWithBackoff({ start: native.start, isCurrent: () => true, maxAttempts: MIC_START_MAX_ATTEMPTS });
    assert.equal(result.ok, false);
    assert.equal(result.attempts, 1);
    assert.deepEqual(clock.sleeps, []);
  });
}

test('a thrown bug (not a native refusal) is not retried', async () => {
  const clock = fakeClock();
  const native = fakeNative(clock, [new Error('[RECORDER] streamingCallback is required'), null]);
  const result = await createMicStarter(clock).startWithBackoff({ start: native.start, isCurrent: () => true, maxAttempts: MIC_START_MAX_ATTEMPTS });
  assert.equal(result.attempts, 1);
  assert.equal(result.ok, false);
});

test('session gone during the backoff: abandoned, no further native start', async () => {
  const clock = fakeClock();
  const native = fakeNative(clock, [micError('no_tap_buffers'), null]);
  let current = true;
  const result = await createMicStarter(clock).startWithBackoff({
    start: native.start,
    isCurrent: () => current,
    maxAttempts: MIC_START_MAX_ATTEMPTS,
    onAttemptFailed: () => { current = false; }, // user hit Stop while we wait
  });
  assert.equal(result.abandoned, true);
  assert.equal(result.ok, false);
  assert.equal(native.calls.length, 1);
});

test('watchdog budget: maxAttempts caps native starts', async () => {
  const clock = fakeClock();
  const native = fakeNative(clock, [micError('no_tap_buffers')]);
  const result = await createMicStarter(clock).startWithBackoff({ start: native.start, isCurrent: () => true, maxAttempts: 2 });
  assert.equal(result.attempts, 2);
  assert.equal(native.calls.length, 2);
});

test('§3: back-to-back callers on one starter still get ≥1s between native starts', async () => {
  const clock = fakeClock();
  const starter = createMicStarter(clock);
  // user start, then the delivery check's immediate restart, then a watchdog restart — all instant
  // and all on the same starter, like production.
  const native = fakeNative(clock, [null], 50);
  for (let i = 0; i < 3; i++) {
    const result = await starter.startWithBackoff({ start: native.start, isCurrent: () => true, maxAttempts: 1 });
    assert.equal(result.ok, true);
  }
  assert.equal(native.calls.length, 3);
  assertGaps(native.calls);
});

test('§3: gap holds when a fast failure is followed by the backoff of a separate caller', async () => {
  const clock = fakeClock();
  const starter = createMicStarter(clock);
  const native = fakeNative(clock, [micError('no_tap_buffers'), micError('no_tap_buffers'), null], 0);
  await starter.startWithBackoff({ start: native.start, isCurrent: () => true, maxAttempts: 1 });
  await starter.startWithBackoff({ start: native.start, isCurrent: () => true, maxAttempts: MIC_START_MAX_ATTEMPTS });
  assertGaps(native.calls);
});
