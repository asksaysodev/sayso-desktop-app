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

// TEST HARNESS ONLY (SAYSO-430): read-only use-after-free. Under MALLOC_ALLOW_READS=1 guard pages
// become readable (read overruns stop faulting) but freed memory stays unmapped; this proves a
// freed-memory READ — the objc_msgSend-on-a-dead-isa shape of SAYSO-APP-82 — still faults.
NAN_METHOD(AsanCanaryUAFRead) {
    volatile int* q = new int[16];
    q[3] = 7;
    delete[] q;
    int v = q[3];   // heap-use-after-free read — the detector must fire here
    info.GetReturnValue().Set(Nan::New<v8::Number>(v));
}

// TEST HARNESS ONLY (SAYSO-361): lets the driver simulate a HAL default-input-device
// notification landing at an arbitrary moment relative to Start/StopMicrophoneCapture calls,
// without needing a real device to plug/unplug. Calls the exact entry point the real
// AudioObjectAddPropertyListener callback uses (SaysoDefaultInputDeviceListenerProc ->
// SaysoScheduleMicRouteDebouncedRestart), so the debounce/generation/forced-restart logic all
// runs for real — only the trigger is synthetic. Fire-and-forget: the real function is
// dispatch_async-based and returns immediately; the restart itself lands on
// SaysoMicRouteRestartQueue after its debounce delay, same as production.
NAN_METHOD(TriggerMicRouteRestart) {
    SaysoScheduleMicRouteDebouncedRestart();
    info.GetReturnValue().Set(Nan::New<v8::Boolean>(true));
}

// Initialize the native module'''
    assert src.count(old) == 1, "Initialize anchor not found — source has drifted, update this script"
    src = src.replace(old, new, 1)

    # 6. SAYSO-361: force the "no_input_node" failure branch on demand, so the harness can
    #    deterministically exercise TryStartMicrophoneCaptureOnce's early-release path (one of
    #    the 3 leak sites this ticket fixes) instead of relying on it happening to fail for real.
    old = '''    g_micEngine = [[AVAudioEngine alloc] init];
    g_micInputNode = [g_micEngine inputNode];

    if (!g_micInputNode) {'''
    new = '''    g_micEngine = [[AVAudioEngine alloc] init];
    g_micInputNode = [g_micEngine inputNode];

    if (getenv("SAYSO_TEST_MIC_FORCE_NO_INPUT_NODE")) {   // TEST HARNESS ONLY
        g_micInputNode = nullptr;
    }

    if (!g_micInputNode) {'''
    assert src.count(old) == 1, "mic input-node anchor not found — source has drifted, update this script"
    src = src.replace(old, new)

    # 7. SAYSO-361/353: emit lifecycle events around the mic route-restart's immediate-attempt
    #    phase so the harness can count real outcomes instead of only "process didn't crash".
    #    Test-harness-only — distinct from the mic_route_recovery_* events SAYSO-353 added to the
    #    real committed source for the backoff phase; these track the 2-try phase that precedes it.
    old = '''    NSLog(@"🔊 [NATIVE] Mic route: restarting engine for default input id=%u (previous opened id=%u)%s",
          (unsigned)currentDefault, (unsigned)g_micOpenedInputDeviceId,
          leavingBluetooth ? " [leaving Bluetooth — widened first attempt]" : "");

    MicEngineTeardownOnly();'''
    new = '''    NSLog(@"🔊 [NATIVE] Mic route: restarting engine for default input id=%u (previous opened id=%u)%s",
          (unsigned)currentDefault, (unsigned)g_micOpenedInputDeviceId,
          leavingBluetooth ? " [leaving Bluetooth — widened first attempt]" : "");

    EmitLifecycleEvent("mic_route_restart_attempt");   // TEST HARNESS ONLY
    MicEngineTeardownOnly();'''
    assert src.count(old) == 1, "mic restart-attempt anchor not found — source has drifted, update this script"
    src = src.replace(old, new)

    # Minimal, single-line-pair anchors rather than spanning the surrounding comment blocks —
    # those keep growing as the ticket evolves and a wider anchor drifts every time (happened
    # twice already). Anchoring on just the two NSLog calls is stable regardless.
    old = '''        NSLog(@"✅ [NATIVE] Mic route restart succeeded; now following default input id=%u",
              (unsigned)currentDefault);
    } else {'''
    new = '''        NSLog(@"✅ [NATIVE] Mic route restart succeeded; now following default input id=%u",
              (unsigned)currentDefault);
        EmitLifecycleEvent("mic_route_restart_ok");   // TEST HARNESS ONLY
    } else {'''
    assert src.count(old) == 1, "mic restart-ok anchor not found — source has drifted, update this script"
    src = src.replace(old, new)

    old = '''        NSLog(@"⚠️ [NATIVE] Mic route: immediate attempts failed for default input id=%u — entering backoff "
              @"recovery", (unsigned)currentDefault);'''
    new = '''        NSLog(@"⚠️ [NATIVE] Mic route: immediate attempts failed for default input id=%u — entering backoff "
              @"recovery", (unsigned)currentDefault);
        EmitLifecycleEvent("mic_route_restart_failed");   // TEST HARNESS ONLY'''
    assert src.count(old) == 1, "mic restart-failed anchor not found — source has drifted, update this script"
    src = src.replace(old, new)

    # 8. SAYSO-361: register the TriggerMicRouteRestart export (defined in step 5's insertion,
    #    alongside asanCanaryUAF). Anchored on the asanCanaryUAF registration line so this only
    #    needs to run after step 5 has already inserted it — see apply() call order below.
    old = '''    Nan::Set(target, Nan::New("asanCanaryUAF").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(AsanCanaryUAF)).ToLocalChecked());
}'''
    new = '''    Nan::Set(target, Nan::New("asanCanaryUAF").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(AsanCanaryUAF)).ToLocalChecked());
    Nan::Set(target, Nan::New("asanCanaryUAFRead").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(AsanCanaryUAFRead)).ToLocalChecked());

    Nan::Set(target, Nan::New("triggerMicRouteRestart").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(TriggerMicRouteRestart)).ToLocalChecked());
}'''
    assert src.count(old) == 1, "asanCanaryUAF export anchor not found — source has drifted, update this script"
    src = src.replace(old, new)

    # 9. SAYSO-353: env-tunable recovery ceiling (default stays ~30s; test can shrink it so the
    #    harness can reach the terminal-failure branch — and prove "a future device change still
    #    triggers a fresh attempt" per this ticket's AC — in well under a second instead of 30s
    #    per cycle). Same getenv-at-use-site pattern as step 1's SCK watchdog override.
    old = '''    int64_t elapsedMs = SaysoNowMs() - g_micRouteRecoveryStartMs;
    if (elapsedMs >= kMicRouteRecoveryCeilingMs) {'''
    new = '''    int64_t elapsedMs = SaysoNowMs() - g_micRouteRecoveryStartMs;
    int64_t ceilingMs = kMicRouteRecoveryCeilingMs;
    if (const char* c = getenv("SAYSO_TEST_MIC_RECOVERY_CEILING_MS")) {   // TEST HARNESS ONLY
        ceilingMs = (int64_t)atoll(c);
    }
    if (elapsedMs >= ceilingMs) {'''
    assert src.count(old) == 1, "recovery-ceiling anchor not found — source has drifted, update this script"
    src = src.replace(old, new)

    # 10. SAYSO-430: REAL HAL churn. triggerMicRouteRestart (step 5) only simulates *our* restart —
    #     it never makes the system fire AVAudioIOUnit's own HAL property listener, which is why
    #     SAYSO-361 passed 400 cycles and SAYSO-APP-82 still crashed in the field. These exports
    #     create a private aggregate device around the built-in mic (no hardware, no installs,
    #     invisible to other processes, gone when this process exits) and flip its nominal sample
    #     rate 44.1k <-> 48k on a timer or in bursts. The aggregate is made the system default
    #     input for the run, so every engine TryStartMicrophoneCaptureOnce builds opens it exactly the
    #     way production opens whatever the default is, and the system delivers real
    #     IOUnitPropertyListener callbacks to our engines — live, stopped, and released ones.
    #     Not done instead: binding each engine's input unit to the aggregate after reading
    #     inputNode. AVAudioEngine has already cached the default device's format by then, so the
    #     nil-format tap mismatches and ~90% of starts fail with -10868 (FormatNotSupported) — two
    #     200-cycle runs that way only exercised engines that never ran.
    #     The aggregates are NOT private: a private aggregate can't become the system default — the
    #     set returns noErr and is silently ignored (2026-10-26 probe), which is how the 10-23 "default
    #     input" run ended up exercising the built-in mic instead. Side effects for the run: two
    #     "Sayso HAL churn" devices are visible system-wide and other apps follow the default-device
    #     toggles. Non-private aggregates outlive a crashed process, so run.sh snapshots the defaults
    #     first and calls halChurnCleanup from a fresh process afterwards.

    old = '''NAN_METHOD(TriggerMicRouteRestart) {
    SaysoScheduleMicRouteDebouncedRestart();
    info.GetReturnValue().Set(Nan::New<v8::Boolean>(true));
}'''
    new = old + '''

// TEST HARNESS ONLY (SAYSO-430) — see apply-test-hooks.py step 10.
static const char* kTestChurnAggUid = "com.sayso.test.halchurn";
static const char* kTestChurnOutAggUid = "com.sayso.test.halchurn.out";
static const char* kTestBuiltInMicUid = "BuiltInMicrophoneDevice";
static AudioObjectID g_testChurnAggDevice = kAudioObjectUnknown;
static AudioObjectID g_testChurnPrevDefaultInput = kAudioObjectUnknown;
static const AudioObjectPropertyAddress kTestDefaultInputAddr = {
    kAudioHardwarePropertyDefaultInputDevice, kAudioObjectPropertyScopeGlobal, kAudioObjectPropertyElementMain};
static dispatch_queue_t g_testChurnQueue = nullptr;
static dispatch_source_t g_testChurnTimer = nullptr;
static id g_testChurnCfgObserver = nil;
static std::atomic<uint64_t> g_testChurnFlips{0};
static std::atomic<uint64_t> g_testChurnFlipErrors{0};
static std::atomic<uint64_t> g_testChurnHalFires{0};
static std::atomic<uint64_t> g_testChurnCfgPosts{0};
static const AudioObjectPropertyAddress kTestNominalRateAddr = {
    kAudioDevicePropertyNominalSampleRate, kAudioObjectPropertyScopeGlobal, kAudioObjectPropertyElementMain};
static const AudioObjectPropertyAddress kTestSubDeviceListAddr = {
    kAudioAggregateDevicePropertyFullSubDeviceList, kAudioObjectPropertyScopeGlobal, kAudioObjectPropertyElementMain};
// Lever: what one churn step changes on the aggregate.
//   "rate"       — nominal sample rate 44.1k <-> 48k. Lands on an idle device, but once an engine is
//                  doing IO the sets stop taking effect (2026-10-23 runs), so it never reaches a
//                  running engine's listener.
//   "subdevices" — sub-device list [built-in] <-> [built-in + a second input]. Lands on a running
//                  engine (every step fires the HAL), but AVAudioEngine posts no config change for it.
//   "default-input"  — system default input aggregate <-> built-in mic (same physical mic). An
//                  AirPods connect/disconnect, minus the hardware.
//   "default-output" — system default output: the original device <-> a private aggregate wrapping
//                  it. A display with speakers being plugged, minus the hardware. Audible glitches
//                  in whatever is playing during the run.
//   "mixed" — default-output, default-input and subdevices in turn, each as an apply/revert pair:
//                  a display or AirPods plug changes several of these at once.
// AVAudioIOUnit::IOUnitPropertyListener is an AudioUnit property listener on the engine's IO unit,
// which follows the system default devices — the two "default-*" levers target it directly.
enum TestChurnLever { kTestLeverRate, kTestLeverSubdevices, kTestLeverDefaultInput, kTestLeverDefaultOutput,
                      kTestLeverMixed };
static TestChurnLever g_testChurnLever = kTestLeverMixed;
static AudioObjectID g_testChurnBuiltInMic = kAudioObjectUnknown;
static AudioObjectID g_testChurnOrigOutput = kAudioObjectUnknown;
static AudioObjectID g_testChurnOutAgg = kAudioObjectUnknown;
static const AudioObjectPropertyAddress kTestDefaultOutputAddr = {
    kAudioHardwarePropertyDefaultOutputDevice, kAudioObjectPropertyScopeGlobal, kAudioObjectPropertyElementMain};
static NSString* g_testChurnSecondUid = nil;   // retained; the input added/removed by "subdevices"

static OSStatus TestChurnHalListener(AudioObjectID, UInt32, const AudioObjectPropertyAddress*, void*) {
    g_testChurnHalFires.fetch_add(1);
    return noErr;
}

static AudioObjectID TestDeviceForUid(const char* uid) {
    CFStringRef cfUid = CFStringCreateWithCString(kCFAllocatorDefault, uid, kCFStringEncodingUTF8);
    AudioObjectID dev = kAudioObjectUnknown;
    AudioValueTranslation t = {&cfUid, sizeof(cfUid), &dev, sizeof(dev)};
    AudioObjectPropertyAddress a = {kAudioHardwarePropertyDeviceForUID, kAudioObjectPropertyScopeGlobal,
                                    kAudioObjectPropertyElementMain};
    UInt32 sz = sizeof(t);
    AudioObjectGetPropertyData(kAudioObjectSystemObject, &a, 0, nullptr, &sz, &t);
    CFRelease(cfUid);
    return dev;
}

static Float64 TestGetNominalRate(AudioObjectID dev) {
    Float64 r = 0;
    UInt32 sz = sizeof(r);
    AudioObjectGetPropertyData(dev, &kTestNominalRateAddr, 0, nullptr, &sz, &r);
    return r;
}

static OSStatus TestSetNominalRate(AudioObjectID dev, Float64 r) {
    return AudioObjectSetPropertyData(dev, &kTestNominalRateAddr, 0, nullptr, sizeof(r), &r);
}

// A second input-capable device for the "subdevices" lever: not the built-in mic, not an aggregate,
// not Bluetooth or Continuity (those come and go on their own). Virtual devices preferred — they're
// always present and never touch real hardware. Returns a +1 NSString or nil.
static NSString* TestFindSecondInputUid() {
    AudioObjectPropertyAddress a = {kAudioHardwarePropertyDevices, kAudioObjectPropertyScopeGlobal,
                                    kAudioObjectPropertyElementMain};
    UInt32 sz = 0;
    if (AudioObjectGetPropertyDataSize(kAudioObjectSystemObject, &a, 0, nullptr, &sz) != noErr) return nil;
    std::vector<AudioObjectID> ids(sz / sizeof(AudioObjectID));
    if (AudioObjectGetPropertyData(kAudioObjectSystemObject, &a, 0, nullptr, &sz, ids.data()) != noErr) return nil;
    NSString* fallback = nil;
    for (AudioObjectID dev : ids) {
        AudioObjectPropertyAddress streams = {kAudioDevicePropertyStreams, kAudioObjectPropertyScopeInput,
                                              kAudioObjectPropertyElementMain};
        UInt32 ssz = 0;
        if (AudioObjectGetPropertyDataSize(dev, &streams, 0, nullptr, &ssz) != noErr || ssz == 0) continue;
        UInt32 transport = 0;
        UInt32 tsz = sizeof(transport);
        AudioObjectPropertyAddress ta = {kAudioDevicePropertyTransportType, kAudioObjectPropertyScopeGlobal,
                                         kAudioObjectPropertyElementMain};
        AudioObjectGetPropertyData(dev, &ta, 0, nullptr, &tsz, &transport);
        if (transport == kAudioDeviceTransportTypeAggregate || transport == kAudioDeviceTransportTypeBluetooth ||
            transport == kAudioDeviceTransportTypeBluetoothLE ||
            transport == kAudioDeviceTransportTypeContinuityCaptureWired ||
            transport == kAudioDeviceTransportTypeContinuityCaptureWireless) {
            continue;
        }
        CFStringRef uid = nullptr;
        UInt32 usz = sizeof(uid);
        AudioObjectPropertyAddress ua = {kAudioDevicePropertyDeviceUID, kAudioObjectPropertyScopeGlobal,
                                         kAudioObjectPropertyElementMain};
        if (AudioObjectGetPropertyData(dev, &ua, 0, nullptr, &usz, &uid) != noErr || !uid) continue;
        NSString* s = (NSString*)uid;   // +1 from the HAL
        if ([s isEqualToString:@(kTestBuiltInMicUid)]) { [s release]; continue; }
        if (transport == kAudioDeviceTransportTypeVirtual) { [fallback release]; return s; }
        if (!fallback) fallback = s; else [s release];
    }
    return fallback;
}

static OSStatus TestSetSubDevices(AudioObjectID agg, bool withSecond) {
    @autoreleasepool {
        NSArray* uids = withSecond ? @[ @(kTestBuiltInMicUid), g_testChurnSecondUid ] : @[ @(kTestBuiltInMicUid) ];
        CFArrayRef arr = (CFArrayRef)uids;
        return AudioObjectSetPropertyData(agg, &kTestSubDeviceListAddr, 0, nullptr, sizeof(arr), &arr);
    }
}

static OSStatus TestSetSystemDevice(const AudioObjectPropertyAddress* addr, AudioObjectID dev) {
    return AudioObjectSetPropertyData(kAudioObjectSystemObject, addr, 0, nullptr, sizeof(dev), &dev);
}

static NSString* TestCopyDeviceUid(AudioObjectID dev) {   // +1 or nil
    CFStringRef uid = nullptr;
    UInt32 usz = sizeof(uid);
    AudioObjectPropertyAddress ua = {kAudioDevicePropertyDeviceUID, kAudioObjectPropertyScopeGlobal,
                                     kAudioObjectPropertyElementMain};
    if (AudioObjectGetPropertyData(dev, &ua, 0, nullptr, &usz, &uid) != noErr) return nil;
    return (NSString*)uid;
}

// One churn step with the configured lever. Runs on g_testChurnQueue.
static void TestChurnStep(AudioObjectID agg) {
    uint64_t k = g_testChurnFlips.fetch_add(1);
    OSStatus st = noErr;
    TestChurnLever lever = g_testChurnLever;
    if (lever == kTestLeverMixed) {
        // Steps 2n and 2n+1 are an apply/revert pair on the same lever, so each lever's k&1 parity
        // below still alternates between its two states.
        static const TestChurnLever kMix[] = {kTestLeverDefaultOutput, kTestLeverDefaultInput, kTestLeverSubdevices};
        lever = kMix[(k / 2) % 3];
    }
    switch (lever) {
        case kTestLeverRate:
            st = TestSetNominalRate(agg, (k & 1) ? 48000.0 : 44100.0);
            break;
        case kTestLeverSubdevices:
            st = TestSetSubDevices(agg, (k & 1) == 0);
            break;
        case kTestLeverDefaultInput:
            st = TestSetSystemDevice(&kTestDefaultInputAddr, (k & 1) ? agg : g_testChurnBuiltInMic);
            break;
        case kTestLeverDefaultOutput:
            st = TestSetSystemDevice(&kTestDefaultOutputAddr, (k & 1) ? g_testChurnOrigOutput : g_testChurnOutAgg);
            break;
        case kTestLeverMixed:
            break;
    }
    if (st != noErr) {
        g_testChurnFlipErrors.fetch_add(1);
    }
}

static double TestQuarantinedCount();   // defined by step 10c

static v8::Local<v8::Object> TestChurnStats() {
    v8::Local<v8::Object> o = Nan::New<v8::Object>();
    Nan::Set(o, Nan::New("flips").ToLocalChecked(), Nan::New<v8::Number>((double)g_testChurnFlips.load()));
    Nan::Set(o, Nan::New("flipErrors").ToLocalChecked(), Nan::New<v8::Number>((double)g_testChurnFlipErrors.load()));
    Nan::Set(o, Nan::New("halFires").ToLocalChecked(), Nan::New<v8::Number>((double)g_testChurnHalFires.load()));
    // Every AVAudioEngineConfigurationChangeNotification from ANY engine (object:nil) — posted from
    // inside AVAudioIOUnit's own property listener, so this is the direct evidence that the listener
    // under suspicion actually ran, not just that the HAL changed.
    Nan::Set(o, Nan::New("cfgPosts").ToLocalChecked(), Nan::New<v8::Number>((double)g_testChurnCfgPosts.load()));
    Nan::Set(o, Nan::New("engineBuilds").ToLocalChecked(), Nan::New<v8::Number>((double)g_micEngineBuilds.load()));
    Nan::Set(o, Nan::New("quarantined").ToLocalChecked(), Nan::New<v8::Number>(TestQuarantinedCount()));
    return o;
}

// halChurnStart(periodMs, lever = "mixed" | "default-output" | "default-input" | "subdevices" | "rate")
//   -> { ok, aggId, builtInRate, lever, secondInput } | { ok:false, reason }
NAN_METHOD(HalChurnStart) {
    @autoreleasepool {
        int periodMs = info.Length() > 0 && info[0]->IsNumber() ? Nan::To<int>(info[0]).FromJust() : 150;
        std::string lever = "mixed";
        if (info.Length() > 1 && info[1]->IsString()) {
            lever = *Nan::Utf8String(info[1]);
        }
        v8::Local<v8::Object> res = Nan::New<v8::Object>();
        if (lever != "subdevices" && lever != "rate" && lever != "default-input" && lever != "default-output" &&
            lever != "mixed") {
            Nan::Set(res, Nan::New("ok").ToLocalChecked(), Nan::False());
            Nan::Set(res, Nan::New("reason").ToLocalChecked(), Nan::New("unknown_lever").ToLocalChecked());
            info.GetReturnValue().Set(res);
            return;
        }
        g_testChurnLever = lever == "rate"             ? kTestLeverRate
                           : lever == "subdevices"     ? kTestLeverSubdevices
                           : lever == "default-input"  ? kTestLeverDefaultInput
                           : lever == "default-output" ? kTestLeverDefaultOutput
                                                       : kTestLeverMixed;
        const bool needsSecondInput = g_testChurnLever == kTestLeverSubdevices || g_testChurnLever == kTestLeverMixed;
        const bool needsOutputAgg = g_testChurnLever == kTestLeverDefaultOutput || g_testChurnLever == kTestLeverMixed;
        if (needsSecondInput && !g_testChurnSecondUid) {
            g_testChurnSecondUid = TestFindSecondInputUid();
            if (!g_testChurnSecondUid) {
                Nan::Set(res, Nan::New("ok").ToLocalChecked(), Nan::False());
                Nan::Set(res, Nan::New("reason").ToLocalChecked(), Nan::New("no_second_input_device").ToLocalChecked());
                info.GetReturnValue().Set(res);
                return;
            }
        }
        if (g_testChurnAggDevice != kAudioObjectUnknown) {
            Nan::Set(res, Nan::New("ok").ToLocalChecked(), Nan::False());
            Nan::Set(res, Nan::New("reason").ToLocalChecked(), Nan::New("already_running").ToLocalChecked());
            info.GetReturnValue().Set(res);
            return;
        }
        if (TestDeviceForUid(kTestChurnAggUid) != kAudioObjectUnknown ||
            TestDeviceForUid(kTestChurnOutAggUid) != kAudioObjectUnknown) {
            Nan::Set(res, Nan::New("ok").ToLocalChecked(), Nan::False());
            Nan::Set(res, Nan::New("reason").ToLocalChecked(),
                     Nan::New("leftover_aggregate_run_halChurnCleanup").ToLocalChecked());
            info.GetReturnValue().Set(res);
            return;
        }
        AudioObjectID builtIn = TestDeviceForUid(kTestBuiltInMicUid);
        g_testChurnBuiltInMic = builtIn;
        NSDictionary* desc = @{
            @kAudioAggregateDeviceUIDKey : @(kTestChurnAggUid),
            @kAudioAggregateDeviceNameKey : @"Sayso HAL churn (race harness)",
            @kAudioAggregateDeviceIsPrivateKey : @0,
            @kAudioAggregateDeviceMainSubDeviceKey : @(kTestBuiltInMicUid),
            @kAudioAggregateDeviceSubDeviceListKey : @[ @{@kAudioSubDeviceUIDKey : @(kTestBuiltInMicUid)} ],
        };
        AudioObjectID agg = kAudioObjectUnknown;
        OSStatus st = builtIn == kAudioObjectUnknown
                          ? (OSStatus)kAudioHardwareBadDeviceError
                          : AudioHardwareCreateAggregateDevice((CFDictionaryRef)desc, &agg);
        if (st != noErr || agg == kAudioObjectUnknown) {
            Nan::Set(res, Nan::New("ok").ToLocalChecked(), Nan::False());
            Nan::Set(res, Nan::New("reason").ToLocalChecked(),
                     Nan::New(("create_aggregate_failed st=" + std::to_string((int)st)).c_str()).ToLocalChecked());
            info.GetReturnValue().Set(res);
            return;
        }
        usleep(300 * 1000);   // aggregate needs a beat before it accepts property sets
        AudioObjectAddPropertyListener(agg, &kTestNominalRateAddr, TestChurnHalListener, nullptr);
        AudioObjectAddPropertyListener(agg, &kTestSubDeviceListAddr, TestChurnHalListener, nullptr);
        g_testChurnCfgObserver = [[[NSNotificationCenter defaultCenter]
            addObserverForName:AVAudioEngineConfigurationChangeNotification
                        object:nil
                         queue:nil
                    usingBlock:^(NSNotification* _Nonnull) { g_testChurnCfgPosts.fetch_add(1); }] retain];
        g_testChurnAggDevice = agg;
        UInt32 dsz = sizeof(g_testChurnPrevDefaultInput);
        AudioObjectGetPropertyData(kAudioObjectSystemObject, &kTestDefaultInputAddr, 0, nullptr, &dsz,
                                   &g_testChurnPrevDefaultInput);
        OSStatus defSt = AudioObjectSetPropertyData(kAudioObjectSystemObject, &kTestDefaultInputAddr, 0, nullptr,
                                                    sizeof(agg), &agg);
        if (defSt != noErr) {
            NSLog(@"[TEST] halChurn: making aggregate %u the default input failed: %d", (unsigned)agg, (int)defSt);
        }

        AudioObjectAddPropertyListener(kAudioObjectSystemObject, &kTestDefaultInputAddr, TestChurnHalListener, nullptr);
        AudioObjectAddPropertyListener(kAudioObjectSystemObject, &kTestDefaultOutputAddr, TestChurnHalListener, nullptr);

        if (needsOutputAgg) {
            UInt32 osz = sizeof(g_testChurnOrigOutput);
            AudioObjectGetPropertyData(kAudioObjectSystemObject, &kTestDefaultOutputAddr, 0, nullptr, &osz,
                                       &g_testChurnOrigOutput);
            NSString* outUid = g_testChurnOrigOutput != kAudioObjectUnknown ? TestCopyDeviceUid(g_testChurnOrigOutput)
                                                                            : nil;
            OSStatus ost = (OSStatus)kAudioHardwareBadDeviceError;
            if (outUid) {
                NSDictionary* outDesc = @{
                    @kAudioAggregateDeviceUIDKey : @(kTestChurnOutAggUid),
                    @kAudioAggregateDeviceNameKey : @"Sayso HAL churn output (race harness)",
                    @kAudioAggregateDeviceIsPrivateKey : @0,
                    @kAudioAggregateDeviceMainSubDeviceKey : outUid,
                    @kAudioAggregateDeviceSubDeviceListKey : @[ @{@kAudioSubDeviceUIDKey : outUid} ],
                };
                ost = AudioHardwareCreateAggregateDevice((CFDictionaryRef)outDesc, &g_testChurnOutAgg);
                [outUid release];
            }
            if (ost != noErr || g_testChurnOutAgg == kAudioObjectUnknown) {
                NSLog(@"[TEST] halChurn: output aggregate create failed: %d", (int)ost);
                g_testChurnOutAgg = kAudioObjectUnknown;
                // A null output device would make the mixed lever's output steps fail silently.
                g_testChurnLever = kTestLeverDefaultInput;   // reported back via "lever" below
                lever = "default-input";
            } else {
                usleep(300 * 1000);
            }
        }

        if (!g_testChurnQueue) {
            g_testChurnQueue = dispatch_queue_create("com.sayso.test.halChurn", DISPATCH_QUEUE_SERIAL);
        }
        if (periodMs > 0) {
            g_testChurnTimer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, g_testChurnQueue);
            dispatch_source_set_timer(g_testChurnTimer, dispatch_time(DISPATCH_TIME_NOW, periodMs * NSEC_PER_MSEC),
                                      periodMs * NSEC_PER_MSEC, 5 * NSEC_PER_MSEC);
            dispatch_source_set_event_handler(g_testChurnTimer, ^{ TestChurnStep(agg); });
            dispatch_resume(g_testChurnTimer);
        }

        Nan::Set(res, Nan::New("ok").ToLocalChecked(), Nan::True());
        Nan::Set(res, Nan::New("aggId").ToLocalChecked(), Nan::New<v8::Number>(agg));
        Nan::Set(res, Nan::New("builtInRate").ToLocalChecked(), Nan::New<v8::Number>(TestGetNominalRate(builtIn)));
        Nan::Set(res, Nan::New("lever").ToLocalChecked(), Nan::New(lever.c_str()).ToLocalChecked());
        if (g_testChurnSecondUid) {
            Nan::Set(res, Nan::New("secondInput").ToLocalChecked(),
                     Nan::New([g_testChurnSecondUid UTF8String]).ToLocalChecked());
        }
        info.GetReturnValue().Set(res);
    }
}

// halChurnBurst(n, gapMs): fire n churn steps gapMs apart on the churn queue, returning immediately —
// so the caller can tear the engine down while the resulting listener callbacks are still queued.
NAN_METHOD(HalChurnBurst) {
    int n = info.Length() > 0 && info[0]->IsNumber() ? Nan::To<int>(info[0]).FromJust() : 4;
    int gapMs = info.Length() > 1 && info[1]->IsNumber() ? Nan::To<int>(info[1]).FromJust() : 5;
    AudioObjectID agg = g_testChurnAggDevice;
    if (agg == kAudioObjectUnknown) {
        info.GetReturnValue().Set(Nan::False());
        return;
    }
    dispatch_async(g_testChurnQueue, ^{
        for (int i = 0; i < n; i++) {
            TestChurnStep(agg);
            if (gapMs > 0) usleep((useconds_t)gapMs * 1000);
        }
    });
    info.GetReturnValue().Set(Nan::True());
}

// pumpRunLoop(): drain the main thread's CFRunLoop once. HAL property notifications (and the
// AudioUnit property listeners AVAudioEngine builds on them) are delivered on the main run loop,
// which Electron pumps and plain node never does — without this, system-level device changes reach
// no listener at all in the harness (2026-10-26: 87 default-device toggles, 0 HAL fires). The
// driver calls it every few ms from the JS thread, which is node's main thread.
NAN_METHOD(PumpRunLoop) {
    @autoreleasepool {
        int handled = 0;
        while (handled < 64 && CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0, true) == kCFRunLoopRunHandledSource) {
            handled++;
        }
        info.GetReturnValue().Set(Nan::New<v8::Number>(handled));
    }
}

NAN_METHOD(HalChurnStats) {
    info.GetReturnValue().Set(TestChurnStats());
}

// halChurnStop(restoreBuiltInRate) -> final stats. Caller must stopMicrophoneCapture first.
NAN_METHOD(HalChurnStop) {
    @autoreleasepool {
        Float64 restore = info.Length() > 0 && info[0]->IsNumber() ? Nan::To<double>(info[0]).FromJust() : 48000.0;
        if (g_testChurnTimer) {
            dispatch_source_cancel(g_testChurnTimer);
            dispatch_sync(g_testChurnQueue, ^{});   // drain an in-flight flip
            dispatch_release(g_testChurnTimer);
            g_testChurnTimer = nullptr;
        }
        AudioObjectID agg = g_testChurnAggDevice;
        g_testChurnAggDevice = kAudioObjectUnknown;
        AudioObjectRemovePropertyListener(kAudioObjectSystemObject, &kTestDefaultInputAddr, TestChurnHalListener, nullptr);
        AudioObjectRemovePropertyListener(kAudioObjectSystemObject, &kTestDefaultOutputAddr, TestChurnHalListener, nullptr);
        if (g_testChurnOrigOutput != kAudioObjectUnknown) {
            TestSetSystemDevice(&kTestDefaultOutputAddr, g_testChurnOrigOutput);
            g_testChurnOrigOutput = kAudioObjectUnknown;
        }
        if (g_testChurnOutAgg != kAudioObjectUnknown) {
            AudioHardwareDestroyAggregateDevice(g_testChurnOutAgg);
            g_testChurnOutAgg = kAudioObjectUnknown;
        }
        if (g_testChurnPrevDefaultInput != kAudioObjectUnknown) {
            AudioObjectID prev = g_testChurnPrevDefaultInput;
            AudioObjectSetPropertyData(kAudioObjectSystemObject, &kTestDefaultInputAddr, 0, nullptr, sizeof(prev), &prev);
            g_testChurnPrevDefaultInput = kAudioObjectUnknown;
        }
        if (agg != kAudioObjectUnknown) {
            AudioObjectRemovePropertyListener(agg, &kTestNominalRateAddr, TestChurnHalListener, nullptr);
            AudioObjectRemovePropertyListener(agg, &kTestSubDeviceListAddr, TestChurnHalListener, nullptr);
            AudioHardwareDestroyAggregateDevice(agg);
        }
        if (g_testChurnCfgObserver) {
            [[NSNotificationCenter defaultCenter] removeObserver:g_testChurnCfgObserver];
            [g_testChurnCfgObserver release];
            g_testChurnCfgObserver = nil;
        }
        AudioObjectID builtIn = TestDeviceForUid(kTestBuiltInMicUid);
        if (builtIn != kAudioObjectUnknown && TestGetNominalRate(builtIn) != restore) {
            TestSetNominalRate(builtIn, restore);
        }
        info.GetReturnValue().Set(TestChurnStats());
    }
}

// halChurnSnapshotDefaults() -> { input, output } device UIDs. run.sh records these before a run.
NAN_METHOD(HalChurnSnapshotDefaults) {
    @autoreleasepool {
        v8::Local<v8::Object> o = Nan::New<v8::Object>();
        AudioObjectID in = kAudioObjectUnknown, out = kAudioObjectUnknown;
        UInt32 sz = sizeof(in);
        AudioObjectGetPropertyData(kAudioObjectSystemObject, &kTestDefaultInputAddr, 0, nullptr, &sz, &in);
        sz = sizeof(out);
        AudioObjectGetPropertyData(kAudioObjectSystemObject, &kTestDefaultOutputAddr, 0, nullptr, &sz, &out);
        NSString* inUid = TestCopyDeviceUid(in);
        NSString* outUid = TestCopyDeviceUid(out);
        Nan::Set(o, Nan::New("input").ToLocalChecked(), Nan::New(inUid ? [inUid UTF8String] : "").ToLocalChecked());
        Nan::Set(o, Nan::New("output").ToLocalChecked(), Nan::New(outUid ? [outUid UTF8String] : "").ToLocalChecked());
        [inUid release];
        [outUid release];
        info.GetReturnValue().Set(o);
    }
}

// halChurnCleanup(inputUid, outputUid, builtInRate) — from a FRESH process after a run, crashed or
// not: destroy any leftover test aggregates (non-private ones outlive their process), put the
// default devices back by UID, and restore the built-in mic's nominal rate. Returns what it did.
NAN_METHOD(HalChurnCleanup) {
    @autoreleasepool {
        std::string inUid = info.Length() > 0 && info[0]->IsString() ? *Nan::Utf8String(info[0]) : "";
        std::string outUid = info.Length() > 1 && info[1]->IsString() ? *Nan::Utf8String(info[1]) : "";
        Float64 rate = info.Length() > 2 && info[2]->IsNumber() ? Nan::To<double>(info[2]).FromJust() : 48000.0;
        int destroyed = 0;
        for (const char* uid : {kTestChurnAggUid, kTestChurnOutAggUid}) {
            AudioObjectID dev = TestDeviceForUid(uid);
            if (dev != kAudioObjectUnknown && AudioHardwareDestroyAggregateDevice(dev) == noErr) destroyed++;
        }
        if (destroyed) usleep(300 * 1000);   // let the HAL pick new defaults before we override them
        AudioObjectID in = inUid.empty() ? kAudioObjectUnknown : TestDeviceForUid(inUid.c_str());
        AudioObjectID out = outUid.empty() ? kAudioObjectUnknown : TestDeviceForUid(outUid.c_str());
        if (in != kAudioObjectUnknown) TestSetSystemDevice(&kTestDefaultInputAddr, in);
        if (out != kAudioObjectUnknown) TestSetSystemDevice(&kTestDefaultOutputAddr, out);
        AudioObjectID builtIn = TestDeviceForUid(kTestBuiltInMicUid);
        if (builtIn != kAudioObjectUnknown && TestGetNominalRate(builtIn) != rate) {
            TestSetNominalRate(builtIn, rate);
        }
        v8::Local<v8::Object> o = Nan::New<v8::Object>();
        Nan::Set(o, Nan::New("destroyed").ToLocalChecked(), Nan::New<v8::Number>(destroyed));
        Nan::Set(o, Nan::New("inputRestored").ToLocalChecked(), Nan::New<v8::Boolean>(in != kAudioObjectUnknown));
        Nan::Set(o, Nan::New("outputRestored").ToLocalChecked(), Nan::New<v8::Boolean>(out != kAudioObjectUnknown));
        info.GetReturnValue().Set(o);
    }
}'''
    assert src.count(old) == 1, "TriggerMicRouteRestart body not found — step 5 changed, update this script"
    # 10c. Engines currently parked by SaysoQuarantineMicEngine (SAYSO-430 fix). Optional, so the
    #      same hooks still apply to pre-fix source for the before/after comparison; -1 = no quarantine.
    src = src.replace(old, new)
    quarantine_decl = 'static std::deque<SaysoParkedMicEngine>& g_quarantinedMicEngines = *new std::deque<SaysoParkedMicEngine>();'
    # 10d. SAYSO_TEST_MIC_QUARANTINE_MS overrides the quarantine delay (fixed source only), so the
    #      same run can compare 5s against ~0 or effectively-never (the pre-SAYSO-361 leak).
    quarantine_ms = 'kMicEngineQuarantineMs * NSEC_PER_MSEC'
    if src.count(quarantine_ms) == 1:
        src = src.replace(quarantine_ms,
                          '(getenv("SAYSO_TEST_MIC_QUARANTINE_MS")   /* TEST HARNESS ONLY */\n'
                          '                        ? (int64_t)atoll(getenv("SAYSO_TEST_MIC_QUARANTINE_MS"))\n'
                          '                        : kMicEngineQuarantineMs) * NSEC_PER_MSEC')
    if src.count(quarantine_decl) == 1:
        src = src.replace(quarantine_decl, quarantine_decl + """

// TEST HARNESS ONLY (SAYSO-430 step 10c)
static double TestQuarantinedCount() {
    __block size_t n = 0;
    dispatch_sync(SaysoMicRouteRestartQueue(), ^{ n = g_quarantinedMicEngines.size(); });
    return (double)n;
}""")
    else:
        old = 'NAN_MODULE_INIT(Init) {'
        assert src.count(old) == 1, "NAN_MODULE_INIT anchor not found — source has drifted, update this script"
        src = src.replace(old, "static double TestQuarantinedCount() { return -1; }   // TEST HARNESS ONLY: pre-fix source\n\n" + old)

    old = '''    Nan::Set(target, Nan::New("triggerMicRouteRestart").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(TriggerMicRouteRestart)).ToLocalChecked());
}'''
    new = '''    Nan::Set(target, Nan::New("triggerMicRouteRestart").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(TriggerMicRouteRestart)).ToLocalChecked());

    Nan::Set(target, Nan::New("halChurnStart").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(HalChurnStart)).ToLocalChecked());
    Nan::Set(target, Nan::New("halChurnBurst").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(HalChurnBurst)).ToLocalChecked());
    Nan::Set(target, Nan::New("pumpRunLoop").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(PumpRunLoop)).ToLocalChecked());
    Nan::Set(target, Nan::New("halChurnStats").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(HalChurnStats)).ToLocalChecked());
    Nan::Set(target, Nan::New("halChurnStop").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(HalChurnStop)).ToLocalChecked());
    Nan::Set(target, Nan::New("halChurnSnapshotDefaults").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(HalChurnSnapshotDefaults)).ToLocalChecked());
    Nan::Set(target, Nan::New("halChurnCleanup").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(HalChurnCleanup)).ToLocalChecked());
}'''
    assert src.count(old) == 1, "triggerMicRouteRestart export anchor not found — step 8 changed, update this script"
    src = src.replace(old, new)

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
