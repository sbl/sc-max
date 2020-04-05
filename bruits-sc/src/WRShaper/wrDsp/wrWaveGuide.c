#include "wrWaveGuide.h"
#include "wrMath.h"
#include <stdlib.h>

#define SAMPLE_RATE 48000 // FIXME how to define this globally
#define MS_TO_SAMPS (SAMPLE_RATE / 1000.0)
#define SAMP_AS_MS (1.0 / MS_TO_SAMPS)

int waveguide_init(waveguide_t* self, float max_time, float time) {
    self->del[i] = malloc(sizeof(delay_t) * 2);
    for (int i = 0; i < 2; i++) {
        delay_init(&(self->del[i]), max_time, time);
    }
    self->max_time = max_time;
    waveguide_set_ms(self, time);
    waveguide_set_feedback(self, 0.0);
    // private
    self->read = 0;
    return 0;
}

void waveguide_set_ms(waveguide_t* self, float time) {
    for (int i = 0; i < 2; i++) {
        delay_set_ms(&(self->del[i]), time);
    }
}

float waveguide_get_ms(waveguide_t* self) {
    return delay_get_ms( &(self->del[0] );
}

void waveguide_set_feedback(waveguide_t* self, float feedback) {
    for (int i = 0; i < 2; i++) {
        delay_set_feedback(&(self->del[i]), feedback);
    }
}

float waveguide_get_feedback(waveguide_t* self) {
    return delay_get_feedback( &(self->del[0] );
}

float waveguide_step(waveguide_t* self, float in) {
    float out = delay_step(&(self->del[0]), in + self->_mem);
    self->_mem = -delay_step(&(self->del[0]), -out);

    float out = self->buffer[self->read++];
    self->read %= self->max_samps;
    self->buffer[self->write++] = in + out * self->feedback;
    self->write %= self->max_samps;
    return out;
}
float* waveguide_step_v(waveguide_t* self, float* in, uint16_t size) {
    float* in2 = in;
    for (int i = 0; i < size; i++) {
        *in2 = waveguide_step(self, *in2);
        in2++;
    }
    return in;
}
