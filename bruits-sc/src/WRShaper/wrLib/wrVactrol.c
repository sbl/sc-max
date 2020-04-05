#include "wrVactrol.h"

#include <wrGlobals.h>
#include <wrMath.h>

/////////////////
// VACTROL EMU //
/////////////////

// only the basic version
// takes single param to time-scale the response
void vac_init(vactrol_t* self) {
    self->slew = 0.0f; // speed
    self->dest = 0.0f; // destination
    self->y = 0.0f; // current
}

void vac_time(vactrol_t* self, float slew) {
    self->slew = lim_f_0_1(slew); // set vactrol decay
}

void vac_set(vactrol_t* self, float dest) {
    self->dest = dest; // set destination value
}

float vac_step(vactrol_t* self) {
    float sub_diff, slew_mod;

    // process a sample of vactrol movement

    // difference between current & dest
    sub_diff = self->dest - self->y;

    if (sub_diff > nFloor) {
        slew_mod = 0.008f * self->slew; // if rising, set attack rate
    } else if (sub_diff < -nFloor) {
        slew_mod = 0.0004f * self->slew;
    } else { // below noise floor
        self->y = self->dest;
        return self->y; // EARLY EXIT !!!
    }

    slew_mod = slew_mod
        + slew_mod * self->y * self->y
            * 2.0f; // some kind of hysteresis: out += 2 * in * previous^2

    self->y = self->y + slew_mod * sub_diff;

    return self->y; // needs to be limited to 0-1f
                    // return self->dest; // bypass
}

void vac_step_v(vactrol_t* self, float* out, uint16_t size) {
    float sub_diff, slew_mod, slew_fix;
    float* out2 = out;
    float* out3 = out;

    // always converging to self->dest, so each block stays on the same side
    sub_diff = self->dest - self->y;

    if (sub_diff > nFloor) {
        slew_fix = 0.008f * self->slew; // if rising, set attack rate
    } else if (sub_diff < -nFloor) {
        slew_fix = 0.0004f * self->slew; // else falling, 16x slower
    } else { // below noise floor
        for (uint16_t i = 0; i < size; i++) {
            *out2++ = self->dest;
        }
        self->y = self->dest;
        return; // EARLY EXIT !!!
    }

    // some kind of hysteresis: out += 2 * in * previous^2
    slew_mod = slew_fix + slew_fix * self->y * self->y * 2.0f;
    *out2++ = self->y + slew_mod * sub_diff;

    for (uint16_t i = 0; i < (size - 1); i++) {
        sub_diff = self->dest - (*out3);
        slew_mod = slew_fix
            + slew_fix * (*out3) * (*out3)
                * 2.0f; // some kind of hysteresis: out += 2 * in * previous^2
        *out2++ = *out3++ + slew_mod * sub_diff;
    }
    // save
    self->y = *out3;
}

///////////////////
// FANCY VACTROL //
///////////////////

void vtl_init(vtl_env_t* self) {
    self->dest = 0.0f; // f
    self->id = 0.0f; // f
    self->mode = 1; // sustain mode
    self->vel = 1.0f; // destination level, ignores zeroes
    self->rtime = 0.5f;
    self->ftime = 0.5f;
}

void vtl_mode(vtl_env_t* self, uint8_t md) {
    self->mode = md;
    if (md != 1) { // not sustain mode
        self->dest = 0.0f;
    }
}

void vtl_prep(vtl_env_t* self, float slew, float att) {
    // set rtn flag to signal prep has updated
    float rt = self->rtime;
    math_get_ramps(lim_f_0_1(att), &(self->rtime), &(self->ftime));
    float ttime = lim_f_0_1(slew) * 0.007f; // 0.5 = vtl5c3
    ttime = lim_f(ttime, 0.000001f, 1.0f); // what is crashing?
    self->rtime *= ttime;
    self->ftime *= ttime;
}

// unroll this in your loop if calling it per sample (otherwise it's minor
// improvement)
void vtl_dest(vtl_env_t* self, float dest) {
    self->dest = dest; // set destination value

    if (dest > nFloor) {
        self->vel = dest; // treat as 'velocity' input
    }
}

// restart the VTL from zero, & go to input destination
void vtl_reset(vtl_env_t* self, float dest) {
    self->dest = dest; // set destination value
    // self->vel = rest;
    self->id = 0.0f;
    if (dest > nFloor) {
        self->vel = dest;
    }
}

float vtl_get_id(vtl_env_t* self) {
    return self->id;
}

float vtl_step(vtl_env_t* self) {
    float slew_mod;
    float location = self->id;

    // difference between current & dest
    float sub_diff = self->dest - self->id;

    if (sub_diff > 0.0f) { // rising
        slew_mod = self->rtime;
        slew_mod =
            min_f(slew_mod + slew_mod * location * location * 2.0f,
                  0.2f); // some kind of hysteresis: out += 2 * in * previous^2
        self->id += slew_mod * sub_diff;

        if (sub_diff < nFloor) { // we're close enough! (~58dB)
            if (self->mode != 1) {
                self->dest = 0; // go toward zero if not sustaining
            }
        }
    } else { // falling
        slew_mod = self->ftime;
        slew_mod =
            min_f(slew_mod + slew_mod * location * location * 2.0f,
                  0.2f); // some kind of hysteresis: out += 2 * in * previous^2
        self->id += slew_mod * sub_diff;

        if (sub_diff > -nFloor) { // if we've gone past the dest
            if (self->mode == 2) {
                self->dest = self->vel; // rise if cycling
            }
        }
    }
    return self->id; // needs to be limited to 0-1f
}

void vtl_step_v(vtl_env_t* self, float* out, uint16_t size) {
    float slew_mod, slew_fix;
    float* out2 = out;
    float* out3 = out;
    uint16_t i;

    // difference between current & dest
    float sub_diff = self->dest - self->id;

    if (sub_diff > 0.0f) { // rising
        if (sub_diff < nFloor) { // call it even
            if (self->mode != 1) { // not sustain
                self->dest = 0.0f; // go toward zero
                slew_fix = self->ftime;
            } else { // sustain mode, so hold val
                // escape w/ fixed output value
                for (i = 0; i < size; i++) {
                    *out2++ = self->dest;
                }
                self->id = self->dest; // save last val
                return; // EARLY EXIT
            }
        } else { // normal rise
            slew_fix = self->rtime;
        }
    } else { // falling
        if (sub_diff > -nFloor) { // call it even
            if (self->mode == 2) { // cycle mode
                if (self->dest == self->vel) { // AT MAX!
                    self->dest = 0.0f; // go to fall
                    slew_fix = self->ftime;
                } else { // go to rise
                    self->dest = self->vel;
                    slew_fix = self->rtime;
                }
            } else { // hit bottom
                for (i = 0; i < size; i++) {
                    *out2++ = self->dest;
                }
                self->id = self->dest;
                return; // EARLY EXIT
            }
        } else { // normal falling
            slew_fix = self->ftime;
        }
    }

    // some kind of hysteresis: out += 2 * in * previous^2
    slew_mod = slew_fix + slew_fix * self->id * self->id * 2.0f;
    if (slew_mod > 0.2f) {
        slew_mod = 0.2f;
    } // limit rate to 1/5 per samp
    *out2++ = self->id + (slew_mod * sub_diff);

    for (i = 1; i < size; i++) {
        sub_diff = self->dest - *out3;

        if (sub_diff > 0.0f) { // rising
            if (sub_diff < nFloor) { // call it even
                if (self->mode != 1) { // not sustain
                    self->dest = 0.0f; // go toward zero
                    slew_fix = self->ftime;
                } else { // sustain mode, so hold val
                    for (i; i < size; i++) {
                        *out2++ = self->dest;
                    }
                    self->id = self->dest; // save last val
                    return; // EARLY EXIT
                }
            } else { // normal rise
                slew_fix = self->rtime;
            }
        } else { // falling
            if (sub_diff > -nFloor) { // call it even
                if (self->mode == 2) { // cycle mode
                    if (self->dest == self->vel) { // AT MAX!
                        self->dest = 0.0f; // go to fall
                        slew_fix = self->ftime;
                    } else { // go to rise
                        self->dest = self->vel;
                        slew_fix = self->rtime;
                    }
                } else { // hit bottom
                    for (i; i < size; i++) {
                        *out2++ = self->dest;
                    }
                    self->id = self->dest;
                    return; // EARLY EXIT
                }
            } else { // normal falling
                slew_fix = self->ftime;
            }
        }

        slew_mod = slew_fix + slew_fix * *out3 * *out3 * 2.0f;
        if (slew_mod > 0.2f) {
            slew_mod = 0.2f;
        } // limit rate to 1/5 per samp
        *out2++ = (*out3++) + (slew_mod * sub_diff);
    }
    // save
    self->id = *out3;
    return;
}
