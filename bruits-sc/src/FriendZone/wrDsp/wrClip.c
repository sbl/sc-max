#include "wrClip.h"

#include <stdio.h> // printf
#include <stdlib.h> // NULL

// from emb's SoftClip (from softcut on norns)
// "two-stage quadratic soft clipper with variable gain
//  nice odd harmonics, kinda carbon-mic sound"

///////////////////////////////
// private declarations

void clip_soft_calc_coeffs(clip_soft_t* self);

///////////////////////////////
// public fns

clip_soft_t* clip_soft_init_default(void) {
    return clip_soft_init(0.68, 1.2);
}
clip_soft_t* clip_soft_init(float threshold, float gain) {
    clip_soft_t* self = malloc(sizeof(clip_soft_t));
    if (!self) {
        printf("clip_soft: couldn't malloc\n");
        return NULL;
    }

    self->t = threshold;
    self->g = gain;
    clip_soft_calc_coeffs(self);

    return self;
}

void clip_soft_deinit(clip_soft_t* self) {
    free(self);
    self = NULL;
}

void clip_soft_set_threshold(clip_soft_t* self, float threshold) {
    self->t = threshold;
    clip_soft_calc_coeffs(self);
}
void clip_soft_set_gain(clip_soft_t* self, float gain) {
    self->g = gain;
    clip_soft_calc_coeffs(self);
}

#include <math.h> // fabs
float clip_soft_step(clip_soft_t* self, float in) {
    float ax = fabs(in);

    if (ax > 1.0) {
        ax = 1.0;
    }

    if (ax < self->t) {
        return in * self->g;
    } else {
        float sx = in > 0.0 ? 1.0 : -1.0;
        float q = ax - 1.0;
        float y = (self->c * q * q) + self->o;
        return sx * y;
    }
}

float* clip_soft_step_v(clip_soft_t* self, float* in, int size) {
    float* s = in;
    // FIXME: unroll the single-sample version
    for (int i = 0; i < size; i++) {
        *s = clip_soft_step(self, *s);
        s++;
    }
    return in;
}

///////////////////////////////
// private fns

void clip_soft_calc_coeffs(clip_soft_t* self) {
    // match derivative at knee point
    // FIXME: should be able to factor this such that o=1 always, user sets g only
    // if o==1 then max amplitude is 1.0!
    float t_1 = self->t - 1.0;
    if (t_1 < 0.0) {
        self->c = self->g / (2.0 * t_1);
        self->o = self->g * self->t - (self->c * t_1 * t_1);
    } else {
        self->c = 0.0;
        self->o = 1.0;
    }
}
