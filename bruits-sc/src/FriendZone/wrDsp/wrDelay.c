#include "wrDelay.h"
#include "wrMath.h"
#include <stdio.h> // printf
#include <stdlib.h>

#define SAMPLE_RATE 48000 // FIXME how to define this globally
#define MS_TO_SAMPS (SAMPLE_RATE / 1000.0)
#define SAMP_AS_MS (1.0 / MS_TO_SAMPS)

// private declarations
float wrap(float input, float modulo);
float peek(delay_t* self, float tap);
void poke(delay_t* self, float input);

// public defns
delay_t* delay_init(float max_time, float time) {
    delay_t* self = malloc(sizeof(delay_t));
    if (!self) {
        printf("delay: couldn't malloc\n");
        return NULL;
    }

    self->max_time = max_time;
    self->max_samps = max_time * MS_TO_SAMPS;
    self->buffer = malloc(sizeof(float) * (int)(self->max_samps + 1));
    if (!self->buffer) {
        printf("delay: couldn't malloc buf\n");
        free(self);
        return NULL;
    }

    for (int i = 0; i < (int)(self->max_samps + 1); i++) {
        self->buffer[i] = 0.0;
    }
    self->rate = 1.0;
    self->tap_write = 0.0;
    self->tap_read = 0.0;
    self->tap_fb = 0.0;
    delay_set_ms(self, time);
    delay_set_read_phase(self, time);
    delay_set_feedback(self, 0.0);

    return self;
}

void delay_deinit(delay_t* self) {
    free(self->buffer);
    self->buffer = NULL;
    free(self);
    self = NULL;
}

void delay_set_ms(delay_t* self, float time) {
    self->tap_fb = wrap(self->tap_write - (time * MS_TO_SAMPS), self->max_samps);
}

void delay_set_time_percent(delay_t* self, float percent) {
    delay_set_ms(self, self->max_time * lim_f_0_1(percent));
}

void delay_set_rate(delay_t* self, float rate) {
    self->rate = lim_f(rate, 1.0 / 16.0, 16.0);
}

void delay_set_read_phase(delay_t* self, float percent) {
    self->tap_read =
        wrap(self->tap_write - (lim_f_0_1(percent) * self->max_time * MS_TO_SAMPS),
             self->max_samps);
}

float delay_get_ms(delay_t* self) {
    return SAMP_AS_MS * wrap(self->tap_write - self->tap_fb, self->max_samps);
}

void delay_set_feedback(delay_t* self, float feedback) {
    self->feedback = lim_f(feedback, -0.999, 0.999);
}

float delay_get_feedback(delay_t* self) {
    return self->feedback;
}

float delay_step(delay_t* self, float in) {
    self->tap_fb = wrap(self->tap_fb + self->rate, self->max_samps);
    self->tap_read = wrap(self->tap_read + self->rate, self->max_samps);
    self->tap_write = wrap(self->tap_write + self->rate, self->max_samps);
    float out = peek(self, self->tap_read);
    poke(self,
         in * ((self->rate >= 1.0) ? 1.0 : self->rate)
             + self->feedback * peek(self, self->tap_fb));
    return out;
}

float* delay_step_v(delay_t* self, float* buffer, int b_size) {
    float* in = buffer;
    float* out = buffer;
    for (int i = 0; i < b_size; i++) {
        *out++ = delay_step(self, *in++);
    }
    return buffer;
}

// private defns
float wrap(float input, float modulo) {
    while (input >= modulo) {
        input -= modulo;
    }
    while (input < 0.0) {
        input += modulo;
    }
    return input;
}

float peek(delay_t* self, float tap) {
    int ixA = (int)tap;
    int ixB = (int)wrap(tap + 1, self->max_samps);
    float c = tap - (float)ixA;
    return self->buffer[ixA] + c * (self->buffer[ixB] - self->buffer[ixA]);
}

void poke(delay_t* self, float input) {
    int ixA = (int)self->tap_write;
    int ixB = (int)wrap(self->tap_write + 1, self->max_samps);
    float c = self->tap_write - (float)ixA;
    // these coeffs seem backward to me, but reversed is terrible aliasing?!
    self->buffer[ixA] = input * c;
    self->buffer[ixB] = input * (1.0 - c);
}
