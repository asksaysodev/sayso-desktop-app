// ============================================================================
// native_audio — Windows backend (WASAPI)
//
// Implements the frozen 13-method NAN surface documented in
// docs/NATIVE_AUDIO_CONTRACT.md, mirroring the responsibilities of the macOS
// reference implementation (src/audio_device_manager.mm) with the Windows/
// WASAPI equivalents documented in docs/AUDIO_MODULE_WINDOWS_ASSESSMENT.md and
// docs/WINDOWS_AUDIO_PLAN.md. index.js and binding.gyp are frozen inputs —
// nothing here changes what JS calls or how the addon is built.
//
// STAGE (this file is built up in compiling checkpoints):
//   1. Native state, COM/device infrastructure, all 13 NAN exports, callback
//      plumbing, synchronization primitives, async delivery infrastructure.
//      [THIS STAGE — system-audio and mic *capture* bodies are still
//      placeholders; everything else below is real.]
//   2. Microphone capture (open/close the default capture endpoint).
//   3. System-audio loopback capture (open/close the default render endpoint).
//   4. Device-change recovery (debounce/coalesce/backoff on both paths).
//
// ----------------------------------------------------------------------------
// Design decision — streaming delivery: bounded FIFO, not a single-slot
// mailbox (see docs/WINDOWS_AUDIO_PLAN.md §7/§18).
//
// The macOS reference attaches one StreamingData* directly to uv_async_t::data
// as a single-slot mailbox: if the audio thread enqueues a second buffer
// before the JS thread drains the first, the earlier pointer is silently
// overwritten (leaked, and its audio lost). This file instead gives each
// streaming path (system audio, microphone) a mutex-guarded, capacity-bounded
// std::deque<StreamingData*> (AudioDeliveryChannel below). uv_async_send is
// used purely as a wake signal; the JS-thread drain empties the whole deque in
// FIFO order per wakeup, invoking callback(buffer, format) once per queued
// chunk — the JS-facing signature and per-chunk semantics are unchanged.
// Capacity is fixed (64 entries per path, ~640ms of headroom at a typical
// ~10ms WASAPI period) so this cannot grow unboundedly; on overflow the
// *oldest* entry is dropped (bounds latency) and a
// "<path>_queue_overflow dropped=N" lifecycle event is emitted, so a drop is
// observable instead of silent. This is implemented entirely in this file —
// no JS/public API change.
//
// ----------------------------------------------------------------------------
// Thread model (see docs/WINDOWS_AUDIO_PLAN.md §9/§19 — refined here now that
// WASAPI's actual API shape is in front of us):
//
//   1. Node/JS (libuv) thread — not created by us. Runs every NAN method body,
//      all Buffer/format construction, all Nan::Callback invocations. Never
//      touches WASAPI or blocks on device I/O beyond the bounded waits in
//      Initialize()/Stop*Capture() below.
//
//   2. COM-infrastructure thread (g_comInfraThread) — ONE, created once in
//      Initialize(), lives for the module's process lifetime. NECESSARY
//      because a Windows multithreaded COM apartment (MTA) tears down once its
//      last member thread exits; capture threads and (stage 4) timer
//      callbacks only join the MTA transiently, so something must hold it
//      open continuously or IMMDeviceEnumerator/IMMNotificationClient
//      registration would go stale between capture sessions. Owns:
//      g_enumerator, g_notificationClient (created on this thread once, then
//      safe to use from any other MTA-joined thread — MMDevice API objects
//      are documented as agile/free-threaded within a process's MTA). Exits
//      only if g_comInfraShutdown is signaled (nothing in the current 13-
//      method surface does this — mirrors the mac reference never tearing
//      down its HAL listener either). Apartment: MTA (COINIT_MULTITHREADED).
//
//   3. Loopback capture thread — transient, one per active system-audio
//      session (stage 3). Created by StartSystemAudioCapture's worker,
//      destroyed by StopSystemAudioCapture: signaled via a stop event, joined
//      by the thread that calls Stop *before* any IAudioClient/
//      IAudioCaptureClient/IMMDevice it used is released. Apartment: MTA.
//
//   4. Mic capture thread — transient, one per active mic session (stage 2).
//      Same shape and join-before-release discipline as #3, fully
//      independent lifecycle.
//
//   5. Thread-pool timer callbacks (stage 4) — ephemeral, OS-managed threads
//      used for the debounce/stability/backoff delays on both paths. Each
//      callback joins the MTA (CoInitializeEx) at entry and leaves
//      (CoUninitialize) at exit; safe because #2 already holds the MTA open,
//      so this never risks a teardown gap. A per-path std::mutex
//      (PathState::engineLock) — not "run only on one thread" — is what
//      actually serializes engine mutation, replacing the mac reference's
//      dispatch_queue exclusivity.
//
//   6. IMMNotificationClient callback thread — owned by COM, not us; the
//      documented context is an unspecified, possibly-reentrant internal
//      thread. Callback bodies here only read the changed device id and emit
//      a lifecycle event (stage 1) / hand off to the per-path debounce chain
//      (stage 4) — never touch a WASAPI object directly (frozen requirement).
//
// No thread here exists merely to mirror macOS's dispatch queues 1:1 — #2 is
// a genuine Windows-COM necessity with no mac analog, and mac's two
// dispatch-queue "restart worker" queues are replaced by per-path mutexes plus
// OS thread-pool timers rather than two more dedicated always-alive threads.
// ============================================================================

#include <nan.h>
#include <node.h>
#include <v8.h>
#include <uv.h>

// initguid.h must precede mmdeviceapi.h: PKEY_AudioEndpoint_FormFactor is
// defined (via DEFINE_PROPERTYKEY) directly inside mmdeviceapi.h itself, and
// that macro only emits real storage (vs. an extern declaration with no
// storage anywhere to link against) when INITGUID is already defined at the
// point mmdeviceapi.h is first processed.
#include <initguid.h>
#include <windows.h>
#include <mmdeviceapi.h>
#include <audioclient.h>
#include <ksmedia.h>
#include <threadpoolapiset.h>
#include <propsys.h>
#include <functiondiscoverykeys_devpkey.h>

#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <cstdio>
#include <cstring>
#include <deque>
#include <functional>
#include <mutex>
#include <string>
#include <vector>

using namespace v8;

namespace {

// ── Lifecycle diagnostics channel (SAYSO-355 shape; unconditionally logged,
//    and forwarded to JS once a callback has been registered at least once).
//    The uv_async_t is allocated once on first registration and intentionally
//    NEVER closed — freeing it on callback-clear is the exact UAF class
//    SAYSO-349 documents. There is no equivalent "Stop" for this channel; it
//    lives for the module's lifetime, matching the mac reference exactly. ──

std::mutex g_lifecycleMutex;
std::vector<std::string> g_lifecycleQueue;   // guarded by g_lifecycleMutex
uv_async_t* g_lifecycleAsyncHandle = nullptr; // allocated once, never freed
Nan::Persistent<v8::Function> g_lifecycleCallback;

void LifecycleAsyncCallback(uv_async_t* /*handle*/) {
  std::vector<std::string> events;
  {
    std::lock_guard<std::mutex> lk(g_lifecycleMutex);
    events.swap(g_lifecycleQueue);
  }
  if (g_lifecycleCallback.IsEmpty()) {
    return;
  }
  Nan::HandleScope scope;
  Local<Function> callback = Nan::New(g_lifecycleCallback);
  for (const auto& event : events) {
    Local<Value> argv[1] = {Nan::New<v8::String>(event.c_str()).ToLocalChecked()};
    Nan::Call(callback, Nan::GetCurrentContext()->Global(), 1, argv);
  }
}

// Safe from any thread, at any time (including before the channel is wired).
void EmitLifecycleEvent(const std::string& message) {
  OutputDebugStringA(("[native_audio] " + message + "\n").c_str());
  std::lock_guard<std::mutex> lk(g_lifecycleMutex);
  if (!g_lifecycleAsyncHandle) {
    return;  // channel not wired yet (or older JS) — the debug log above is the only sink
  }
  g_lifecycleQueue.push_back(message);
  uv_async_send(g_lifecycleAsyncHandle);
}

// ── Streaming payload + bounded per-path delivery channel (see file header
//    "Design decision — streaming delivery" above). ──

struct StreamingData {
  std::vector<char> audioData;
  double sampleRate = 0;
  int channels = 0;
  int bitDepth = 0;
  bool isFloat = false;
};

struct AudioDeliveryChannel {
  const char* name;
  const size_t capacity;
  std::mutex mutex;
  std::deque<StreamingData*> queue;             // guarded by mutex
  std::atomic<uint64_t> dropped{0};
  uv_async_t* asyncHandle = nullptr;            // JS thread only: create/close
  Nan::Persistent<v8::Function> callback;       // JS thread only: set/clear

  AudioDeliveryChannel(const char* n, size_t cap) : name(n), capacity(cap) {}

  // Producer side. Called from a WASAPI capture thread. Never touches V8.
  void Push(StreamingData* data) {
    bool overflowed = false;
    {
      std::lock_guard<std::mutex> lk(mutex);
      if (queue.size() >= capacity) {
        delete queue.front();
        queue.pop_front();
        overflowed = true;
      }
      queue.push_back(data);
    }
    if (overflowed) {
      dropped.fetch_add(1, std::memory_order_relaxed);
    }
    if (asyncHandle) {
      uv_async_send(asyncHandle);
    }
  }

  // Consumer side. Runs on the JS thread only (the uv_async_t callback).
  void Drain() {
    std::deque<StreamingData*> local;
    {
      std::lock_guard<std::mutex> lk(mutex);
      local.swap(queue);
    }
    uint64_t droppedNow = dropped.exchange(0, std::memory_order_relaxed);
    if (droppedNow > 0) {
      EmitLifecycleEvent(std::string(name) + "_queue_overflow dropped=" + std::to_string(droppedNow));
    }
    if (local.empty()) {
      return;
    }
    Nan::HandleScope scope;
    bool hasCallback = !callback.IsEmpty();
    Local<Function> fn;
    if (hasCallback) {
      fn = Nan::New(callback);
    }
    for (StreamingData* data : local) {
      if (hasCallback) {
        Local<Object> buffer =
            Nan::CopyBuffer(data->audioData.data(), static_cast<uint32_t>(data->audioData.size())).ToLocalChecked();
        Local<Object> format = Nan::New<Object>();
        Nan::Set(format, Nan::New("sampleRate").ToLocalChecked(), Nan::New<Number>(data->sampleRate));
        Nan::Set(format, Nan::New("channels").ToLocalChecked(), Nan::New<Integer>(data->channels));
        Nan::Set(format, Nan::New("bitDepth").ToLocalChecked(), Nan::New<Integer>(data->bitDepth));
        Nan::Set(format, Nan::New("isFloat").ToLocalChecked(), Nan::New<v8::Boolean>(data->isFloat));
        Local<Value> argv[2] = {buffer, format};
        Nan::Call(fn, Nan::GetCurrentContext()->Global(), 2, argv);
      }
      delete data;
    }
  }

  // JS thread only. Lazily creates the async handle the first time a real
  // (non-null) callback is registered.
  void EnsureAsyncHandle(uv_async_cb cb) {
    if (!asyncHandle) {
      asyncHandle = new uv_async_t();
      asyncHandle->data = this;
      uv_async_init(uv_default_loop(), asyncHandle, cb);
    }
  }

  // JS thread only. Called from Stop*Capture teardown ONLY, once the producer
  // (capture) thread is provably joined/gone — never from a callback-clear
  // path. This is a deliberate hardening over the mac reference: mac's
  // SetStreamingCallback/SetMicrophoneStreamingCallback close the async
  // handle immediately on clear(null), which is a narrow TOCTOU race against
  // an audio thread that already passed its "handle is non-null" check and is
  // about to call uv_async_send. Here, clearing a callback (see
  // SetStreamingCallback/SetMicrophoneStreamingCallback below) only resets
  // the persistent JS function; the handle itself is torn down exclusively
  // here, at a point where the producer thread is already gone, closing that
  // race for both streaming channels (in addition to the lifecycle channel,
  // which never closes its handle at all).
  void CloseAsyncHandle() {
    if (asyncHandle) {
      uv_async_t* h = asyncHandle;
      asyncHandle = nullptr;
      uv_close(reinterpret_cast<uv_handle_t*>(h), [](uv_handle_t* handle) {
        delete reinterpret_cast<uv_async_t*>(handle);
      });
    }
    std::lock_guard<std::mutex> lk(mutex);
    for (StreamingData* data : queue) {
      delete data;
    }
    queue.clear();
  }
};

void ChannelDrainCallback(uv_async_t* handle) {
  static_cast<AudioDeliveryChannel*>(handle->data)->Drain();
}

AudioDeliveryChannel g_systemChannel("system_audio", 64);
AudioDeliveryChannel g_micChannel("microphone", 64);

// ── COM / MMDevice infrastructure ──────────────────────────────────────────

std::string WideToUtf8(LPCWSTR wide) {
  if (!wide) {
    return "(null)";
  }
  int len = WideCharToMultiByte(CP_UTF8, 0, wide, -1, nullptr, 0, nullptr, nullptr);
  if (len <= 0) {
    return "(?)";
  }
  std::string out(static_cast<size_t>(len - 1), '\0');
  WideCharToMultiByte(CP_UTF8, 0, wide, -1, out.data(), len, nullptr, nullptr);
  return out;
}

// Forward declarations: defined in the device-change recovery section below
// (after PathState exists), called from the notification callbacks here.
// Each does only an atomic check + an atomic generation bump + scheduling a
// thread-pool timer — no WASAPI/COM call happens on the calling thread,
// which may be the IMMNotificationClient callback thread itself (frozen
// requirement #8: these callbacks must never touch WASAPI directly).
void ScheduleMicRouteDebouncedRestart();
void ScheduleLoopbackRouteDebouncedRestart();
// Returns false if CreateThreadpoolTimer failed and `work` was never
// scheduled (a real, if rare, failure mode — see the definition below and
// each call site for how the caller handles it; never assume scheduling
// succeeded without checking).
bool ScheduleOnce(int64_t delayMs, std::function<void()> work);
// Checks `id` against both paths' currently-opened endpoint (PathState isn't
// visible yet at this point in the file, hence a plain free function rather
// than inlining the comparison here) and triggers the matching path's
// debounce chain if it matches. Defined alongside PathState below.
void HandleDeviceStateChangedToGone(const std::wstring& id);

// Minimal IMMNotificationClient. Callback bodies stay non-blocking and never
// touch a WASAPI object directly (frozen requirement #8): they only read the
// notified device id/state and hand off to the relevant path's debounce
// chain. The calling thread is COM-owned with an unspecified,
// possibly-reentrant context (see file header thread model #6).
class SaysoNotificationClient : public IMMNotificationClient {
 public:
  SaysoNotificationClient() : ref_(1) {}

  ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&ref_); }

  ULONG STDMETHODCALLTYPE Release() override {
    ULONG ref = InterlockedDecrement(&ref_);
    if (ref == 0) {
      delete this;
    }
    return ref;
  }

  HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv) override {
    if (!ppv) {
      return E_POINTER;
    }
    if (riid == __uuidof(IUnknown) || riid == __uuidof(IMMNotificationClient)) {
      *ppv = static_cast<IMMNotificationClient*>(this);
      AddRef();
      return S_OK;
    }
    *ppv = nullptr;
    return E_NOINTERFACE;
  }

  HRESULT STDMETHODCALLTYPE OnDefaultDeviceChanged(EDataFlow flow, ERole role,
                                                    LPCWSTR pwstrDefaultDeviceId) override {
    if (role != eConsole) {
      return S_OK;  // only the console role feeds GetDefaultAudioEndpoint(..., eConsole) reads elsewhere
    }
    if (flow == eCapture) {
      EmitLifecycleEvent("default_capture_device_changed id=" + WideToUtf8(pwstrDefaultDeviceId));
      ScheduleMicRouteDebouncedRestart();
    } else if (flow == eRender) {
      EmitLifecycleEvent("default_render_device_changed id=" + WideToUtf8(pwstrDefaultDeviceId));
      ScheduleLoopbackRouteDebouncedRestart();
    }
    return S_OK;
  }

  HRESULT STDMETHODCALLTYPE OnDeviceStateChanged(LPCWSTR pwstrDeviceId, DWORD dwNewState) override {
    if (dwNewState != DEVICE_STATE_NOTPRESENT && dwNewState != DEVICE_STATE_UNPLUGGED) {
      return S_OK;
    }
    EmitLifecycleEvent("device_state_changed id=" + WideToUtf8(pwstrDeviceId) + " state=gone");
    if (pwstrDeviceId) {
      HandleDeviceStateChangedToGone(pwstrDeviceId);
    }
    return S_OK;
  }

  HRESULT STDMETHODCALLTYPE OnDeviceAdded(LPCWSTR) override { return S_OK; }
  HRESULT STDMETHODCALLTYPE OnDeviceRemoved(LPCWSTR) override { return S_OK; }
  HRESULT STDMETHODCALLTYPE OnPropertyValueChanged(LPCWSTR, const PROPERTYKEY) override { return S_OK; }

 private:
  LONG ref_;
};

std::atomic<bool> g_initialized{false};
IMMDeviceEnumerator* g_enumerator = nullptr;         // set once by g_comInfraThread; read-only afterward
SaysoNotificationClient* g_notificationClient = nullptr;
HANDLE g_comInfraThread = nullptr;
HANDLE g_comInfraReady = nullptr;     // signaled once setup succeeds or fails
HANDLE g_comInfraShutdown = nullptr; // never signaled today — see file header
HRESULT g_comInfraResult = S_OK;

DWORD WINAPI ComInfraThreadProc(LPVOID /*param*/) {
  HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
  if (FAILED(hr)) {
    g_comInfraResult = hr;
    SetEvent(g_comInfraReady);
    return 0;
  }

  IMMDeviceEnumerator* enumerator = nullptr;
  hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),
                        reinterpret_cast<void**>(&enumerator));
  if (FAILED(hr)) {
    g_comInfraResult = hr;
    CoUninitialize();
    SetEvent(g_comInfraReady);
    return 0;
  }

  SaysoNotificationClient* notifier = new SaysoNotificationClient();
  hr = enumerator->RegisterEndpointNotificationCallback(notifier);
  if (FAILED(hr)) {
    notifier->Release();
    enumerator->Release();
    g_comInfraResult = hr;
    CoUninitialize();
    SetEvent(g_comInfraReady);
    return 0;
  }

  g_enumerator = enumerator;
  g_notificationClient = notifier;
  g_comInfraResult = S_OK;
  SetEvent(g_comInfraReady);

  // Hold the process's MTA open for as long as the module is loaded — see the
  // file header's thread-model note #2 on why this thread must not exit.
  WaitForSingleObject(g_comInfraShutdown, INFINITE);

  if (g_notificationClient) {
    if (g_enumerator) {
      g_enumerator->UnregisterEndpointNotificationCallback(g_notificationClient);
    }
    g_notificationClient->Release();
    g_notificationClient = nullptr;
  }
  if (g_enumerator) {
    g_enumerator->Release();
    g_enumerator = nullptr;
  }
  CoUninitialize();
  return 0;
}

// ── Per-path native capture state ──────────────────────────────────────────
// One instance each for the microphone (default capture endpoint) and system-
// audio (default render endpoint, loopback) paths. Deliberately shared shape:
// the two paths are structurally symmetric and fully independent — stopping
// one must never affect the other (frozen requirement #2), which is exactly
// what having two separate instances (not shared state) guarantees.
//
// engineLock serializes every open/close/restart of device/audioClient/
// captureClient for its path — this is the mutex that replaces the mac
// reference's dispatch_queue exclusivity (see file header thread model #5).
// It must be held whenever those four members are read or written outside of
// the capture thread's own tight GetBuffer/ReleaseBuffer poll loop.
struct PathState {
  std::atomic<bool> capturing{false};
  std::atomic<bool> routeRecovering{false};
  std::atomic<uint64_t> routeRestartGeneration{0};

  std::mutex engineLock;
  int64_t routeRecoveryStartMs = 0;         // guarded by engineLock
  std::wstring openedEndpointId;            // guarded by engineLock
  IMMDevice* device = nullptr;              // guarded by engineLock
  IAudioClient* audioClient = nullptr;      // guarded by engineLock
  IAudioCaptureClient* captureClient = nullptr;  // guarded by engineLock
  WAVEFORMATEX* format = nullptr;           // guarded by engineLock; CoTaskMemFree'd on teardown
  HANDLE captureThread = nullptr;           // guarded by engineLock
  HANDLE stopEvent = nullptr;               // guarded by engineLock

  // True from the moment a route-restart attempt (Perform*RouteRestart or
  // Attempt*RouteRecoveryStep) releases engineLock to do its unlocked
  // WASAPI join/open work, until it reacquires the lock and finishes
  // deciding the outcome (publish, discard-as-superseded, or enter
  // backoff) — guarded by engineLock, like the rest of this struct.
  // StopMicrophoneCapture/StopSystemAudioCapture wait on claimReleasedCv
  // while this is true instead of reporting "stopped" while that unlocked
  // work — and the OS thread it may ultimately own — could still be live.
  std::atomic<bool> claimActive{false};
  std::condition_variable claimReleasedCv;  // notified whenever claimActive is cleared
};

PathState g_micState;
PathState g_systemState;

template <typename T>
void SafeRelease(T** ppT) {
  if (*ppT) {
    (*ppT)->Release();
    *ppT = nullptr;
  }
}

// Derives {bitDepth, isFloat} from a negotiated WAVEFORMATEX/WAVEFORMATEXTENSIBLE
// so callers can report the buffer's ACTUAL format truthfully (frozen
// requirement — never force 48kHz/float; audioConverter.js does the rest).
void DescribeMixFormat(const WAVEFORMATEX* fmt, int* outBitDepth, bool* outIsFloat) {
  *outBitDepth = fmt->wBitsPerSample;
  if (fmt->wFormatTag == WAVE_FORMAT_IEEE_FLOAT) {
    *outIsFloat = true;
  } else if (fmt->wFormatTag == WAVE_FORMAT_EXTENSIBLE && fmt->cbSize >= 22) {
    const WAVEFORMATEXTENSIBLE* ext = reinterpret_cast<const WAVEFORMATEXTENSIBLE*>(fmt);
    *outIsFloat = (ext->SubFormat == KSDATAFORMAT_SUBTYPE_IEEE_FLOAT);
  } else {
    *outIsFloat = false;  // WAVE_FORMAT_PCM, or an extensible subtype other than IEEE float
  }
}

// ── Device-change recovery: shared helpers ─────────────────────────────────
// (docs/WINDOWS_AUDIO_PLAN.md §15/§16, ported from the mac reference's
// debounce/coalesce/backoff design, adapted to WASAPI.)

// Runs `work` once, after delayMs, on a Windows thread-pool timer thread.
// This is what replaces the mac reference's dispatch_after on its two
// restart-worker dispatch queues — no dedicated always-alive thread is
// created just to host these delays (see file header thread model #5); the
// per-path PathState::engineLock is what actually serializes the resulting
// engine mutation, the same job the mac reference's *queue* does.
struct ScheduledWork {
  std::function<void()> work;
  PTP_TIMER timer = nullptr;
};

VOID CALLBACK ScheduledWorkCallback(PTP_CALLBACK_INSTANCE /*instance*/, PVOID context, PTP_TIMER /*timer*/) {
  ScheduledWork* sw = static_cast<ScheduledWork*>(context);
  sw->work();
  CloseThreadpoolTimer(sw->timer);
  delete sw;
}

bool ScheduleOnce(int64_t delayMs, std::function<void()> work) {
  ScheduledWork* sw = new ScheduledWork();
  sw->work = std::move(work);
  sw->timer = CreateThreadpoolTimer(ScheduledWorkCallback, sw, nullptr);
  if (!sw->timer) {
    delete sw;
    // CreateThreadpoolTimer failing is rare (thread-pool/resource
    // exhaustion) but real, and `work` is now permanently dropped — every
    // call site must check this return and handle it explicitly rather
    // than silently assuming the callback will eventually fire. See each
    // call site's comment for what "handle it" means there.
    return false;
  }
  ULARGE_INTEGER due;
  due.QuadPart = static_cast<ULONGLONG>(-(delayMs * 10000LL));  // relative time, 100ns units, negative = relative
  FILETIME ft;
  ft.dwLowDateTime = due.LowPart;
  ft.dwHighDateTime = due.HighPart;
  SetThreadpoolTimer(sw->timer, &ft, 0, 0);
  return true;
}

int64_t NowMs() { return static_cast<int64_t>(GetTickCount64()); }

// Reads the current default endpoint id for `flow`. Empty string on failure.
// Caller's thread must already be CoInitializeEx'd.
std::wstring GetDefaultEndpointId(EDataFlow flow) {
  IMMDevice* device = nullptr;
  if (FAILED(g_enumerator->GetDefaultAudioEndpoint(flow, eConsole, &device)) || !device) {
    return std::wstring();
  }
  LPWSTR idStr = nullptr;
  std::wstring result;
  if (SUCCEEDED(device->GetId(&idStr)) && idStr) {
    result = idStr;
    CoTaskMemFree(idStr);
  }
  device->Release();
  return result;
}

// Best-effort "is this endpoint likely wireless" check, used only to widen
// restart timing (docs/WINDOWS_AUDIO_PLAN.md §15 point 6). Windows has no
// single property as clean as CoreAudio's transport-type, so this uses
// PKEY_AudioEndpoint_FormFactor as an approximation and — matching the mac
// reference's own stated bias — treats any query failure as "assume
// wireless": the wider, safer timing is the correct default when uncertain,
// not the narrower one (worst case for a wired device that happens to
// briefly vanish is a slightly longer wait, not an incorrect one).
bool IsLikelyWirelessEndpoint(const std::wstring& endpointId) {
  if (endpointId.empty()) {
    return true;
  }
  IMMDevice* device = nullptr;
  if (FAILED(g_enumerator->GetDevice(endpointId.c_str(), &device)) || !device) {
    return true;
  }
  IPropertyStore* store = nullptr;
  HRESULT hr = device->OpenPropertyStore(STGM_READ, &store);
  device->Release();
  if (FAILED(hr) || !store) {
    return true;
  }
  PROPVARIANT pv;
  PropVariantInit(&pv);
  bool wireless = true;
  if (SUCCEEDED(store->GetValue(PKEY_AudioEndpoint_FormFactor, &pv)) && pv.vt == VT_UI4) {
    EndpointFormFactor formFactor = static_cast<EndpointFormFactor>(pv.ulVal);
    wireless = (formFactor == RemoteNetworkDevice || formFactor == Headset || formFactor == Headphones);
  }
  PropVariantClear(&pv);
  store->Release();
  return wireless;
}

// Matches the mac reference's backoff schedule and 30s ceiling exactly
// (SAYSO-353). Shared by both paths — the numbers themselves aren't
// path-specific, only the state (routeRecovering/routeRecoveryStartMs) they
// gate is, and that already lives per-path on PathState.
const int64_t kRouteRecoveryBackoffMs[] = {250, 500, 1000, 2000, 4000};
const size_t kRouteRecoveryBackoffSteps = sizeof(kRouteRecoveryBackoffMs) / sizeof(kRouteRecoveryBackoffMs[0]);
const int64_t kRouteRecoveryCeilingMs = 30000;

// Uniform "not implemented" throw for the still-placeholder action methods.
// index.js awaits these, so a synchronous throw surfaces as a rejected
// promise — a legitimate rejection reason at this stage of the port.
void NotImplemented(const char* fn) {
  std::string msg = "native_audio(win): ";
  msg += fn;
  msg += " not implemented yet — see docs/WINDOWS_AUDIO_PLAN.md";
  Nan::ThrowError(msg.c_str());
}

// SAYSO-347 shape: startMicrophoneCapture must return `true` on success or
// `{ ok: false, reason }` on failure — never a bare `false` that collapses
// distinguishable failure modes. Ported unchanged from the mac reference.
Local<Value> MicStartResult(bool ok, const char* reason = nullptr) {
  if (ok) {
    return Nan::New<v8::Boolean>(true);
  }
  Local<Object> result = Nan::New<Object>();
  Nan::Set(result, Nan::New("ok").ToLocalChecked(), Nan::New<v8::Boolean>(false));
  if (reason) {
    Nan::Set(result, Nan::New("reason").ToLocalChecked(), Nan::New<String>(reason).ToLocalChecked());
  }
  return result;
}

// ── Microphone capture thread ──────────────────────────────────────────────
// One thread per mic session: it performs the entire WASAPI open sequence
// AND the steady-state poll loop, so there is exactly one long-lived thread
// per active mic session, not a separate transient "start worker" plus a
// separate capture thread (see file header thread model #4).
//
// Ownership discipline (frozen requirements #20/#21): this thread creates
// device/audioClient/captureClient/format and is the only thread that reads
// or writes them WHILE it is running. On a clean stop it calls
// IAudioClient::Stop() (halts the hardware stream) and returns — it does NOT
// Release() the COM interfaces or CoTaskMemFree the format itself. Actually
// releasing them is StopMicrophoneCapture's job, on the JS thread, and only
// AFTER WaitForSingleObject confirms this thread has fully exited — so the
// interfaces are never released while this thread could still touch them.
// Heap-allocated (never a stack local) because the JS thread's wait on
// readyEvent is bounded — if it times out, the capture thread may still be
// running and touching this struct. Ownership is decided once, via
// settleMutex/settled: whichever side observes `!settled` first becomes the
// "normal" path (JS thread proceeds to read fields + eventually deletes ctx);
// the other side becomes the "abandoned" path, where the capture thread — not
// JS — owns tearing everything down and deleting ctx. This mirrors the mac
// reference's generation-guarded settle-once pattern for its SCK start, sized
// down for the mic path's simpler (single worker thread, no chained OS
// completion handlers) shape.
struct MicStartContext {
  HANDLE readyEvent = nullptr;
  HANDLE stopEvent = nullptr;
  std::mutex settleMutex;
  bool settled = false;    // guarded by settleMutex
  bool abandoned = false;  // guarded by settleMutex; true only on the JS-timeout path
  std::atomic<bool> started{false};
  const char* failReason = nullptr;

  // Published by the capture thread exactly once, on the non-abandoned path,
  // before SetEvent(readyEvent) — see the settle protocol above.
  IMMDevice* device = nullptr;
  IAudioClient* audioClient = nullptr;
  IAudioCaptureClient* captureClient = nullptr;
  WAVEFORMATEX* format = nullptr;
  std::wstring endpointId;
};

// Settle-once primitive shared by both sides of the handoff. Returns true if
// THIS call is the first to settle `ctx` (caller proceeds as the owner of
// whatever `asAbandoned` implies); false if the other side already settled it
// first (caller must defer to that side's decision instead).
bool TrySettle(MicStartContext* ctx, bool asAbandoned) {
  std::lock_guard<std::mutex> lk(ctx->settleMutex);
  if (ctx->settled) {
    return false;
  }
  ctx->settled = true;
  ctx->abandoned = asAbandoned;
  return true;
}

// Called by the capture thread at each point where it would otherwise hand
// off to the JS thread. Returns true if the JS thread already gave up
// (timeout) and this call is therefore responsible for full cleanup —
// including `delete ctx` — instead of the normal SetEvent handoff.
bool ClaimAbandoned(MicStartContext* ctx) { return !TrySettle(ctx, /*asAbandoned=*/false); }

// Matches the mac reference's first-attempt/retry timing exactly
// (TryStartMicrophoneCaptureOnce's 1200ms / 1500ms waits).
const int kMicFirstBufferWaitMs[2] = {1200, 1500};

void PushCapturedBuffer(AudioDeliveryChannel* channel, BYTE* data, UINT32 framesAvailable, DWORD flags,
                        UINT32 blockAlign, double sampleRate, int channels, int bitDepth, bool isFloat) {
  StreamingData* payload = new StreamingData();
  payload->audioData.resize(static_cast<size_t>(framesAvailable) * blockAlign);
  if (flags & AUDCLNT_BUFFERFLAGS_SILENT) {
    std::fill(payload->audioData.begin(), payload->audioData.end(), 0);
  } else {
    memcpy(payload->audioData.data(), data, payload->audioData.size());
  }
  payload->sampleRate = sampleRate;
  payload->channels = channels;
  payload->bitDepth = bitDepth;
  payload->isFloat = isFloat;
  channel->Push(payload);
}

DWORD WINAPI MicCaptureThreadProc(LPVOID param) {
  MicStartContext* ctx = static_cast<MicStartContext*>(param);

  if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED))) {
    ctx->failReason = "no_input_node";
    SetEvent(ctx->readyEvent);
    return 0;
  }

  IMMDevice* device = nullptr;
  IAudioClient* audioClient = nullptr;
  IAudioCaptureClient* captureClient = nullptr;
  WAVEFORMATEX* format = nullptr;
  UINT32 blockAlign = 0;
  int bitDepth = 0;
  bool isFloat = false;
  double sampleRate = 0;
  int channels = 0;
  std::wstring endpointId;
  const char* failReason = "no_tap_buffers";
  bool started = false;

  auto cleanupAttempt = [&]() {
    SafeRelease(&captureClient);
    SafeRelease(&audioClient);
    SafeRelease(&device);
    if (format) {
      CoTaskMemFree(format);
      format = nullptr;
    }
  };

  for (int attempt = 0; attempt < 2 && !started; attempt++) {
    HRESULT hr = g_enumerator->GetDefaultAudioEndpoint(eCapture, eConsole, &device);
    if (FAILED(hr) || !device) {
      failReason = "no_input_node";
      cleanupAttempt();
      continue;
    }

    hr = device->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr, reinterpret_cast<void**>(&audioClient));
    if (FAILED(hr) || !audioClient) {
      failReason = "no_input_node";
      cleanupAttempt();
      continue;
    }

    hr = audioClient->GetMixFormat(&format);
    if (FAILED(hr) || !format) {
      failReason = "tap_install_failed";
      cleanupAttempt();
      continue;
    }

    WAVEFORMATEX* closest = nullptr;
    HRESULT supportHr = audioClient->IsFormatSupported(AUDCLNT_SHAREMODE_SHARED, format, &closest);
    if (closest) {
      CoTaskMemFree(closest);
    }
    if (supportHr != S_OK && supportHr != S_FALSE) {
      failReason = "tap_install_failed";
      cleanupAttempt();
      continue;
    }

    const REFERENCE_TIME kBufferDuration = 200 * 10000;  // 200ms, in 100ns units
    hr = audioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, kBufferDuration, 0, format, nullptr);
    if (FAILED(hr)) {
      failReason = "tap_install_failed";
      cleanupAttempt();
      continue;
    }

    hr = audioClient->GetService(__uuidof(IAudioCaptureClient), reinterpret_cast<void**>(&captureClient));
    if (FAILED(hr) || !captureClient) {
      failReason = "tap_install_failed";
      cleanupAttempt();
      continue;
    }

    LPWSTR idStr = nullptr;
    if (SUCCEEDED(device->GetId(&idStr)) && idStr) {
      endpointId = idStr;
      CoTaskMemFree(idStr);
    }
    DescribeMixFormat(format, &bitDepth, &isFloat);
    sampleRate = format->nSamplesPerSec;
    channels = format->nChannels;
    blockAlign = format->nBlockAlign;

    hr = audioClient->Start();
    if (FAILED(hr)) {
      failReason = "engine_start_failed";
      cleanupAttempt();
      continue;
    }

    // Wait (bounded per attempt) for the first real buffer. Note: unlike the
    // steady-state loop below, this does NOT check stopEvent — a Stop cannot
    // race an in-flight Start today because JS is single-threaded and blocks
    // on this same call (see NAN_METHOD(StartMicrophoneCapture)); stage 4's
    // device-change recovery is the first path that will need to reconsider
    // this once it can trigger restarts independent of a JS-driven call.
    bool firstBufferSeen = false;
    bool fatal = false;
    DWORD waitStartTick = GetTickCount();
    while (!firstBufferSeen && (GetTickCount() - waitStartTick) <= static_cast<DWORD>(kMicFirstBufferWaitMs[attempt])) {
      UINT32 packetLength = 0;
      hr = captureClient->GetNextPacketSize(&packetLength);
      if (FAILED(hr)) {
        fatal = true;
        break;
      }
      if (packetLength == 0) {
        Sleep(5);
        continue;
      }
      BYTE* data = nullptr;
      UINT32 framesAvailable = 0;
      DWORD flags = 0;
      hr = captureClient->GetBuffer(&data, &framesAvailable, &flags, nullptr, nullptr);
      if (FAILED(hr)) {
        fatal = true;
        break;
      }
      if (framesAvailable > 0) {
        PushCapturedBuffer(&g_micChannel, data, framesAvailable, flags, blockAlign, sampleRate, channels, bitDepth,
                            isFloat);
        firstBufferSeen = true;
      }
      captureClient->ReleaseBuffer(framesAvailable);
    }

    if (!firstBufferSeen) {
      failReason = "no_tap_buffers";
      (void)fatal;  // same bucket either way — see PushCapturedBuffer call site comment above
      audioClient->Stop();
      cleanupAttempt();
      continue;
    }

    started = true;
  }

  if (!started) {
    if (ClaimAbandoned(ctx)) {
      // JS already gave up waiting — nobody will ever read ctx again. We own
      // freeing it (there is nothing else to release: cleanupAttempt() has
      // already run on every failed-attempt path above).
      CloseHandle(ctx->readyEvent);
      CloseHandle(ctx->stopEvent);
      delete ctx;
      CoUninitialize();
      return 0;
    }
    ctx->failReason = failReason;
    SetEvent(ctx->readyEvent);
    CoUninitialize();
    return 0;
  }

  if (ClaimAbandoned(ctx)) {
    // Orphaned: capture actually succeeded, but the JS-side start already
    // reported failure and moved on. Tear down rather than publish/loop —
    // direct analog of the mac reference's orphaned-SCK-stream handling.
    EmitLifecycleEvent("mic_orphan_capture_stopped");
    audioClient->Stop();
    cleanupAttempt();
    CloseHandle(ctx->readyEvent);
    CloseHandle(ctx->stopEvent);
    delete ctx;
    CoUninitialize();
    return 0;
  }

  // Capture stopEvent locally BEFORE the handoff below: the instant
  // SetEvent(ctx->readyEvent) fires, the JS thread may read ctx and `delete`
  // it at any time (it owns ctx from that point on in the normal path) — the
  // loop below must never dereference `ctx` again after this line.
  HANDLE stopEventLocal = ctx->stopEvent;

  // Hand off to the JS thread (normal path): publish fields before
  // SetEvent(readyEvent) — the WaitForSingleObject on the other side provides
  // the happens-before edge, so this is a safe unsynchronized handoff.
  ctx->device = device;
  ctx->audioClient = audioClient;
  ctx->captureClient = captureClient;
  ctx->format = format;
  ctx->endpointId = endpointId;
  ctx->started.store(true, std::memory_order_release);
  SetEvent(ctx->readyEvent);

  // Steady-state loop: local variables only from here on, no further ctx
  // access (ctx may already be freed by the JS thread by this point). Runs
  // until Stop signals stopEventLocal.
  while (WaitForSingleObject(stopEventLocal, 5) != WAIT_OBJECT_0) {
    UINT32 packetLength = 0;
    if (FAILED(captureClient->GetNextPacketSize(&packetLength))) {
      break;
    }
    while (packetLength > 0) {
      BYTE* data = nullptr;
      UINT32 framesAvailable = 0;
      DWORD flags = 0;
      if (FAILED(captureClient->GetBuffer(&data, &framesAvailable, &flags, nullptr, nullptr))) {
        packetLength = 0;
        break;
      }
      if (framesAvailable > 0) {
        PushCapturedBuffer(&g_micChannel, data, framesAvailable, flags, blockAlign, sampleRate, channels, bitDepth,
                            isFloat);
      }
      captureClient->ReleaseBuffer(framesAvailable);
      if (FAILED(captureClient->GetNextPacketSize(&packetLength))) {
        packetLength = 0;
      }
    }
  }

  audioClient->Stop();
  CoUninitialize();
  return 0;
}

// ── Microphone device-change recovery ──────────────────────────────────────
// Deliberately a fresh, self-contained copy of the open-sequence rather than
// a shared refactor of MicCaptureThreadProc above: this keeps stage 2's
// already-tested Start path byte-for-byte unchanged, at the cost of a second
// copy of the WASAPI dance. Given this is the final stage, that tradeoff
// favors "cannot regress approved behavior" over "zero duplication".

struct MicOpenResult {
  IMMDevice* device = nullptr;
  IAudioClient* audioClient = nullptr;
  IAudioCaptureClient* captureClient = nullptr;
  WAVEFORMATEX* format = nullptr;
  std::wstring endpointId;
  double sampleRate = 0;
  int channels = 0;
  int bitDepth = 0;
  bool isFloat = false;
  UINT32 blockAlign = 0;
};

void ReleaseMicOpenResult(MicOpenResult* r) {
  SafeRelease(&r->captureClient);
  SafeRelease(&r->audioClient);
  SafeRelease(&r->device);
  if (r->format) {
    CoTaskMemFree(r->format);
    r->format = nullptr;
  }
}

// Single attempt: open the default capture endpoint, negotiate format, start
// the engine, and wait up to waitForFirstBufferMs for the first real buffer
// (delivered to g_micChannel like any other captured buffer, same as any
// other mic audio — a restart's first buffer is not special-cased). Same
// shape as MicCaptureThreadProc's per-attempt logic; see the comment above.
bool TryOpenMicCaptureOnce(int waitForFirstBufferMs, MicOpenResult* out, const char** failReason) {
  HRESULT hr = g_enumerator->GetDefaultAudioEndpoint(eCapture, eConsole, &out->device);
  if (FAILED(hr) || !out->device) {
    *failReason = "no_input_node";
    ReleaseMicOpenResult(out);
    return false;
  }
  hr = out->device->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr,
                              reinterpret_cast<void**>(&out->audioClient));
  if (FAILED(hr) || !out->audioClient) {
    *failReason = "no_input_node";
    ReleaseMicOpenResult(out);
    return false;
  }
  hr = out->audioClient->GetMixFormat(&out->format);
  if (FAILED(hr) || !out->format) {
    *failReason = "tap_install_failed";
    ReleaseMicOpenResult(out);
    return false;
  }
  WAVEFORMATEX* closest = nullptr;
  HRESULT supportHr = out->audioClient->IsFormatSupported(AUDCLNT_SHAREMODE_SHARED, out->format, &closest);
  if (closest) {
    CoTaskMemFree(closest);
  }
  if (supportHr != S_OK && supportHr != S_FALSE) {
    *failReason = "tap_install_failed";
    ReleaseMicOpenResult(out);
    return false;
  }
  const REFERENCE_TIME kBufferDuration = 200 * 10000;
  hr = out->audioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, kBufferDuration, 0, out->format, nullptr);
  if (FAILED(hr)) {
    *failReason = "tap_install_failed";
    ReleaseMicOpenResult(out);
    return false;
  }
  hr = out->audioClient->GetService(__uuidof(IAudioCaptureClient), reinterpret_cast<void**>(&out->captureClient));
  if (FAILED(hr) || !out->captureClient) {
    *failReason = "tap_install_failed";
    ReleaseMicOpenResult(out);
    return false;
  }
  LPWSTR idStr = nullptr;
  if (SUCCEEDED(out->device->GetId(&idStr)) && idStr) {
    out->endpointId = idStr;
    CoTaskMemFree(idStr);
  }
  DescribeMixFormat(out->format, &out->bitDepth, &out->isFloat);
  out->sampleRate = out->format->nSamplesPerSec;
  out->channels = out->format->nChannels;
  out->blockAlign = out->format->nBlockAlign;

  hr = out->audioClient->Start();
  if (FAILED(hr)) {
    *failReason = "engine_start_failed";
    ReleaseMicOpenResult(out);
    return false;
  }

  bool firstBufferSeen = false;
  DWORD waitStartTick = GetTickCount();
  while (!firstBufferSeen && (GetTickCount() - waitStartTick) <= static_cast<DWORD>(waitForFirstBufferMs)) {
    UINT32 packetLength = 0;
    if (FAILED(out->captureClient->GetNextPacketSize(&packetLength))) {
      break;
    }
    if (packetLength == 0) {
      Sleep(5);
      continue;
    }
    BYTE* data = nullptr;
    UINT32 framesAvailable = 0;
    DWORD flags = 0;
    if (FAILED(out->captureClient->GetBuffer(&data, &framesAvailable, &flags, nullptr, nullptr))) {
      break;
    }
    if (framesAvailable > 0) {
      PushCapturedBuffer(&g_micChannel, data, framesAvailable, flags, out->blockAlign, out->sampleRate,
                          out->channels, out->bitDepth, out->isFloat);
      firstBufferSeen = true;
    }
    out->captureClient->ReleaseBuffer(framesAvailable);
  }

  if (!firstBufferSeen) {
    *failReason = "no_tap_buffers";
    out->audioClient->Stop();
    ReleaseMicOpenResult(out);
    return false;
  }
  return true;
}

struct MicPollOnlyParams {
  IAudioCaptureClient* captureClient;
  IAudioClient* audioClient;  // only for Stop() at the end — never Release()d here
  HANDLE stopEvent;
  UINT32 blockAlign;
  double sampleRate;
  int channels;
  int bitDepth;
  bool isFloat;
};

// The long-lived thread a successful restart spawns: identical
// ownership/threading discipline to MicCaptureThreadProc's own steady-state
// loop (only calls Stop() on exit; g_micState's owner releases the COM
// interfaces after joining this thread) but with no open sequence of its own
// — the caller already opened everything before spawning this.
DWORD WINAPI MicPollOnlyThreadProc(LPVOID param) {
  MicPollOnlyParams* p = static_cast<MicPollOnlyParams*>(param);
  IAudioCaptureClient* captureClient = p->captureClient;
  IAudioClient* audioClient = p->audioClient;
  HANDLE stopEvent = p->stopEvent;
  UINT32 blockAlign = p->blockAlign;
  double sampleRate = p->sampleRate;
  int channels = p->channels;
  int bitDepth = p->bitDepth;
  bool isFloat = p->isFloat;
  delete p;

  HRESULT hrInit = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

  while (WaitForSingleObject(stopEvent, 5) != WAIT_OBJECT_0) {
    UINT32 packetLength = 0;
    if (FAILED(captureClient->GetNextPacketSize(&packetLength))) {
      break;
    }
    while (packetLength > 0) {
      BYTE* data = nullptr;
      UINT32 framesAvailable = 0;
      DWORD flags = 0;
      if (FAILED(captureClient->GetBuffer(&data, &framesAvailable, &flags, nullptr, nullptr))) {
        packetLength = 0;
        break;
      }
      if (framesAvailable > 0) {
        PushCapturedBuffer(&g_micChannel, data, framesAvailable, flags, blockAlign, sampleRate, channels, bitDepth,
                            isFloat);
      }
      captureClient->ReleaseBuffer(framesAvailable);
      if (FAILED(captureClient->GetNextPacketSize(&packetLength))) {
        packetLength = 0;
      }
    }
  }

  audioClient->Stop();
  if (SUCCEEDED(hrInit)) {
    CoUninitialize();
  }
  return 0;
}

// A previous session's handles, exclusively owned by whoever holds the
// claim — see ClaimSessionLocked. Default-constructed (all null) means
// "nothing was claimed." Shared by both the mic and loopback paths —
// g_micState and g_systemState are both PathState instances, so one claim
// type and one pair of claim/join functions (parameterized over
// PathState&) serve both, instead of duplicating each field-for-field per
// path.
struct SessionClaim {
  HANDLE captureThread = nullptr;
  HANDLE stopEvent = nullptr;
  IAudioCaptureClient* captureClient = nullptr;
  IAudioClient* audioClient = nullptr;
  IMMDevice* device = nullptr;
  WAVEFORMATEX* format = nullptr;
};

// Must be called with state.engineLock already held. Snapshots the current
// session's handles into the returned claim and immediately clears the
// corresponding PathState fields (including openedEndpointId — an earlier
// version of this left it stale, still naming the old device, for as long
// as the claim was outstanding) — this is the only part of tearing a
// session down that actually needs the lock (it's a handful of pointer
// copies, not a wait). Once this returns, the claim is the sole owner of
// those handles: the PathState no longer references them, so a concurrent
// Stop or a fresh route-restart attempt cannot collide with whatever the
// caller does with the claim next (see JoinAndReleaseSessionClaim, which
// does not need the lock at all). Also marks state.claimActive — see its
// declaration on PathState — so Stop and any other in-flight restart
// attempt can tell "actively being torn down/reopened right now" apart
// from "nothing to do." Callers must eventually clear claimActive and
// notify claimReleasedCv once they're done with the claim, under the lock
// — see PerformMicRouteRestart for the canonical shape.
SessionClaim ClaimSessionLocked(PathState& state) {
  SessionClaim claim;
  claim.captureThread = state.captureThread;
  claim.stopEvent = state.stopEvent;
  claim.captureClient = state.captureClient;
  claim.audioClient = state.audioClient;
  claim.device = state.device;
  claim.format = state.format;
  state.captureThread = nullptr;
  state.stopEvent = nullptr;
  state.captureClient = nullptr;
  state.audioClient = nullptr;
  state.device = nullptr;
  state.format = nullptr;
  state.openedEndpointId.clear();
  state.claimActive.store(true, std::memory_order_release);
  return claim;
}

// No lock required — once ClaimSessionLocked has returned, `claim`'s
// handles aren't reachable through the owning PathState anymore, so nothing
// else can be concurrently operating on them. Same bounded-wait-then-
// conditionally-release contract fix #1 established (mirroring
// StopMicrophoneCapture): on a join timeout, the claim's captureThread and
// stopEvent HANDLEs are deliberately left un-closed, and its COM
// objects/format allocation deliberately left un-released — all of it a
// one-time leak, not just "the WASAPI objects" as an earlier version of
// this comment undersold it, since the thread may still be touching any of
// it. Unlike the old Teardown*SessionLocked, a timeout here does NOT leave
// anything for a later attempt to retry joining — the handle is already
// unreachable from the owning PathState, so it's leaked exactly once, not
// retried on every subsequent recovery attempt. That's a deliberate
// trade-off for being able to run this without holding engineLock: retrying
// the same join later would require re-publishing the claim back into the
// PathState under lock, which reopens the exact race (a concurrent Stop
// touching the same handles) this claim/join split exists to avoid.
// `timeoutEventName` lets each call site log its own path-specific
// lifecycle event on timeout (mic vs. system-audio).
bool JoinAndReleaseSessionClaim(SessionClaim& claim, const char* timeoutEventName) {
  if (claim.captureThread) {
    SetEvent(claim.stopEvent);
    DWORD waitResult = WaitForSingleObject(claim.captureThread, 3000);
    if (waitResult != WAIT_OBJECT_0) {
      EmitLifecycleEvent(timeoutEventName);
      return false;
    }
    CloseHandle(claim.captureThread);
    CloseHandle(claim.stopEvent);
    claim.captureThread = nullptr;
    claim.stopEvent = nullptr;
  }
  SafeRelease(&claim.captureClient);
  SafeRelease(&claim.audioClient);
  SafeRelease(&claim.device);
  if (claim.format) {
    CoTaskMemFree(claim.format);
    claim.format = nullptr;
  }
  return true;
}

// Must be called with g_micState.engineLock already held. Returns false if
// either the stop event or the poll-only thread couldn't be created — in
// that case `result`'s freshly-opened WASAPI objects are stopped and
// released right here, nothing is published to g_micState, and the caller
// must treat this exactly like a failed open attempt (same backoff/retry
// path), not a successful restart with no thread actually polling it.
bool PublishMicRestartSuccessLocked(MicOpenResult& result) {
  HANDLE stopEvent = CreateEventW(nullptr, TRUE, FALSE, nullptr);
  if (!stopEvent) {
    result.audioClient->Stop();
    ReleaseMicOpenResult(&result);
    return false;
  }
  MicPollOnlyParams* pollParams = new MicPollOnlyParams{result.captureClient, result.audioClient, stopEvent,
                                                         result.blockAlign,   result.sampleRate,   result.channels,
                                                         result.bitDepth,     result.isFloat};
  HANDLE thread = CreateThread(nullptr, 0, MicPollOnlyThreadProc, pollParams, 0, nullptr);
  if (!thread) {
    delete pollParams;
    CloseHandle(stopEvent);
    result.audioClient->Stop();
    ReleaseMicOpenResult(&result);
    return false;
  }
  g_micState.device = result.device;
  g_micState.audioClient = result.audioClient;
  g_micState.captureClient = result.captureClient;
  g_micState.format = result.format;
  g_micState.openedEndpointId = result.endpointId;
  g_micState.captureThread = thread;
  g_micState.stopEvent = stopEvent;
  return true;
}

void ScheduleMicRouteRecoveryRetry(uint64_t wave, size_t stepIndex, bool widen);

// One backoff attempt (mac reference: SaysoAttemptMicRouteRecovery). A single
// try — not the two-attempt shape PerformMicRouteRestart uses — matching the
// mac reference's own asymmetry between the initial post-debounce restart
// and each subsequent backoff step.
void AttemptMicRouteRecoveryStep(uint64_t wave, size_t stepIndex, bool widen) {
  if (!g_micState.capturing.load(std::memory_order_acquire)) {
    return;
  }
  if (wave != g_micState.routeRestartGeneration.load(std::memory_order_acquire)) {
    return;
  }
  HRESULT hrInit = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

  // Validate under a brief lock, then do the actual device-open attempt (up
  // to ~2.5s) unlocked — this runs on every backoff retry, potentially many
  // times across the 30s ceiling, so holding engineLock here would block a
  // concurrent StartMicrophoneCapture/StopMicrophoneCapture for that long on
  // every single retry. No claim step is needed here since there's no
  // existing session to tear down — the previous attempt already cleaned
  // itself up via ReleaseMicOpenResult on failure — but this attempt still
  // marks claimActive (and waits for any already-in-flight attempt to
  // finish first) so Stop and any other concurrent attempt see this open
  // work as "actively happening right now", not just "scheduled."
  bool claimBlocked = false;
  {
    std::unique_lock<std::mutex> lk(g_micState.engineLock);
    if (!g_micState.capturing.load(std::memory_order_acquire) ||
        wave != g_micState.routeRestartGeneration.load(std::memory_order_acquire)) {
      if (SUCCEEDED(hrInit)) CoUninitialize();
      return;
    }
    if (g_micState.claimActive.load(std::memory_order_acquire)) {
      // An older wave is still mid-flight (our generation check above just
      // passed, so it's necessarily stale and will discard itself once it
      // reacquires the lock). Wait for it to clear rather than racing it
      // with a second concurrent WASAPI open. Bounded generously above the
      // unlocked work's own worst case (join 3s + up to two opens
      // 2.5s+1.5s ≈ 7s) so a normal attempt is never mistaken for a wedge.
      bool cleared = g_micState.claimReleasedCv.wait_for(lk, std::chrono::milliseconds(8000), [] {
        return !g_micState.claimActive.load(std::memory_order_acquire);
      });
      if (!g_micState.capturing.load(std::memory_order_acquire) ||
          wave != g_micState.routeRestartGeneration.load(std::memory_order_acquire)) {
        if (SUCCEEDED(hrInit)) CoUninitialize();
        return;
      }
      if (!cleared) {
        // The other attempt is STILL active after our wait bound. The
        // invariant that matters is: never start a second concurrent
        // WASAPI open while claimActive is true — so do not claim. Fall
        // through and treat this exactly like a failed open attempt (same
        // backoff/retry path below). We don't own claimActive here — the
        // still-active attempt does — so we must not touch or clear it.
        EmitLifecycleEvent("mic_route_recovery_claim_wait_timeout");
        claimBlocked = true;
      }
    }
    if (!claimBlocked) {
      g_micState.claimActive.store(true, std::memory_order_release);
    }
  }

  bool ok = false;
  MicOpenResult result;
  if (!claimBlocked) {
    const char* failReason = "no_tap_buffers";
    ok = TryOpenMicCaptureOnce(widen ? 2500 : 1200, &result, &failReason);
  }

  bool succeeded = false;
  {
    std::lock_guard<std::mutex> lk(g_micState.engineLock);
    if (!claimBlocked) {
      g_micState.claimActive.store(false, std::memory_order_release);
      g_micState.claimReleasedCv.notify_all();
    }

    bool stillCurrent = g_micState.capturing.load(std::memory_order_acquire) &&
                         wave == g_micState.routeRestartGeneration.load(std::memory_order_acquire);
    if (!stillCurrent) {
      // Superseded while unlocked — see PerformMicRouteRestart's identical
      // reasoning (and the bug it fixes): discard whatever we opened and do
      // nothing further for a wave that's no longer current. Do NOT touch
      // routeRecovering or schedule another retry here.
      if (ok) {
        result.audioClient->Stop();
        ReleaseMicOpenResult(&result);
      }
      if (SUCCEEDED(hrInit)) CoUninitialize();
      return;
    }
    if (ok && PublishMicRestartSuccessLocked(result)) {
      g_micState.routeRecovering.store(false, std::memory_order_release);
      succeeded = true;
    }
  }
  if (SUCCEEDED(hrInit)) {
    CoUninitialize();
  }
  if (succeeded) {
    EmitLifecycleEvent("mic_route_recovery_succeeded attempts=" + std::to_string(stepIndex + 1));
    return;
  }
  ScheduleMicRouteRecoveryRetry(wave, stepIndex + 1, widen);
}

// Schedules the next backoff attempt, or gives up (mac reference:
// SaysoScheduleMicRouteRecoveryRetry) once the ~30s ceiling — measured from
// the FIRST failure of this recovery episode, not from each individual retry
// — is reached. Capture stays logically active but stops producing data;
// EmitLifecycleEvent's "_failed" event is what lets a JS-side watchdog (or
// Sentry) notice, matching the "no silent latch-off" contract bar.
void ScheduleMicRouteRecoveryRetry(uint64_t wave, size_t stepIndex, bool widen) {
  int64_t startMs;
  {
    std::lock_guard<std::mutex> lk(g_micState.engineLock);
    startMs = g_micState.routeRecoveryStartMs;
  }
  int64_t elapsedMs = NowMs() - startMs;
  if (elapsedMs >= kRouteRecoveryCeilingMs) {
    g_micState.routeRecovering.store(false, std::memory_order_release);
    EmitLifecycleEvent("mic_route_recovery_failed elapsedMs=" + std::to_string(elapsedMs));
    return;
  }
  int64_t delayMs = kRouteRecoveryBackoffMs[std::min(stepIndex, kRouteRecoveryBackoffSteps - 1)];
  if (!ScheduleOnce(delayMs, [wave, stepIndex, widen]() { AttemptMicRouteRecoveryStep(wave, stepIndex, widen); })) {
    // CreateThreadpoolTimer failed — the retry this function exists to
    // schedule will now never fire. Left unhandled, this is structurally
    // the same bug already fixed elsewhere in this file: routeRecovering
    // was set true expecting a future attempt to eventually clear it (on
    // success, or via the ceiling check above), and nothing else will ever
    // call back in to do that — routeRecovering would be stuck true
    // indefinitely. Give up cleanly instead, same as the ceiling case.
    g_micState.routeRecovering.store(false, std::memory_order_release);
    EmitLifecycleEvent("mic_route_recovery_schedule_failed");
  }
}

// Post-debounce/stability restart attempt (mac reference:
// SaysoPerformMicRestartIfCapturing) — tries twice (widened first attempt if
// leaving what looks like a wireless device) before handing off to the
// backoff schedule above on double failure.
void PerformMicRouteRestart(uint64_t wave, bool widen) {
  if (!g_micState.capturing.load(std::memory_order_acquire)) {
    return;
  }
  HRESULT hrInit = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

  // Claim the current session under the lock (a handful of pointer copies —
  // effectively instant), then do all of the actual WASAPI work (the join
  // wait, both open attempts) without holding engineLock at all. This is the
  // fix for the client-reported freeze: previously this entire sequence
  // (teardown join + up to two open retries, ~7s worst case) ran inside the
  // lock, so a JS-thread call into StartMicrophoneCapture/StopMicrophoneCapture
  // — which also need this lock — would queue behind it for that whole time.
  SessionClaim claim;
  bool claimBlocked = false;
  {
    std::unique_lock<std::mutex> lk(g_micState.engineLock);
    if (!g_micState.capturing.load(std::memory_order_acquire) ||
        wave != g_micState.routeRestartGeneration.load(std::memory_order_acquire)) {
      if (SUCCEEDED(hrInit)) CoUninitialize();
      return;
    }
    if (g_micState.claimActive.load(std::memory_order_acquire)) {
      // Another (necessarily older, since our generation check above just
      // passed) wave is already mid-flight. Wait for it to clear instead of
      // claiming an empty session and racing it with a second concurrent
      // WASAPI open — it will discard itself once it reacquires the lock,
      // since its own wave is now stale.
      bool cleared = g_micState.claimReleasedCv.wait_for(lk, std::chrono::milliseconds(8000), [] {
        return !g_micState.claimActive.load(std::memory_order_acquire);
      });
      if (!g_micState.capturing.load(std::memory_order_acquire) ||
          wave != g_micState.routeRestartGeneration.load(std::memory_order_acquire)) {
        if (SUCCEEDED(hrInit)) CoUninitialize();
        return;
      }
      if (!cleared) {
        // The other wave is STILL active after our wait bound. Never claim
        // while claimActive is true — that's the whole invariant this
        // mechanism exists to enforce — so fall through to the backoff
        // branch below exactly as if our own open attempt had failed. We
        // don't own claimActive here, so we must not touch or clear it.
        EmitLifecycleEvent("mic_route_recovery_claim_wait_timeout");
        claimBlocked = true;
      }
    }
    if (!claimBlocked) {
      claim = ClaimSessionLocked(g_micState);
    }
  }

  bool torn = false;
  bool ok = false;
  MicOpenResult result;
  if (!claimBlocked) {
    torn = JoinAndReleaseSessionClaim(claim, "mic_capture_thread_join_timeout");
    if (torn) {
      const char* failReason = "no_tap_buffers";
      ok = TryOpenMicCaptureOnce(widen ? 2500 : 1200, &result, &failReason);
      if (!ok) {
        ok = TryOpenMicCaptureOnce(1500, &result, &failReason);
      }
    }
  }
  // !torn falls straight through to the backoff branch below — the old
  // thread didn't join, so its WASAPI objects (now owned solely by `claim`,
  // already unreachable from g_micState) are still potentially live;
  // opening a new session right now is exactly the use-after-free fix #1
  // guards against. Nothing further to do with `claim` here — see
  // JoinAndReleaseSessionClaim's own comment on why a timeout isn't retried
  // on a later attempt. claimBlocked falls through the same way.

  bool succeeded = false;
  bool enteredBackoff = false;
  {
    std::lock_guard<std::mutex> lk(g_micState.engineLock);
    if (!claimBlocked) {
      g_micState.claimActive.store(false, std::memory_order_release);
      g_micState.claimReleasedCv.notify_all();
    }

    // Re-validate: engineLock was released for the join+open work above, so
    // a concurrent StopMicrophoneCapture (or a newer wave superseding this
    // one) may have run in the meantime. StopMicrophoneCapture bumps
    // routeRestartGeneration unconditionally before it does anything else,
    // so either check catches it.
    bool stillCurrent = g_micState.capturing.load(std::memory_order_acquire) &&
                         wave == g_micState.routeRestartGeneration.load(std::memory_order_acquire);

    if (!stillCurrent) {
      // Superseded while we were unlocked: either an explicit Stop already
      // reset capturing/routeRecovering itself, or a newer recovery wave is
      // now current and owns routeRecovering going forward. This wave has
      // nothing further to do — discard whatever it opened, but do NOT
      // touch routeRecovering or schedule a retry for a wave that's no
      // longer current.
      //
      // Bug fix: this used to fall through to the backoff branch below
      // unconditionally, re-arming routeRecovering=true even after a clean
      // Stop had just reset it to false, then calling
      // ScheduleMicRouteRecoveryRetry(wave, ...) for the stale wave. The
      // scheduled AttemptMicRouteRecoveryStep bails on its very first
      // capturing/generation check — before it ever reaches the 30s-ceiling
      // logic that resets routeRecovering — so the flag stayed stuck true
      // until the next explicit Start, with isMicRouteRecovering() lying to
      // JS-side watchdogs in the meantime.
      if (ok) {
        result.audioClient->Stop();
        ReleaseMicOpenResult(&result);
      }
      if (SUCCEEDED(hrInit)) CoUninitialize();
      return;
    }

    if (ok) {
      ok = PublishMicRestartSuccessLocked(result);
    }

    if (ok) {
      g_micState.routeRecovering.store(false, std::memory_order_release);
      succeeded = true;
    } else {
      // Only start the ~30s ceiling clock on the FIRST failure of a recovery
      // episode — a flapping device must not get a fresh 30s budget on
      // every single notification (mac reference's exact reasoning).
      if (!g_micState.routeRecovering.exchange(true, std::memory_order_acq_rel)) {
        g_micState.routeRecoveryStartMs = NowMs();
        enteredBackoff = true;
      }
    }
  }
  if (SUCCEEDED(hrInit)) {
    CoUninitialize();
  }

  if (succeeded) {
    EmitLifecycleEvent("mic_route_recovery_succeeded");
    return;
  }
  if (enteredBackoff) {
    EmitLifecycleEvent("mic_route_recovery_started");
  }
  ScheduleMicRouteRecoveryRetry(wave, 0, widen);
}

// Coalesces rapid default-input notifications (mac reference:
// SaysoScheduleMicRouteDebouncedRestart). Called directly from
// IMMNotificationClient's callback thread — everything past the atomic
// bump + ScheduleOnce(0, ...) below runs on a thread-pool timer thread
// instead, so this function itself never blocks or touches WASAPI/COM.
void ScheduleMicRouteDebouncedRestart() {
  if (!g_micState.capturing.load(std::memory_order_acquire)) {
    return;
  }
  const uint64_t wave = g_micState.routeRestartGeneration.fetch_add(1, std::memory_order_acq_rel) + 1;

  if (!ScheduleOnce(0, [wave]() {
    if (!g_micState.capturing.load(std::memory_order_acquire)) {
      return;
    }
    if (wave != g_micState.routeRestartGeneration.load(std::memory_order_acquire)) {
      return;
    }
    HRESULT hrInit = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    std::wstring openedId;
    {
      std::lock_guard<std::mutex> lk(g_micState.engineLock);
      openedId = g_micState.openedEndpointId;
    }
    std::wstring halNow = GetDefaultEndpointId(eCapture);
    const bool changed = !halNow.empty() && !openedId.empty() && halNow != openedId;
    const bool leavingWireless = changed && IsLikelyWirelessEndpoint(openedId);
    const bool fastPath = changed && !leavingWireless;
    if (SUCCEEDED(hrInit)) {
      CoUninitialize();
    }

    const int64_t debounceMs = fastPath ? 80 : leavingWireless ? 450 : 600;
    const int64_t stabilityMs = fastPath ? 60 : 150;

    if (!ScheduleOnce(debounceMs, [wave, stabilityMs, leavingWireless]() {
      if (!g_micState.capturing.load(std::memory_order_acquire)) {
        return;
      }
      if (wave != g_micState.routeRestartGeneration.load(std::memory_order_acquire)) {
        return;
      }
      HRESULT hrInit2 = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
      std::wstring firstDefault = GetDefaultEndpointId(eCapture);
      if (SUCCEEDED(hrInit2)) {
        CoUninitialize();
      }
      if (firstDefault.empty()) {
        return;
      }

      if (!ScheduleOnce(stabilityMs, [wave, leavingWireless, firstDefault]() {
        if (!g_micState.capturing.load(std::memory_order_acquire)) {
          return;
        }
        if (wave != g_micState.routeRestartGeneration.load(std::memory_order_acquire)) {
          return;
        }
        HRESULT hrInit3 = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
        std::wstring secondDefault = GetDefaultEndpointId(eCapture);
        if (SUCCEEDED(hrInit3)) {
          CoUninitialize();
        }
        if (secondDefault.empty()) {
          return;
        }
        if (firstDefault != secondDefault) {
          // Still changing — coalesce again rather than restart against a
          // target that's about to be stale (mac reference's exact logic).
          ScheduleMicRouteDebouncedRestart();
          return;
        }
        PerformMicRouteRestart(wave, leavingWireless);
      })) {
        // No stuck state to unwind here (unlike the backoff-retry case) —
        // routeRecovering/capturing were never touched on this path. Still
        // worth a diagnostic per the "no silent latch-off" bar: this
        // specific device-change notification's restart just never
        // happens, silently, without this.
        EmitLifecycleEvent("mic_route_debounce_schedule_failed stage=stability");
      }
    })) {
      EmitLifecycleEvent("mic_route_debounce_schedule_failed stage=debounce");
    }
  })) {
    EmitLifecycleEvent("mic_route_debounce_schedule_failed stage=dispatch");
  }
}

// ── Lifecycle / permissions ────────────────────────────────────────────────

NAN_METHOD(Initialize) {
  if (g_initialized.load(std::memory_order_acquire)) {
    info.GetReturnValue().Set(Nan::True());
    return;
  }

  g_comInfraReady = CreateEventW(nullptr, TRUE, FALSE, nullptr);
  g_comInfraShutdown = CreateEventW(nullptr, TRUE, FALSE, nullptr);
  if (!g_comInfraReady || !g_comInfraShutdown) {
    Nan::ThrowError("native_audio(win): failed to create synchronization events during initialize");
    return;
  }

  g_comInfraThread = CreateThread(nullptr, 0, ComInfraThreadProc, nullptr, 0, nullptr);
  if (!g_comInfraThread) {
    Nan::ThrowError("native_audio(win): failed to start COM infrastructure thread");
    return;
  }

  DWORD waitResult = WaitForSingleObject(g_comInfraReady, 5000);
  if (waitResult != WAIT_OBJECT_0 || FAILED(g_comInfraResult)) {
    char msg[256];
    snprintf(msg, sizeof(msg), "native_audio(win): initialize failed (wait=%lu hr=0x%08lX)",
             static_cast<unsigned long>(waitResult), static_cast<unsigned long>(g_comInfraResult));
    Nan::ThrowError(msg);
    return;
  }

  g_initialized.store(true, std::memory_order_release);
  info.GetReturnValue().Set(Nan::True());
}

// boolean (sync, non-prompting). Windows has no screen-recording gate for
// loopback capture.
NAN_METHOD(CheckScreenRecordingGranted) { info.GetReturnValue().Set(Nan::True()); }

// No OS screen-recording prompt on Windows — no-op resolve.
NAN_METHOD(RequestScreenRecordingPermission) { info.GetReturnValue().Set(Nan::True()); }

// ── System audio (loopback / "prospect") ───────────────────────────────────
//
// Unlike microphone start (synchronous from JS's perspective — see
// MicStartContext above), startSystemAudioCapture's contract is a genuine
// Promise<boolean>: the NAN method must return an already-pending promise
// immediately, without blocking the JS thread, and settle it later. This
// mirrors the mac reference's async SCK start (getShareableContent ->
// addStreamOutput -> startCaptureWithCompletionHandler, settled via a
// generation-guarded PendingSckStart), simplified for WASAPI's shape: there
// is exactly one worker thread doing one linear, synchronous open sequence
// (no chained OS completion handlers to race), so only ONE settlement point
// exists in the normal case, plus a watchdog and a stop-cancel that can each
// independently settle it first. All three go through the same
// generation-guarded SettleLoopbackStart, so whichever wins
// triggers the identical JS-thread cleanup path — there is no separate
// "abandoned ownership" handoff to design here the way MicStartContext
// needed one (that need was specific to StartMicrophoneCapture's *blocking*
// wait-with-timeout shape, which this path does not have).
//
// stopSystemAudioCapture, by contrast, IS synchronous/blocking here — a
// deliberate divergence from the mac reference's async
// stopCaptureWithCompletionHandler. Mac's stop is async specifically because
// SCK carries real extra teardown weight (SAYSO-337's own comment notes SCK
// runs an internal video pipeline even for audio-only capture). WASAPI
// loopback has no such hidden pipeline — its teardown is exactly as
// lightweight as the mic path's already-approved synchronous
// StopMicrophoneCapture, so it reuses that same bounded-wait-then-release
// shape rather than introducing a second async completion-callback
// machinery for a cost (a few-ms-to-3s JS-thread block in the pathological
// case) mac's own reason for going async doesn't apply to here.

Local<Promise> MakeRejectedPromise(Isolate* isolate, const char* msg) {
  Local<Context> context = isolate->GetCurrentContext();
  MaybeLocal<Promise::Resolver> maybe = Promise::Resolver::New(context);
  if (maybe.IsEmpty()) {
    return Local<Promise>();
  }
  Local<Promise::Resolver> resolver = maybe.ToLocalChecked();
  resolver->Reject(context, Nan::Error(msg)).Check();
  return resolver->GetPromise();
}

// Heap-allocated; deleted exactly once, from LoopbackStartSettledCb (JS
// thread), after uv_close completes — the same lifetime shape as the mac
// reference's PendingSckStart.
struct PendingLoopbackStart {
  uv_async_t async;
  Nan::Persistent<v8::Promise::Resolver> resolver;
  uint64_t generation = 0;
  bool settled = false;  // guarded by g_loopbackStartMutex
  bool reject = false;
  std::string message;

  HANDLE captureThread = nullptr;  // set by the JS thread right after CreateThread succeeds
  HANDLE stopEvent = nullptr;      // created by the JS thread before the worker thread starts

  // Published by the worker thread on the success path, before settling —
  // read by the JS-thread settle callback only if this generation's settle
  // call is the one that wins (see LoopbackCaptureThreadProc's orphan-stream
  // handling for the losing case).
  IMMDevice* device = nullptr;
  IAudioClient* audioClient = nullptr;
  IAudioCaptureClient* captureClient = nullptr;
  WAVEFORMATEX* format = nullptr;
  std::wstring endpointId;
};

PendingLoopbackStart* g_loopbackStartPending = nullptr;
std::mutex g_loopbackStartMutex;  // guards g_loopbackStartPending + settled transitions
uint64_t g_loopbackStartNextGeneration = 0;  // JS thread only

void LoopbackStartSettledCb(uv_async_t* handle);

// Settle the pending loopback start for `generation` exactly once. Returns
// false when that generation is no longer current (already settled by a
// competing path, superseded, or gone) — callers must treat false as "you
// were abandoned": clean up anything created locally and touch no globals.
// Settles generation `generation` exactly once. On success, the payload
// (device/audioClient/captureClient/format/endpointId) is written directly
// into g_loopbackStartPending WHILE STILL HOLDING g_loopbackStartMutex —
// this is what makes it safe for the worker thread to call this with only a
// bare `generation` value and no pointer into PendingLoopbackStart at all:
// as long as the mutex-protected check above finds the generation still
// current and unsettled, g_loopbackStartPending is provably not yet deleted
// (deletion only ever happens later, via LoopbackStartSettledCb on the JS
// thread, strictly after a winning call here has released this same mutex),
// so writing the payload fields here is safe. This deliberately replaces an
// earlier version that took a PendingLoopbackStart* directly and read/wrote
// it outside any lock from the worker thread — a genuine use-after-free: the
// worker thread's first instructions could run (or be scheduled) at any time
// after CreateThread, including after a racing StopSystemAudioCapture had
// already settled AND fully deleted the pending struct.
bool SettleLoopbackStart(uint64_t generation, bool reject, const std::string& message, IMMDevice* device = nullptr,
                          IAudioClient* audioClient = nullptr, IAudioCaptureClient* captureClient = nullptr,
                          WAVEFORMATEX* format = nullptr, const std::wstring& endpointId = std::wstring()) {
  PendingLoopbackStart* winner = nullptr;
  {
    std::lock_guard<std::mutex> lk(g_loopbackStartMutex);
    if (!g_loopbackStartPending || g_loopbackStartPending->generation != generation ||
        g_loopbackStartPending->settled) {
      return false;
    }
    g_loopbackStartPending->settled = true;
    g_loopbackStartPending->reject = reject;
    g_loopbackStartPending->message = message;
    if (!reject) {
      g_loopbackStartPending->device = device;
      g_loopbackStartPending->audioClient = audioClient;
      g_loopbackStartPending->captureClient = captureClient;
      g_loopbackStartPending->format = format;
      g_loopbackStartPending->endpointId = endpointId;
    }
    winner = g_loopbackStartPending;
  }
  uv_async_send(&winner->async);
  return true;
}

void LoopbackStartSettledCb(uv_async_t* handle) {
  PendingLoopbackStart* pending = static_cast<PendingLoopbackStart*>(handle->data);
  if (!pending) {
    return;
  }
  {
    std::lock_guard<std::mutex> lk(g_loopbackStartMutex);
    if (g_loopbackStartPending == pending) {
      g_loopbackStartPending = nullptr;
    }
  }

  Nan::HandleScope scope;
  Isolate* isolate = Isolate::GetCurrent();
  Local<Context> context = isolate->GetCurrentContext();
  Local<Promise::Resolver> resolver = Nan::New(pending->resolver);
  pending->resolver.Reset();

  if (pending->reject) {
    const char* msg = pending->message.empty() ? "System audio capture failed" : pending->message.c_str();
    resolver->Reject(context, Nan::Error(msg)).Check();
  } else {
    g_systemState.device = pending->device;
    g_systemState.audioClient = pending->audioClient;
    g_systemState.captureClient = pending->captureClient;
    g_systemState.format = pending->format;
    g_systemState.openedEndpointId = pending->endpointId;
    g_systemState.captureThread = pending->captureThread;
    g_systemState.stopEvent = pending->stopEvent;
    g_systemState.capturing.store(true, std::memory_order_release);
    g_systemState.routeRecovering.store(false, std::memory_order_release);
    resolver->Resolve(context, Nan::True()).Check();
  }

  uv_close(reinterpret_cast<uv_handle_t*>(handle),
           [](uv_handle_t* h) { delete static_cast<PendingLoopbackStart*>(h->data); });
}

// SAYSO-355-style watchdog: WASAPI device activation/Initialize/Start are
// plain blocking Win32/COM calls with no OS-guaranteed completion signal the
// way SCK's completion handlers are documented to (not always) provide —
// a misbehaving driver could in principle hang one of them. If nothing
// settles this generation within 10s, reject it so a caller can retry
// without an app relaunch, matching the mac reference's reliability bar.
DWORD WINAPI LoopbackStartWatchdogProc(LPVOID param) {
  uint64_t gen = static_cast<uint64_t>(reinterpret_cast<uintptr_t>(param));
  Sleep(10000);
  if (SettleLoopbackStart(gen, true, "sys_audio_start_timeout: WASAPI loopback start did not settle within 10s")) {
    EmitLifecycleEvent("sys_audio_start_watchdog_fired gen=" + std::to_string(gen));
  }
  return 0;
}

// Thread-start parameters, DELIBERATELY separate from PendingLoopbackStart
// and owned by no one but this thread: generation is a plain value-copy and
// stopEvent is a HANDLE (a kernel object reference, not memory owned by this
// struct or by PendingLoopbackStart), so freeing this struct here can never
// race anything. This decoupling is required, not just tidy: the worker
// thread's actual creation (CreateThread) and the OS actually scheduling its
// first instruction are two different moments with no ordering guarantee
// relative to a concurrent StopSystemAudioCapture, which can settle (and,
// shortly after, delete) a PendingLoopbackStart entirely independently of
// whether this thread has run yet. An earlier version of this function took
// a PendingLoopbackStart* directly and read pending->generation /
// pending->stopEvent as its first lines — a genuine use-after-free: nothing
// prevented StopSystemAudioCapture's cancel-and-delete path from completing
// before this thread's first instruction ever executed.
struct LoopbackThreadStartParams {
  uint64_t generation;
  HANDLE stopEvent;
};

// One thread per system-audio session: performs the WASAPI loopback open
// sequence, then — on success — continues directly into the steady-state
// poll loop (same "one thread, no separate start-worker" shape as
// MicCaptureThreadProc). Ownership discipline is identical to the mic path
// (frozen requirements #20/#21): this thread only calls IAudioClient::Stop()
// on exit; StopSystemAudioCapture releases the COM interfaces on the JS
// thread, only after confirming (via WaitForSingleObject) that this thread
// has fully exited. Note this thread NEVER dereferences a
// PendingLoopbackStart* — see LoopbackThreadStartParams and SettleLoopbackStart's
// mutex-protected publish for why that's necessary, not just cautious.
//
// Unlike mic capture, there is no "wait for first buffer" retry here: for
// loopback, silence is a normal steady state (nothing may currently be
// rendering to the output device), not a failure signal the way an
// unresponsive mic tap is. "Genuinely running" (per the contract) means
// IAudioClient::Start() succeeded — this resolves the start promise `true`
// without needing to observe any actual audio data first.
DWORD WINAPI LoopbackCaptureThreadProc(LPVOID param) {
  LoopbackThreadStartParams* startParams = static_cast<LoopbackThreadStartParams*>(param);
  const uint64_t gen = startParams->generation;
  HANDLE stopEventLocal = startParams->stopEvent;
  delete startParams;  // both fields already copied out; nothing else needs this struct

  if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED))) {
    SettleLoopbackStart(gen, true, "sys_audio_com_init_failed");
    return 0;
  }

  IMMDevice* device = nullptr;
  IAudioClient* audioClient = nullptr;
  IAudioCaptureClient* captureClient = nullptr;
  WAVEFORMATEX* format = nullptr;
  std::wstring endpointId;

  auto fail = [&](const char* msg) {
    SafeRelease(&captureClient);
    SafeRelease(&audioClient);
    SafeRelease(&device);
    if (format) {
      CoTaskMemFree(format);
      format = nullptr;
    }
    // Return value intentionally ignored: whether we win (reject the
    // promise) or lose (someone else — the watchdog or a stop-cancel —
    // already rejected it first) this generation's settlement, there is
    // nothing left of ours to release either way; we already did that above.
    SettleLoopbackStart(gen, true, msg);
    CoUninitialize();
  };

  HRESULT hr = g_enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &device);
  if (FAILED(hr) || !device) {
    fail("sys_audio_no_render_endpoint");
    return 0;
  }

  hr = device->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr, reinterpret_cast<void**>(&audioClient));
  if (FAILED(hr) || !audioClient) {
    fail("sys_audio_activate_failed");
    return 0;
  }

  hr = audioClient->GetMixFormat(&format);
  if (FAILED(hr) || !format) {
    fail("sys_audio_get_mix_format_failed");
    return 0;
  }

  WAVEFORMATEX* closest = nullptr;
  HRESULT supportHr = audioClient->IsFormatSupported(AUDCLNT_SHAREMODE_SHARED, format, &closest);
  if (closest) {
    CoTaskMemFree(closest);
  }
  if (supportHr != S_OK && supportHr != S_FALSE) {
    fail("sys_audio_format_not_supported");
    return 0;
  }

  const REFERENCE_TIME kBufferDuration = 200 * 10000;  // 200ms, in 100ns units
  hr = audioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_LOOPBACK, kBufferDuration, 0, format,
                                nullptr);
  if (FAILED(hr)) {
    fail("sys_audio_initialize_failed");
    return 0;
  }

  hr = audioClient->GetService(__uuidof(IAudioCaptureClient), reinterpret_cast<void**>(&captureClient));
  if (FAILED(hr) || !captureClient) {
    fail("sys_audio_get_capture_client_failed");
    return 0;
  }

  LPWSTR idStr = nullptr;
  if (SUCCEEDED(device->GetId(&idStr)) && idStr) {
    endpointId = idStr;
    CoTaskMemFree(idStr);
  }
  int bitDepth = 0;
  bool isFloat = false;
  DescribeMixFormat(format, &bitDepth, &isFloat);
  double sampleRate = format->nSamplesPerSec;
  int channels = format->nChannels;
  UINT32 blockAlign = format->nBlockAlign;

  hr = audioClient->Start();
  if (FAILED(hr)) {
    fail("sys_audio_start_failed");
    return 0;
  }

  // Settle + publish are one atomic step inside SettleLoopbackStart (under
  // g_loopbackStartMutex) — see that function's comment for why this must
  // not be split into "write pending-> fields, then settle" the way an
  // earlier version did.
  if (!SettleLoopbackStart(gen, false, "", device, audioClient, captureClient, format, endpointId)) {
    // Orphaned: capture actually succeeded, but a stop-cancel or the
    // watchdog already settled this generation as rejected first. Tear down
    // rather than publish/loop — direct analog of the mac reference's
    // orphaned-SCK-stream handling.
    EmitLifecycleEvent("sys_audio_orphan_stream_stopped gen=" + std::to_string(gen));
    audioClient->Stop();
    SafeRelease(&captureClient);
    SafeRelease(&audioClient);
    SafeRelease(&device);
    CoTaskMemFree(format);
    CoUninitialize();
    return 0;
  }

  // Steady-state loop: local variables only from here on, no further
  // `pending` access (it may be freed by the JS-thread settle callback at
  // any moment now). Runs until Stop signals stopEventLocal.
  while (WaitForSingleObject(stopEventLocal, 5) != WAIT_OBJECT_0) {
    UINT32 packetLength = 0;
    if (FAILED(captureClient->GetNextPacketSize(&packetLength))) {
      break;
    }
    while (packetLength > 0) {
      BYTE* data = nullptr;
      UINT32 framesAvailable = 0;
      DWORD flags = 0;
      if (FAILED(captureClient->GetBuffer(&data, &framesAvailable, &flags, nullptr, nullptr))) {
        packetLength = 0;
        break;
      }
      if (framesAvailable > 0) {
        PushCapturedBuffer(&g_systemChannel, data, framesAvailable, flags, blockAlign, sampleRate, channels, bitDepth,
                            isFloat);
      }
      captureClient->ReleaseBuffer(framesAvailable);
      if (FAILED(captureClient->GetNextPacketSize(&packetLength))) {
        packetLength = 0;
      }
    }
  }

  audioClient->Stop();
  CoUninitialize();
  return 0;
}

// ── System-audio (loopback) device-change recovery ─────────────────────────
// Structurally parallel to the microphone recovery section above (same
// debounce/stability/backoff shape, same generation-token discipline), but
// against g_systemState/eRender/g_systemChannel, and with no first-buffer
// wait in the open sequence — silence is a valid steady state for loopback
// (see LoopbackCaptureThreadProc's own comment), so a restart's success is
// judged purely by IAudioClient::Start() succeeding, same as the initial
// start.

struct LoopbackOpenResult {
  IMMDevice* device = nullptr;
  IAudioClient* audioClient = nullptr;
  IAudioCaptureClient* captureClient = nullptr;
  WAVEFORMATEX* format = nullptr;
  std::wstring endpointId;
  double sampleRate = 0;
  int channels = 0;
  int bitDepth = 0;
  bool isFloat = false;
  UINT32 blockAlign = 0;
};

void ReleaseLoopbackOpenResult(LoopbackOpenResult* r) {
  SafeRelease(&r->captureClient);
  SafeRelease(&r->audioClient);
  SafeRelease(&r->device);
  if (r->format) {
    CoTaskMemFree(r->format);
    r->format = nullptr;
  }
}

bool TryOpenLoopbackCaptureOnce(LoopbackOpenResult* out, const char** failReason) {
  HRESULT hr = g_enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &out->device);
  if (FAILED(hr) || !out->device) {
    *failReason = "sys_audio_no_render_endpoint";
    ReleaseLoopbackOpenResult(out);
    return false;
  }
  hr = out->device->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr,
                              reinterpret_cast<void**>(&out->audioClient));
  if (FAILED(hr) || !out->audioClient) {
    *failReason = "sys_audio_activate_failed";
    ReleaseLoopbackOpenResult(out);
    return false;
  }
  hr = out->audioClient->GetMixFormat(&out->format);
  if (FAILED(hr) || !out->format) {
    *failReason = "sys_audio_get_mix_format_failed";
    ReleaseLoopbackOpenResult(out);
    return false;
  }
  WAVEFORMATEX* closest = nullptr;
  HRESULT supportHr = out->audioClient->IsFormatSupported(AUDCLNT_SHAREMODE_SHARED, out->format, &closest);
  if (closest) {
    CoTaskMemFree(closest);
  }
  if (supportHr != S_OK && supportHr != S_FALSE) {
    *failReason = "sys_audio_format_not_supported";
    ReleaseLoopbackOpenResult(out);
    return false;
  }
  const REFERENCE_TIME kBufferDuration = 200 * 10000;
  hr = out->audioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_LOOPBACK, kBufferDuration, 0,
                                     out->format, nullptr);
  if (FAILED(hr)) {
    *failReason = "sys_audio_initialize_failed";
    ReleaseLoopbackOpenResult(out);
    return false;
  }
  hr = out->audioClient->GetService(__uuidof(IAudioCaptureClient), reinterpret_cast<void**>(&out->captureClient));
  if (FAILED(hr) || !out->captureClient) {
    *failReason = "sys_audio_get_capture_client_failed";
    ReleaseLoopbackOpenResult(out);
    return false;
  }
  LPWSTR idStr = nullptr;
  if (SUCCEEDED(out->device->GetId(&idStr)) && idStr) {
    out->endpointId = idStr;
    CoTaskMemFree(idStr);
  }
  DescribeMixFormat(out->format, &out->bitDepth, &out->isFloat);
  out->sampleRate = out->format->nSamplesPerSec;
  out->channels = out->format->nChannels;
  out->blockAlign = out->format->nBlockAlign;

  hr = out->audioClient->Start();
  if (FAILED(hr)) {
    *failReason = "sys_audio_start_failed";
    ReleaseLoopbackOpenResult(out);
    return false;
  }
  return true;
}

struct LoopbackPollOnlyParams {
  IAudioCaptureClient* captureClient;
  IAudioClient* audioClient;  // only for Stop() at the end — never Release()d here
  HANDLE stopEvent;
  UINT32 blockAlign;
  double sampleRate;
  int channels;
  int bitDepth;
  bool isFloat;
};

DWORD WINAPI LoopbackPollOnlyThreadProc(LPVOID param) {
  LoopbackPollOnlyParams* p = static_cast<LoopbackPollOnlyParams*>(param);
  IAudioCaptureClient* captureClient = p->captureClient;
  IAudioClient* audioClient = p->audioClient;
  HANDLE stopEvent = p->stopEvent;
  UINT32 blockAlign = p->blockAlign;
  double sampleRate = p->sampleRate;
  int channels = p->channels;
  int bitDepth = p->bitDepth;
  bool isFloat = p->isFloat;
  delete p;

  HRESULT hrInit = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

  while (WaitForSingleObject(stopEvent, 5) != WAIT_OBJECT_0) {
    UINT32 packetLength = 0;
    if (FAILED(captureClient->GetNextPacketSize(&packetLength))) {
      break;
    }
    while (packetLength > 0) {
      BYTE* data = nullptr;
      UINT32 framesAvailable = 0;
      DWORD flags = 0;
      if (FAILED(captureClient->GetBuffer(&data, &framesAvailable, &flags, nullptr, nullptr))) {
        packetLength = 0;
        break;
      }
      if (framesAvailable > 0) {
        PushCapturedBuffer(&g_systemChannel, data, framesAvailable, flags, blockAlign, sampleRate, channels, bitDepth,
                            isFloat);
      }
      captureClient->ReleaseBuffer(framesAvailable);
      if (FAILED(captureClient->GetNextPacketSize(&packetLength))) {
        packetLength = 0;
      }
    }
  }

  audioClient->Stop();
  if (SUCCEEDED(hrInit)) {
    CoUninitialize();
  }
  return 0;
}

// Must be called with g_systemState.engineLock already held. Returns false
// if either the stop event or the poll-only thread couldn't be created — in
// that case `result`'s freshly-opened WASAPI objects are stopped and
// released right here, nothing is published to g_systemState, and the
// caller must treat this exactly like a failed open attempt (same
// backoff/retry path), not a successful restart with no thread actually
// polling it.
bool PublishLoopbackRestartSuccessLocked(LoopbackOpenResult& result) {
  HANDLE stopEvent = CreateEventW(nullptr, TRUE, FALSE, nullptr);
  if (!stopEvent) {
    result.audioClient->Stop();
    ReleaseLoopbackOpenResult(&result);
    return false;
  }
  LoopbackPollOnlyParams* pollParams =
      new LoopbackPollOnlyParams{result.captureClient, result.audioClient, stopEvent,        result.blockAlign,
                                  result.sampleRate,    result.channels,    result.bitDepth,  result.isFloat};
  HANDLE thread = CreateThread(nullptr, 0, LoopbackPollOnlyThreadProc, pollParams, 0, nullptr);
  if (!thread) {
    delete pollParams;
    CloseHandle(stopEvent);
    result.audioClient->Stop();
    ReleaseLoopbackOpenResult(&result);
    return false;
  }
  g_systemState.device = result.device;
  g_systemState.audioClient = result.audioClient;
  g_systemState.captureClient = result.captureClient;
  g_systemState.format = result.format;
  g_systemState.openedEndpointId = result.endpointId;
  g_systemState.captureThread = thread;
  g_systemState.stopEvent = stopEvent;
  return true;
}

void ScheduleLoopbackRouteRecoveryRetry(uint64_t wave, size_t stepIndex);

void AttemptLoopbackRouteRecoveryStep(uint64_t wave, size_t stepIndex) {
  if (!g_systemState.capturing.load(std::memory_order_acquire)) {
    return;
  }
  if (wave != g_systemState.routeRestartGeneration.load(std::memory_order_acquire)) {
    return;
  }
  HRESULT hrInit = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

  // See AttemptMicRouteRecoveryStep's identical restructuring and comment —
  // validate briefly locked, wait out any already-in-flight attempt, open
  // unlocked, re-validate before publish.
  bool claimBlocked = false;
  {
    std::unique_lock<std::mutex> lk(g_systemState.engineLock);
    if (!g_systemState.capturing.load(std::memory_order_acquire) ||
        wave != g_systemState.routeRestartGeneration.load(std::memory_order_acquire)) {
      if (SUCCEEDED(hrInit)) CoUninitialize();
      return;
    }
    if (g_systemState.claimActive.load(std::memory_order_acquire)) {
      bool cleared = g_systemState.claimReleasedCv.wait_for(lk, std::chrono::milliseconds(8000), [] {
        return !g_systemState.claimActive.load(std::memory_order_acquire);
      });
      if (!g_systemState.capturing.load(std::memory_order_acquire) ||
          wave != g_systemState.routeRestartGeneration.load(std::memory_order_acquire)) {
        if (SUCCEEDED(hrInit)) CoUninitialize();
        return;
      }
      if (!cleared) {
        // See AttemptMicRouteRecoveryStep's identical reasoning: never
        // claim while claimActive is true. Fall through to the backoff
        // path without touching claimActive — we don't own it.
        EmitLifecycleEvent("sys_audio_route_recovery_claim_wait_timeout");
        claimBlocked = true;
      }
    }
    if (!claimBlocked) {
      g_systemState.claimActive.store(true, std::memory_order_release);
    }
  }

  bool ok = false;
  LoopbackOpenResult result;
  if (!claimBlocked) {
    const char* failReason = "sys_audio_start_failed";
    ok = TryOpenLoopbackCaptureOnce(&result, &failReason);
  }

  bool succeeded = false;
  {
    std::lock_guard<std::mutex> lk(g_systemState.engineLock);
    if (!claimBlocked) {
      g_systemState.claimActive.store(false, std::memory_order_release);
      g_systemState.claimReleasedCv.notify_all();
    }

    bool stillCurrent = g_systemState.capturing.load(std::memory_order_acquire) &&
                         wave == g_systemState.routeRestartGeneration.load(std::memory_order_acquire);
    if (!stillCurrent) {
      if (ok) {
        result.audioClient->Stop();
        ReleaseLoopbackOpenResult(&result);
      }
      if (SUCCEEDED(hrInit)) CoUninitialize();
      return;
    }
    if (ok && PublishLoopbackRestartSuccessLocked(result)) {
      g_systemState.routeRecovering.store(false, std::memory_order_release);
      succeeded = true;
    }
  }
  if (SUCCEEDED(hrInit)) {
    CoUninitialize();
  }
  if (succeeded) {
    EmitLifecycleEvent("sys_audio_route_recovery_succeeded attempts=" + std::to_string(stepIndex + 1));
    return;
  }
  ScheduleLoopbackRouteRecoveryRetry(wave, stepIndex + 1);
}

void ScheduleLoopbackRouteRecoveryRetry(uint64_t wave, size_t stepIndex) {
  int64_t startMs;
  {
    std::lock_guard<std::mutex> lk(g_systemState.engineLock);
    startMs = g_systemState.routeRecoveryStartMs;
  }
  int64_t elapsedMs = NowMs() - startMs;
  if (elapsedMs >= kRouteRecoveryCeilingMs) {
    g_systemState.routeRecovering.store(false, std::memory_order_release);
    EmitLifecycleEvent("sys_audio_route_recovery_failed elapsedMs=" + std::to_string(elapsedMs));
    return;
  }
  int64_t delayMs = kRouteRecoveryBackoffMs[std::min(stepIndex, kRouteRecoveryBackoffSteps - 1)];
  if (!ScheduleOnce(delayMs, [wave, stepIndex]() { AttemptLoopbackRouteRecoveryStep(wave, stepIndex); })) {
    // See ScheduleMicRouteRecoveryRetry's identical reasoning: without this,
    // routeRecovering would be stuck true indefinitely.
    g_systemState.routeRecovering.store(false, std::memory_order_release);
    EmitLifecycleEvent("sys_audio_route_recovery_schedule_failed");
  }
}

void PerformLoopbackRouteRestart(uint64_t wave) {
  if (!g_systemState.capturing.load(std::memory_order_acquire)) {
    return;
  }
  HRESULT hrInit = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

  // See the identical structure (and its comment) in PerformMicRouteRestart:
  // claim under the lock (instant), then do the join wait and open attempt
  // without holding engineLock — that's what stops a StartSystemAudioCapture
  // / StopSystemAudioCapture call from queuing behind this ~4-7s of WASAPI
  // work.
  SessionClaim claim;
  bool claimBlocked = false;
  {
    std::unique_lock<std::mutex> lk(g_systemState.engineLock);
    if (!g_systemState.capturing.load(std::memory_order_acquire) ||
        wave != g_systemState.routeRestartGeneration.load(std::memory_order_acquire)) {
      if (SUCCEEDED(hrInit)) CoUninitialize();
      return;
    }
    if (g_systemState.claimActive.load(std::memory_order_acquire)) {
      bool cleared = g_systemState.claimReleasedCv.wait_for(lk, std::chrono::milliseconds(8000), [] {
        return !g_systemState.claimActive.load(std::memory_order_acquire);
      });
      if (!g_systemState.capturing.load(std::memory_order_acquire) ||
          wave != g_systemState.routeRestartGeneration.load(std::memory_order_acquire)) {
        if (SUCCEEDED(hrInit)) CoUninitialize();
        return;
      }
      if (!cleared) {
        // See PerformMicRouteRestart's identical reasoning: never claim
        // while claimActive is true. Fall through to the backoff path
        // without touching claimActive — we don't own it.
        EmitLifecycleEvent("sys_audio_route_recovery_claim_wait_timeout");
        claimBlocked = true;
      }
    }
    if (!claimBlocked) {
      claim = ClaimSessionLocked(g_systemState);
    }
  }

  bool torn = false;
  bool ok = false;
  LoopbackOpenResult result;
  if (!claimBlocked) {
    torn = JoinAndReleaseSessionClaim(claim, "sys_audio_capture_thread_join_timeout");
    if (torn) {
      const char* failReason = "sys_audio_start_failed";
      ok = TryOpenLoopbackCaptureOnce(&result, &failReason);
    }
  }
  // !torn falls straight through to the backoff branch below — see the
  // mic-side comment in PerformMicRouteRestart for why opening a new
  // session on top of an un-joined thread's WASAPI objects is unsafe.
  // claimBlocked falls through the same way.

  bool succeeded = false;
  bool enteredBackoff = false;
  {
    std::lock_guard<std::mutex> lk(g_systemState.engineLock);
    if (!claimBlocked) {
      g_systemState.claimActive.store(false, std::memory_order_release);
      g_systemState.claimReleasedCv.notify_all();
    }

    // Re-validate — see the identical check (and its comment) in
    // PerformMicRouteRestart.
    bool stillCurrent = g_systemState.capturing.load(std::memory_order_acquire) &&
                         wave == g_systemState.routeRestartGeneration.load(std::memory_order_acquire);

    if (!stillCurrent) {
      // Superseded while we were unlocked — see the identical check, bug
      // description, and fix in PerformMicRouteRestart. Do NOT touch
      // routeRecovering or schedule a retry for a wave that's no longer
      // current.
      if (ok) {
        result.audioClient->Stop();
        ReleaseLoopbackOpenResult(&result);
      }
      if (SUCCEEDED(hrInit)) CoUninitialize();
      return;
    }

    if (ok) {
      ok = PublishLoopbackRestartSuccessLocked(result);
    }

    if (ok) {
      g_systemState.routeRecovering.store(false, std::memory_order_release);
      succeeded = true;
    } else {
      if (!g_systemState.routeRecovering.exchange(true, std::memory_order_acq_rel)) {
        g_systemState.routeRecoveryStartMs = NowMs();
        enteredBackoff = true;
      }
    }
  }
  if (SUCCEEDED(hrInit)) {
    CoUninitialize();
  }

  if (succeeded) {
    EmitLifecycleEvent("sys_audio_route_recovery_succeeded");
    return;
  }
  if (enteredBackoff) {
    EmitLifecycleEvent("sys_audio_route_recovery_started");
  }
  ScheduleLoopbackRouteRecoveryRetry(wave, 0);
}

// Coalesces rapid default-render notifications, mirroring
// ScheduleMicRouteDebouncedRestart exactly (see that function's comment for
// why this never blocks/touches WASAPI on the calling — possibly
// notification-callback — thread).
void ScheduleLoopbackRouteDebouncedRestart() {
  if (!g_systemState.capturing.load(std::memory_order_acquire)) {
    return;
  }
  const uint64_t wave = g_systemState.routeRestartGeneration.fetch_add(1, std::memory_order_acq_rel) + 1;

  if (!ScheduleOnce(0, [wave]() {
    if (!g_systemState.capturing.load(std::memory_order_acquire)) {
      return;
    }
    if (wave != g_systemState.routeRestartGeneration.load(std::memory_order_acquire)) {
      return;
    }
    HRESULT hrInit = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    std::wstring openedId;
    {
      std::lock_guard<std::mutex> lk(g_systemState.engineLock);
      openedId = g_systemState.openedEndpointId;
    }
    std::wstring halNow = GetDefaultEndpointId(eRender);
    const bool changed = !halNow.empty() && !openedId.empty() && halNow != openedId;
    const bool leavingWireless = changed && IsLikelyWirelessEndpoint(openedId);
    const bool fastPath = changed && !leavingWireless;
    if (SUCCEEDED(hrInit)) {
      CoUninitialize();
    }

    const int64_t debounceMs = fastPath ? 80 : leavingWireless ? 450 : 600;
    const int64_t stabilityMs = fastPath ? 60 : 150;

    if (!ScheduleOnce(debounceMs, [wave, stabilityMs]() {
      if (!g_systemState.capturing.load(std::memory_order_acquire)) {
        return;
      }
      if (wave != g_systemState.routeRestartGeneration.load(std::memory_order_acquire)) {
        return;
      }
      HRESULT hrInit2 = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
      std::wstring firstDefault = GetDefaultEndpointId(eRender);
      if (SUCCEEDED(hrInit2)) {
        CoUninitialize();
      }
      if (firstDefault.empty()) {
        return;
      }

      if (!ScheduleOnce(stabilityMs, [wave, firstDefault]() {
        if (!g_systemState.capturing.load(std::memory_order_acquire)) {
          return;
        }
        if (wave != g_systemState.routeRestartGeneration.load(std::memory_order_acquire)) {
          return;
        }
        HRESULT hrInit3 = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
        std::wstring secondDefault = GetDefaultEndpointId(eRender);
        if (SUCCEEDED(hrInit3)) {
          CoUninitialize();
        }
        if (secondDefault.empty()) {
          return;
        }
        if (firstDefault != secondDefault) {
          ScheduleLoopbackRouteDebouncedRestart();
          return;
        }
        PerformLoopbackRouteRestart(wave);
      })) {
        // See ScheduleMicRouteDebouncedRestart's identical reasoning — no
        // stuck state to unwind, just a silent missed restart without this.
        EmitLifecycleEvent("sys_audio_route_debounce_schedule_failed stage=stability");
      }
    })) {
      EmitLifecycleEvent("sys_audio_route_debounce_schedule_failed stage=debounce");
    }
  })) {
    EmitLifecycleEvent("sys_audio_route_debounce_schedule_failed stage=dispatch");
  }
}

// Defined here (needs both debounce functions above already declared).
// Called from IMMNotificationClient::OnDeviceStateChanged, itself already
// deferred once via ScheduleOnce(0, ...) before this ever runs, so touching
// PathState::engineLock here (a quick, bounded string compare, never held
// during a WASAPI call) is safe — it is NOT running on the notification
// callback thread itself.
void HandleDeviceStateChangedToGone(const std::wstring& id) {
  if (!ScheduleOnce(0, [id]() {
    bool matchesMic = false;
    bool matchesSystem = false;
    {
      std::lock_guard<std::mutex> lk(g_micState.engineLock);
      matchesMic = g_micState.capturing.load(std::memory_order_acquire) && g_micState.openedEndpointId == id;
    }
    {
      std::lock_guard<std::mutex> lk(g_systemState.engineLock);
      matchesSystem =
          g_systemState.capturing.load(std::memory_order_acquire) && g_systemState.openedEndpointId == id;
    }
    if (matchesMic) {
      ScheduleMicRouteDebouncedRestart();
    }
    if (matchesSystem) {
      ScheduleLoopbackRouteDebouncedRestart();
    }
  })) {
    // No stuck state to unwind (nothing was touched yet) — but per the "no
    // silent latch-off" bar, a device-unplug notification that silently
    // fails to even check whether it matters is worth logging.
    EmitLifecycleEvent("device_gone_schedule_failed");
  }
}

NAN_METHOD(StartSystemAudioCapture) {
  Isolate* isolate = info.GetIsolate();

  // Authoritative reset, unconditionally, mirroring StartMicrophoneCapture's
  // identical reasoning: an explicit Start means any prior recovery state no
  // longer applies, even if the already-active guard below rejects this call.
  g_systemState.routeRecovering.store(false, std::memory_order_release);

  if (g_systemState.capturing.load(std::memory_order_acquire)) {
    info.GetReturnValue().Set(MakeRejectedPromise(isolate, "System audio capture already active"));
    return;
  }
  {
    std::lock_guard<std::mutex> lk(g_loopbackStartMutex);
    if (g_loopbackStartPending != nullptr) {
      info.GetReturnValue().Set(MakeRejectedPromise(isolate, "System audio capture start already in progress"));
      return;
    }
  }

  Local<Context> context = isolate->GetCurrentContext();
  MaybeLocal<Promise::Resolver> maybeResolver = Promise::Resolver::New(context);
  if (maybeResolver.IsEmpty()) {
    info.GetReturnValue().Set(MakeRejectedPromise(isolate, "Failed to create promise resolver"));
    return;
  }
  Local<Promise::Resolver> resolver = maybeResolver.ToLocalChecked();
  info.GetReturnValue().Set(resolver->GetPromise());

  PendingLoopbackStart* pending = new PendingLoopbackStart();
  pending->resolver.Reset(resolver);
  pending->stopEvent = CreateEventW(nullptr, TRUE, FALSE, nullptr);
  if (!pending->stopEvent) {
    Local<Promise::Resolver> res = Nan::New(pending->resolver);
    res->Reject(context, Nan::Error("native_audio(win): failed to create stop event")).Check();
    delete pending;
    return;
  }
  pending->async.data = pending;
  if (uv_async_init(uv_default_loop(), &pending->async, LoopbackStartSettledCb) != 0) {
    Local<Promise::Resolver> res = Nan::New(pending->resolver);
    res->Reject(context, Nan::Error("native_audio(win): failed to init async notifier")).Check();
    CloseHandle(pending->stopEvent);
    delete pending;
    return;
  }
  pending->generation = ++g_loopbackStartNextGeneration;
  const uint64_t gen = pending->generation;
  {
    std::lock_guard<std::mutex> lk(g_loopbackStartMutex);
    g_loopbackStartPending = pending;
  }

  // Deliberately NOT `pending` — see LoopbackThreadStartParams's comment for
  // why the worker thread must never receive a pointer whose lifetime is
  // governed by the settle/delete path a racing Stop can trigger.
  LoopbackThreadStartParams* startParams = new LoopbackThreadStartParams();
  startParams->generation = gen;
  startParams->stopEvent = pending->stopEvent;

  HANDLE thread = CreateThread(nullptr, 0, LoopbackCaptureThreadProc, startParams, 0, nullptr);
  if (!thread) {
    delete startParams;
    SettleLoopbackStart(gen, true, "native_audio(win): failed to create capture thread");
    return;
  }
  pending->captureThread = thread;

  HANDLE watchdog = CreateThread(nullptr, 0, LoopbackStartWatchdogProc,
                                  reinterpret_cast<LPVOID>(static_cast<uintptr_t>(gen)), 0, nullptr);
  if (watchdog) {
    CloseHandle(watchdog);  // detached: it settles-or-no-ops and exits on its own within 10s
  }
}

// Synchronous/blocking teardown for the "really capturing" branch — see the
// design note above NAN_METHOD(StartSystemAudioCapture) for why this
// deliberately does not mirror the mac reference's async stop.
NAN_METHOD(StopSystemAudioCapture) {
  Isolate* isolate = info.GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();

  // Builds and resolves the {success, filePath: null, actualStartMs: null,
  // error?} promise this method always returns — factored out since every
  // exit path below needs it, differing only in `success`/`error`.
  auto resolveStop = [&](bool success, const char* error) {
    MaybeLocal<Promise::Resolver> maybeResolver = Promise::Resolver::New(context);
    if (maybeResolver.IsEmpty()) {
      Nan::ThrowError("native_audio(win): failed to create promise resolver for stop");
      return;
    }
    Local<Promise::Resolver> resolver = maybeResolver.ToLocalChecked();
    Local<Object> result = Nan::New<Object>();
    Nan::Set(result, Nan::New("success").ToLocalChecked(), Nan::New<v8::Boolean>(success));
    Nan::Set(result, Nan::New("filePath").ToLocalChecked(), Nan::Null());
    Nan::Set(result, Nan::New("actualStartMs").ToLocalChecked(), Nan::Null());
    if (error) {
      Nan::Set(result, Nan::New("error").ToLocalChecked(), Nan::New(error).ToLocalChecked());
    }
    resolver->Resolve(context, result).Check();
    info.GetReturnValue().Set(resolver->GetPromise());
  };

  // Bumped unconditionally, before anything else — mirrors
  // StopMicrophoneCapture's identical reasoning: invalidates any in-flight
  // debounce/backoff chain even if one is running concurrently right now.
  g_systemState.routeRestartGeneration.fetch_add(1, std::memory_order_acq_rel);

  // A start may be pending (worker thread not yet settled). Cancel it so a
  // wedged/slow start cannot outlive a stop: reject its promise now; if the
  // worker thread's own settle call arrives later anyway, it sees itself
  // abandoned and tears down its own orphaned stream (see
  // LoopbackCaptureThreadProc).
  {
    uint64_t pendingGen = 0;
    bool hasPending = false;
    {
      std::lock_guard<std::mutex> lk(g_loopbackStartMutex);
      if (g_loopbackStartPending && !g_loopbackStartPending->settled) {
        hasPending = true;
        pendingGen = g_loopbackStartPending->generation;
      }
    }
    if (hasPending) {
      SettleLoopbackStart(pendingGen, true, "System audio capture start canceled by stop");
    }
  }

  SessionClaim claim;
  {
    std::unique_lock<std::mutex> lk(g_systemState.engineLock);

    if (!g_systemState.capturing.load(std::memory_order_acquire)) {
      resolveStop(false, nullptr);
      return;
    }

    if (g_systemState.claimActive.load(std::memory_order_acquire)) {
      // See StopMicrophoneCapture's identical reasoning: wait for the
      // in-flight route-recovery attempt to finish rather than reporting
      // "stopped" while its unlocked work (and the OS thread it may
      // ultimately own) could still be live. Bounded generously above that
      // work's own worst case (join 3s + open up to ~4s) so a normal
      // attempt is never mistaken for a wedge.
      bool cleared = g_systemState.claimReleasedCv.wait_for(lk, std::chrono::milliseconds(8000), [] {
        return !g_systemState.claimActive.load(std::memory_order_acquire);
      });
      // Not a correctness gap if this times out: unlike the route-restart
      // wait above, we don't unconditionally claim afterward — the
      // captureThread null-check right below still gates that, and while
      // claimActive is true captureThread is guaranteed null (the holder
      // moved it into their own local claim), so we can never claim out
      // from under them regardless of `cleared`. Still worth a diagnostic:
      // it means we're finalizing "stopped" without having confirmed the
      // other side's OS thread has actually exited yet (it will, eventually
      // — routeRestartGeneration was already bumped above, so its own
      // re-validation will discard its result whenever it does finish).
      if (!cleared) {
        EmitLifecycleEvent("sys_audio_stop_claim_wait_timeout");
      }
      // No need to re-check capturing here — see the identical reasoning
      // in StopMicrophoneCapture.
    }

    if (!g_systemState.captureThread) {
      // Still nothing to join — see StopMicrophoneCapture's identical
      // reasoning. Finalize directly.
      g_systemState.capturing.store(false, std::memory_order_release);
      g_systemState.routeRecovering.store(false, std::memory_order_release);
      resolveStop(true, nullptr);
      return;
    }

    claim = ClaimSessionLocked(g_systemState);
  }

  // Bounded wait, unlocked — see StopMicrophoneCapture's identical
  // reasoning.
  bool torn = JoinAndReleaseSessionClaim(claim, "sys_audio_capture_thread_join_timeout");

  std::lock_guard<std::mutex> lk2(g_systemState.engineLock);
  // See StopMicrophoneCapture's identical fix/comment: this call took the
  // claim above, so it's responsible for clearing claimActive and notifying
  // claimReleasedCv.
  g_systemState.claimActive.store(false, std::memory_order_release);
  g_systemState.claimReleasedCv.notify_all();
  if (!torn) {
    g_systemState.capturing.store(false, std::memory_order_release);
    g_systemState.routeRecovering.store(false, std::memory_order_release);
    resolveStop(false, "capture thread join timeout");
    return;
  }

  // Safe only here: the thread is provably joined, so nothing can still be
  // racing Push() against this — see the identical reasoning in
  // StopMicrophoneCapture.
  g_systemChannel.CloseAsyncHandle();

  g_systemState.capturing.store(false, std::memory_order_release);
  g_systemState.routeRecovering.store(false, std::memory_order_release);
  resolveStop(true, nullptr);
}

// Includes a start still settling: callers use this probe to decide whether
// teardown is needed before a fresh start, and a pending start needs
// teardown too (matches the mac reference's SAYSO-355 reasoning exactly).
NAN_METHOD(IsSystemAudioCaptureActive) {
  bool pendingStart = false;
  {
    std::lock_guard<std::mutex> lk(g_loopbackStartMutex);
    pendingStart = (g_loopbackStartPending != nullptr && !g_loopbackStartPending->settled);
  }
  info.GetReturnValue().Set(
      Nan::New<v8::Boolean>(g_systemState.capturing.load(std::memory_order_acquire) || pendingStart));
}

// ── Microphone ("user") — body lands in stage 2 ────────────────────────────

// Synchronous from JS's perspective (index.js awaits a plain return value,
// not a Promise) — matches the mac reference, which also runs the actual
// engine work on a different execution context (dispatch_sync onto its
// serial queue) while the calling NAN method blocks. Here that "different
// execution context" is MicCaptureThreadProc: it must run off the JS thread
// because it makes WASAPI/COM calls, and the JS thread is never
// CoInitializeEx'd (see file header thread model).
NAN_METHOD(StartMicrophoneCapture) {
  if (g_micChannel.callback.IsEmpty()) {
    info.GetReturnValue().Set(MicStartResult(false, "callback_empty"));
    return;
  }

  std::unique_lock<std::mutex> lk(g_micState.engineLock);
  // Authoritative reset, unconditionally — matches the mac reference exactly
  // (SAYSO-353 review finding): an explicit Start means any prior recovery
  // state no longer applies, even if the already-active guard below is about
  // to reject this call. Without this, a stale backoff retry's generation
  // check could otherwise leave routeRecovering stuck true across a
  // Stop-then-quick-Start sequence.
  g_micState.routeRecovering.store(false, std::memory_order_release);

  if (g_micState.capturing.load(std::memory_order_acquire)) {
    info.GetReturnValue().Set(MicStartResult(false, "already_active"));
    return;
  }

  MicStartContext* ctx = new MicStartContext();
  ctx->readyEvent = CreateEventW(nullptr, TRUE, FALSE, nullptr);
  ctx->stopEvent = CreateEventW(nullptr, TRUE, FALSE, nullptr);
  if (!ctx->readyEvent || !ctx->stopEvent) {
    if (ctx->readyEvent) CloseHandle(ctx->readyEvent);
    if (ctx->stopEvent) CloseHandle(ctx->stopEvent);
    delete ctx;
    info.GetReturnValue().Set(MicStartResult(false, "no_input_node"));
    return;
  }

  HANDLE thread = CreateThread(nullptr, 0, MicCaptureThreadProc, ctx, 0, nullptr);
  if (!thread) {
    CloseHandle(ctx->readyEvent);
    CloseHandle(ctx->stopEvent);
    delete ctx;
    info.GetReturnValue().Set(MicStartResult(false, "no_input_node"));
    return;
  }

  // Release engineLock for the wait below — this NAN method runs on
  // Electron's main (JS) thread, so holding the lock here for up to 6s does
  // not protect anything JS-side (JS is single-threaded: no other call into
  // this addon can be in flight concurrently with this one regardless of
  // what we hold), it only serializes against a background route-recovery
  // attempt on a thread-pool thread. Recovery only ever runs while
  // g_micState.capturing is true, and we haven't set it true yet, so
  // releasing here is safe — recovery will just see capturing == false and
  // no-op. Not releasing was the actual bug (SAYSO client review): a
  // concurrent recovery attempt holding engineLock across its own ~7s of
  // WASAPI work would make even this uncontended-in-practice lock
  // acquisition block for that long, stacking on top of this wait.
  lk.unlock();

  // Bounded wait: both retry attempts' first-buffer windows (1200 + 1500ms)
  // plus generous slack for device open overhead.
  DWORD waitResult = WaitForSingleObject(ctx->readyEvent, 6000);
  if (waitResult != WAIT_OBJECT_0) {
    // The thread may still be doing WASAPI work with no safe way to
    // interrupt it mid-open-sequence. Try to claim the abandon path first —
    // only if we win do we drop our reference to the thread object (the
    // thread keeps running independently either way; this merely stops us
    // from waiting on/joining it). If we win, the capture thread will notice
    // ctx->abandoned at its next check and owns freeing everything (ctx,
    // both events, and the WASAPI objects if it turns out to have actually
    // succeeded) itself — we touch nothing further.
    EmitLifecycleEvent("mic_start_wait_timeout");
    if (TrySettle(ctx, /*asAbandoned=*/true)) {
      CloseHandle(thread);
      info.GetReturnValue().Set(MicStartResult(false, "engine_start_failed"));
      return;
    }
    // We lost the race: the capture thread had already settled this as
    // "not abandoned" a moment before our timeout fired (an extremely
    // narrow window — the whole open sequence normally completes in low
    // seconds against this 6s wait) and believes we will read ctx, free it,
    // and — on success — take ownership of `thread` normally. Do not close
    // `thread` here; wait for the event it's about to (or already did)
    // signal, then fall through to the exact same handling as the
    // WAIT_OBJECT_0 path below, which still owns `thread` correctly.
    WaitForSingleObject(ctx->readyEvent, INFINITE);
  }

  if (!ctx->started.load(std::memory_order_acquire)) {
    const char* reason = ctx->failReason ? ctx->failReason : "no_tap_buffers";
    CloseHandle(thread);
    CloseHandle(ctx->readyEvent);
    CloseHandle(ctx->stopEvent);
    delete ctx;
    info.GetReturnValue().Set(MicStartResult(false, reason));
    return;
  }

  // Reacquire before touching g_micState. No re-validation of `capturing` is
  // needed here: this is the only code path that ever sets it true, JS is
  // single-threaded so no second Start call could have run concurrently with
  // this one, and route recovery never acts while capturing is false — so
  // nothing could have changed while we were unlocked.
  lk.lock();

  g_micState.device = ctx->device;
  g_micState.audioClient = ctx->audioClient;
  g_micState.captureClient = ctx->captureClient;
  g_micState.format = ctx->format;
  g_micState.openedEndpointId = ctx->endpointId;
  g_micState.captureThread = thread;
  g_micState.stopEvent = ctx->stopEvent;
  CloseHandle(ctx->readyEvent);
  delete ctx;
  g_micState.capturing.store(true, std::memory_order_release);
  g_micState.routeRecovering.store(false, std::memory_order_release);

  info.GetReturnValue().Set(MicStartResult(true));
}

NAN_METHOD(StopMicrophoneCapture) {
  // Bumped BEFORE taking the lock, unconditionally, matching the mac
  // reference: this invalidates any in-flight debounce/backoff chain (they
  // check g_micState.routeRestartGeneration before doing any work) even if
  // one happens to be running concurrently on a thread-pool timer thread
  // right now — an explicit Stop is authoritative.
  g_micState.routeRestartGeneration.fetch_add(1, std::memory_order_acq_rel);

  SessionClaim claim;
  {
    std::unique_lock<std::mutex> lk(g_micState.engineLock);
    if (!g_micState.capturing.load(std::memory_order_acquire)) {
      info.GetReturnValue().Set(Nan::True());
      return;
    }

    if (g_micState.claimActive.load(std::memory_order_acquire)) {
      // A route-recovery attempt currently holds this session's handles via
      // a SessionClaim and may be actively using them right now (not just
      // scheduled for a later retry). Wait for it to finish — it always
      // clears claimActive and notifies claimReleasedCv before releasing
      // engineLock, whatever the outcome — so this call never reports
      // "stopped" while that work (and the OS thread it may ultimately
      // own) could still be live. Bounded generously above that work's own
      // worst case (teardown join 3s + up to two open attempts
      // 2.5s+1.5s ≈ 7s) so a normal in-flight attempt is never mistaken for
      // a wedge.
      bool cleared = g_micState.claimReleasedCv.wait_for(lk, std::chrono::milliseconds(8000), [] {
        return !g_micState.claimActive.load(std::memory_order_acquire);
      });
      // Not a correctness gap if this times out — see
      // StopSystemAudioCapture's identical reasoning: the captureThread
      // null-check right below still gates claiming regardless of
      // `cleared`, since captureThread is guaranteed null while claimActive
      // is true. Still worth a diagnostic.
      if (!cleared) {
        EmitLifecycleEvent("mic_stop_claim_wait_timeout");
      }
      // No need to re-check capturing here: nothing but this very call can
      // clear it (JS is single-threaded, so no second Stop can be racing
      // us), and route-recovery never touches it on its own discard path —
      // only routeRecovering.
    }

    if (!g_micState.captureThread) {
      // Still nothing to join — either nothing was ever active, the claim
      // holder discarded its attempt without publishing a new session
      // (routeRecovering handles its own state in that case), or we gave
      // up waiting on a wedged one. Finalize directly; there's nothing left
      // in g_micState to tear down.
      g_micState.capturing.store(false, std::memory_order_release);
      g_micState.routeRecovering.store(false, std::memory_order_release);
      info.GetReturnValue().Set(Nan::True());
      return;
    }

    // A live session exists (either it was there all along, or a claim
    // holder published a fresh one while we waited) — claim it the same
    // way route-recovery does, so the join wait below doesn't need
    // engineLock either.
    claim = ClaimSessionLocked(g_micState);
  }

  // Bounded wait, unlocked — see PathState::captureThread's join-before-
  // release requirement. A timeout here is a real bug (device/driver
  // wedged the thread), not something to paper over: JoinAndReleaseSessionClaim
  // logs it and deliberately does NOT release the WASAPI objects (they may
  // still be in use), accepting a one-time resource leak over a
  // use-after-free.
  bool torn = JoinAndReleaseSessionClaim(claim, "mic_capture_thread_join_timeout");

  std::lock_guard<std::mutex> lk2(g_micState.engineLock);
  // Bug fix: ClaimSessionLocked above set claimActive=true; this call is
  // the one that took the claim, so it's also the one responsible for
  // clearing it and waking anyone waiting on claimReleasedCv (another Stop
  // call, or a concurrent route-restart attempt) — forgetting this left
  // claimActive stuck true forever after every normal Stop, making every
  // subsequent Stop call block for the full 8s wait below for no reason.
  g_micState.claimActive.store(false, std::memory_order_release);
  g_micState.claimReleasedCv.notify_all();
  if (torn) {
    // Safe only here: the thread is provably joined, so nothing can still
    // be racing Push() against this. Drops any chunks the thread queued in
    // its last poll(s) before it noticed stopEvent — without this, a
    // handful of already-captured buffers can drain to JS a few ms after
    // this call resolves, which is confusing for a consumer that just
    // started a new session. Also closes the async handle;
    // setMicrophoneStreamingCallback lazily recreates it on the next
    // session.
    g_micChannel.CloseAsyncHandle();
  }
  g_micState.capturing.store(false, std::memory_order_release);
  g_micState.routeRecovering.store(false, std::memory_order_release);
  info.GetReturnValue().Set(Nan::True());
}

NAN_METHOD(IsMicrophoneCaptureActive) {
  info.GetReturnValue().Set(Nan::New<v8::Boolean>(g_micState.capturing.load(std::memory_order_acquire)));
}

NAN_METHOD(IsMicRouteRecovering) {
  info.GetReturnValue().Set(Nan::New<v8::Boolean>(g_micState.routeRecovering.load(std::memory_order_acquire)));
}

// ── Streaming callbacks ─────────────────────────────────────────────────────

NAN_METHOD(SetStreamingCallback) {
  if (info.Length() < 1 || info[0]->IsNull() || info[0]->IsUndefined()) {
    // Clears the callback only — never closes the async handle here. See
    // AudioDeliveryChannel::CloseAsyncHandle's comment for why (SAYSO-349
    // class; also closes a TOCTOU race the mac reference has on this path).
    g_systemChannel.callback.Reset();
    info.GetReturnValue().Set(Nan::True());
    return;
  }
  if (!info[0]->IsFunction()) {
    Nan::ThrowTypeError("Callback must be a function");
    return;
  }
  g_systemChannel.callback.Reset(Nan::To<Function>(info[0]).ToLocalChecked());
  g_systemChannel.EnsureAsyncHandle(ChannelDrainCallback);
  info.GetReturnValue().Set(Nan::True());
}

NAN_METHOD(SetMicrophoneStreamingCallback) {
  if (info.Length() < 1 || info[0]->IsNull() || info[0]->IsUndefined()) {
    g_micChannel.callback.Reset();
    info.GetReturnValue().Set(Nan::True());
    return;
  }
  if (!info[0]->IsFunction()) {
    Nan::ThrowTypeError("Callback must be a function");
    return;
  }
  g_micChannel.callback.Reset(Nan::To<Function>(info[0]).ToLocalChecked());
  g_micChannel.EnsureAsyncHandle(ChannelDrainCallback);
  info.GetReturnValue().Set(Nan::True());
}

// Diagnostics channel (SAYSO-355). The async handle is created once on first
// registration and — unlike the two streaming channels above — is NEVER
// closed by anything in this file, matching the mac reference and the
// contract's explicit SAYSO-349 rule for this specific channel.
NAN_METHOD(SetLifecycleEventCallback) {
  if (info.Length() < 1 || info[0]->IsNull() || info[0]->IsUndefined()) {
    g_lifecycleCallback.Reset();
    info.GetReturnValue().Set(Nan::True());
    return;
  }
  if (!info[0]->IsFunction()) {
    Nan::ThrowTypeError("Callback must be a function");
    return;
  }
  g_lifecycleCallback.Reset(Nan::To<Function>(info[0]).ToLocalChecked());
  {
    std::lock_guard<std::mutex> lk(g_lifecycleMutex);
    if (!g_lifecycleAsyncHandle) {
      g_lifecycleAsyncHandle = new uv_async_t();
      uv_async_init(uv_default_loop(), g_lifecycleAsyncHandle, LifecycleAsyncCallback);
    }
  }
  info.GetReturnValue().Set(Nan::True());
}

}  // namespace

// ── Module init — exports exactly the 13-method frozen surface ─────────────
NAN_MODULE_INIT(Init) {
  Nan::Set(target, Nan::New("initialize").ToLocalChecked(),
           Nan::GetFunction(Nan::New<v8::FunctionTemplate>(Initialize)).ToLocalChecked());
  Nan::Set(target, Nan::New("checkScreenRecordingGranted").ToLocalChecked(),
           Nan::GetFunction(Nan::New<v8::FunctionTemplate>(CheckScreenRecordingGranted)).ToLocalChecked());
  Nan::Set(target, Nan::New("requestScreenRecordingPermission").ToLocalChecked(),
           Nan::GetFunction(Nan::New<v8::FunctionTemplate>(RequestScreenRecordingPermission)).ToLocalChecked());
  Nan::Set(target, Nan::New("startSystemAudioCapture").ToLocalChecked(),
           Nan::GetFunction(Nan::New<v8::FunctionTemplate>(StartSystemAudioCapture)).ToLocalChecked());
  Nan::Set(target, Nan::New("stopSystemAudioCapture").ToLocalChecked(),
           Nan::GetFunction(Nan::New<v8::FunctionTemplate>(StopSystemAudioCapture)).ToLocalChecked());
  Nan::Set(target, Nan::New("isSystemAudioCaptureActive").ToLocalChecked(),
           Nan::GetFunction(Nan::New<v8::FunctionTemplate>(IsSystemAudioCaptureActive)).ToLocalChecked());
  Nan::Set(target, Nan::New("startMicrophoneCapture").ToLocalChecked(),
           Nan::GetFunction(Nan::New<v8::FunctionTemplate>(StartMicrophoneCapture)).ToLocalChecked());
  Nan::Set(target, Nan::New("stopMicrophoneCapture").ToLocalChecked(),
           Nan::GetFunction(Nan::New<v8::FunctionTemplate>(StopMicrophoneCapture)).ToLocalChecked());
  Nan::Set(target, Nan::New("isMicrophoneCaptureActive").ToLocalChecked(),
           Nan::GetFunction(Nan::New<v8::FunctionTemplate>(IsMicrophoneCaptureActive)).ToLocalChecked());
  Nan::Set(target, Nan::New("isMicRouteRecovering").ToLocalChecked(),
           Nan::GetFunction(Nan::New<v8::FunctionTemplate>(IsMicRouteRecovering)).ToLocalChecked());
  Nan::Set(target, Nan::New("setStreamingCallback").ToLocalChecked(),
           Nan::GetFunction(Nan::New<v8::FunctionTemplate>(SetStreamingCallback)).ToLocalChecked());
  Nan::Set(target, Nan::New("setMicrophoneStreamingCallback").ToLocalChecked(),
           Nan::GetFunction(Nan::New<v8::FunctionTemplate>(SetMicrophoneStreamingCallback)).ToLocalChecked());
  Nan::Set(target, Nan::New("setLifecycleEventCallback").ToLocalChecked(),
           Nan::GetFunction(Nan::New<v8::FunctionTemplate>(SetLifecycleEventCallback)).ToLocalChecked());
}

NODE_MODULE(native_audio, Init)
