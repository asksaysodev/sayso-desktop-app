// C interface to WebRTC AudioProcessing (AEC3), built as a static library
// inside a WebRTC checkout (see BUILD.gn / scripts/build-webrtc-apm.ps1) and
// linked into native_audio.node. Plain C so the addon, compiled by MSVC, never
// depends on WebRTC's C++ ABI, which is built by WebRTC's own clang-cl.
//
// All calls for one handle must come from one thread at a time.

#ifndef SAYSO_APM_H_
#define SAYSO_APM_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SaysoApm SaysoApm;

typedef struct SaysoApmStats {
  // Each field is -1 when WebRTC has not produced a value yet.
  int delay_ms;
  double echo_return_loss;              // dB
  double echo_return_loss_enhancement;  // dB
  double divergent_filter_fraction;     // 0..1
  double residual_echo_likelihood;      // 0..1
} SaysoApmStats;

// Mono, echo cancellation only (noise suppression and gain control off).
// sample_rate_hz must be one WebRTC supports natively: 16000, 32000 or 48000.
// Returns NULL on failure.
SaysoApm* sayso_apm_create(int sample_rate_hz);

// Exactly one 10 ms frame (sample_rate_hz / 100 samples). Returns 0 on success,
// a negative WebRTC error code otherwise.
int sayso_apm_process_render(SaysoApm* apm, const float* frame, size_t samples);
int sayso_apm_process_capture(SaysoApm* apm, const float* in, float* out, size_t samples, int stream_delay_ms);

int sayso_apm_get_stats(SaysoApm* apm, SaysoApmStats* stats);

void sayso_apm_destroy(SaysoApm* apm);

#ifdef __cplusplus
}
#endif

#endif  // SAYSO_APM_H_
