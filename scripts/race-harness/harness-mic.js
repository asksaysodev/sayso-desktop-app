// SAYSO-361 race harness — Scenario E: mic engine leak/release + route-restart stress.
// Runs against a scratch-built, test-hook-instrumented copy of native_audio.node under a
// memory-error detector (see README.md). NEVER runs against the real Electron-ABI build.
// Requires microphone permission for the resigned node-unhardened binary — grant it if prompted
// on first run, same as `orphan` mode needs screen-recording permission.

const addon = require('./build/Release/native_audio.node');
const CYCLES = parseInt(process.env.HARNESS_CYCLES || '200', 10);
const sleep = (ms) => new Promise((r) => setTimeout(r, ms));
const rand = (n) => Math.floor(Math.random() * n);

let lifecycleEvents = {};
// SAYSO-353: pass/fail counters for the recovery-specific branches — "didn't crash" alone isn't
// enough evidence these branches actually reached the code they're meant to exercise, same lesson
// as the orphan scenario's stopped===0 guard.
let rtTerminalHits = 0;
let rtNotLatchedHits = 0;
let rsRecoveredHits = 0;

function start() {
  const r = addon.startMicrophoneCapture();
  if (r === true) return 'ok';
  if (r && r.ok === false) return r.reason || 'err';
  return 'err';
}

function stop() {
  return addon.stopMicrophoneCapture();
}

// SAYSO-353: poll instead of a fixed sleep — the backoff chain's real timing (debounce +
// however many retry steps it takes to cross the ceiling) isn't worth hand-deriving in the
// driver when we can just wait for the lifecycle counter to move.
async function waitForLifecycleCount(key, atLeast, timeoutMs) {
  const deadline = Date.now() + timeoutMs;
  while (Date.now() < deadline) {
    if ((lifecycleEvents[key] || 0) >= atLeast) return true;
    await sleep(50);
  }
  return (lifecycleEvents[key] || 0) >= atLeast;
}

async function main() {
  await addon.initialize();
  addon.setLifecycleEventCallback((e) => {
    const key = String(e).split(' ')[0];
    lifecycleEvents[key] = (lifecycleEvents[key] || 0) + 1;
  });
  // Set once, never cleared during the run: clearing it deletes+recreates the uv_async_t, which
  // is SAYSO-362's still-open bug, not this ticket's. Matches how the real app sets this once per
  // app lifetime, not once per session.
  addon.setMicrophoneStreamingCallback(() => {});

  const counts = {};
  const t0 = Date.now();
  for (let i = 0; i < CYCLES; i++) {
    const roll = rand(30);
    let key;
    if (roll < 8) {
      // E-A: clean start/stop, no restart — common-path regression watch, the 99% path every
      // real session takes.
      const r = start();
      await sleep(rand(40));
      const s = stop();
      key = `A:${r}/${s}`;
    } else if (roll < 17) {
      // E-B: start, let a simulated device-change restart run to completion, then stop. Exercises
      // MicEngineTeardownOnly -> TryStartMicrophoneCaptureOnce under a real teardown/rebuild driven
      // through SaysoMicRouteRestartQueue, same code path a real AirPods removal would hit.
      const r = start();
      addon.triggerMicRouteRestart();
      await sleep(700 + rand(300)); // internal debounce is ~750ms when the HAL id doesn't change
      const s = stop();
      key = `B:${r}/${s}`;
    } else if (roll < 23) {
      // E-C: force the no_input_node failure branch deterministically (both the primary attempt
      // and the internal retry see the env var), so the early-release leak site this ticket fixed
      // gets hit twice in a row within a single StartMicrophoneCapture call, every time — not left
      // to chance on real hardware failures.
      process.env.SAYSO_TEST_MIC_FORCE_NO_INPUT_NODE = '1';
      const r = start();
      delete process.env.SAYSO_TEST_MIC_FORCE_NO_INPUT_NODE;
      const s = stop();
      key = `C:${r}/${s}`;
    } else if (roll < 27) {
      // E-D: trigger a restart, then stop almost immediately — without waiting for the restart's
      // debounce to settle. Races StopMicrophoneCapture's dispatch_sync against a restart still
      // queued (or mid-debounce-timer) on the same canonical queue.
      const r = start();
      addon.triggerMicRouteRestart();
      await sleep(rand(60));
      const s = stop();
      // Let any in-flight debounce timers from the triggered restart drain before the next cycle
      // reuses the globals — SaysoPerformMicRestartIfCapturing no-ops once g_isMicCapturing is
      // false, but give it the chance to actually reach that check rather than piling up.
      await sleep(700 + rand(300));
      key = `D:${r}/${s}`;
    } else if (roll < 29) {
      // E-RT (recovery-to-terminal): force both immediate attempts to fail so the restart falls
      // into the new backoff loop, and shrink the ceiling so it exhausts in well under a second.
      // Confirms mic_route_recovery_started -> ...failed fires, then — the actual point of this
      // ticket's "never permanently latches" AC — clears the forced failure and triggers a FRESH
      // notification, which must still be able to recover normally.
      const r = start(); // must succeed cleanly first, or g_isMicCapturing never goes true and
                         // triggerMicRouteRestart's SaysoPerformMicRestartIfCapturing no-ops
      process.env.SAYSO_TEST_MIC_FORCE_NO_INPUT_NODE = '1';
      process.env.SAYSO_TEST_MIC_RECOVERY_CEILING_MS = '300';
      const startedBefore = lifecycleEvents['mic_route_recovery_started'] || 0;
      const failedBefore = lifecycleEvents['mic_route_recovery_failed'] || 0;
      addon.triggerMicRouteRestart();
      const gotTerminal = await waitForLifecycleCount('mic_route_recovery_failed', failedBefore + 1, 5000);
      const gotStarted = (lifecycleEvents['mic_route_recovery_started'] || 0) > startedBefore;
      delete process.env.SAYSO_TEST_MIC_FORCE_NO_INPUT_NODE;
      delete process.env.SAYSO_TEST_MIC_RECOVERY_CEILING_MS;
      // Not latched off: a later notification for the same still-capturing session must still
      // reach a normal (non-forced) restart attempt.
      const attemptsBefore = lifecycleEvents['mic_route_restart_attempt'] || 0;
      addon.triggerMicRouteRestart();
      const recoveredAfterTerminal = await waitForLifecycleCount('mic_route_restart_attempt', attemptsBefore + 1, 5000);
      const s = stop();
      if (gotTerminal) rtTerminalHits++;
      if (recoveredAfterTerminal) rtNotLatchedHits++;
      key = `RT:${r}/${s}/started=${gotStarted}/terminal=${gotTerminal}/notLatched=${recoveredAfterTerminal}`;
    } else {
      // E-RS (recovery-to-success): force failure just long enough to enter backoff, then clear it
      // before the ceiling — a scheduled retry step should pick up the now-real (unforced) attempt
      // and succeed, emitting mic_route_recovery_succeeded.
      const r = start(); // must succeed cleanly first, same reason as E-RT above
      process.env.SAYSO_TEST_MIC_FORCE_NO_INPUT_NODE = '1';
      const startedBefore = lifecycleEvents['mic_route_recovery_started'] || 0;
      const succeededBefore = lifecycleEvents['mic_route_recovery_succeeded'] || 0;
      addon.triggerMicRouteRestart();
      await waitForLifecycleCount('mic_route_recovery_started', startedBefore + 1, 3000);
      await sleep(150 + rand(150)); // let at least one forced-fail backoff attempt happen
      delete process.env.SAYSO_TEST_MIC_FORCE_NO_INPUT_NODE;
      const recovered = await waitForLifecycleCount('mic_route_recovery_succeeded', succeededBefore + 1, 6000);
      const s = stop();
      if (recovered) rsRecoveredHits++;
      key = `RS:${r}/${s}/recovered=${recovered}`;
    }
    counts[key] = (counts[key] || 0) + 1;
    if ((i + 1) % 20 === 0) {
      console.log(`[mic] ${i + 1}/${CYCLES} ${JSON.stringify(counts)} lifecycle=${JSON.stringify(lifecycleEvents)} elapsed=${((Date.now() - t0) / 1000).toFixed(0)}s`);
    }
  }
  await sleep(900);
  stop();
  console.log(`[mic] DONE ${JSON.stringify(counts)} lifecycle=${JSON.stringify(lifecycleEvents)}`);

  const attempts = lifecycleEvents['mic_route_restart_attempt'] || 0;
  if (attempts === 0) {
    console.error('[mic] WARNING: zero route-restart attempts observed — triggerMicRouteRestart never reached SaysoPerformMicRestartIfCapturing. Not a pass; re-tune timing and re-run.');
    process.exit(4);
    return;
  }
  console.log(`[mic] route-restart attempts=${attempts} ok=${lifecycleEvents['mic_route_restart_ok'] || 0} failed=${lifecycleEvents['mic_route_restart_failed'] || 0}`);
  console.log(`[mic] recovery: terminal-failure hits=${rtTerminalHits} not-latched-after hits=${rtNotLatchedHits} recovered-mid-backoff hits=${rsRecoveredHits}`);

  if (rtTerminalHits === 0 || rtNotLatchedHits === 0 || rsRecoveredHits === 0) {
    console.error('[mic] WARNING: zero coverage on one or more recovery branches (terminal / not-latched / mid-backoff-success) — timing likely never reached the target code. Not a pass; re-tune and re-run.');
    process.exit(5);
    return;
  }
  console.log('[mic] survived all cycles — PASS');
  process.exit(0);
}

main().catch((e) => { console.error('[mic] driver error:', e); process.exit(3); });
