#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// use 0x00800000 as float (smallest +ve value)
#define MIN_POS_FLOAT (0.00000001f)

// should be an enum, so more like a type for public access
#define FNGEN_TRANSIENT 0
#define FNGEN_SUSTAIN 1
#define FNGEN_CYCLE 2

typedef enum { fg_transient, fg_sustain, fg_cycle } fg_mode_t;

typedef struct func_gen {
    int8_t go;
    float id;
    float rate;
    uint8_t tr_state;
    float fm_ix;
    int8_t loop; // nb: -1 is infinite loop. +ve is number of retrigs left.
    uint8_t s_mode;
    uint8_t sustain_state;
    uint8_t sustaining;
    uint8_t zc;
    float r_up;
    float r_down;
} func_gen_t;

// Initialization
func_gen_t* function_init(int loop);
void function_deinit(func_gen_t*);

// Param functions

// Triggers
void function_trig_reset(func_gen_t* self, uint8_t state);
void function_trig_wait(func_gen_t* self, uint8_t state);
void function_trig_sustain(func_gen_t* self, uint8_t state);
void function_trig_vari(func_gen_t* self, uint8_t state, float cutoff);
void function_trig_burst(func_gen_t* self, uint8_t state, float count);
void function_trig_spill(func_gen_t* self, uint8_t state, float cutoff);

void function_mode(func_gen_t* self, uint8_t mode);
void function_loop(func_gen_t* self, int8_t loop);
void function_sustain(func_gen_t* self, uint8_t sust);
void function_rate(func_gen_t* self, float rate);
void function_fm_ix(func_gen_t* self, float ix);

float function_get_rate(func_gen_t* self);

// Audio rate process
void function_ramp(func_gen_t* self, float skew);
float* function_ramp_v(func_gen_t* self, float ctrl_rate, float* audio_rate,
                       float* two_ramps, int b_size);
void function_ramp_v_global(uint16_t b_size, float ctrl_rate, float* audio_rate,
                            float* ramp_up, float* ramp_down);

float function_step(func_gen_t* self, float fm_in);
float* function_v(func_gen_t* self, float* ramps, float* fm_return, int b_size);
void function_fmix_v(func_gen_t* self, uint16_t b_size, float* r_up, float* r_dn,
                     float* fm_in, float* fm_ix, float* out);

float function_lookup(float id);

#ifdef __cplusplus
}
#endif
