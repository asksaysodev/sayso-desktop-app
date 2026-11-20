#include "sayso_apm/sayso_apm.h"

#include <algorithm>
#include <memory>
#include <vector>

#include "api/audio/audio_processing.h"
#include "api/audio/builtin_audio_processing_builder.h"
#include "api/environment/environment_factory.h"

struct SaysoApm {
  webrtc::scoped_refptr<webrtc::AudioProcessing> apm;
  webrtc::StreamConfig config;
  // ProcessReverseStream wants a destination; the render output is unused,
  // so it lands here instead of allocating per frame.
  std::vector<float> renderScratch;

  SaysoApm(webrtc::scoped_refptr<webrtc::AudioProcessing> processor, int rate)
      : apm(std::move(processor)), config(rate, 1), renderScratch(config.num_frames()) {}
};

extern "C" {

SaysoApm* sayso_apm_create(int sample_rate_hz) {
  if (sample_rate_hz != 16000 && sample_rate_hz != 32000 && sample_rate_hz != 48000) {
    return nullptr;
  }
  // Same configuration the Windows POC validated: echo cancellation alone, so
  // nothing else changes what transcription hears.
  webrtc::AudioProcessing::Config config;
  config.echo_canceller.enabled = true;
  config.noise_suppression.enabled = false;
  config.gain_controller1.enabled = false;
  config.gain_controller2.enabled = false;

  auto apm = webrtc::BuiltinAudioProcessingBuilder(config).Build(webrtc::CreateEnvironment());
  if (!apm) {
    return nullptr;
  }
  return new SaysoApm(std::move(apm), sample_rate_hz);
}

int sayso_apm_process_render(SaysoApm* apm, const float* frame, size_t samples) {
  if (!apm || !frame) return -1;
  if (samples != apm->config.num_frames()) return -2;
  const float* src[] = {frame};
  float* dst[] = {apm->renderScratch.data()};
  return apm->apm->ProcessReverseStream(src, apm->config, apm->config, dst);
}

int sayso_apm_process_capture(SaysoApm* apm, const float* in, float* out, size_t samples, int stream_delay_ms) {
  if (!apm || !in || !out) return -1;
  if (samples != apm->config.num_frames()) return -2;
  apm->apm->set_stream_delay_ms(std::max(0, stream_delay_ms));
  const float* src[] = {in};
  float* dst[] = {out};
  return apm->apm->ProcessStream(src, apm->config, apm->config, dst);
}

int sayso_apm_get_stats(SaysoApm* apm, SaysoApmStats* stats) {
  if (!apm || !stats) return -1;
  const webrtc::AudioProcessingStats s = apm->apm->GetStatistics();
  stats->delay_ms = s.delay_ms.value_or(-1);
  stats->echo_return_loss = s.echo_return_loss.value_or(-1);
  stats->echo_return_loss_enhancement = s.echo_return_loss_enhancement.value_or(-1);
  stats->divergent_filter_fraction = s.divergent_filter_fraction.value_or(-1);
  stats->residual_echo_likelihood = s.residual_echo_likelihood.value_or(-1);
  return 0;
}

void sayso_apm_destroy(SaysoApm* apm) { delete apm; }

}  // extern "C"
