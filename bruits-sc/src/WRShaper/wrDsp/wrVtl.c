#include "wrVtl.h"

#include <stdio.h> // printf
#include <stdlib.h>

#include "wrGlobals.h"
#include "wrMath.h" // math_get_ramps()

vtl_t* vtl_init(void) {
    vtl_t* self = malloc(sizeof(vtl_t));
    if (self == NULL) {
        printf("VTL: can't malloc!\n");
    }

    self->dest = 0.0;
    self->level = 0.0;
    self->vel = 1.0;
    vtl_mode(self, vtl_mode_sustain);
    vtl_params(self, 100.0, 0.1);
    return self;
}

void vtl_deinit(vtl_t* self) {
    free(self);
    self = NULL;
}

void vtl_mode(vtl_t* self, vtl_mode_t mode) {
    self->mode = mode;
    if (mode != vtl_mode_sustain) {
        self->dest = 0.0;
    }
}

void vtl_params(vtl_t* self, float speed, float ARratio) {
    self->rtime = 0.5 / (0.998 * lim_f_0_1(ARratio) + 0.001);
    self->ftime = 1.0 / (2.0 - (1.0 / self->rtime));
    float ttime = lim_f(lim_f_0_1(speed) * 0.007, 0.000001, 1.0);
    self->rtime *= ttime;
    self->ftime *= ttime;
}

// unroll this in your loop if calling it per sample (otherwise it's minor
// improvement)
void vtl_dest(vtl_t* self, float dest) {
    self->dest = dest; // set destination value

    if (dest > nFloor) {
        self->vel = dest; // treat as 'velocity' input
    }
}

float vtl_get_level(vtl_t* self) {
    return self->level;
}

float vtl_step(vtl_t* self) {
    float slew_mod;
    float location = self->level;

    // difference between current & dest
    float sub_diff = self->dest - self->level;

    if (sub_diff > 0.0) { // rising
        slew_mod = self->rtime;
        slew_mod =
            min_f(slew_mod + slew_mod * location * location * 2.0,
                  0.2); // some kind of hysteresis: out += 2 * in * previous^2
        self->level += slew_mod * sub_diff;

        if (sub_diff < nFloor) { // we're close enough! (~58dB)
            if (self->mode != vtl_mode_sustain) {
                self->dest = 0; // go toward zero if not sustaining
            }
        }
    } else { // falling
        slew_mod = self->ftime;
        slew_mod =
            min_f(slew_mod + slew_mod * location * location * 2.0,
                  0.2); // some kind of hysteresis: out += 2 * in * previous^2
        self->level += slew_mod * sub_diff;

        if (sub_diff > -nFloor) { // if we've gone past the dest
            if (self->mode == vtl_mode_cycle) {
                self->dest = self->vel; // rise if cycling
            }
        }
    }
    return self->level; // needs to be limited to 0-1f
}

float* vtl_step_v(vtl_t* self, float* out, int b_size) {
    float slew_mod, slew_fix;
    float* out2 = out;
    float* out3 = out;
    uint16_t i;

    // difference between current & dest
    float sub_diff = self->dest - self->level;
    // 1.0          // 0.007

    if (sub_diff > 0.0) { // rising
        if (sub_diff < nFloor) { // call it even
            if (self->mode != vtl_mode_sustain) {
                self->dest = 0.0; // go toward zero
                slew_fix = self->ftime;
            } else { // sustain mode, so hold val
                // escape w/ fixed output value
                for (i = 0; i < b_size; i++) {
                    *out2++ = self->dest;
                }
                self->level = self->dest; // save last val
                return out; // EARLY EXIT
            }
        } else { // normal rise
            slew_fix = self->rtime;
        }
    } else { // falling
        if (sub_diff > -nFloor) { // call it even
            if (self->mode == vtl_mode_cycle) {
                if (self->dest == self->vel) { // AT MAX!
                    self->dest = 0.0; // go to fall
                    slew_fix = self->ftime;
                } else { // go to rise
                    self->dest = self->vel;
                    slew_fix = self->rtime;
                }
            } else { // hit bottom
                for (i = 0; i < b_size; i++) {
                    *out2++ = self->dest;
                }
                self->level = self->dest;
                return out; // EARLY EXIT
            }
        } else { // normal falling
            slew_fix = self->ftime;
        }
    }

    // some kind of hysteresis: out += 2 * in * previous^2
    slew_mod = slew_fix + slew_fix * self->level * self->level * 2.0;
    if (slew_mod > 0.2) {
        slew_mod = 0.2;
    } // limit rate to 1/5 per samp
    *out2++ = self->level + (slew_mod * sub_diff);

    for (i = 1; i < b_size; i++) {
        sub_diff = self->dest - *out3;

        if (sub_diff > 0.0) { // rising
            if (sub_diff < nFloor) { // call it even
                if (self->mode != vtl_mode_sustain) {
                    self->dest = 0.0; // go toward zero
                    slew_fix = self->ftime;
                } else { // sustain mode, so hold val
                    while (i++ < b_size) {
                        *out2++ = self->dest;
                    }
                    self->level = self->dest; // save last val
                    return out; // EARLY EXIT
                }
            } else { // normal rise
                slew_fix = self->rtime;
            }
        } else { // falling
            if (sub_diff > -nFloor) { // call it even
                if (self->mode == vtl_mode_cycle) {
                    if (self->dest == self->vel) { // AT MAX!
                        self->dest = 0.0; // go to fall
                        slew_fix = self->ftime;
                    } else { // go to rise
                        self->dest = self->vel;
                        slew_fix = self->rtime;
                    }
                } else { // hit bottom
                    while (i++ < b_size) {
                        *out2++ = self->dest;
                    }
                    self->level = self->dest;
                    return out; // EARLY EXIT
                }
            } else { // normal falling
                slew_fix = self->ftime;
            }
        }

        slew_mod = slew_fix + slew_fix * *out3 * *out3 * 2.0;
        if (slew_mod > 0.2) {
            slew_mod = 0.2;
        } // limit rate to 1/5 per samp
        *out2++ = (*out3++) + (slew_mod * sub_diff);
    }
    // save
    self->level = *out3;

    return out;
}
