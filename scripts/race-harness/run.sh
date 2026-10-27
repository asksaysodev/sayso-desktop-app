#!/usr/bin/env bash
# SAYSO-355 / SAYSO-349 native-audio race harness.
#
# Builds an instrumented scratch copy of native_audio.node against the SYSTEM node ABI
# (never the Electron ABI — the real app build is untouched by any of this) and stress-tests
# the SCK start/stop settlement logic for use-after-free / double-settle races under a
# memory-error detector, following the team's rule that concurrency fixes are verified
# against real races, not reasoned about — see PartitionAlloc-crash-triage lesson.
#
# ASan does not work on macOS 26.5 (runtime CHECK-fails at init, both under Electron and
# plain node). This uses libgmalloc instead: every free unmaps the page, so any touch of
# freed memory is an instant SIGSEGV/SIGBUS with a clean stack pointing at the guilty
# instruction, no different in evidentiary value than an ASan report for this bug class.
#
# Usage:
#   ./scripts/race-harness/run.sh              # full run: canary + all scenario sets
#   ./scripts/race-harness/run.sh canary        # canary only (~5s)
#   ./scripts/race-harness/run.sh race          # scenarios A/B/C only (~90s, no SCK permission needed)
#   ./scripts/race-harness/run.sh orphan        # scenario D only (~3min, needs screen-recording permission)
#   ./scripts/race-harness/run.sh mic           # scenario E only (~3min, needs mic permission) — SAYSO-361
#   ./scripts/race-harness/run.sh halchurn      # scenario F only (~5min, needs mic permission) — SAYSO-430
#   HARNESS_CYCLES=1000 ./scripts/race-harness/run.sh race
set -euo pipefail

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
NATIVE_SRC="$HERE/../../electron/native-audio"
SCRATCH="$(mktemp -d /tmp/sayso-race-harness.XXXXXX)"
MODE="${1:-all}"

echo "[run] scratch dir: $SCRATCH"
cleanup() { rm -rf "$SCRATCH"; }
trap cleanup EXIT

mkdir -p "$SCRATCH/src"
cp "$NATIVE_SRC/binding.gyp" "$SCRATCH/binding.gyp"
ln -sfn "$NATIVE_SRC/node_modules" "$SCRATCH/node_modules"   # nan lives here, not client root
cp "$HERE/harness-node.js" "$HERE/harness-orphan.js" "$HERE/harness-mic.js" "$HERE/harness-halchurn.js" "$SCRATCH/"

# HARNESS_SOURCE_MM: instrument a different .mm instead of the working tree's — e.g. the pre-fix
# source from `git show <ref>:electron/native-audio/src/audio_device_manager.mm` for a before/after run.
python3 "$HERE/apply-test-hooks.py" "${HARNESS_SOURCE_MM:-$NATIVE_SRC/src/audio_device_manager.mm}" "$SCRATCH/src/audio_device_manager.mm"

echo "[run] building instrumented addon against system node ABI (NOT the Electron build)..."
( cd "$SCRATCH" && npx node-gyp rebuild >/tmp/race-harness-build.log 2>&1 ) \
  || { echo "[run] build failed, see /tmp/race-harness-build.log"; exit 1; }

# Unhardened node binary: stock node has the hardened-runtime entitlement flag, which
# strips DYLD_INSERT_LIBRARIES. Ad-hoc resign without it so libgmalloc can attach.
NODE_BIN="$SCRATCH/node-unhardened"
cp "$(command -v node)" "$NODE_BIN"
codesign --force --sign - "$NODE_BIN"

GMALLOC=/usr/lib/libgmalloc.dylib
# HARNESS_GMALLOC_ALLOW_READS=1: guard pages become readable, so read OVERRUNS (e.g. CoreAudio
# over-reading a format buffer mid-device-change, which normal malloc never notices) stop faulting,
# while reads and writes of FREED memory still do. The canary then also proves a UAF read faults.
if [ "${HARNESS_GMALLOC_ALLOW_READS:-0}" = "1" ]; then
  export MALLOC_ALLOW_READS=1
  echo "[run] MALLOC_ALLOW_READS=1 — read overruns tolerated, use-after-free still fatal"
fi
if [ ! -f "$GMALLOC" ]; then
  echo "[run] libgmalloc.dylib not found at $GMALLOC — are you on macOS?"; exit 1
fi

run_canary() {
  echo "[run] === canary: proving the memory-error detector actually intercepts allocations ==="
  echo "[run] this call is a DELIBERATE use-after-free — a crash/SIGSEGV here is SUCCESS"
  set +e
  DYLD_INSERT_LIBRARIES="$GMALLOC" "$NODE_BIN" -e "
    const a = require('$SCRATCH/build/Release/native_audio.node');
    a.asanCanaryUAF();
    console.log('[canary] !!! SURVIVED the UAF — detector is NOT working, do not trust any race run');
    process.exit(1);
  "
  local rc=$?
  set -e
  if [ "$rc" -eq 0 ]; then
    echo "[run] CANARY FAILED (exit 0, no crash) — detector is not catching UAFs on this machine. STOP."
    exit 1
  fi
  echo "[run] canary crashed as expected (exit $rc) — detector confirmed working"
  if [ "${HARNESS_GMALLOC_ALLOW_READS:-0}" = "1" ]; then
    set +e
    DYLD_INSERT_LIBRARIES="$GMALLOC" "$NODE_BIN" -e "
      const a = require('$SCRATCH/build/Release/native_audio.node');
      a.asanCanaryUAFRead();
      console.log('[canary] !!! SURVIVED the UAF read — MALLOC_ALLOW_READS hides freed-memory reads, do not trust this mode');
      process.exit(1);
    "
    rc=$?
    set -e
    if [ "$rc" -eq 0 ]; then
      echo "[run] READ CANARY FAILED — freed-memory reads don't fault with MALLOC_ALLOW_READS. STOP."
      exit 1
    fi
    echo "[run] read canary crashed as expected (exit $rc) — UAF reads still caught with MALLOC_ALLOW_READS"
  fi
}

run_race() {
  echo "[run] === scenarios A/B/C: watchdog-wins, stop-cancel, clean cycles (${HARNESS_CYCLES:-300} cycles) ==="
  ( cd "$SCRATCH" && DYLD_INSERT_LIBRARIES="$GMALLOC" ./node-unhardened harness-node.js )
}

run_orphan() {
  echo "[run] === scenario D: running-stream-orphaned (${HARNESS_CYCLES:-60} cycles) ==="
  echo "[run] uses real ScreenCaptureKit — grant screen-recording permission to node-unhardened if prompted"
  ( cd "$SCRATCH" && DYLD_INSERT_LIBRARIES="$GMALLOC" ./node-unhardened harness-orphan.js )
}

run_mic() {
  echo "[run] === scenario E: mic engine leak/release + route-restart stress (${HARNESS_CYCLES:-200} cycles) ==="
  echo "[run] uses real AVAudioEngine — grant microphone permission to node-unhardened if prompted"
  ( cd "$SCRATCH" && DYLD_INSERT_LIBRARIES="$GMALLOC" ./node-unhardened harness-mic.js )
}

run_halchurn() {
  echo "[run] === scenario F: mic start/stop/reset under REAL HAL churn (${HARNESS_CYCLES:-200} cycles, lever ${HARNESS_CHURN_LEVER:-mixed}) ==="
  echo "[run] creates two 'Sayso HAL churn' aggregate devices and toggles system audio defaults for the run —"
  echo "[run] other apps follow them. Grant microphone permission if prompted."
  local addon="$SCRATCH/build/Release/native_audio.node"
  # Non-private aggregates outlive a crashed process: snapshot the defaults now, restore after.
  local snap
  snap="$("$NODE_BIN" -e "const s = require('$addon').halChurnSnapshotDefaults(); console.log(JSON.stringify(s));")"
  echo "[run] default devices before: $snap"
  set +e
  # HARNESS_NODE_FLAGS: e.g. --disable-wasm-trap-handler, so node's SIGSEGV handler doesn't re-deliver the
  # fault and the crash report names the thread that actually touched freed memory.
  if [ "${HARNESS_UNDER_LLDB:-0}" = "1" ]; then
    # ReportCrash throttles after many crashes of the same binary in a day; lldb always gets the
    # stack. Prints every thread's backtrace when the process stops on a fault or trap.
    ( cd "$SCRATCH" && lldb --batch \
        -o "settings set target.env-vars DYLD_INSERT_LIBRARIES=$GMALLOC${MALLOC_ALLOW_READS:+ MALLOC_ALLOW_READS=1}" \
        -o "process handle SIGPIPE SIGUSR1 SIGUSR2 -s false -p true" \
        -o run -k "thread backtrace all -c 18" -k "quit 139" \
        -- ./node-unhardened ${HARNESS_NODE_FLAGS:-} harness-halchurn.js )
  else
    ( cd "$SCRATCH" && DYLD_INSERT_LIBRARIES="$GMALLOC" ./node-unhardened ${HARNESS_NODE_FLAGS:-} harness-halchurn.js )
  fi
  local rc=$?
  set -e
  "$NODE_BIN" -e "
    const s = $snap;
    const r = require('$addon').halChurnCleanup(s.input, s.output, ${HARNESS_RESTORE_RATE:-48000});
    console.log('[run] cleanup: ' + JSON.stringify(r));
  "
  if [ "$rc" -ne 0 ]; then
    echo "[run] scenario F exited $rc — if that's 139/138 (SIGSEGV/SIGBUS) the detector caught a fault;"
    echo "[run] the crash report is the newest node-unhardened-*.ips in ~/Library/Logs/DiagnosticReports"
    exit "$rc"
  fi
}

case "$MODE" in
  canary) run_canary ;;
  race)   run_canary; run_race ;;
  orphan) run_canary; run_orphan ;;
  mic)    run_canary; run_mic ;;
  halchurn) run_canary; run_halchurn ;;
  all)    run_canary; run_race; run_orphan; run_mic ;;
  *) echo "unknown mode: $MODE (expected canary|race|orphan|mic|halchurn|all)"; exit 1 ;;
esac

echo "[run] all requested scenarios completed with zero memory-error detector findings."
