#pragma once

#include <stdint.h>

extern const float log_lut[]; // make LUT public to project

float shaper_apply(float input, // input
                   uint8_t zone, // [0:3]
                   float coeff // (0,1)
);
float* shaper_apply_v(float shape, float* shape_audio, float* io, int b_size);
