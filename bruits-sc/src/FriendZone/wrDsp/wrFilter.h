#pragma once

#include <stdint.h>

typedef struct filter_lp1 {
    float x; // destination
    float y;
    float c;
} filter_lp1_t;

typedef struct filter_lp1_a {
    float y;
    float c_rise;
    float c_fall;
} filter_lp1_a_t;

typedef struct filter_awin {
    float* history;
    float out;
    int win_size;
    int win_ix;
    float win_scale;
    float slope_sense;
} filter_awin_t;

typedef struct filter_sr {
    float ramp;
    float rate;
} filter_sr_t;

typedef struct _filter_dc {
    float coeff;
    float prev_in;
    float prev_out;
} filter_dc_t;

typedef struct filter_svf {
    float x[3];
    float q;
    float c;
    uint8_t mode;
    int sample_rate;
} filter_svf_t;

// Lowpass: 1-pole
filter_lp1_t* lp1_init(void);
void lp1_deinit(filter_lp1_t* f);
void lp1_set_dest(filter_lp1_t* f, float in);
float lp1_get_dest(filter_lp1_t* f);
void lp1_set_out(filter_lp1_t* f, float level);
float lp1_get_out(filter_lp1_t* f);
float lp1_step(filter_lp1_t* f, float in);
float lp1_step_internal(filter_lp1_t* f);
void lp1_set_coeff(filter_lp1_t* f, float c);
float lp1_get_coeff(filter_lp1_t* f);
void lp1_set_freq(filter_lp1_t* f, float freq);
void lp1_step_v(filter_lp1_t* f, float* in, float* out, uint16_t size);
float* lp1_step_c_v(filter_lp1_t* f, float* buffer, uint16_t size);

// Lowpass: 1-pole assymetrical
filter_lp1_a_t* lp1_a_init(void);
void lp1_a_deinit(filter_lp1_a_t* self);
float lp1_a_step(filter_lp1_a_t* f, float in);
void lp1_a_set_coeff(filter_lp1_a_t* f, float c_rise, float c_fall);
void lp1_a_step_v(filter_lp1_a_t* f, float* in, float* out, uint16_t size);

// Switch & Ramp: smooth discontinuities
filter_sr_t* switch_ramp_init(void);
void switch_ramp_deinit(filter_sr_t* f);
void switch_ramp_set_rate(filter_sr_t* f, float rate);
void switch_ramp_jump(filter_sr_t* f, float step_size);
float* switch_ramp_step_v(filter_sr_t* f, float* io, int size);

// Windowed average smoother
filter_awin_t* awin_init(int win_size);
void awin_deinit(filter_awin_t* self);
void awin_slope(filter_awin_t* f, float slope_sensitivity);
float awin_step(filter_awin_t* f, float input);
float awin_get_out(filter_awin_t* f);
float awin_get_in(filter_awin_t* f);

// DC-Blocker: Leaky Integrator -> Differentiator
filter_dc_t* dc_init(void);
void dc_deinit(filter_dc_t* self);
void dc_time(filter_dc_t* self, float hpc);
float dc_step(filter_dc_t* self, float in);
float* dc_step_v(filter_dc_t* self, float* buffer, int b_size);

// State-variable: 2-pole
filter_svf_t* svf_init(uint8_t mode, int sample_rate);
void svf_deinit(filter_svf_t* self);
float svf_process_frame(filter_svf_t* f, float input);
float svf_step(filter_svf_t* f, float input);
void svf_set_mode(filter_svf_t* f, uint8_t mode);
void svf_set_coeff(filter_svf_t* f, float coeff);
void svf_set_freq(filter_svf_t* f, float freq);
void svf_set_q(filter_svf_t* f, float quality);
