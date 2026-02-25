#ifndef lookupTables_h
#define lookupTables_h

#include <stdint.h>

#define SAMPLE_RATE 1000
#define MIN_BPM 30
#define MAX_BPM 300
#define MAX_BPM_FRACTIONAL 10
#define UPDATE_FREQ 8000

extern const uint32_t lut_bpm_inc[271];
extern const uint32_t lut_bpm_fractional_inc[10];
extern const float lut_phase_inc[256];
extern const float lut_beat_length_inc[271];
extern const float lut_exp[1024];
extern const float lut_inv_exp[1024];

#endif