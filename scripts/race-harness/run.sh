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
cp "$HERE/harness-node.js" "$HERE/harness-orphan.js" "$HERE/harness-mic.js" "$SCRATCH/"

python3 "$HERE/apply-test-hooks.py" "$NATIVE_SRC/src/audio_device_manager.mm" "$SCRATCH/src/audio_device_manager.mm"

echo "[run] building instrumented addon against system node ABI (NOT the Electron build)..."
( cd "$SCRATCH" && npx node-gyp rebuild >/tmp/race-harness-build.log 2>&1 ) \
  || { echo "[run] build failed, see /tmp/race-harness-build.log"; exit 1; }

# Unhardened node binary: stock node has the hardened-runtime entitlement flag, which
# strips DYLD_INSERT_LIBRARIES. Ad-hoc resign without it so libgmalloc can attach.
NODE_BIN="$SCRATCH/node-unhardened"
cp "$(command -v node)" "$NODE_BIN"
codesign --force --sign - "$NODE_BIN"

GMALLOC=/usr/lib/libgmalloc.dylib
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

case "$MODE" in
  canary) run_canary ;;
  race)   run_canary; run_race ;;
  orphan) run_canary; run_orphan ;;
  mic)    run_canary; run_mic ;;
  all)    run_canary; run_race; run_orphan; run_mic ;;
  *) echo "unknown mode: $MODE (expected canary|race|orphan|mic|all)"; exit 1 ;;
esac

echo "[run] all requested scenarios completed with zero memory-error detector findings."
