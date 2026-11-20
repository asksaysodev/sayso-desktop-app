#include "sayso_apm.h"

#include <webrtc/modules/audio_processing/include/audio_processing.h>

#include <new>

#ifndef SAYSO_APM_VERSION
#define SAYSO_APM_VERSION "webrtc-audio-processing"
#endif

struct sayso_apm {
  rtc::scoped_refptr<webrtc::AudioProcessing> apm;
  webrtc::StreamConfig config{SAYSO_APM_SAMPLE_RATE, 1};
};

extern "C" {

sayso_apm* sayso_apm_create(void) {
  sayso_apm* h = new (std::nothrow) sayso_apm();
  if (!h) return nullptr;
  h->apm = webrtc::AudioProcessingBuilder().Create();
  if (!h->apm) {
    delete h;
    return nullptr;
  }
  webrtc::AudioProcessing::Config cfg;
  cfg.echo_canceller.enabled = true;
  cfg.echo_canceller.mobile_mode = false;
  cfg.high_pass_filter.enabled = true;
  cfg.noise_suppression.enabled = false;
  cfg.gain_controller1.enabled = false;
  cfg.gain_controller2.enabled = false;
  h->apm->ApplyConfig(cfg);
  return h;
}

void sayso_apm_destroy(sayso_apm* apm) { delete apm; }

int sayso_apm_process_reverse(sayso_apm* apm, const int16_t* frame) {
  if (!apm || !frame) return -1;
  int16_t scratch[SAYSO_APM_FRAME_SAMPLES];
  return apm->apm->ProcessReverseStream(frame, apm->config, apm->config, scratch);
}

int sayso_apm_process_capture(sayso_apm* apm, const int16_t* in, int16_t* out) {
  if (!apm || !in || !out) return -1;
  return apm->apm->ProcessStream(in, apm->config, apm->config, out);
}

void sayso_apm_get_stats(sayso_apm* apm, sayso_apm_stats* out) {
  if (!out) return;
  *out = sayso_apm_stats{};
  if (!apm) return;
  const auto st = apm->apm->GetStatistics();
  if (st.echo_return_loss_enhancement) {
    out->has_erle = 1;
    out->erle_db = *st.echo_return_loss_enhancement;
  }
  if (st.delay_ms) {
    out->has_delay = 1;
    out->delay_ms = static_cast<double>(*st.delay_ms);
  }
}

const char* sayso_apm_version(void) { return SAYSO_APM_VERSION; }

}
