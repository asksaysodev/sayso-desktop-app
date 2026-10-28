// NAN glue: start(callback) / stop() / mode().
// Same delivery model as native-audio's lifecycle channel: producers push onto a
// mutex-guarded queue and uv_async_send; the JS thread swaps the queue out and
// invokes the callback once per event.
#include <nan.h>

#include <chrono>
#include <mutex>
#include <vector>

#include "mic_activity.h"

using v8::Function;
using v8::Local;
using v8::Object;
using v8::Value;

namespace {

std::mutex g_mutex;
std::vector<MicEvent> g_pending;
uv_async_t* g_async = nullptr;  // guarded by g_mutex; null when stopped
Nan::Persistent<Function> g_callback;
bool g_running = false;  // JS thread only

void SetString(Local<Object> obj, const char* key, const std::string& value) {
    if (value.empty()) return;
    Nan::Set(obj, Nan::New(key).ToLocalChecked(), Nan::New(value).ToLocalChecked());
}

void SetNumber(Local<Object> obj, const char* key, double value) {
    Nan::Set(obj, Nan::New(key).ToLocalChecked(), Nan::New<v8::Number>(value));
}

void DrainAsync(uv_async_t* /*handle*/) {
    std::vector<MicEvent> events;
    {
        std::lock_guard<std::mutex> lk(g_mutex);
        events.swap(g_pending);
    }
    if (g_callback.IsEmpty()) return;

    Nan::HandleScope scope;
    Local<Function> callback = Nan::New(g_callback);
    for (const auto& event : events) {
        Local<Object> obj = Nan::New<Object>();
        Nan::Set(obj, Nan::New("type").ToLocalChecked(),
                 Nan::New(event.running ? "start" : "stop").ToLocalChecked());
        SetNumber(obj, "pid", event.pid);
        SetNumber(obj, "responsiblePid", event.responsiblePid);
        SetString(obj, "bundleId", event.bundleId);
        SetString(obj, "appPath", event.appPath);
        SetString(obj, "appBundleId", event.appBundleId);
        SetString(obj, "exePath", event.exePath);
        SetNumber(obj, "ts", event.ts);
        Local<Value> argv[1] = {obj};
        Nan::Call(callback, Nan::GetCurrentContext()->Global(), 1, argv);
    }
}

void CloseAsync() {
    uv_async_t* handle;
    {
        std::lock_guard<std::mutex> lk(g_mutex);
        handle = g_async;
        g_async = nullptr;
        g_pending.clear();
    }
    if (handle) {
        uv_close(reinterpret_cast<uv_handle_t*>(handle),
                 [](uv_handle_t* h) { delete reinterpret_cast<uv_async_t*>(h); });
    }
}

}  // namespace

void EmitMicEvent(MicEvent event) {
    std::lock_guard<std::mutex> lk(g_mutex);
    if (!g_async) return;
    g_pending.push_back(std::move(event));
    uv_async_send(g_async);
}

double MicEventNowMs() {
    using namespace std::chrono;
    return static_cast<double>(
        duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());
}

NAN_METHOD(Start) {
    if (info.Length() < 1 || !info[0]->IsFunction()) {
        return Nan::ThrowTypeError("start(callback) requires a function");
    }
    if (g_running) {
        return Nan::ThrowError("mic-activity monitor already running");
    }

    g_callback.Reset(info[0].As<Function>());
    auto* handle = new uv_async_t;
    uv_async_init(Nan::GetCurrentEventLoop(), handle, DrainAsync);
    {
        std::lock_guard<std::mutex> lk(g_mutex);
        g_async = handle;
    }

    std::string error;
    if (!PlatformStart(&error)) {
        CloseAsync();
        g_callback.Reset();
        return Nan::ThrowError(error.empty() ? "mic-activity start failed" : error.c_str());
    }
    g_running = true;
}

NAN_METHOD(Stop) {
    if (!g_running) return;
    PlatformStop();
    CloseAsync();
    g_callback.Reset();
    g_running = false;
}

NAN_METHOD(Mode) {
    info.GetReturnValue().Set(Nan::New(PlatformMode()).ToLocalChecked());
}

NAN_MODULE_INIT(Init) {
    Nan::SetMethod(target, "start", Start);
    Nan::SetMethod(target, "stop", Stop);
    Nan::SetMethod(target, "mode", Mode);
}

NODE_MODULE(mic_activity, Init)
