# native-audio race harness (SAYSO-355 / SAYSO-349)

Stress-tests the `native-audio` SCK start/stop settlement logic for
use-after-free and double-settle races, under a memory-error detector, per
the team rule that concurrency fixes get verified against real races, not
reasoned about (a clean code review does not close a UAF ticket).

## Why this exists

SAYSO-355 rewrote the ScreenCaptureKit start path to add a watchdog timeout,
a stop-cancel path, and settle-once semantics — three ways a
`PendingSckStart` can now be settled, where before there was only one. That's
exactly the kind of change PartitionAlloc-crash-triage warns about: a
plausible-looking fix for a memory-lifetime bug can just as easily introduce
a new one (see SAYSO-349, née a `uv_async_t` UAF in this same file). This
harness exists to make that verifiable instead of assumed.

## Why not ASan

ASan does not work on this team's current macOS (26.5, Apple clang 16): the
runtime hard-fails at init (`sanitizer_malloc_mac.inc:189
asan_init_is_running`) under both Electron's allocator shim and plain node.
This uses **libgmalloc** instead — every allocation gets its own guard pages
and every free unmaps the page, so any touch of freed memory is an instant
SIGSEGV/SIGBUS with a clean stack pointing at the exact instruction. For this
bug class (touch-after-free, not a subtle overflow) it has the same
evidentiary value as an ASan report. If a future macOS/toolchain combination
gets ASan working again, swap `DYLD_INSERT_LIBRARIES` in `run.sh` for the
ASan dylib — the harness scripts and hooks don't need to change.

## What it does NOT touch

- `electron/native-audio/src/audio_device_manager.mm` — the real, committed
  source — is never modified. `apply-test-hooks.py` writes an instrumented
  copy to a scratch dir.
- The real Electron-ABI build (`electron/native-audio/build/`) is never
  rebuilt by this harness. Everything here builds against the **system
  node** ABI in an isolated scratch directory, deleted on exit.
- Nothing here ships. There is no path by which the test hooks (env-gated
  timing knobs, and a function that deliberately triggers a UAF) reach a
  packaged build.

## Usage

```bash
./scripts/race-harness/run.sh              # canary + all scenarios (~5 min)
./scripts/race-harness/run.sh canary       # just prove the detector works (~5s)
./scripts/race-harness/run.sh race         # scenarios A/B/C (~90s @ 300 cycles)
./scripts/race-harness/run.sh orphan       # scenario D (~3 min @ 60 cycles, needs screen-recording permission)
HARNESS_CYCLES=1000 ./scripts/race-harness/run.sh race
```

The first screen-recording permission prompt will be for a binary named
`node-unhardened` (a resigned copy of your system `node`, used because stock
node's hardened-runtime entitlement strips `DYLD_INSERT_LIBRARIES`). Grant or
deny — `race` mode needs no permission at all; `orphan` mode needs it granted
to reach real ScreenCaptureKit capture.

**Always run canary first** (`run.sh` does this automatically in every mode).
It deliberately triggers a use-after-free and must crash. If it doesn't, the
detector isn't intercepting allocations in that process and a clean race run
proves nothing — `run.sh` will refuse to proceed past canary failure.

## What each scenario covers

| Scenario | What it races | Why it matters |
|---|---|---|
| **A** (40% of `race`) | SCK's `getShareableContent` callback held past the watchdog timeout, then fires into an already-abandoned generation, sometimes overlapped by the *next* start | The core SAYSO-355 hazard: freed-struct + stale-generation, the shape that produced SAYSO-APP-5T |
| **B** (30% of `race`) | `stopSystemAudioCapture` cancels a start while it's still pending, with the SCK callback landing afterward | Stop-cancel racing a pending start |
| **C** (30% of `race`) | Clean start/stop, no injected delay | Common-path regression watch — this is the 99.9% path every real session takes |
| **D** (`orphan`) | Watchdog wins *after* `SCStream` is genuinely running (real capture), must self-stop without touching globals that may belong to a newer start | The one branch `race` mode structurally cannot reach — needs a live SCK session |

Each scenario's pass condition is "the process didn't crash under the
detector" — not "the promise resolved a particular way." Rejections
(`sck_start_timeout`, `sck_start_canceled_by_stop`, etc.) are expected and
counted, not failures.

## Timing is coupled, not independent

Real `getShareableContent` calls take 600ms+ under instrumentation. If the
watchdog is shorter than that, it always wins before the stream is even
built, and `orphan` mode silently gets zero coverage of its target branch —
this happened during development (`stopped=0` on a "passing" 100-cycle run).
`run.sh orphan` checks for this and treats `stopped == 0` as a failure, not a
pass; if you change the timing constants in `harness-orphan.js`, watch that
guard.

## Baseline results (2026-08-06, macOS 26.5, Apple clang 16)

- Canary: use-after-free caught, SIGSEGV, exit 139.
- `race`: 300 cycles (108 watchdog-timeout, 102 stop-canceled, 90 clean),
  420 lifecycle events observed, zero detector findings.
- `orphan`: 60 cycles, 60/60 orphaned real SCK streams stopped cleanly
  (`sck_orphan_stream_stopped`), including 24 overlapped by a concurrent
  clean start, zero `stop_failed`, zero detector findings.

## Extending for SAYSO-349

349's verification AC needs the same harness with a mic-path scenario added
(rapid `startMicrophoneCapture`/`stopMicrophoneCapture` cycling plus an
injected device-change notification, targeting the `uv_async_t` lifetime and
`AVAudioEngine` release issues in that ticket). `apply-test-hooks.py` and
`run.sh` are written to make that an additive scenario + hook, not a new
harness — see the `getenv`-gated pattern already used for the SCK hooks.
