#!/usr/bin/env python3
"""
SAYSO-355 / SAYSO-349 race-harness instrumentation.

Applies env-gated test hooks to a SCRATCH COPY of audio_device_manager.mm —
never to the real source in electron/native-audio/src/. All hooks are no-ops
unless the corresponding SAYSO_TEST_* env var is set, but they are still
production-shipping-unsafe (one hook is a deliberate use-after-free, used to
prove the memory-error detector is actually catching things before trusting a
clean run — see README.md "canary-first"). Keeping them out of the committed
.mm is deliberate: no extra getenv() calls in the real start/stop path, and no
UAF-triggering function ever reachable from JS in a shipped build.

Usage: see README.md. Called by run.sh; not normally run directly.
"""
import sys

def apply(src: str) -> str:
    # 1. Env-tunable watchdog (default stays 10s; test can shrink it so a
    #    300-cycle run takes minutes instead of an hour).
    old = 'const int64_t kSckStartWatchdogNs = 10 * NSEC_PER_SEC;'
    new = '''int64_t kSckStartWatchdogNs = 10 * NSEC_PER_SEC;
    if (const char* wd = getenv("SAYSO_TEST_SCK_WATCHDOG_MS")) {   // TEST HARNESS ONLY
        kSckStartWatchdogNs = (int64_t)atoll(wd) * NSEC_PER_MSEC;
    }'''
    assert src.count(old) == 1, "watchdog anchor not found — source has drifted, update this script"
    src = src.replace(old, new)

    # 2. Watchdog queue: dispatch_get_main_queue() never runs under a plain
    #    node host (no NSApplication run loop pumping it). Route to a global
    #    queue in that mode — same cross-thread race geometry as production,
    #    where the watchdog fires from a different thread than JS/uv anyway.
    old = '''    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, kSckStartWatchdogNs), dispatch_get_main_queue(), ^{'''
    new = '''    dispatch_queue_t wdQueue = dispatch_get_main_queue();
    if (getenv("SAYSO_TEST_DISPATCH_GLOBAL")) {   // TEST HARNESS ONLY: node host pumps no main queue
        wdQueue = dispatch_get_global_queue(QOS_CLASS_DEFAULT, 0);
    }
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, kSckStartWatchdogNs), wdQueue, ^{'''
    assert src.count(old) == 1, "watchdog-queue anchor not found — source has drifted, update this script"
    src = src.replace(old, new)

    # 3. Delay hook #1: holds the getShareableContent completion so it lands
    #    AFTER the watchdog has already settled — this is scenario A/B, the
    #    "late callback into an abandoned generation" race.
    old = '''    [SCShareableContent getShareableContentWithCompletionHandler:^(SCShareableContent *content, NSError *error) {
        if (SckStartAbandoned(startGen)) {'''
    new = '''    [SCShareableContent getShareableContentWithCompletionHandler:^(SCShareableContent *content, NSError *error) {
        if (const char* dl = getenv("SAYSO_TEST_SCK_DELAY_MS")) {   // TEST HARNESS ONLY
            long ms = atol(dl);
            if (ms > 0) { usleep((useconds_t)(ms * 1000)); }
        }
        if (SckStartAbandoned(startGen)) {'''
    assert src.count(old) == 1, "shareable-content anchor not found — source has drifted, update this script"
    src = src.replace(old, new)

    # 4. Delay hook #2: holds INSIDE startCaptureWithCompletionHandler, i.e.
    #    after the SCStream is genuinely running. Hook #1 alone can never
    #    reach the orphan-stream branch — it gates before the stream is even
    #    built, so the watchdog always wins upstream of that code. This is
    #    scenario D: a live capture loses settlement and must self-stop.
    old = '''        [stream startCaptureWithCompletionHandler:^(NSError *startErr) {
            if (startErr) {'''
    new = '''        [stream startCaptureWithCompletionHandler:^(NSError *startErr) {
            if (const char* dl2 = getenv("SAYSO_TEST_SCK_DELAY2_MS")) {   // TEST HARNESS ONLY:
                long ms2 = atol(dl2);                                     // stream is RUNNING here;
                if (ms2 > 0) { usleep((useconds_t)(ms2 * 1000)); }        // delay -> watchdog wins -> orphan path
            }
            if (startErr) {'''
    assert src.count(old) == 1, "startCapture anchor not found — source has drifted, update this script"
    src = src.replace(old, new)

    # 5. Canary export: deliberate heap-use-after-free, callable only from the
    #    harness driver. Proves the memory-error detector (gmalloc/ASan) is
    #    actually intercepting allocations in THIS process before any clean
    #    race run is trusted — see README.md "canary-first". Never exported
    #    in the real build; this is why the whole harness lives in a scratch
    #    copy instead of behind a runtime flag in the shipped module.
    old = '''    Nan::Set(target, Nan::New("setLifecycleEventCallback").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(SetLifecycleEventCallback)).ToLocalChecked());
}'''
    new = '''    Nan::Set(target, Nan::New("setLifecycleEventCallback").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(SetLifecycleEventCallback)).ToLocalChecked());

    Nan::Set(target, Nan::New("asanCanaryUAF").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(AsanCanaryUAF)).ToLocalChecked());
}'''
    assert src.count(old) == 1, "module-init anchor not found — source has drifted, update this script"
    src = src.replace(old, new)

    old = '// Initialize the native module'
    new = '''// TEST HARNESS ONLY: deliberate use-after-free so the harness can prove the
// memory-error detector actually intercepts allocations in this process
// before any clean race run is trusted.
NAN_METHOD(AsanCanaryUAF) {
    volatile int* q = new int[16];
    delete[] q;
    q[3] = 42;   // heap-use-after-free — the detector must fire here
    info.GetReturnValue().Set(Nan::New<v8::Boolean>(true));
}

// Initialize the native module'''
    assert src.count(old) == 1, "Initialize anchor not found — source has drifted, update this script"
    src = src.replace(old, new, 1)

    return src


def main():
    if len(sys.argv) != 3:
        print(f"usage: {sys.argv[0]} <src.mm> <dest.mm>", file=sys.stderr)
        sys.exit(1)
    src_path, dest_path = sys.argv[1], sys.argv[2]
    with open(src_path) as f:
        src = f.read()
    patched = apply(src)
    with open(dest_path, 'w') as f:
        f.write(patched)
    print(f"[apply-test-hooks] wrote instrumented copy: {dest_path}")


if __name__ == '__main__':
    main()
