#include "echo_engine.h"

#include <cstring>

#ifdef SAYSO_HAVE_APM
#include "sayso_apm.h"
#endif

namespace sayso::echo {

struct EchoEngine::Impl {
#ifdef SAYSO_HAVE_APM
  sayso_apm* apm = nullptr;
#endif
  EngineStats stats;
};

EchoEngine::EchoEngine() : impl_(std::make_unique<Impl>()) {}

EchoEngine::~EchoEngine() { Stop(); }

bool EchoEngine::IsCompiledIn() {
#ifdef SAYSO_HAVE_APM
  return true;
#else
  return false;
#endif
}

const char* EchoEngine::Version() {
#ifdef SAYSO_HAVE_APM
  return sayso_apm_version();
#else
  return nullptr;
#endif
}

bool EchoEngine::Start() {
#ifdef SAYSO_HAVE_APM
  if (impl_->apm) return true;
  impl_->apm = sayso_apm_create();
  if (!impl_->apm) {
    impl_->stats.startFailures++;
    return false;
  }
  return true;
#else
  impl_->stats.startFailures++;
  return false;
#endif
}

void EchoEngine::Stop() {
#ifdef SAYSO_HAVE_APM
  if (impl_->apm) {
    sayso_apm_destroy(impl_->apm);
    impl_->apm = nullptr;
  }
#endif
  impl_->stats.hasErle = false;
  impl_->stats.erleDb = 0;
  impl_->stats.hasDelay = false;
  impl_->stats.delayMs = 0;
}

bool EchoEngine::running() const {
#ifdef SAYSO_HAVE_APM
  return impl_->apm != nullptr;
#else
  return false;
#endif
}

bool EchoEngine::Reset() {
  Stop();
  impl_->stats.resets++;
  return Start();
}

bool EchoEngine::ProcessReference(const int16_t* frame) {
#ifdef SAYSO_HAVE_APM
  if (!impl_->apm || !frame) return false;
  if (sayso_apm_process_reverse(impl_->apm, frame) != 0) {
    impl_->stats.errors++;
    return false;
  }
  impl_->stats.referenceFrames++;
  return true;
#else
  (void)frame;
  return false;
#endif
}

bool EchoEngine::ProcessCapture(const int16_t* in, int16_t* out) {
  if (!in || !out) return false;
#ifdef SAYSO_HAVE_APM
  if (impl_->apm) {
    if (sayso_apm_process_capture(impl_->apm, in, out) == 0) {
      impl_->stats.captureFrames++;
      return true;
    }
    impl_->stats.errors++;
  }
#endif
  if (in != out) std::memcpy(out, in, kFrameSamples * sizeof(int16_t));
  impl_->stats.passthroughFrames++;
  return false;
}

EngineStats EchoEngine::PollStats() {
#ifdef SAYSO_HAVE_APM
  if (impl_->apm) {
    sayso_apm_stats st;
    sayso_apm_get_stats(impl_->apm, &st);
    impl_->stats.hasErle = st.has_erle != 0;
    impl_->stats.erleDb = st.erle_db;
    impl_->stats.hasDelay = st.has_delay != 0;
    impl_->stats.delayMs = st.delay_ms;
  }
#endif
  return impl_->stats;
}

}
