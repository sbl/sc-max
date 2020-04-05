#include "wrFuncGen.h"

#include <math.h>
#include <stdio.h> // printf
#include <stdlib.h> // malloc

#include "wrMath.h"

func_gen_t* function_init(int loop) {
    func_gen_t* self = malloc(sizeof(func_gen_t));
    if (!self) {
        printf("!func_gen\n");
        return NULL;
    }
    self->go = 0; // stopped
    self->id = 0;
    self->rate = 0.05;
    self->tr_state = 0;
    self->fm_ix = 0;
    self->loop = loop;
    if (loop != 0) {
        self->go = 1; // start if looping
    }
    self->s_mode = 0;
    self->sustain_state = 0;
    self->sustaining = 0;
    self->zc = 0;
    self->r_up = 1;
    self->r_down = 1;
    return self;
}

void function_deinit(func_gen_t* self) {
    free(self);
    self = NULL;
}

// Param Functions

// Called on trigger edges only

/* function_reset is same as cutoff = -1;
   function_trig  is same as cutoff = 0; */

void function_trig_reset(func_gen_t* self, uint8_t state) {
    if (state) { // release stage/stopped
        self->id = MIN_POS_FLOAT; // reset
        self->go = 1;
    }
    self->sustain_state = state;
}
void function_trig_wait(func_gen_t* self, uint8_t state) {
    if (state && (!self->go)) { // release stage/stopped
        self->id = MIN_POS_FLOAT; // reset
        self->go = 1;
    }
    self->sustain_state = state;
}
void function_trig_sustain(func_gen_t* self, uint8_t state) {
    /*if(state && !self->go){ // stopped & high trigger
        self->id = MIN_POS_FLOAT;
        self->go = 1;
    } else if( state
            && self->id <=0.0 ){ // release phase & high trigger
        float eye = shaper_rev_lookup( &friends.shaper
                                     , self->id
                                     , BLOCK_SIZE-1
                                     );
        self->id = eye
    } else if( !state
            && self->id > 0.0 ){ // attack phase & release
        self->id = -(self->id); // naive flip to release
    }
    self->sustain_state = state;*/
}
void function_trig_vari(func_gen_t* self, uint8_t state, float cutoff) {
    // -1 is always, 0 is only in release, +1 is never
    uint8_t tr;
    (cutoff >= 0.0f)
        // EOR to EOC
        ? (tr = (self->id <= 0.0f) // is falling AND
               && (self->id > (cutoff - 1.0f))) //
        // Always to EOR
        : (tr = (self->id <= 0.0f) // is falling     OR
               || (self->id > (1.0f + cutoff))); // is rising
    if ((state && tr) // is sensitive!
        || self->go == 0) { // or channel is already stopped
        self->id = MIN_POS_FLOAT; // reset
        self->go = 1;
    }
    self->sustain_state = state;
}
void function_trig_burst(func_gen_t* self, uint8_t state, float count) {
    // -1 is zero, 0 is 6, +1 is 36
    if (state) { // release stage/stopped
        self->id = MIN_POS_FLOAT; // reset
        if (count <= -0.7f) { // choke channel if below -3v5
            self->go = 0;
            self->loop = 0;
        } else {
            self->go = 1;
            self->loop = (int8_t)powf(6.0f, count + 1.0f) - 1.0f;
        }
    }
    self->sustain_state = state;
}
void function_trig_spill(func_gen_t* self, uint8_t state, float cutoff) {
    // -1 is always, 0 is only in release, +1 is never
    uint8_t tr;
    (cutoff >= 0.0f) ? (tr = (self->id <= 0.0f) && (self->id > -(cutoff)))
                     : (tr = (self->id <= 0.0f) || (self->id > (1.0f + cutoff)));
    if (state && tr) { // release stage/stopped
        self->id = -(self->id); // flip (soft-sync)
        if (!self->go) { // explicit start required
            self->id = MIN_POS_FLOAT;
        }
        self->go = 1;
    }
    self->sustain_state = state;
}

void function_mode(func_gen_t* self, uint8_t mode) {
    function_loop(self, 0 - (FNGEN_CYCLE == mode)); // 0 or -1
    function_sustain(self, (FNGEN_SUSTAIN == mode));
}

void function_loop(func_gen_t* self, int8_t loop) {
    self->loop = loop;
    if (loop != 0) {
        self->go = 1;
    }
}

void function_sustain(func_gen_t* self, uint8_t sust) {
    self->s_mode = sust;
}

void function_rate(func_gen_t* self, float rate) {
    self->rate = lim_f_n1_1(rate);
}

float function_get_rate(func_gen_t* self) {
    return self->rate;
}

void function_fm_ix(func_gen_t* self, float ix) {
    self->fm_ix = lim_f_0_1(ix) * 0.1f;
}

// Audio Rate Process (helper function)
void function_ramp(func_gen_t* self, float skew) {
    math_get_ramps(skew, &(self->r_up), &(self->r_down));
    // self->r_up = 0.5 / (0.998 * level + 0.001);
    // self->r_down = 1/ (2- (1/ self->r_up));
}

void function_ramp_v_global(uint16_t b_size, float ctrl_rate, float* audio_rate,
                            float* ramp_up, float* ramp_down) {
    float* audio_rate2 = audio_rate;
    float* ramp_up2 = ramp_up;
    float* ramp_down2 = ramp_down;

    // bandlimiting based on base pitch of function
    for (uint16_t i = 0; i < b_size; i++) {
        float t = ctrl_rate + *audio_rate2++;
        *ramp_up2 = 0.501002
            / (_Lim01(t) // careful it's a macro!
               + 0.001002);
        *ramp_down2++ = *ramp_up2 / (*ramp_up2 * 2.0 - 1.0);
        ramp_up2++;
    }
}
float* function_ramp_v(func_gen_t* self, float ctrl_rate, float* audio_rate,
                       float* two_ramps, int b_size) {
    float* audio_rate2 = audio_rate;
    float* ramp_up2 = &two_ramps[0];
    float* ramp_down2 = &two_ramps[b_size];

    float f = fabsf(self->rate);
    float max = 1.0 - f; // (-1,1 range)
    float min = 0.0 + f; // (-1,1 range)
    // bandlimiting based on base pitch of function
    for (uint16_t i = 0; i < b_size; i++) {
        float t = ctrl_rate + *audio_rate2++;
        *ramp_up2 = 0.501002
            / (_Lim(t, min, max) // careful it's a macro!
               + 0.001002);
        *ramp_down2++ = *ramp_up2 / (*ramp_up2 * 2.0 - 1.0);
        ramp_up2++;
    }
    return two_ramps;
}

float function_step(func_gen_t* self, float fm_in) {
    if (self->go) {
        float move;

        // determine rate based on direction
        if (self->id >= 0) {
            move = self->rate * self->r_up + (fm_in * self->fm_ix); // (+ phase mod)
        } else {
            move = self->rate * self->r_down + (fm_in * self->fm_ix);
        }
        // increment w/ overflow protection
        while (move != 0.0f) {
            if (self->id >= 0.0f) { // are we above zero BEFORE moving
                self->id += move;
                move = 0.0f;
                if (self->id >= 1.0f) {
                    move = (self->id - 1.0f) * self->r_down / self->r_up;
                    self->id = -1.0f;
                } else if (self->id < 0.0f) {
                    if (self->loop != 0) {
                        move = self->id * self->r_down / self->r_up;
                        if (self->loop > 0) {
                            self->loop += 1;
                        }
                    }
                    self->id = 0.0f;
                }
            } else {
                self->id += move;
                move = 0.0f;
                if (self->id >= 0.0f) {
                    if (self->loop != 0) {
                        move = self->id * self->r_up / self->r_down;
                        if (self->loop > 0) {
                            self->loop -= 1;
                        }
                        self->id = MIN_POS_FLOAT;
                    } else {
                        self->go = 0;
                        self->id = 0.0f;
                    }
                } else if (self->id < -1.0f) {
                    move = (self->id + 1.0f) * self->r_up / self->r_down;
                    self->id = 1.0f;
                }
            }
        }
    }
    return self->id;
    // NB: this is +/-1 sawtooth, with 0 as 'stopped'
    // use function_lookup(self->id) to convert
}

// compiler inlines this anyway
float sign(float n) {
    return ((n > 0.0f) - (n < 0.0f));
}

float function_lookup(float id) {
    return (sign(id) * 2.0f - 1.0f);
}

float* function_v(func_gen_t* self, float* ramps // array of 2xb_size for UP/DOWN
                  ,
                  float* fm_return // MODIFIED IN PLACE
                  ,
                  int b_size) {
    float* fm_in2 = fm_return;
    float* out2 = fm_return;
    float* r_up2 = &ramps[0];
    float* r_down2 = &ramps[b_size];

    self->sustaining = 0;
    self->zc = 0;

    if (self->go) {
        for (uint16_t i = 0; i < b_size; i++) {
            float move = (*fm_in2++ * self->fm_ix) // linear FM
                + self->rate // base freq
                    * ((self->id >= 0.0f) // is rising?
                           ? (*r_up2)
                           : (*r_down2));
            while (move != 0.0f) {
                if (self->id > 0.0f) { // attack
                    self->id += move;
                    move = 0.0f;
                    if (self->id >= 1.0f) {
                        if (self->s_mode && self->sustain_state) {
                            // fill rest of block with 1s
                            self->sustaining = 1;
                            self->id = 1.0f;
                            while (i++ < b_size) {
                                *out2++ = self->id;
                            }
                            return fm_return;
                        }
                        move = (self->id - 1.0f) * (*r_down2) / (*r_up2);
                        self->id = -1.0f;
                    } else if (self->id < 0.0f) { // rev TZ
                        self->zc = 1;
                        if (self->loop) {
                            move = self->id * (*r_down2) / (*r_up2);
                            if (self->loop > 0.0f) {
                                self->loop++;
                            } // TZ adds to burst!
                        }
                        self->id = 0.0f;
                    }
                } else { // release
                    self->id += move;
                    move = 0.0f;
                    if (self->id >= 0.0f) { // rel -> ?atk
                        self->zc = 1;
                        if (self->loop) {
                            move = self->id * (*r_up2) / (*r_down2);
                            if (self->loop > 0) {
                                self->loop--;
                            }
                            self->id = MIN_POS_FLOAT; // get into attack case
                        } else {
                            // fill rest of block with 0s
                            self->id = 0.0f; // only for STOP
                            self->go = 0;
                            while (i++ < b_size) {
                                *out2++ = self->id;
                            }
                            return fm_return;
                        }
                    } else if (self->id < -1.0f) { // TZ back to attack
                        move = (self->id + 1.0f) * (*r_up2) / (*r_down2);
                        self->id = 1.0f;
                    }
                }
            }
            *out2++ = self->id;
            r_up2++;
            r_down2++;
        }
    } else {
        self->id = 0.0f;
        for (uint16_t i = 0; i < b_size; i++) {
            *out2++ = self->id;
        }
    }
    return fm_return;
}
void function_fmix_v(func_gen_t* self, uint16_t b_size, float* r_up, float* r_dn,
                     float* fm_in, float* fm_ix, float* out) {
    float* out2 = out;
    float* r_up2 = r_up;
    float* r_down2 = r_dn;
    float* fm_in2 = fm_in;

    self->sustaining = 0;
    self->zc = 0;

    if (self->go) {
        for (uint16_t i = 0; i < b_size; i++) {
            float move = (*fm_in2++ // FM audio source
                          * (self->fm_ix // FM pot
                             + 0.1 * *fm_ix++) // FM index adds to pot
                          ) // linear FM
                + self->rate // base freq
                    * ((self->id >= 0.0f) // is rising?
                           ? (*r_up2)
                           : (*r_down2));
            while (move != 0.0f) {
                if (self->id > 0.0f) { // attack
                    self->id += move;
                    move = 0.0f;
                    if (self->id >= 1.0f) {
                        if (self->s_mode && self->sustain_state) {
                            // fill rest of block with 1s
                            self->sustaining = 1;
                            self->id = 1.0f;
                            while (i++ < b_size) {
                                *out2++ = self->id;
                            }
                            return;
                        }
                        move = (self->id - 1.0f) * (*r_down2) / (*r_up2);
                        self->id = -1.0f;
                    } else if (self->id < 0.0f) { // rev TZ
                        self->zc = 1;
                        if (self->loop) {
                            move = self->id * (*r_down2) / (*r_up2);
                            if (self->loop > 0.0f) {
                                self->loop++;
                            } // TZ adds to burst!
                        }
                        self->id = 0.0f;
                    }
                } else { // release
                    self->id += move;
                    move = 0.0f;
                    if (self->id >= 0.0f) { // rel -> ?atk
                        self->zc = 1;
                        if (self->loop) {
                            move = self->id * (*r_up2) / (*r_down2);
                            if (self->loop > 0) {
                                self->loop--;
                            }
                            self->id = MIN_POS_FLOAT; // get into attack case
                        } else {
                            // fill rest of block with 0s
                            self->id = 0.0f; // only for STOP
                            self->go = 0;
                            while (i++ < b_size) {
                                *out2++ = self->id;
                            }
                            return;
                        }
                    } else if (self->id < -1.0f) { // TZ back to attack
                        move = (self->id + 1.0f) * (*r_up2) / (*r_down2);
                        self->id = 1.0f;
                    }
                }
            }
            *out2++ = self->id;
            r_up2++;
            r_down2++;
        }
    } else {
        self->id = 0.0f;
        for (uint16_t i = 0; i < b_size; i++) {
            *out2++ = self->id;
        }
    }
    return;
}
