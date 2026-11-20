#include "aec_engine.h"

#ifdef SAYSO_HAVE_WEBRTC_APM
#include "sayso_apm.h"
#endif

namespace sayso::aec {

#ifdef SAYSO_HAVE_WEBRTC_APM

namespace {

class WebRtcAecEngine : public AecEngine {
 public:
  WebRtcAecEngine(SaysoApm* apm, int sampleRate) : apm_(apm), frameSamples_(static_cast<size_t>(sampleRate / 100)) {}
  ~WebRtcAecEngine() override { sayso_apm_destroy(apm_); }

  const char* Name() const override { return "webrtc_aec3"; }

  int ProcessRender(const float* frame) override { return sayso_apm_process_render(apm_, frame, frameSamples_); }

  // Stream delay 0: the pipeline already lines capture and reference up by
  // timestamp, and AEC3 estimates the remaining acoustic delay itself.
  int ProcessCapture(const float* in, float* out) override {
    return sayso_apm_process_capture(apm_, in, out, frameSamples_, 0);
  }

  bool GetStats(EngineStats* stats) override {
    SaysoApmStats s;
    if (sayso_apm_get_stats(apm_, &s) != 0) {
      return false;
    }
    stats->delayMs = s.delay_ms;
    stats->erlDb = s.echo_return_loss;
    stats->erleDb = s.echo_return_loss_enhancement;
    return true;
  }

 private:
  SaysoApm* apm_;
  const size_t frameSamples_;
};

}  // namespace

std::unique_ptr<AecEngine> CreateAecEngine(int sampleRate, std::string* failReason) {
  SaysoApm* apm = sayso_apm_create(sampleRate);
  if (!apm) {
    *failReason = "create_failed";
    return nullptr;
  }
  return std::make_unique<WebRtcAecEngine>(apm, sampleRate);
}

#else

std::unique_ptr<AecEngine> CreateAecEngine(int /*sampleRate*/, std::string* failReason) {
  *failReason = "not_built";
  return nullptr;
}

#endif

}  // namespace sayso::aec
