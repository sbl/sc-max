#include "wrConvert.h"

int16_t _sf_to_s12(float cv) {
    const float f_to_s12 = (float)(1 << 10);
    return ((int16_t)(f_to_s12 * cv));
}

float _s12_to_sf(int16_t cv) {
    const float s12_to_f = 1.0 / (float)(1 << 10);
    return (s12_to_f * (float)cv);
}