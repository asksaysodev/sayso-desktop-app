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
./scripts/race-harness/run.sh              # canary + all scenarios (~8 min)
./scripts/race-harness/run.sh canary       # just prove the detector works (~5s)
./scripts/race-harness/run.sh race         # scenarios A/B/C (~90s @ 300 cycles)
./scripts/race-harness/run.sh orphan       # scenario D (~3 min @ 60 cycles, needs screen-recording permission)
./scripts/race-harness/run.sh mic          # scenario E (~3 min @ 200 cycles, needs mic permission)
HARNESS_CYCLES=1000 ./scripts/race-harness/run.sh race
```

The first permission prompt will be for a binary named `node-unhardened` (a
resigned copy of your system `node`, used because stock node's
hardened-runtime entitlement strips `DYLD_INSERT_LIBRARIES`). Grant or deny —
`race` mode needs no permission at all; `orphan` mode needs screen-recording
granted to reach real ScreenCaptureKit capture; `mic` mode needs microphone
granted to reach real `AVAudioEngine` capture.

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
| **E** (`mic`) | Mic `startMicrophoneCapture`/`stopMicrophoneCapture` cycling, interleaved with a synthetic device-change trigger (`triggerMicRouteRestart`, a test-only export that calls the real `SaysoScheduleMicRouteDebouncedRestart` entry point) landing before, during, and right up against start/stop calls; one branch forces the `no_input_node` failure path deterministically | SAYSO-361's target: the 3 `AVAudioEngine` leak-then-release sites and the single-canonical-queue refactor (`StartMicrophoneCapture`/`StopMicrophoneCapture` now `dispatch_sync` onto the same queue `SaysoPerformMicRestartIfCapturing` uses) |
| **E-RT / E-RS** (`mic`, sub-scenarios) | Force both immediate restart attempts to fail so the real restart falls into SAYSO-353's backoff recovery loop. RT shrinks the ceiling and drives it to terminal failure, then clears the force-fail and fires a fresh notification — must still recover (the "never permanently latches" AC). RS clears the force-fail mid-backoff so a scheduled retry step succeeds for real. | SAYSO-353's target: the backoff/recovery state machine (`SaysoScheduleMicRouteRecoveryRetry`/`SaysoAttemptMicRouteRecovery`) and the new `AVAudioEngineConfigurationChangeNotification` observer add/remove pairing, which every cycle in every scenario now exercises incidentally |

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

## SAYSO-361 (mic scenario)

Added scenario E / `mic` mode: rapid `startMicrophoneCapture`/
`stopMicrophoneCapture` cycling plus a synthetic device-change trigger,
targeting the `AVAudioEngine` leak-then-release sites and the
single-canonical-queue refactor from SAYSO-361 (split off SAYSO-349).

**Baseline results (2026-08-13, macOS, Apple clang, real AVAudioEngine +
microphone permission granted):**

- Canary: use-after-free caught, SIGSEGV, exit 139.
- `mic`: 400 cycles (126 clean start/stop, 142 restart-then-stop, 72
  forced-`no_input_node`-failure, 60 restart-racing-stop), 98 real
  `mic_route_restart_attempt` events observed (via `triggerMicRouteRestart`),
  98/98 succeeded, 0 failed, zero detector findings. ~4.5 min wall time.
- The forced-failure branch (E-C) exercises the early-release leak site twice
  per cycle (primary attempt + internal retry) — 72 cycles × 2 = 144
  deterministic hits on that path alone, on top of whatever real hardware
  failures happened to occur elsewhere.

## SAYSO-353 (recovery scenario)

Added E-RT/E-RS sub-scenarios exercising the backoff/recovery state machine
and the new `AVAudioEngineConfigurationChangeNotification` observer.

**Baseline results (2026-08-13, macOS, Apple clang, real AVAudioEngine +
microphone permission granted):**

- Canary: use-after-free caught, SIGSEGV, exit 139.
- `mic`: 400 cycles, 161 real `mic_route_restart_attempt` events (121 ok, 40
  failed → entered backoff recovery), zero detector findings. ~5.5 min wall
  time.
- **31/31 terminal-failure cycles (E-RT) confirmed not permanently latched**:
  each one reached `mic_route_recovery_failed` (ceiling exhausted) and then
  successfully triggered a fresh, non-forced restart attempt afterward — the
  ticket's core AC, that a restart failure never permanently disables
  recovery, verified 31 times under a memory-error detector, not just reasoned
  about.
- 9/9 mid-backoff recoveries (E-RS) succeeded once the forced failure was
  cleared, emitting `mic_route_recovery_succeeded`.
- Every cycle in every scenario (A–D plus E-RT/E-RS) now incidentally
  exercises the `AVAudioEngineConfigurationChangeNotification` observer's
  add/remove pairing (registered in `TryStartMicrophoneCaptureOnce`, removed
  in `MicEngineTeardownOnly`) — zero detector findings there either.

Still open for a future ticket: the `uv_async_t` lifetime / `handle->data`
races (SAYSO-362/363) aren't covered by any scenario here yet — scenario E
deliberately sets the mic streaming callback once and never clears it, so it
does not exercise the delete-on-clear path those tickets fix.
