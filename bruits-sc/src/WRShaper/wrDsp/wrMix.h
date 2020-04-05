#pragma once

#include "wrFilter.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct mix_tanh {
    float bus; // mix bus
    filter_dc_t* hpf;
    uint16_t b_size; // block size
    float* bus_v; // mix bus vector
} mix_tanh_t;

typedef struct mix_peaks {
    float bus; // mix bus
    uint16_t b_size; // block size
    float* scale; // inverse scaler table
    float* bus_v; // mix bus vector
} mix_peaks_t;

// TANH
int8_t mix_tanh_init(mix_tanh_t* mix, uint16_t size);
// per-sample
void mix_tanh_add(mix_tanh_t* mix, float input);
float mix_tanh_step(mix_tanh_t* mix);
// vectorized
void mix_tanh_add_v(mix_tanh_t* mix, float* input);
void mix_tanh_v(mix_tanh_t* mix, float* output);

// PEAKS
int8_t mix_peaks_init(mix_peaks_t* mix, uint16_t size, uint16_t chans);
// per-sample
void mix_peaks_add(mix_peaks_t* mix, float input, uint16_t ix);
float mix_peaks_step(mix_peaks_t* mix);
// vectorized
void mix_peaks_add_v(mix_peaks_t* mix, float* input, uint16_t ix);
void mix_peaks_v(mix_peaks_t* mix, float* output);

/*void mix_compress_clear(void);
void mix_compress_add(float* input, uint16_t size);
void mix_compress_calc(float* output, uint16_t size);

void mix_peaks_clear(void);
void mix_peaks_add(float* input, uint16_t scale, uint16_t size);
*/

#ifdef __cplusplus
}
#endif
