// Windows mic-usage watcher (WASAPI audio sessions).
//
// Every active capture endpoint has an IAudioSessionManager2 listing one audio
// session per client stream, each with a process id and an active/inactive
// state. We watch all sessions on all capture endpoints, aggregate them per
// pid, and emit start/stop when a pid goes from no active session to one (and
// back).
//
// Threading: one dedicated MTA worker thread owns the COM objects and does all
// registration/teardown. WASAPI delivers callbacks on its own threads; those
// only update counts under g_stateMutex (state changes) or queue work for the
// worker (new sessions, device changes), since registering from inside a
// WASAPI callback is not allowed.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <audiopolicy.h>
#include <mmdeviceapi.h>
#include <tlhelp32.h>

#include <atomic>
#include <future>
#include <map>
#include <mutex>
#include <set>
#include <string>
#include <thread>
#include <vector>

#include "mic_activity.h"

namespace {

std::string WideToUtf8(const std::wstring& wide) {
    if (wide.empty()) return {};
    int len = WideCharToMultiByte(CP_UTF8, 0, wide.c_str(), static_cast<int>(wide.size()), nullptr, 0, nullptr, nullptr);
    std::string out(len, '\0');
    WideCharToMultiByte(CP_UTF8, 0, wide.c_str(), static_cast<int>(wide.size()), out.data(), len, nullptr, nullptr);
    return out;
}

std::wstring ExePath(DWORD pid) {
    HANDLE process = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
    if (!process) return {};
    wchar_t buf[MAX_PATH * 2];
    DWORD size = static_cast<DWORD>(sizeof(buf) / sizeof(buf[0]));
    std::wstring path;
    if (QueryFullProcessImageNameW(process, 0, buf, &size)) path.assign(buf, size);
    CloseHandle(process);
    return path;
}

// Browsers capture from a utility child process with the same image as the
// browser. Walk up while the parent runs the same exe; the top one is the app.
DWORD ResponsiblePid(DWORD pid, const std::wstring& exe) {
    if (exe.empty()) return pid;
    std::map<DWORD, DWORD> parents;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) return pid;
    PROCESSENTRY32W entry{};
    entry.dwSize = sizeof(entry);
    if (Process32FirstW(snapshot, &entry)) {
        do {
            parents[entry.th32ProcessID] = entry.th32ParentProcessID;
        } while (Process32NextW(snapshot, &entry));
    }
    CloseHandle(snapshot);

    DWORD current = pid;
    for (int depth = 0; depth < 16; ++depth) {
        auto it = parents.find(current);
        if (it == parents.end() || it->second == 0 || it->second == current) break;
        if (_wcsicmp(ExePath(it->second).c_str(), exe.c_str()) != 0) break;
        current = it->second;
    }
    return current;
}

// Minimal IUnknown for our callback objects.
template <typename Interface>
class ComCallback : public Interface {
public:
    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&refs_); }
    ULONG STDMETHODCALLTYPE Release() override {
        ULONG refs = InterlockedDecrement(&refs_);
        if (refs == 0) delete this;
        return refs;
    }
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** out) override {
        if (riid == __uuidof(IUnknown) || riid == __uuidof(Interface)) {
            *out = static_cast<Interface*>(this);
            AddRef();
            return S_OK;
        }
        *out = nullptr;
        return E_NOINTERFACE;
    }

protected:
    virtual ~ComCallback() = default;

private:
    LONG refs_ = 1;
};

class SessionWatcher;
void OnSessionActivity(SessionWatcher* watcher, bool active);
void QueueNewSession(IAudioSessionControl* control);
void RequestRebuild();

class SessionWatcher : public ComCallback<IAudioSessionEvents> {
public:
    SessionWatcher(IAudioSessionControl2* control, DWORD pid) : control(control), pid(pid) { control->AddRef(); }

    HRESULT STDMETHODCALLTYPE OnDisplayNameChanged(LPCWSTR, LPCGUID) override { return S_OK; }
    HRESULT STDMETHODCALLTYPE OnIconPathChanged(LPCWSTR, LPCGUID) override { return S_OK; }
    HRESULT STDMETHODCALLTYPE OnSimpleVolumeChanged(float, BOOL, LPCGUID) override { return S_OK; }
    HRESULT STDMETHODCALLTYPE OnChannelVolumeChanged(DWORD, float[], DWORD, LPCGUID) override { return S_OK; }
    HRESULT STDMETHODCALLTYPE OnGroupingParamChanged(LPCGUID, LPCGUID) override { return S_OK; }
    HRESULT STDMETHODCALLTYPE OnStateChanged(AudioSessionState state) override {
        OnSessionActivity(this, state == AudioSessionStateActive);
        return S_OK;
    }
    HRESULT STDMETHODCALLTYPE OnSessionDisconnected(AudioSessionDisconnectReason) override {
        OnSessionActivity(this, false);
        return S_OK;
    }

    IAudioSessionControl2* control;
    const DWORD pid;
    bool active = false;  // guarded by g_stateMutex

protected:
    ~SessionWatcher() override { control->Release(); }
};

class SessionNotifier : public ComCallback<IAudioSessionNotification> {
public:
    HRESULT STDMETHODCALLTYPE OnSessionCreated(IAudioSessionControl* control) override {
        QueueNewSession(control);
        return S_OK;
    }
};

class EndpointNotifier : public ComCallback<IMMNotificationClient> {
public:
    HRESULT STDMETHODCALLTYPE OnDeviceStateChanged(LPCWSTR, DWORD) override { RequestRebuild(); return S_OK; }
    HRESULT STDMETHODCALLTYPE OnDeviceAdded(LPCWSTR) override { RequestRebuild(); return S_OK; }
    HRESULT STDMETHODCALLTYPE OnDeviceRemoved(LPCWSTR) override { RequestRebuild(); return S_OK; }
    HRESULT STDMETHODCALLTYPE OnDefaultDeviceChanged(EDataFlow, ERole, LPCWSTR) override { return S_OK; }
    HRESULT STDMETHODCALLTYPE OnPropertyValueChanged(LPCWSTR, const PROPERTYKEY) override { return S_OK; }
};

struct ManagerEntry {
    IAudioSessionManager2* manager;
    SessionNotifier* notifier;
};

// Worker-thread-owned COM state.
IMMDeviceEnumerator* g_enumerator = nullptr;
EndpointNotifier* g_endpointNotifier = nullptr;
std::vector<ManagerEntry> g_managers;
std::vector<SessionWatcher*> g_watchers;
std::set<std::wstring> g_sessionIds;

// Cross-thread state.
std::mutex g_stateMutex;
struct Reported {
    std::string exePath;
    DWORD responsiblePid;
};
std::map<DWORD, Reported> g_reported;  // pids we've emitted "start" for
std::vector<IAudioSessionControl*> g_newSessions;
bool g_rebuildRequested = false;

std::thread g_worker;
HANDLE g_wake = nullptr;
std::atomic<bool> g_stopRequested{false};

// Diff the pids with an active session against what we've reported. Caller
// holds g_stateMutex.
void ReconcileLocked() {
    std::set<DWORD> active;
    for (SessionWatcher* watcher : g_watchers) {
        if (watcher->active) active.insert(watcher->pid);
    }
    for (auto it = g_reported.begin(); it != g_reported.end();) {
        if (active.count(it->first)) {
            ++it;
            continue;
        }
        MicEvent event;
        event.running = false;
        event.pid = static_cast<int32_t>(it->first);
        event.responsiblePid = static_cast<int32_t>(it->second.responsiblePid);
        event.exePath = it->second.exePath;
        event.ts = MicEventNowMs();
        EmitMicEvent(std::move(event));
        it = g_reported.erase(it);
    }
    for (DWORD pid : active) {
        if (g_reported.count(pid)) continue;
        std::wstring exe = ExePath(pid);
        Reported reported{WideToUtf8(exe), ResponsiblePid(pid, exe)};
        MicEvent event;
        event.running = true;
        event.pid = static_cast<int32_t>(pid);
        event.responsiblePid = static_cast<int32_t>(reported.responsiblePid);
        event.exePath = reported.exePath;
        event.ts = MicEventNowMs();
        EmitMicEvent(std::move(event));
        g_reported.emplace(pid, std::move(reported));
    }
}

void OnSessionActivity(SessionWatcher* watcher, bool active) {
    std::lock_guard<std::mutex> lk(g_stateMutex);
    if (watcher->active == active) return;
    watcher->active = active;
    ReconcileLocked();
}

void QueueNewSession(IAudioSessionControl* control) {
    control->AddRef();
    {
        std::lock_guard<std::mutex> lk(g_stateMutex);
        g_newSessions.push_back(control);
    }
    if (g_wake) SetEvent(g_wake);
}

void RequestRebuild() {
    {
        std::lock_guard<std::mutex> lk(g_stateMutex);
        g_rebuildRequested = true;
    }
    if (g_wake) SetEvent(g_wake);
}

// ─── worker thread ────────────────────────────────────────────────────────────

// Rebuild passes reconcile=false and diffs once at the end, so re-adding
// sessions one by one doesn't emit stops for pids not re-added yet.
void AddSession(IAudioSessionControl* control, bool reconcile) {
    IAudioSessionControl2* control2 = nullptr;
    if (FAILED(control->QueryInterface(__uuidof(IAudioSessionControl2), reinterpret_cast<void**>(&control2)))) return;

    // RegisterSessionNotification and the enumerator can both report a session
    // created in between; the instance id dedupes them.
    LPWSTR instanceId = nullptr;
    std::wstring id;
    if (SUCCEEDED(control2->GetSessionInstanceIdentifier(&instanceId)) && instanceId) {
        id = instanceId;
        CoTaskMemFree(instanceId);
    }
    DWORD pid = 0;
    if (control2->IsSystemSoundsSession() == S_OK || FAILED(control2->GetProcessId(&pid)) || pid == 0 ||
        (!id.empty() && g_sessionIds.count(id))) {
        control2->Release();
        return;
    }

    auto* watcher = new SessionWatcher(control2, pid);
    control2->Release();
    if (FAILED(watcher->control->RegisterAudioSessionNotification(watcher))) {
        watcher->Release();
        return;
    }
    if (!id.empty()) g_sessionIds.insert(id);

    AudioSessionState state = AudioSessionStateInactive;
    watcher->control->GetState(&state);
    std::lock_guard<std::mutex> lk(g_stateMutex);
    g_watchers.push_back(watcher);
    watcher->active = state == AudioSessionStateActive;
    if (reconcile) ReconcileLocked();
}

void TearDownSessions() {
    std::vector<SessionWatcher*> watchers;
    {
        std::lock_guard<std::mutex> lk(g_stateMutex);
        watchers.swap(g_watchers);
    }
    for (SessionWatcher* watcher : watchers) {
        watcher->control->UnregisterAudioSessionNotification(watcher);
        watcher->Release();
    }
    for (ManagerEntry& entry : g_managers) {
        entry.manager->UnregisterSessionNotification(entry.notifier);
        entry.notifier->Release();
        entry.manager->Release();
    }
    g_managers.clear();
    g_sessionIds.clear();
}

// Re-enumerate every active capture endpoint. Reported pids are diffed rather
// than reset, so a device change mid-call doesn't emit a stop/start pair.
void Rebuild() {
    TearDownSessions();

    IMMDeviceCollection* devices = nullptr;
    if (SUCCEEDED(g_enumerator->EnumAudioEndpoints(eCapture, DEVICE_STATE_ACTIVE, &devices))) {
        UINT count = 0;
        devices->GetCount(&count);
        for (UINT i = 0; i < count; ++i) {
            IMMDevice* device = nullptr;
            if (FAILED(devices->Item(i, &device))) continue;
            IAudioSessionManager2* manager = nullptr;
            HRESULT hr = device->Activate(__uuidof(IAudioSessionManager2), CLSCTX_ALL, nullptr,
                                          reinterpret_cast<void**>(&manager));
            device->Release();
            if (FAILED(hr)) continue;

            auto* notifier = new SessionNotifier();
            if (FAILED(manager->RegisterSessionNotification(notifier))) {
                notifier->Release();
                manager->Release();
                continue;
            }
            g_managers.push_back({manager, notifier});

            // Enumerating is what arms RegisterSessionNotification.
            IAudioSessionEnumerator* sessions = nullptr;
            if (SUCCEEDED(manager->GetSessionEnumerator(&sessions))) {
                int sessionCount = 0;
                sessions->GetCount(&sessionCount);
                for (int s = 0; s < sessionCount; ++s) {
                    IAudioSessionControl* control = nullptr;
                    if (SUCCEEDED(sessions->GetSession(s, &control))) {
                        AddSession(control, false);
                        control->Release();
                    }
                }
                sessions->Release();
            }
        }
        devices->Release();
    }

    std::lock_guard<std::mutex> lk(g_stateMutex);
    ReconcileLocked();
}

void WorkerMain(std::promise<std::string> started) {
    HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (FAILED(hr)) {
        started.set_value("CoInitializeEx failed");
        return;
    }
    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),
                          reinterpret_cast<void**>(&g_enumerator));
    if (FAILED(hr)) {
        CoUninitialize();
        started.set_value("IMMDeviceEnumerator unavailable");
        return;
    }
    g_endpointNotifier = new EndpointNotifier();
    g_enumerator->RegisterEndpointNotificationCallback(g_endpointNotifier);
    Rebuild();
    started.set_value("");

    while (!g_stopRequested) {
        WaitForSingleObject(g_wake, INFINITE);
        if (g_stopRequested) break;

        std::vector<IAudioSessionControl*> fresh;
        bool rebuild;
        {
            std::lock_guard<std::mutex> lk(g_stateMutex);
            fresh.swap(g_newSessions);
            rebuild = g_rebuildRequested;
            g_rebuildRequested = false;
        }
        if (rebuild) Rebuild();
        for (IAudioSessionControl* control : fresh) {
            if (!rebuild) AddSession(control, true);  // a rebuild already enumerated it
            control->Release();
        }
    }

    g_enumerator->UnregisterEndpointNotificationCallback(g_endpointNotifier);
    g_endpointNotifier->Release();
    g_endpointNotifier = nullptr;
    TearDownSessions();
    {
        std::lock_guard<std::mutex> lk(g_stateMutex);
        for (IAudioSessionControl* control : g_newSessions) control->Release();
        g_newSessions.clear();
        g_reported.clear();
        g_rebuildRequested = false;
    }
    g_enumerator->Release();
    g_enumerator = nullptr;
    CoUninitialize();
}

}  // namespace

bool PlatformStart(std::string* error) {
    g_stopRequested = false;
    g_wake = CreateEventW(nullptr, FALSE, FALSE, nullptr);
    if (!g_wake) {
        *error = "CreateEvent failed";
        return false;
    }
    std::promise<std::string> started;
    std::future<std::string> result = started.get_future();
    g_worker = std::thread(WorkerMain, std::move(started));
    std::string err = result.get();
    if (!err.empty()) {
        g_worker.join();
        CloseHandle(g_wake);
        g_wake = nullptr;
        *error = err;
        return false;
    }
    return true;
}

void PlatformStop() {
    if (!g_worker.joinable()) return;
    g_stopRequested = true;
    SetEvent(g_wake);
    g_worker.join();
    CloseHandle(g_wake);
    g_wake = nullptr;
}

const char* PlatformMode() {
    return "per-process";
}
