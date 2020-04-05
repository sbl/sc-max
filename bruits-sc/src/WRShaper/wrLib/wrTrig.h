#pragma once

#include <stdint.h>

float tanh_fast(float in);
float* tanh_fast_v(float* buffer, int b_size);
