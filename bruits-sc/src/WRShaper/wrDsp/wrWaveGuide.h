#pragma once

#include "wrDelay.h"
#include <stdint.h>

typedef struct {
    float max_time;
    float time;
    float feedback;
    // private
    float _mem; // TODO init
    int max_samps;
    int read;
    int write;
    delay_t* del[2];
} waveguide_t;

int waveguide_init(waveguide_t* self, float max_time, float time);
void waveguide_set_ms(waveguide_t* self, float time);
float waveguide_get_ms(waveguide_t* self);
void waveguide_set_feedback(waveguide_t* self, float feedback);
float waveguide_get_feedback(waveguide_t* self);
float waveguide_step(waveguide_t* self, float in);
float* waveguide_step_v(waveguide_t* self, float* in, uint16_t size);
