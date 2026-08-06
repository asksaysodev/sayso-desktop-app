// SAYSO-355 race harness — Scenarios A/B/C: watchdog-wins, stop-cancel, clean cycles.
// Runs against a scratch-built, test-hook-instrumented copy of native_audio.node under a
// memory-error detector (see README.md). NEVER runs against the real Electron-ABI build.
process.env.SAYSO_TEST_DISPATCH_GLOBAL = '1';

const addon = require('./build/Release/native_audio.node');
const CYCLES = parseInt(process.env.HARNESS_CYCLES || '300', 10);
const sleep = (ms) => new Promise((r) => setTimeout(r, ms));
const rand = (n) => Math.floor(Math.random() * n);

let lifecycleEvents = 0;

async function start() {
  try {
    await addon.startSystemAudioCapture({ streamingOnly: true });
    return 'ok';
  } catch (e) {
    const m = String(e.message);
    if (m.startsWith('sck_start_timeout')) return 'timeout';
    if (m.startsWith('sck_start_canceled_by_stop')) return 'canceled';
    if (m.includes('already')) return 'already';
    return 'err';
  }
}

async function main() {
  await addon.initialize();
  addon.setLifecycleEventCallback(() => { lifecycleEvents++; });

  const counts = {};
  const t0 = Date.now();
  for (let i = 0; i < CYCLES; i++) {
    const roll = rand(10);
    let key;
    if (roll < 4) {
      // A: wedge -> watchdog wins -> late callback into abandoned gen, overlapped by next start
      process.env.SAYSO_TEST_SCK_WATCHDOG_MS = '40';
      process.env.SAYSO_TEST_SCK_DELAY_MS = String(80 + rand(120));
      const r = await start();
      if (rand(2)) {
        process.env.SAYSO_TEST_SCK_DELAY_MS = '0';
        process.env.SAYSO_TEST_SCK_WATCHDOG_MS = '5000';
        await start();
        await addon.stopSystemAudioCapture().catch(() => {});
      }
      await sleep(40 + rand(180));
      key = `A:${r}`;
    } else if (roll < 7) {
      // B: stop-cancel races the pending start; late callback lands after
      process.env.SAYSO_TEST_SCK_WATCHDOG_MS = '400';
      process.env.SAYSO_TEST_SCK_DELAY_MS = String(120 + rand(120));
      const p = start();
      await sleep(rand(80));
      await addon.stopSystemAudioCapture().catch(() => {});
      const r = await p;
      await sleep(30 + rand(140));
      key = `B:${r}`;
    } else {
      // C: clean start/stop (common-path regression watch)
      process.env.SAYSO_TEST_SCK_DELAY_MS = '0';
      process.env.SAYSO_TEST_SCK_WATCHDOG_MS = '5000';
      const r = await start();
      await sleep(rand(50));
      await addon.stopSystemAudioCapture().catch(() => {});
      key = `C:${r}`;
    }
    counts[key] = (counts[key] || 0) + 1;
    if ((i + 1) % 25 === 0) {
      console.log(`[harness] ${i + 1}/${CYCLES} ${JSON.stringify(counts)} lifecycle=${lifecycleEvents} elapsed=${((Date.now() - t0) / 1000).toFixed(0)}s`);
    }
  }
  await sleep(600);
  await addon.stopSystemAudioCapture().catch(() => {});
  console.log(`[harness] DONE ${JSON.stringify(counts)} lifecycleEvents=${lifecycleEvents}`);
  console.log('[harness] survived all cycles — PASS');
  process.exit(0);
}

main().catch((e) => { console.error('[harness] driver error:', e); process.exit(3); });
