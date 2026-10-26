// macOS mic-usage watcher.
//
// 14.2+ ("per-process"): Core Audio exposes one AudioObject per client process
// (kAudioHardwarePropertyProcessObjectList). Each carries its pid, bundle id and
// kAudioProcessPropertyIsRunningInput. We listen to the list and to every
// process object, re-read IsRunningInput, and emit start/stop on transitions.
//
// < 14.2 ("unsupported"): only the device-wide IsRunningSomewhere flag exists,
// and "Hey Siri" (com.apple.CoreSpeech) keeps it on permanently, so a call start
// is invisible for those users. Detection is off there; start() throws.
//
// All Core Audio listeners run on one private serial queue, which also owns
// every piece of state below. Built with ARC (see binding.gyp).
#import <AppKit/AppKit.h>
#import <CoreAudio/CoreAudio.h>
#import <Foundation/Foundation.h>

#include <dlfcn.h>
#include <string.h>
#include <libproc.h>

#include <map>
#include <set>
#include <vector>

#include "mic_activity.h"

namespace {

dispatch_queue_t g_queue = nil;
bool g_started = false;  // g_queue only; late-delivered listener blocks check it

struct ProcState {
    pid_t pid = -1;
    std::string bundleId;
    bool running = false;
    AudioObjectPropertyListenerBlock block = nil;
};
std::map<AudioObjectID, ProcState> g_procs;
AudioObjectPropertyListenerBlock g_listBlock = nil;

AudioObjectPropertyAddress GlobalAddress(AudioObjectPropertySelector selector) {
    return {selector, kAudioObjectPropertyScopeGlobal, kAudioObjectPropertyElementMain};
}

// Per-process listeners use the wildcard: IsRunningInput does not notify on its
// own (macOS 26.5: starting capture only fires kAudioProcessPropertyDevices), so
// we re-read it on any change to the process object.
AudioObjectPropertyAddress AnyPropertyAddress() {
    return {kAudioObjectPropertySelectorWildcard, kAudioObjectPropertyScopeWildcard,
            kAudioObjectPropertyElementWildcard};
}

std::string Utf8(NSString* str) {
    const char* utf8 = str.UTF8String;
    return utf8 ? std::string(utf8) : std::string();
}

template <typename T>
bool ReadScalar(AudioObjectID object, AudioObjectPropertySelector selector, T* out) {
    AudioObjectPropertyAddress address = GlobalAddress(selector);
    UInt32 size = sizeof(T);
    return AudioObjectGetPropertyData(object, &address, 0, nullptr, &size, out) == noErr;
}

std::string ReadString(AudioObjectID object, AudioObjectPropertySelector selector) {
    CFStringRef value = nullptr;
    if (!ReadScalar(object, selector, &value) || !value) return {};
    return Utf8(CFBridgingRelease(value));
}

// Private but long-stable libSystem export. Maps an XPC helper (e.g. Safari's
// com.apple.WebKit.GPU) to the app it works for. Optional: fall back to the pid.
pid_t ResponsiblePid(pid_t pid) {
    using Fn = pid_t (*)(pid_t);
    static Fn fn = reinterpret_cast<Fn>(dlsym(RTLD_DEFAULT, "responsibility_get_pid_responsible_for_pid"));
    if (!fn) return pid;
    pid_t responsible = fn(pid);
    return responsible > 0 ? responsible : pid;
}

// Outermost .app containing the pid's executable. Chrome's helpers live at
// Google Chrome.app/Contents/Frameworks/.../Google Chrome Helper.app, so the
// first ".app/" is the browser itself. Path-based, so it's only the fallback:
// Chrome can run from a code-sign clone ("Google Chrome.app.bundle/...").
std::string OuterAppPath(pid_t pid) {
    char buf[PROC_PIDPATHINFO_MAXSIZE];
    if (pid <= 0 || proc_pidpath(pid, buf, sizeof(buf)) <= 0) return {};
    std::string path(buf);
    size_t pos = path.find(".app/");
    return pos == std::string::npos ? std::string() : path.substr(0, pos + 4);
}

// Fills appPath / appBundleId for the app that owns `pid`. NSRunningApplication
// knows any process registered with LaunchServices regardless of where its
// binary sits; daemons and bare helpers fall back to the path.
void ResolveApp(pid_t pid, MicEvent* event) {
    NSRunningApplication* app = pid > 0 ? [NSRunningApplication runningApplicationWithProcessIdentifier:pid] : nil;
    if (app.bundleIdentifier) {
        event->appBundleId = Utf8(app.bundleIdentifier);
        event->appPath = Utf8(app.bundleURL.path);
        return;
    }
    event->appPath = OuterAppPath(pid);
    if (event->appPath.empty()) return;
    NSBundle* bundle = [NSBundle bundleWithPath:[NSString stringWithUTF8String:event->appPath.c_str()]];
    event->appBundleId = Utf8(bundle.bundleIdentifier);
}

void EmitProcess(bool running, const ProcState& state) {
    MicEvent event;
    event.running = running;
    event.pid = state.pid;
    event.responsiblePid = ResponsiblePid(state.pid);
    event.bundleId = state.bundleId;
    ResolveApp(event.responsiblePid, &event);
    if (event.appBundleId.empty() && event.responsiblePid != state.pid) ResolveApp(state.pid, &event);
    event.ts = MicEventNowMs();
    EmitMicEvent(std::move(event));
}

// ─── per-process (14.2+) ──────────────────────────────────────────────────────

API_AVAILABLE(macos(14.2))
void RefreshProcess(AudioObjectID object) {
    auto it = g_procs.find(object);
    if (!g_started || it == g_procs.end()) return;
    UInt32 running = 0;
    if (!ReadScalar(object, kAudioProcessPropertyIsRunningInput, &running)) return;
    bool now = running != 0;
    if (now == it->second.running) return;
    it->second.running = now;
    EmitProcess(now, it->second);
}

API_AVAILABLE(macos(14.2))
void AddProcess(AudioObjectID object) {
    ProcState state;
    ReadScalar(object, kAudioProcessPropertyPID, &state.pid);
    state.bundleId = ReadString(object, kAudioProcessPropertyBundleID);

    AudioObjectPropertyAddress address = AnyPropertyAddress();
    AudioObjectPropertyListenerBlock block = ^(UInt32, const AudioObjectPropertyAddress*) {
        RefreshProcess(object);
    };
    if (AudioObjectAddPropertyListenerBlock(object, &address, g_queue, block) == noErr) {
        state.block = block;
    }

    UInt32 running = 0;
    ReadScalar(object, kAudioProcessPropertyIsRunningInput, &running);
    state.running = running != 0;
    auto inserted = g_procs.emplace(object, std::move(state)).first;
    if (inserted->second.running) EmitProcess(true, inserted->second);
}

API_AVAILABLE(macos(14.2))
void RemoveProcess(std::map<AudioObjectID, ProcState>::iterator it, bool emitStop) {
    if (it->second.block) {
        AudioObjectPropertyAddress address = AnyPropertyAddress();
        AudioObjectRemovePropertyListenerBlock(it->first, &address, g_queue, it->second.block);
    }
    if (emitStop && it->second.running) EmitProcess(false, it->second);
    g_procs.erase(it);
}

API_AVAILABLE(macos(14.2))
void SyncProcessList() {
    if (!g_started) return;
    AudioObjectPropertyAddress address = GlobalAddress(kAudioHardwarePropertyProcessObjectList);
    UInt32 size = 0;
    if (AudioObjectGetPropertyDataSize(kAudioObjectSystemObject, &address, 0, nullptr, &size) != noErr) return;
    std::vector<AudioObjectID> ids(size / sizeof(AudioObjectID));
    if (!ids.empty() &&
        AudioObjectGetPropertyData(kAudioObjectSystemObject, &address, 0, nullptr, &size, ids.data()) != noErr) {
        return;
    }
    ids.resize(size / sizeof(AudioObjectID));
    std::set<AudioObjectID> current(ids.begin(), ids.end());

    for (auto it = g_procs.begin(); it != g_procs.end();) {
        auto next = std::next(it);
        if (!current.count(it->first)) RemoveProcess(it, true);
        it = next;
    }
    for (AudioObjectID id : current) {
        if (!g_procs.count(id)) AddProcess(id);
    }
}

API_AVAILABLE(macos(14.2))
bool StartPerProcess(std::string* error) {
    AudioObjectPropertyAddress address = GlobalAddress(kAudioHardwarePropertyProcessObjectList);
    g_listBlock = ^(UInt32, const AudioObjectPropertyAddress*) {
        SyncProcessList();
    };
    OSStatus status = AudioObjectAddPropertyListenerBlock(kAudioObjectSystemObject, &address, g_queue, g_listBlock);
    if (status != noErr) {
        g_listBlock = nil;
        *error = "process list listener failed: OSStatus " + std::to_string(status);
        return false;
    }
    SyncProcessList();
    return true;
}

API_AVAILABLE(macos(14.2))
void StopPerProcess() {
    if (g_listBlock) {
        AudioObjectPropertyAddress address = GlobalAddress(kAudioHardwarePropertyProcessObjectList);
        AudioObjectRemovePropertyListenerBlock(kAudioObjectSystemObject, &address, g_queue, g_listBlock);
        g_listBlock = nil;
    }
    while (!g_procs.empty()) RemoveProcess(g_procs.begin(), false);
}

}  // namespace

bool PlatformStart(std::string* error) {
    if (strcmp(PlatformMode(), "per-process") != 0) {
        *error = "call detection requires macOS 14.2";
        return false;
    }
    if (!g_queue) g_queue = dispatch_queue_create("com.asksayso.mic-activity", DISPATCH_QUEUE_SERIAL);
    __block bool ok = false;
    __block std::string err;
    dispatch_sync(g_queue, ^{
        g_started = true;
        if (@available(macOS 14.2, *)) ok = StartPerProcess(&err);
        if (!ok) g_started = false;
    });
    if (!ok) *error = err;
    return ok;
}

void PlatformStop() {
    if (!g_queue) return;
    dispatch_sync(g_queue, ^{
        g_started = false;
        if (@available(macOS 14.2, *)) StopPerProcess();
    });
}

const char* PlatformMode() {
    if (@available(macOS 14.2, *)) return "per-process";
    return "unsupported";
}
