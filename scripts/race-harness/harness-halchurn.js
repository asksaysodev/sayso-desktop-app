// SAYSO-430 race harness — Scenario F: mic start/stop/reset under REAL HAL property churn.
// Runs against a scratch-built, test-hook-instrumented copy of native_audio.node under a
// memory-error detector (see README.md). NEVER runs against the real Electron-ABI build.
//
// Scenario E (harness-mic.js) only simulates *our* route restart; the system never fires
// AVAudioIOUnit's own HAL property listener there. Here halChurnStart (apply-test-hooks.py step 10)
// flips a private aggregate device's sample rate 44.1k <-> 48k while every engine we build is bound
// to it, so the system queues real IOUnitPropertyListener blocks against our engines — including
// ones MicEngineTeardownOnly is releasing at that moment. On unfixed code the expected outcome is a
// detector fault in IOUnitPropertyListener (SAYSO-APP-82); on fixed code, zero faults with nonzero
// cfgPosts.
//
// Progress is printed every cycle on purpose: when the detector kills the process, the last line is
// the cycle count and listener-fire count at the moment of the fault.

const addon = require('./build/Release/native_audio.node');
const CYCLES = parseInt(process.env.HARNESS_CYCLES || '200', 10);
// 0 (default) = no steady timer, bursts only. A steady 120ms timer was tried first on 1.3.x code:
// 200 cycles, 0 faults — flips that fast keep every engine renegotiating, so ~75% of starts failed
// and engines were rarely running when a flip landed. Bursts aimed at a running engine, with the
// teardown inside the burst, are the field geometry (free while the callback backlog is largest).
const PERIOD_MS = parseInt(process.env.HARNESS_CHURN_PERIOD_MS || '0', 10);
// mixed (default) | default-output | default-input | subdevices | rate — see TestChurnLever in
// apply-test-hooks.py step 10. "rate" lands on a running engine but the engine never reacts.
const LEVER = process.env.HARNESS_CHURN_LEVER || 'mixed';
// Burst shape for F-B. Default (unset) = 2..7 steps, 0..7ms apart: a pile-up of device changes far
// harsher than a real plug. HARNESS_BURST_STEPS=2 HARNESS_BURST_GAP_MS=250 = one apply/revert pair,
// i.e. a single plug then unplug — the realistic shape.
const BURST_STEPS = process.env.HARNESS_BURST_STEPS ? parseInt(process.env.HARNESS_BURST_STEPS, 10) : null;
const BURST_GAP_MS = process.env.HARNESS_BURST_GAP_MS ? parseInt(process.env.HARNESS_BURST_GAP_MS, 10) : null;
const RESTORE_RATE = parseFloat(process.env.HARNESS_RESTORE_RATE || '48000');
// Every QUIET_EVERY cycles, idle with no churn for 3.5-6s so the quiet-gated engine release (SAYSO-430)
// actually fires mid-run and the next cycle's churn lands right after it. 0 = never.
const QUIET_EVERY = parseInt(process.env.HARNESS_QUIET_EVERY || '12', 10);
const sleep = (ms) => new Promise((r) => setTimeout(r, ms));
const rand = (n) => Math.floor(Math.random() * n);

const lifecycleEvents = {};

function start() {
  const r = addon.startMicrophoneCapture();
  if (r === true) return 'ok';
  if (r && r.ok === false) return r.reason || 'err';
  return 'err';
}

async function main() {
  // Electron pumps the main CFRunLoop; plain node doesn't, and HAL notifications are delivered on
  // it. HARNESS_PUMP_RUNLOOP=0 reproduces the pre-10-26 (blind) behaviour for comparison.
  const pumpTimer = process.env.HARNESS_PUMP_RUNLOOP === '0' ? null : setInterval(() => addon.pumpRunLoop(), 4);
  await addon.initialize();
  addon.setLifecycleEventCallback((e) => {
    const key = String(e).split(' ')[0];
    lifecycleEvents[key] = (lifecycleEvents[key] || 0) + 1;
  });
  // Set once, never cleared — same reasoning as harness-mic.js (clearing it is SAYSO-362's bug).
  addon.setMicrophoneStreamingCallback(() => {});

  const churn = addon.halChurnStart(PERIOD_MS, LEVER);
  if (!churn.ok) {
    console.error(`[halchurn] could not start HAL churn: ${churn.reason}`);
    process.exit(6);
  }
  console.log(`[halchurn] churn on: aggregate id=${churn.aggId} lever=${churn.lever} secondInput=${churn.secondInput || '-'} period=${PERIOD_MS}ms builtInRate=${churn.builtInRate}`);

  const counts = {};
  let peakQuarantined = -1;
  const t0 = Date.now();
  for (let i = 0; i < CYCLES; i++) {
    const roll = rand(10);
    let key;
    const r = start();
    if (roll < 6) {
      // F-B: let the engine run, fire a burst of real rate changes at it, and tear down while the
      // listener callbacks they queue are still in flight. The next cycle's start follows right
      // away, like a Cue reset.
      await sleep(200 + rand(1200));
      addon.halChurnBurst(BURST_STEPS ?? 2 + rand(6), BURST_GAP_MS ?? rand(8));
      await sleep(rand(30));
      addon.stopMicrophoneCapture();
      key = `B:${r}`;
    } else if (roll < 8) {
      // F-R: Cue reset — short session, stop, and the next cycle's start follows immediately. The
      // shape of both matching SAYSO-APP-82 events (crash 2.5s / 8s after a reset teardown).
      await sleep(rand(800));
      addon.stopMicrophoneCapture();
      key = `R:${r}`;
    } else if (roll < 9) {
      // F-H: hold long enough for the SAYSO-353 config-change observer to drive its own debounced
      // teardown/rebuild (600ms debounce + 150ms stability) at least once under churn.
      addon.halChurnBurst(1, 0);
      await sleep(1200 + rand(1800));
      addon.halChurnBurst(3, 2);
      addon.stopMicrophoneCapture();
      key = `H:${r}`;
    } else {
      // F-Q: immediate stop, then churn with only stopped engines alive.
      addon.stopMicrophoneCapture();
      addon.halChurnBurst(4, 20);
      await sleep(rand(400));
      key = `Q:${r}`;
    }
    counts[key] = (counts[key] || 0) + 1;
    if (QUIET_EVERY > 0 && (i + 1) % QUIET_EVERY === 0) {
      await sleep(3500 + rand(2500));
      counts.quiet = (counts.quiet || 0) + 1;
    }
    const s = addon.halChurnStats();
    peakQuarantined = Math.max(peakQuarantined, s.quarantined);
    console.log(`[halchurn] ${i + 1}/${CYCLES} ${key} flips=${s.flips} halFires=${s.halFires} cfgPosts=${s.cfgPosts} builds=${s.engineBuilds} quarantined=${s.quarantined} restarts=${lifecycleEvents['mic_route_restart_attempt'] || 0} t=${((Date.now() - t0) / 1000).toFixed(0)}s`);
  }

  addon.stopMicrophoneCapture();
  // Let any delayed engine releases land while churn is still on — a fix that merely postpones the
  // UAF past the loop must still be caught here.
  for (let k = 0; k < 7; k++) {
    addon.halChurnBurst(3, 10);
    await sleep(1000);
  }
  const stop = addon.halChurnStop(RESTORE_RATE);
  // Releases wait for device quiet, so they land after churn stops. Keep pumping and give them up to
  // 15s; anything still parked after that is a leak.
  for (let k = 0; k < 30 && addon.halChurnStats().quarantined > 0; k++) await sleep(500);
  const fin = { ...stop, quarantined: addon.halChurnStats().quarantined };
  if (pumpTimer) clearInterval(pumpTimer);
  console.log(`[halchurn] DONE ${JSON.stringify(counts)} lifecycle=${JSON.stringify(lifecycleEvents)}`);
  console.log(`[halchurn] final flips=${fin.flips} flipErrors=${fin.flipErrors} halFires=${fin.halFires} cfgPosts=${fin.cfgPosts} engineBuilds=${fin.engineBuilds} quarantined=${fin.quarantined} peakQuarantined=${peakQuarantined}`);

  if (fin.cfgPosts === 0) {
    console.error('[halchurn] WARNING: zero AVAudioEngineConfigurationChangeNotification posts — AVAudioIOUnit\'s listener never fired, so this run says nothing about SAYSO-APP-82. Not a pass.');
    process.exit(4);
  }
  // -1 = pre-fix source with no quarantine. Otherwise every parked engine must have been released
  // by now (the 7s wait exceeds kMicEngineQuarantineMs): no unbounded growth.
  if (fin.quarantined > 0) {
    console.error(`[halchurn] FAIL: ${fin.quarantined} engines still quarantined after the drain wait — engines are not being released.`);
    process.exit(7);
  }
  console.log('[halchurn] survived all cycles under real HAL churn — PASS');
  process.exit(0);
}

main().catch((e) => { console.error('[halchurn] driver error:', e); process.exit(3); });
