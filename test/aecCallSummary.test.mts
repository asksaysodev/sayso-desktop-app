// Run: npm test
import { test } from 'node:test';
import assert from 'node:assert/strict';
import { aecCallSummaryTags, createAecCallTracker, formatAecCallSummary } from '../electron/audio/aecCallSummary.ts';

const STOPPED =
  'aec_stopped mode=aec capture_frames=6000 aec_frames=5400 passthrough_frames=600 ref_missing_ms=6000 ' +
  'engine_errors=0 resets=1 mic_resyncs=0 system_resyncs=0 mic_dropped=0 system_dropped=0 late_frames_dropped=0 ' +
  'delay_ms=12 erle_db=18.2 mic_dbfs=-40.0 clean_dbfs=-58.0 ref_dbfs=-30.0 reduction_db=18.0 ' +
  'latency_ms_avg=21.5 latency_ms_max=30.1 ref_late_frames=3 process_us_avg=90 process_us_max=400 ' +
  'enabled=1 delivering_clean=0 route_fallbacks=0 uncovered_ms=0';

test('a call with echo cancellation off and no events has no summary', () => {
  const call = createAecCallTracker();
  call.reset(false);
  call.onEvent('mic_route_recovery_succeeded');
  assert.equal(call.summary(), null);
});

test('a normal call: active, delay, reduction, only the start-of-call missing reference', () => {
  const call = createAecCallTracker();
  call.reset(true);
  call.onEvent('aec_started rate=48000 frame_ms=10');
  call.onEvent('aec_reference_missing reason=system_audio_not_started');
  call.onEvent('aec_engine_ready engine=webrtc_aec3 reason=system_stream_changed');
  call.onEvent('aec_reference_present');
  call.onEvent(STOPPED);
  const s = call.summary();
  assert.ok(s);
  assert.equal(s.enabled, true);
  assert.equal(s.active, true);
  assert.equal(s.activePercent, 90);
  assert.equal(s.delayMs, 12);
  assert.equal(s.reductionDb, 18);
  assert.equal(s.addedLatencyMs, 21.5);
  assert.equal(s.engine, 'webrtc_aec3');
  assert.deepEqual(s.fallbacks, { 'aec_reference_missing:system_audio_not_started': 1 });
  assert.equal(s.hadProblem, false);
  assert.equal(s.lastError, null);
  assert.deepEqual(aecCallSummaryTags(s), { aec_enabled: 'true', aec_active: 'true', aec_fallback: 'no_reference' });
});

test('a failure type reaches Sentry once per call, and again in the next call', () => {
  const call = createAecCallTracker();
  call.reset(true);
  const failed = 'aec_failed reason=process_error code=-11';
  assert.equal(call.shouldReport(failed), true);
  assert.equal(call.shouldReport('aec_failed reason=process_error code=-7'), false, 'same type, other code');
  assert.equal(call.shouldReport('aec_failed reason=engine_create_failed'), true, 'another type');
  call.reset(true);
  assert.equal(call.shouldReport(failed), true);
});

test('failures and dropped audio count as problems, with the last error kept', () => {
  const call = createAecCallTracker();
  call.reset(true);
  call.onEvent('aec_failed reason=process_error code=-11');
  call.onEvent('aec_failed reason=process_error code=-11');
  call.onEvent('aec_backlog_dropped frames=480 total=480');
  const s = call.summary();
  assert.ok(s);
  assert.equal(s.hadProblem, true);
  assert.equal(s.lastError, 'aec_failed reason=process_error code=-11');
  assert.deepEqual(s.fallbacks, { 'aec_failed:process_error': 2, aec_backlog_dropped: 1 });
  assert.equal(aecCallSummaryTags(s).aec_fallback, 'problem');
  assert.match(formatAecCallSummary(s), /fallbacks=aec_failed:process_errorx2,aec_backlog_droppedx1/);
});

test('switched on mid-call; totals add up over two mic sessions (a mic restart)', () => {
  const call = createAecCallTracker();
  call.reset(false);
  call.onEvent('aec_switched on');
  call.onEvent(STOPPED);
  call.onEvent(STOPPED.replace('aec_frames=5400', 'aec_frames=0').replace('delay_ms=12', 'delay_ms=-1'));
  const s = call.summary();
  assert.ok(s);
  assert.equal(s.enabled, true);
  assert.equal(s.activePercent, 45);
  assert.equal(s.delayMs, 12, 'an unknown delay (-1) keeps the last known one');
});

test('a build without the library reports why it never cancelled', () => {
  const call = createAecCallTracker();
  call.reset(true);
  call.onEvent('aec_engine_unavailable reason=not_built');
  const s = call.summary();
  assert.ok(s);
  assert.equal(s.active, false);
  assert.equal(s.hadProblem, true);
  assert.equal(s.lastError, 'aec_engine_unavailable reason=not_built');
});
