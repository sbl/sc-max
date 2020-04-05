#pragma once

#include <stdint.h>

#define LUT_SIN_SIZE ((uint32_t)1024)
#define LUT_SIN_HALF (LUT_SIN_SIZE >> 1)
extern const float sine_lut[];

typedef struct osc_sine {
    float rate;
    float id;
    int8_t zero_x;
} osc_sine_t;

// initialization
osc_sine_t* sine_init(void);

// input fns
void osc_sine_time(osc_sine_t* self, float time);
void osc_sine_reset(osc_sine_t* self);

// status
int8_t osc_sine_get_zc(osc_sine_t* self);

// process
float osc_sine_step(osc_sine_t* self, float fm);
void osc_sine_process_v(osc_sine_t* self, uint16_t b_size, float* exp_fm,
                        float* lin_fm, float* out);
float* sine_process_base_v(osc_sine_t* self, float* out, uint16_t b_size);
