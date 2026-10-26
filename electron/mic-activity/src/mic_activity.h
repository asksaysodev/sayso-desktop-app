// Shared surface between the NAN glue (mic_activity.cc) and the per-OS watchers
// (mic_activity_mac.mm, mic_activity_win.cpp). The watchers run on their own
// threads and never touch V8: they hand events to EmitMicEvent, which queues
// them and wakes the JS thread through uv_async_send.
#pragma once

#include <cstdint>
#include <string>

struct MicEvent {
    bool running = false;        // true = started using the mic, false = stopped
    int32_t pid = 0;
    int32_t responsiblePid = 0;  // owning app's pid (macOS responsibility / Windows same-exe ancestor)
    std::string bundleId;        // macOS: bundle id Core Audio reports for the process
    std::string appPath;         // macOS: outermost .app of the responsible process
    std::string appBundleId;     // macOS: bundle id of appPath
    std::string exePath;         // Windows: full image path of the process
    double ts = 0;               // ms since epoch
};

// Safe from any thread. Dropped when the monitor isn't running.
void EmitMicEvent(MicEvent event);
double MicEventNowMs();

// Implemented per OS. PlatformStart/PlatformStop are called on the JS thread;
// PlatformStop must return only once no further EmitMicEvent calls can happen.
bool PlatformStart(std::string* error);
void PlatformStop();
const char* PlatformMode();
