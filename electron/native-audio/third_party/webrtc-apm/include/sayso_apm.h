#ifndef SAYSO_APM_H_
#define SAYSO_APM_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SAYSO_APM_SAMPLE_RATE 16000
#define SAYSO_APM_FRAME_SAMPLES 160

typedef struct sayso_apm sayso_apm;

typedef struct {
  int has_erle;
  double erle_db;
  int has_delay;
  double delay_ms;
} sayso_apm_stats;

sayso_apm* sayso_apm_create(void);
void sayso_apm_destroy(sayso_apm* apm);

int sayso_apm_process_reverse(sayso_apm* apm, const int16_t* frame);

int sayso_apm_process_capture(sayso_apm* apm, const int16_t* in, int16_t* out);

void sayso_apm_get_stats(sayso_apm* apm, sayso_apm_stats* out);

const char* sayso_apm_version(void);

#ifdef __cplusplus
}
#endif
#endif
