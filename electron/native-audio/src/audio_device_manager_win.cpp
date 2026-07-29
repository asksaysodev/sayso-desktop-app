// ============================================================================
// native_audio — Windows stub (WASAPI)
//
// Compiling scaffold for the Windows audio backend. Exports the LIVE NAN surface
// index.js actually calls (11 methods) so native-audio/index.js can load
// build/Release/native_audio.node identically on both platforms. Every method is
// intentionally NOT IMPLEMENTED — it throws or returns a safe default — until the
// WASAPI capture work lands.
//
// NOTE: the macOS .mm additionally exports listOutputDevices /
// createMultiOutputDevice / deleteMultiOutputDevice — these are DEAD (zero JS
// callers, removed in SAYSO-327) and deliberately NOT part of this surface. See
// docs/NATIVE_AUDIO_CONTRACT.md ("do not implement these on Windows").
//
// Interface spec:            docs/NATIVE_AUDIO_CONTRACT.md
// Architecture / WASAPI plan: docs/AUDIO_MODULE_WINDOWS_ASSESSMENT.md
//
// Implementation notes for whoever fills this in:
//   * System audio  = WASAPI loopback on the default render endpoint
//                     ("what the prospect says"). Must exclude Sayso's own
//                     render session (self-exclusion), mirroring macOS
//                     excludesCurrentProcessAudio.
//   * Microphone    = WASAPI capture on the default capture endpoint
//                     ("what the user says"), delivered on a SEPARATE callback.
//   * Deliver PCM to JS ONLY via uv_async_send onto the libuv loop thread —
//     never call into V8 from the WASAPI audio thread. Report the buffer's true
//     { sampleRate, channels, bitDepth, isFloat }; audioConverter.js resamples.
//   * Device-change resilience (IMMNotificationClient) — see the contract's
//     "reliability bar" section. Not required for this stub to compile.
// ============================================================================

#include <nan.h>
#include <string>

namespace {

// Uniform "not implemented" throw for the action (Promise-returning) methods.
// index.js awaits these, so a synchronous throw surfaces as a rejected promise.
inline void NotImplemented(const char* fn) {
  std::string msg = "native_audio(win): ";
  msg += fn;
  msg += " not implemented — WASAPI TODO (see docs/NATIVE_AUDIO_CONTRACT.md)";
  Nan::ThrowError(msg.c_str());
}

// ── Lifecycle / permissions ────────────────────────────────────────────────

// Promise<void>. Allocate WASAPI clients + register the device-change listener.
NAN_METHOD(Initialize) {
  NotImplemented("initialize");
}

// boolean (sync, non-prompting). Windows has no screen-recording gate for
// loopback capture — return true, matching NATIVE_AUDIO_CONTRACT.md and the
// WindowsAudioProvider it will replace. (This is the one method whose "real"
// value is already known, so the stub returns it rather than a false default.)
NAN_METHOD(CheckScreenRecordingGranted) {
  info.GetReturnValue().Set(Nan::True());
}

// Promise<void>. No OS screen-recording prompt on Windows; likely a no-op once
// implemented. Stub throws to signal "not wired yet".
NAN_METHOD(RequestScreenRecordingPermission) {
  NotImplemented("requestScreenRecordingPermission");
}

// ── System audio (loopback / "prospect") ───────────────────────────────────

// Promise<boolean>. Start WASAPI loopback; resolve true once audio flows.
// Reject on failure or when a start/stop is already in flight. Self-exclude
// Sayso's own render session.
NAN_METHOD(StartSystemAudioCapture) {
  NotImplemented("startSystemAudioCapture");
}

// Promise<{ success, filePath?, actualStartMs?, error? }>. Stop + full teardown,
// resolve after teardown so the next start is safe. filePath/actualStartMs are
// legacy (streaming-only now) — return { success: true } or { success: false, error }.
NAN_METHOD(StopSystemAudioCapture) {
  NotImplemented("stopSystemAudioCapture");
}

// boolean (sync). Liveness of the system-audio path.
NAN_METHOD(IsSystemAudioCaptureActive) {
  info.GetReturnValue().Set(Nan::False());
}

// ── Microphone ("user") ────────────────────────────────────────────────────

// boolean. Start WASAPI capture on the default capture endpoint. Delivers via
// the microphone callback (separate from the system-audio callback).
NAN_METHOD(StartMicrophoneCapture) {
  info.GetReturnValue().Set(Nan::False());
}

// boolean. Stop the mic path ONLY — must not tear down system audio.
NAN_METHOD(StopMicrophoneCapture) {
  info.GetReturnValue().Set(Nan::False());
}

// boolean (sync). Liveness of the mic path.
NAN_METHOD(IsMicrophoneCaptureActive) {
  info.GetReturnValue().Set(Nan::False());
}

// ── Streaming callbacks ─────────────────────────────────────────────────────

// void. Register/clear the JS callback for SYSTEM-AUDIO chunks: callback(buffer, format).
// Store as a persistent Nan::Callback; invoke only from the uv_async handler.
NAN_METHOD(SetStreamingCallback) {
  // no-op stub
}

// void. Register/clear the JS callback for MICROPHONE chunks. Kept separate so
// stopping one path never silences the other.
NAN_METHOD(SetMicrophoneStreamingCallback) {
  // no-op stub
}

}  // namespace

// ── Module init — must export the exact surface index.js expects ────────────
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
  Nan::Set(target, Nan::New("setStreamingCallback").ToLocalChecked(),
           Nan::GetFunction(Nan::New<v8::FunctionTemplate>(SetStreamingCallback)).ToLocalChecked());
  Nan::Set(target, Nan::New("setMicrophoneStreamingCallback").ToLocalChecked(),
           Nan::GetFunction(Nan::New<v8::FunctionTemplate>(SetMicrophoneStreamingCallback)).ToLocalChecked());
}

NODE_MODULE(native_audio, Init)
