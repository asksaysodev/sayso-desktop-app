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

function start() {
  const r = addon.startMicrophoneCapture();
  if (r === true) return 'ok';
  if (r && r.ok === false) return r.reason || 'err';
  return 'err';
}

function stop() {
  return addon.stopMicrophoneCapture();
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
    const roll = rand(20);
    let key;
    if (roll < 6) {
      // E-A: clean start/stop, no restart — common-path regression watch, the 99% path every
      // real session takes.
      const r = start();
      await sleep(rand(40));
      const s = stop();
      key = `A:${r}/${s}`;
    } else if (roll < 13) {
      // E-B: start, let a simulated device-change restart run to completion, then stop. Exercises
      // MicEngineTeardownOnly -> TryStartMicrophoneCaptureOnce under a real teardown/rebuild driven
      // through SaysoMicRouteRestartQueue, same code path a real AirPods removal would hit.
      const r = start();
      addon.triggerMicRouteRestart();
      await sleep(700 + rand(300)); // internal debounce is ~750ms when the HAL id doesn't change
      const s = stop();
      key = `B:${r}/${s}`;
    } else if (roll < 17) {
      // E-C: force the no_input_node failure branch deterministically (both the primary attempt
      // and the internal retry see the env var), so the early-release leak site this ticket fixed
      // gets hit twice in a row within a single StartMicrophoneCapture call, every time — not left
      // to chance on real hardware failures.
      process.env.SAYSO_TEST_MIC_FORCE_NO_INPUT_NODE = '1';
      const r = start();
      delete process.env.SAYSO_TEST_MIC_FORCE_NO_INPUT_NODE;
      const s = stop();
      key = `C:${r}/${s}`;
    } else {
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
  console.log('[mic] survived all cycles — PASS');
  process.exit(0);
}

main().catch((e) => { console.error('[mic] driver error:', e); process.exit(3); });
