#include "wrTrig.h"

// based on padé expansion
// accepts input values -3 to +3, outside this range needs to be clamped to +/-1
float tanh_fast(float in) {
    float sq, out;

    if (in <= -3) { // clamp negative values
        out = -1;
    } else if (in >= 3) { // clamp positive values
        out = 1;
    } else {
        sq = in * in; // input squared
        out = in * (27 + sq) / (27 + 9 * sq);
    }

    return out;
}

float* tanh_fast_v(float* buffer, int b_size) {
    float* in = buffer;
    float* out = buffer;

    for (int i = 0; i < b_size; i++) {
        if (*in <= -3.0) {
            *out++ = -1.0;
        } else if (*in >= 3.0) {
            *out++ = 1.0;
        } else {
            float squared = *in * *in;
            *out++ = *in * (27.0 + squared) / (27.0 + 9.0 * squared);
        }
        in++;
    }
    return buffer;
}
