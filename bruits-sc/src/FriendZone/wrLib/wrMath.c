#include "wrMath.h"

#include <math.h>

// this function takes 0-1
// does a 1/x lookup with inverse
// particularly useful for ramp->tri->saw morphs
// where total time (pitch) remains stable
// NB: 3 divisions. use a LUT if you're out of cycles
void math_get_ramps(float skew, float* rise, float* fall) {
    *rise = 0.5f / (0.998f * skew + 0.001f);
    *fall = 1.0f / (2.0f - (1.0f / *rise));
}

float max_f(float a, float b) {
    return (a > b ? a : b);
}

float min_f(float a, float b) {
    return (a < b ? a : b);
}

float lim_f(float in, float min, float max) {
    return (in < min ? min : in > max ? max : in);
}

float lim_f_0_1(float in) {
    return (in < 0.0f ? 0.0f : in > 1.0f ? 1.0f : in);
}

float lim_f_n1_1(float in) {
    return (in < -1.0f ? -1.0f : in > 1.0f ? 1.0f : in);
}

float wrap_f(float in, float min, float max) {
    float diff = max - min;

    while (in < min) {
        in += diff;
    }
    while (in >= max) {
        in -= diff;
    }

    return in;
}

float dB_to_amp(float dB) {
    return powf(10.0, dB / 20.0);
}

float amp_to_dB(float amp) {
    return 20.0 * log10f(amp);
}

/*float interp_lin_asm(float in1, float in2, float mix)
{
    // accurate at min & max. arm cortex m4 instruction set
    __asm__(
        "vfnms.f32  s0, s0, s2 \n\t"
        "vfnms.f32  s0, s1, s2 \n\t"
        );
}*/

float interp_lin_f(float in1, float in2, float mix) {
    return (in1 + mix * (in2 - in1));
}
float interp_lin_f_2d(float in1_x, float in2_x, float in1_y, float in2_y,
                      float mix_x, float mix_y) {
    float tmp, tmp2;
    tmp = in1_x + mix_x * (in2_x - in1_x);
    tmp2 = in1_y + mix_x * (in2_y - in1_y);
    return (tmp + mix_y * (tmp2 - tmp));
}

int32_t lim_i24_audio(int32_t in) {
    return ((in < (int32_t)MIN24b) ? (int32_t)MIN24b
                                   : (in > (int32_t)MAX24b) ? (int32_t)MAX24b : in);
}

int32_t lim_i32(int32_t in, int32_t min, int32_t max) {
    return (in < min ? min : in > max ? max : in);
}
int32_t wrap_i32(int32_t in, int32_t min, int32_t max) {
    int32_t diff = max - min;

    while (in < min) {
        in += diff;
    }
    while (in > max) {
        in -= diff;
    }

    return in;
}
int16_t min_u16(uint16_t a, uint16_t b) {
    return (a < b ? a : b);
}
uint8_t min_u8(uint8_t a, uint8_t b) {
    return (a < b ? a : b);
}
uint8_t max_u8(uint8_t a, uint8_t b) {
    return (a > b ? a : b);
}

/////////////////////////////////////
// block processing math functions //
/////////////////////////////////////

void lim_v32_32(int32_t* in, int32_t min, int32_t max, int32_t* out,
                uint16_t size) {
    int32_t* in2 = in;
    int32_t* out2 = out;

    for (uint16_t i = 0; i < size; i++) {
        if (*in2 < min) {
            *out2++ = min; // lower limit
            in2++;
        } else if (*in2 > max) {
            *out2++ = max; // upper limit
            in2++;
        } else {
            *out2++ = *in2++; // echo in range
        }
    }
}
void add_v32_v32_sat24(int32_t* a, int32_t* b, int32_t* out, uint16_t size) {
    int32_t* a2 = a;
    int32_t* b2 = b;
    int32_t* out2 = out;

    for (uint16_t i = 0; i < size; i++) {
        *out2 = (*a2++) + (*b2++);

        if (*out2 < MIN24b) {
            *out2++ = MIN24b;
        } else if (*out2 > MAX24b) {
            *out2++ = MAX24b; // upper limit
        } else {
            out2++;
        }
    }
}

void muladd_v32_f_v32_sat24(int32_t* in, float mul, int32_t* add, int32_t* out,
                            uint16_t size) {
    int32_t* in2 = in;
    int32_t* add2 = add;
    int32_t* out2 = out;

    for (uint16_t i = 0; i < size; i++) {
        // multiply v32 by float (in * thrulevel)
        // add above(vf) to (vi)output
        *out2 = (int32_t)((float)(*in2++) * mul) + (*add2++);
        // saturate to 24b
        if (*out2 < MIN24b) {
            *out2++ = MIN24b;
        } else if (*out2 > MAX24b) {
            *out2++ = MAX24b; // upper limit
        } else {
            out2++;
        }
    }
}

void lim_vf_f(float* in, float min, float max, float* out, uint16_t size) {
    float* in2 = in;
    float* out2 = out;

    for (uint16_t i = 0; i < size; i++) {
        if (*in2 < min) {
            *out2++ = min;
        } // lower limit
        else if (*in2 > max) {
            *out2++ = max;
        } // upper limit
        else {
            *out2++ = *in2;
        } // echo in range
        in2++;
    }
}

float* lim_vf_audio(float* audio, int size) {
    float* s = audio;
    for (int i = 0; i < size; i++) {
        if (*s < -1.0) {
            *s++ = -1.0;
        } // lower limit
        else if (*s > 1.0) {
            *s++ = 1.0;
        } // upper limit
        else {
            s++;
        } // don't change
    }
    return audio;
}

// set an array to a single value
void set_v32_32(int32_t b, int32_t* out, uint16_t size) {
    int32_t* out2 = out; // point to start of arrays

    for (uint16_t i = 0; i < size; i++) {
        *out2++ = b;
    }
}

//////////////////////////
////////////////////// ADD

// add two arrays of floats sequentially
void add_vf_vf(float* a, float* b, float* sum, uint16_t size) {
    float* a2 = a;
    float* b2 = b;
    float* sum2 = sum; // point to start of arrays

    for (uint16_t i = 0; i < size; i++) {
        *sum2++ = (*a2++) + (*b2++);
    }
}

// increment float array by scalar
void add_vf_f(float* a, float b, float* sum, uint16_t size) {
    float* a2 = a;
    float* sum2 = sum; // point to start of arrays

    for (uint16_t i = 0; i < size; i++) {
        *sum2++ = (*a2++) + b;
    }
}
void sub_vf_f(float* a, float b, float* diff, uint16_t size) {
    float* a2 = a;
    float* diff2 = diff; // point to start of arrays

    for (uint16_t i = 0; i < size; i++) {
        *diff2++ = (*a2++) - b;
    }
}
//////////////////////////
////////////////////// MUL

// vector multiplication
void mul_vf_vf(float* a, float* b, float* product, uint16_t size) {
    float* a2 = a;
    float* b2 = b;
    float* product2 = product; // point to start of arrays

    for (uint16_t i = 0; i < size; i++) {
        *product2++ = (*a2++) * (*b2++);
    }
}

// vector x constant multiply
float* mul_vf_f(float* a, float b, int size) {
    float* a2 = a;
    for (int i = 0; i < size; i++) {
        *a2++ *= b;
    }
    return a;
}

void muladd_vf_f_f(float* vin, float mul, float add, float* product,
                   uint16_t size) {
    float* vin2 = vin;
    float* product2 = product; // point to start of arrays

    for (uint16_t i = 0; i < size; i++) {
        *product2++ = (*vin2++) * mul + add;
    }
}
void muladd_vf_f_vf(float* vin, float mul, float* vadd, float* product,
                    uint16_t size) {
    float* vin2 = vin;
    float* vadd2 = vadd;
    float* product2 = product; // point to start of arrays

    for (uint16_t i = 0; i < size; i++) {
        *product2++ = (*vin2++) * mul + (*vadd2++);
    }
}
void mac_vf_f_vf(float* vmul, float mul, float* vadd, uint16_t size) {
    float* vmul2 = vmul;
    float* vadd2 = vadd; // point to start of arrays

    for (uint16_t i = 0; i < size; i++) {
        (*vadd2++) += (*vmul2++) * mul;
    }
}

//////////////////////////
////////////////////// DIV

// vector x constant divion
void div_vf_f(float* a, float b, float* divd, uint16_t size) {
    float* a2 = a;
    float* divd2 = divd; // point to start of arrays

    for (uint16_t i = 0; i < size; i++) {
        *divd2++ = (*a2++) / b;
    }
}

//////////////////////////
/////////////////// INTERP

void interp_lin_f_v(float* a, float* b, float* c, float* out, uint16_t size) {
    float* a2 = a;
    float* b2 = b;
    float* c2 = c;
    float* out2 = out;

    for (uint16_t i = 0; i < size; i++) {
        *out2++ = *a2 + *c2++ * (*b2++ - *a2);
        a2++;
    }
}

void interp_lin_f_vvf(float* a, float* b, float c, float* out, uint16_t size) {
    float* a2 = a;
    float* b2 = b;
    float* out2 = out;

    for (uint16_t i = 0; i < size; i++) {
        *out2++ = *a2 + c * (*b2++ - *a2);
        a2++;
    }
}
//////////////////////////
//////////////// SET VALUE

// copy whole vector
void set_v8_v8(uint8_t* b, uint8_t* out, uint16_t size) {
    uint8_t* b2 = b;
    uint8_t* out2 = out; // point to start of arrays

    for (uint16_t i = 0; i < size; i++) {
        *out2++ = (*b2++);
    }
}

// set vector to a constant
void set_v8_8(uint8_t b, uint8_t* out, uint16_t size) {
    uint8_t* out2 = out; // point to start of arrays

    for (uint16_t i = 0; i < size; i++) {
        *out2++ = b;
    }
}

//////////////////////////
////////////////////// ADD

// add two vectors
void add_v8_v8(uint8_t* a, uint8_t* b, uint8_t* sum, uint16_t size) {
    uint8_t* a2 = a;
    uint8_t* b2 = b;
    uint8_t* sum2 = sum; // point to start of arrays

    for (uint16_t i = 0; i < size; i++) {
        *sum2++ = (*a2++) + (*b2++);
    }
}

// shift vector by scalar
void add_v8_8(uint8_t* a, uint8_t b, uint8_t* sum, uint16_t size) {
    uint8_t* a2 = a;
    uint8_t* sum2 = sum; // point to start of arrays

    for (uint16_t i = 0; i < size; i++) {
        *sum2++ = (*a2++) + b;
    }
}

void add_v32_32(int32_t* a, int32_t b, int32_t* sum, uint16_t size) {
    int32_t* a2 = a;
    int32_t* sum2 = sum; // point to start of arrays

    for (uint16_t i = 0; i < size; i++) {
        *sum2++ = (*a2++) + b;
    }
}

/*
void lim_vf_f(float* in, float min, float max, float* out, uint16_t size) {
    float* in2=in;
    float* out2=out;

    for(uint16_t i=0; i<size; i++) {
        if(*in2 < min) { *out2++ = min; } // lower limit
        else if(*in2 > max) { *out2++ = max; } // upper limit
        else { *out2++ = *in2; } // echo in range
        *in2++;
    }
}
*/

//////////////////////////
////////////////// BITWISE

// bitwise OR a vector with a single mask
void OR_v8_8(uint8_t* a, uint8_t mask, uint8_t* out, uint16_t size) {
    uint8_t* a2 = a;
    uint8_t* out2 = out; // point to start of arrays

    for (uint16_t i = 0; i < size; i++) {
        *out2++ = (*a2++) | mask;
    }
}

// bitwise AND a vector with a single mask
void AND_v8_8(uint8_t* a, uint8_t mask, uint8_t* out, uint16_t size) {
    uint8_t* a2 = a;
    uint8_t* out2 = out; // point to start of arrays

    for (uint16_t i = 0; i < size; i++) {
        *out2++ = (*a2++) & mask;
    }
}

// left bitshift
void SHL_v8_8(uint8_t* a, uint8_t shift, uint8_t* out, uint16_t size) {
    uint8_t* a2 = a;
    uint8_t* out2 = out; // point to start of arrays

    for (uint16_t i = 0; i < size; i++) {
        *out2++ = (*a2++) << shift;
    }
}

// right bitshift
void SHR_v8_8(uint8_t* a, uint8_t shift, uint8_t* out, uint16_t size) {
    uint8_t* a2 = a;
    uint8_t* out2 = out; // point to start of arrays

    for (uint16_t i = 0; i < size; i++) {
        *out2++ = (*a2++) >> shift;
    }
}

// right bitshift
// about 30% faster!
void SHR_v32_32(int32_t* a, uint16_t shift, int32_t* out, uint16_t size) {
    int32_t* a2 = a;
    int32_t* out2 = out; // point to start of arrays

    for (uint16_t i = 0; i < size; i++) {
        *out2++ = (*a2++) >> shift;
    }
}
void SHRadd_v32_32(int32_t* a, uint16_t shift, int32_t add, int32_t* out,
                   uint16_t size) {
    int32_t* a2 = a;
    int32_t* out2 = out; // point to start of arrays

    for (uint16_t i = 0; i < size; i++) {
        *out2++ = ((*a2++) >> shift) + add;
    }
}

int16_t max_s16(int16_t a, int16_t b) {
    return ((a < b) ? a : b);
}
