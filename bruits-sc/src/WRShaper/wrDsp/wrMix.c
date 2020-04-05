#include "wrMix.h"

#include "../wrLib/wrMath.h"
#include "../wrLib/wrTrig.h"
#include <stdlib.h>

// INIT FUNCTIONS
int8_t mix_tanh_init(mix_tanh_t* mix, uint16_t size) {
    int8_t err = 0;
    mix->bus = 0;
    mix->hpf = dc_init();
    mix->b_size = size;
    mix->bus_v = NULL;
    mix->bus_v = malloc(sizeof(float) * size);
    if (mix->bus_v == NULL) {
        err = 1;
    }
    for (uint16_t i = 0; i < size; i++) {
        mix->bus_v[i] = 0.0;
    }

    return err;
}

int8_t mix_peaks_init(mix_peaks_t* mix, uint16_t size, uint16_t chans) {
    int8_t err = 0;
    mix->bus = 0;
    mix->b_size = size;
    mix->scale = NULL;
    mix->scale = malloc(sizeof(float) * chans);
    if (mix->scale == NULL) {
        err = 1;
    }
    for (uint16_t i = 0; i < chans; i++) {
        mix->scale[i] = 1 / (float)(i + 1);
    }
    mix->bus_v = NULL;
    mix->bus_v = malloc(sizeof(float) * size);
    if (mix->bus_v == NULL) {
        err = 2;
    }
    for (uint16_t i = 0; i < size; i++) {
        mix->bus_v[i] = 0.0;
    }

    return err;
}

//////////
// TANH //
//////////

void mix_tanh_add(mix_tanh_t* mix, float input) {
    mix->bus += input;
}

float mix_tanh_step(mix_tanh_t* mix) {
    float tmp = dc_step(mix->hpf, mix->bus);
    mix->bus = 0;
    return tanh_fast(tmp);
}

///////////
// PEAKS //
///////////

void mix_peaks_add(mix_peaks_t* mix, float input, uint16_t ix) {
    float tmp = input * mix->scale[ix];
    // float tmp = input;
    if (tmp > mix->bus) {
        mix->bus = tmp;
    }
}

float mix_peaks_step(mix_peaks_t* mix) {
    float tmp = mix->bus;
    mix->bus = -1.0; // reset bus
    return tmp;
}

////////////
// TANH_V //
////////////

void mix_tanh_add_v(mix_tanh_t* mix, float* input) {
    float* bus2 = mix->bus_v;

    for (uint16_t i = 0; i < (mix->b_size); i++) {
        *bus2++ += *input++;
    }
}

void mix_tanh_v(mix_tanh_t* mix, float* output) {
    float* bus2 = mix->bus_v;

    for (uint16_t i = 0; i < (mix->b_size); i++) {
        output[i] = mix->bus_v[i];
    }
    tanh_fast_v(dc_step_v(mix->hpf, output, mix->b_size), mix->b_size);

    for (uint16_t i = 0; i < (mix->b_size); i++) {
        *bus2++ = 0; // zero mix bus
    }
}

/////////////
// PEAKS_V //
/////////////

void mix_peaks_add_v(mix_peaks_t* mix, float* input, uint16_t ix) {
    float tmp;
    float* bus2 = mix->bus_v;

    for (uint16_t i = 0; i < (mix->b_size); i++) {
        tmp = (*input++) * mix->scale[ix];
        if (tmp > *bus2) {
            *bus2 = tmp;
        }
        bus2++;
    }
}

void mix_peaks_v(mix_peaks_t* mix, float* output) {
    float* bus2 = mix->bus_v;
    float* out2 = output;

    for (uint16_t i = 0; i < (mix->b_size); i++) {
        *out2++ = *bus2;
        *bus2++ = -1.0;
    }
}
