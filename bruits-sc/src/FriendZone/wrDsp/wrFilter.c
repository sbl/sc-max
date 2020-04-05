#include "wrFilter.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "wrMath.h"

///////////////
// 1Pole LPF //
///////////////

filter_lp1_t* lp1_init(void) {
    filter_lp1_t* self = malloc(sizeof(filter_lp1_t));
    if (!self) {
        printf("Lp1: malloc failed\n");
        return NULL;
    }
    self->x = 0;
    self->y = 0;
    self->c = 0.97;
    return self;
}
void lp1_deinit(filter_lp1_t* f) {
    free(f);
    f = NULL;
}

void lp1_set_dest(filter_lp1_t* f, float in) {
    f->x = in;
}
float lp1_get_dest(filter_lp1_t* f) {
    return f->x;
}
void lp1_set_out(filter_lp1_t* f, float level) {
    f->y = level;
}
float lp1_get_out(filter_lp1_t* f) {
    return f->y;
}
float lp1_step(filter_lp1_t* f, float in) {
    f->y = f->y + f->c * (in - f->y);
    return f->y;
}
uint8_t lp1_converged(filter_lp1_t* f) {
    float diff = f->x - f->y;
    if ((diff > -nFloor) && (diff < nFloor)) {
        return 1;
    }
    return 0;
}
float lp1_step_internal(filter_lp1_t* f) {
    if (lp1_converged(f)) {
        f->y = f->x;
    } else {
        f->y = f->y + f->c * (f->x - f->y);
    }
    return f->y;
}
void lp1_set_coeff(filter_lp1_t* f, float c) {
    f->c = c;
}
float lp1_get_coeff(filter_lp1_t* f) {
    return f->c;
}
void lp1_set_freq(filter_lp1_t* f, float freq) {
    f->c = freq / 48000; // expo!
}
void lp1_step_v(filter_lp1_t* f, float* in, float* out, uint16_t size) {
    float* in2 = in;
    float* out2 = out;
    float* out3 = out; // point to start of arrays
    // out3 = y = previous OUT

    // first samp
    *out2++ = f->y + f->c * ((*in2++) - f->y);

    // remainder of samps -> add nFloor early exit to avoid denormals
    for (uint16_t i = 0; i < (size - 1); i++) {
        *out2++ = (*out3) + f->c * ((*in2++) - (*out3));
        out3++;
    }

    f->y = *out3; // last output
}
float* lp1_step_c_v(filter_lp1_t* f, float* buffer, uint16_t size) {
    float* out = buffer;
    float* out2 = buffer;

    // first samp
    // check if we've already converged
    if (lp1_converged(f)) {
        for (uint16_t i = 0; i < size; i++) {
            *out++ = f->x;
        }
        f->y = f->x;
    } else {
        *out++ = f->y + f->c * (f->x - f->y);

        // remainder of samps -> add nFloor early exit to avoid denormals
        for (uint16_t i = 0; i < (size - 1); i++) {
            *out++ = (*out2) + f->c * (f->x - (*out2));
            out2++;
        }

        f->y = *out2; // last output
    }
    return buffer;
}

//////////////////////////////
// 1Pole LPF (Assymetrical) //
//////////////////////////////

filter_lp1_a_t* lp1_a_init(void) {
    filter_lp1_a_t* self = malloc(sizeof(lp1_a_init));
    if (!self) {
        printf("Lp1_A malloc failed\n");
        return NULL;
    }
    self->y = 0;
    self->c_rise = 0.98;
    self->c_fall = 0.8;
    return self;
}

void lp1_a_deinit(filter_lp1_a_t* self) {
    free(self);
    self = NULL;
}

float lp1_a_step(filter_lp1_a_t* f, float in) {
    float c = (in > f->y) ? f->c_rise : f->c_fall;
    f->y = f->y + c * (in - f->y);
    return f->y;
}
void lp1_a_set_coeff(filter_lp1_a_t* f, float c_rise, float c_fall) {
    f->c_rise = c_rise;
    f->c_fall = c_fall;
}
void lp1_a_step_v(filter_lp1_a_t* f, float* in, float* out, uint16_t size) {
    float* in2 = in;
    float* out2 = out;
    float* out3 = out;
    // out3 = y = previous OUT

    // if we match this case, we can't assume same direction this block
    if ((in[0] >= 0.0)
        ^ (in[size - 1] >= 0.0)) { // frame changes direction: case on each sample
        float c = (*in2 > f->y) ? f->c_rise : f->c_fall;
        *out2++ = f->y + c * ((*in2++) - f->y);
        for (uint16_t i = 0; i < (size - 1); i++) {
            c = (*in2 > *out3) ? f->c_rise : f->c_fall;
            *out2++ = (*out3) + c * ((*in2++) - (*out3));
            out3++;
        }
    } else { // assume the whole in frame is same direction relative to output
        float c = (in[0] > f->y) ? f->c_rise : f->c_fall;

        *out2++ = f->y + c * ((*in2++) - f->y);
        for (uint16_t i = 0; i < (size - 1); i++) {
            *out2++ = (*out3) + c * ((*in2++) - (*out3));
            out3++;
        }
    }
    f->y = *out3; // last output
}

///////////////////
// SWITCH & RAMP //
///////////////////

filter_sr_t* switch_ramp_init(void) {
    filter_sr_t* self = malloc(sizeof(filter_sr_t));
    if (!self) {
        printf("SR: malloc failed\n");
        return NULL;
    }
    self->ramp = 0.0;
    self->rate = 0.001; // per-sample step-size, or 1pole coefficient?
    return self;
}
void switch_ramp_deinit(filter_sr_t* f) {
    free(f);
    f = NULL;
}

void switch_ramp_set_rate(filter_sr_t* f, float rate) {
    f->rate = rate;
}
void switch_ramp_jump(filter_sr_t* f, float step_size) {
    f->ramp += step_size; // accumulate in case of overlapping ramps
}
float* switch_ramp_step_v(filter_sr_t* f, float* io, int b_size) {
    float* samp = io;
    if (f->ramp != 0.0) { // passthrough if no ramp
        if (f->ramp >= (b_size * f->rate)) { // positive ramp
            for (int i = 0; i < b_size; i++) {
                *samp++ += f->ramp;
                f->ramp -= f->rate;
            }
        } else if (f->ramp <= -(b_size * f->rate)) { // negative ramp
            for (int i = 0; i < b_size; i++) {
                *samp++ += f->ramp;
                f->ramp += f->rate;
            }
        } else { // almost zero. check each step
            if (f->ramp > 0) { // pos
                for (int i = 0; i < b_size; i++) {
                    *samp++ += f->ramp;
                    f->ramp -= f->rate;
                    if (f->ramp <= 0.0) {
                        f->ramp = 0.0;
                        break;
                    }
                }
            } else { // neg
                for (int i = 0; i < b_size; i++) {
                    *samp++ += f->ramp;
                    f->ramp += f->rate;
                    if (f->ramp >= 0.0) {
                        f->ramp = 0.0;
                        break;
                    }
                }
            }
        }
    }
    return io;
}

////////////////////////////
// AVERAGED WINDOW SMOOTH //
////////////////////////////

filter_awin_t* awin_init(int win_size) {
    filter_awin_t* self = malloc(sizeof(filter_awin_t));
    if (!self) {
        printf("awin malloc failed\n");
        return NULL;
    }
    self->out = 0.5;
    self->win_size = win_size;
    self->win_ix = 0;
    self->win_scale = 1.0 / (float)(win_size + 1);

    self->history = malloc(sizeof(float) * win_size);
    if (!self->history) {
        printf("history!\n");
        return NULL;
    }
    for (int i = 0; i < win_size; i++) {
        self->history[i] = 0.5;
    }

    self->slope_sense = 6000.0 * self->win_scale;
    return self;
}

void awin_deinit(filter_awin_t* self) {
    free(self->history);
    self->history = NULL;
    free(self);
    self = NULL;
}

void awin_slope(filter_awin_t* f, float slope_sensitivity) {
    f->slope_sense = slope_sensitivity;
}
float awin_step(filter_awin_t* f, float input) {
    float windowed_avg = input;
    for (int i = 0; i < (f->win_size); i++) {
        windowed_avg += f->history[i];
    }
    windowed_avg *= f->win_scale;
    f->history[f->win_ix++] = input;
    if (f->win_ix >= f->win_size) {
        f->win_ix = 0;
    }

    // rate of change
    float roc = f->slope_sense * (windowed_avg - f->out);

    roc = lim_f_0_1(max_f(roc * roc, 0.008));

    // slope-sensitive-smoother
    f->out = lim_f_0_1(input + (1.0 - roc) * (f->out - input));
    return f->out;
}
float awin_get_out(filter_awin_t* f) {
    return f->out;
}
float awin_get_in(filter_awin_t* f) {
    int16_t ix = (f->win_ix) - 1;
    if (ix < 0) {
        ix += f->win_size;
    }
    return f->history[ix];
}

////////////////
// DC-Blocker //
////////////////

// Differentiator followed by leaky integrator
filter_dc_t* dc_init(void) {
    filter_dc_t* self = malloc(sizeof(filter_dc_t));
    if (self == NULL) {
        printf("DC: malloc failed\n");
    }

    self->coeff = 0.997;
    self->prev_in = 0.0;
    self->prev_out = 0.0;

    return self;
}
void dc_deinit(filter_dc_t* self) {
    free(self);
    self = NULL;
}

void dc_time(filter_dc_t* self, float hpc) {
    // f->c = 1.0 means DC-coupling
    // f->c = 0.0 means differentiator (only changes are passed)
    self->coeff = lim_f_0_1(hpc);
}

float dc_step(filter_dc_t* self, float in) {
    self->prev_out = in - self->prev_in + (self->coeff * self->prev_out);
    self->prev_in = in; // save previous input
    return self->prev_out;
}

float* dc_step_v(filter_dc_t* self, float* buffer, int b_size) {
    float* in = buffer;
    float* out = buffer;
    for (int i = 0; i < b_size; i++) {
        self->prev_out = *in - self->prev_in + (self->coeff * self->prev_out);
        self->prev_in = *in++;
        *out++ = self->prev_out;
    }
    return buffer;
}

////////////////////////////
// State-variable: 2-pole //
////////////////////////////

filter_svf_t* svf_init(uint8_t mode, int sample_rate) {
    filter_svf_t* self = malloc(sizeof(filter_svf_t));
    if (!self) {
        printf("SVF malloc failed\n");
        return NULL;
    }
    self->x[0] = 0;
    self->x[1] = 0;
    self->x[2] = 0;
    self->q = 0.5;
    self->c = 0.02;
    self->mode = mode;
    self->sample_rate = sample_rate;
    return self;
}

void svf_deinit(filter_svf_t* self) {
    free(self);
    self = NULL;
}

void svf_set_mode(filter_svf_t* f, uint8_t mode) {
    f->mode = mode;
}
float svf_process_frame(filter_svf_t* f, float input) {
    f->x[0] = f->x[0] + (f->c * f->x[1]);
    f->x[2] = (input - (f->q * f->x[1])) - f->x[0];
    f->x[1] = f->x[1] + (f->c * f->x[2]);
    return f->x[f->mode];
}
float svf_step(filter_svf_t* f, float input) {
    // 2x oversample & average
    float out = svf_process_frame(f, input);
    out += svf_process_frame(f, input);
    out *= 0.5;
    return out;
}
void svf_set_coeff(filter_svf_t* f, float coeff) {
    f->c = lim_f(coeff * 0.5, 0.001, 0.499);
}
void svf_set_freq(filter_svf_t* f, float freq) {
    //	f->c = lim_f(freq/(f->sample_rate), 0.001, 0.499); // need expo lookup here
    // when freq = sample_rate/4, f->c = 0.5

    f->c = exp(-2.0 * WR_PI * freq * 1 / f->sample_rate);

    // Set mixfreq to whatever rate your system is using (eg 48Khz)
    // Calculate filter cutoff frequencies
    // es->lf = 2 * sin(M_PI * ((double)lowfreq / (double)mixfreq));
}
void svf_set_q(filter_svf_t* f, float quality) {
    f->q = lim_f(0.5 - (quality * 0.5), 0.001,
                 0.499); // q needs stronger limits to avoid overload
}
