// SAYSO-355 race harness — Scenario D: a RUNNING SCStream loses settlement (watchdog wins
// mid-startCapture) -> the orphan-stop branch must fire, touch no globals, and stop the real
// capture. Requires screen-recording permission for the test host binary (real SCK calls).
process.env.SAYSO_TEST_DISPATCH_GLOBAL = '1';
process.env.SAYSO_TEST_SCK_DELAY_MS = '0';

const addon = require('./build/Release/native_audio.node');
const CYCLES = parseInt(process.env.HARNESS_CYCLES || '60', 10);
const sleep = (ms) => new Promise((r) => setTimeout(r, ms));
const rand = (n) => Math.floor(Math.random() * n);

let events = {};

async function start() {
  try { await addon.startSystemAudioCapture({ streamingOnly: true }); return 'ok'; }
  catch (e) {
    const m = String(e.message);
    return m.startsWith('sck_start_timeout') ? 'timeout'
         : m.startsWith('sck_start_canceled_by_stop') ? 'canceled' : 'err';
  }
}

async function main() {
  await addon.initialize();
  addon.setLifecycleEventCallback((e) => {
    const key = e.split(' ')[0];
    events[key] = (events[key] || 0) + 1;
  });

  const counts = {};
  for (let i = 0; i < CYCLES; i++) {
    // Watchdog must fire AFTER the real SCK setup (~600ms+) completes and startCapture is
    // running, or the orphan branch never gets reached — tune these together, not independently.
    process.env.SAYSO_TEST_SCK_WATCHDOG_MS = '900';
    process.env.SAYSO_TEST_SCK_DELAY2_MS = String(1600 + rand(400));
    const r = await start();
    counts[r] = (counts[r] || 0) + 1;
    // Half the time, overlap the orphan teardown with an immediate clean start.
    if (rand(2)) {
      process.env.SAYSO_TEST_SCK_DELAY2_MS = '0';
      process.env.SAYSO_TEST_SCK_WATCHDOG_MS = '5000';
      const r2 = await start();
      counts[`overlap:${r2}`] = (counts[`overlap:${r2}`] || 0) + 1;
      await addon.stopSystemAudioCapture().catch(() => {});
    }
    await sleep(2300 + rand(300)); // let delay2 elapse + orphan stop complete
    if ((i + 1) % 20 === 0) console.log(`[orphan] ${i + 1}/${CYCLES}`, JSON.stringify(counts), JSON.stringify(events));
  }
  await sleep(800);
  await addon.stopSystemAudioCapture().catch(() => {});
  console.log('[orphan] DONE', JSON.stringify(counts), JSON.stringify(events));
  const stopped = events['sck_orphan_stream_stopped'] || 0;
  const failed = events['sck_orphan_stream_stop_failed'] || 0;
  console.log(`[orphan] orphan streams stopped=${stopped} stop_failed=${failed}`);
  if (stopped === 0) {
    console.error('[orphan] WARNING: zero orphans observed — the watchdog/delay2 timing likely never reached the running-stream branch. Not a pass; re-tune and re-run.');
  }
  process.exit(stopped > 0 && failed === 0 ? 0 : 4);
}

main().catch((e) => { console.error('[orphan] driver error:', e); process.exit(3); });
