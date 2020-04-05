#pragma once

#include "wrGlobals.h"
#include <stdint.h>

///////////////
// ok, so rather than having tiny helper functions
// instead they can be macros!
// use an underscore and first Capital (camel) as a 'this is a macro!' warning
// they type-agnostic! yay!
//
#define _Abs(a) (a < 0 ? -(a) : a)
#define _Max(a, b) ((a) > (b) ? (a) : (b))
#define _Min(a, b) ((a) < (b) ? (a) : (b))
/*#define _Limit(in, min, max) (  \
            (in) > (min)        \
                ? ((in) < (max) \
                    ? (in)      \
                    : (max))    \
                : (min))        \
*/
#define _Swap(a, b)                                                                \
    {                                                                              \
        a ^= b;                                                                    \
        b ^= a;                                                                    \
        a ^= b;                                                                    \
    }
#define _Lim01(a) ((a > 1) ? 1 : ((a < 0) ? 0 : a))
#define _Lim(a, min, max) ((a > max) ? max : ((a < min) ? min : a))

// WARNING: doesn't work for negative numbers
#define _GetFract(a) (a - (float)(int)a)

///////////////////////////////////////////////////////

// WEIRD FUNCTIONS
void math_get_ramps(float skew, float* rise, float* fall);

// FLOAT
float max_f(float a, float b); // returns the higher of two floats
float min_f(float a, float b); // returns the lower of two floats
float lim_f(float in, float min, float max); // saturate
float lim_f_0_1(float in); // saturate (0,1)
float lim_f_n1_1(float in); // saturate (-1,1)
float wrap_f(float in, float min, float max); // wrap within range (circular buf)
// float interp_lin_asm(float in1, float in2, float mix); // linear interpolator
float interp_lin_f(float in1, float in2, float mix); // linear interpolator
float interp_lin_f_2d(float in1_x, float in2_x, float in1_y, float in2_y,
                      float mix_x, float mix_y); // linear interpolator
float dB_to_amp(float dB);
float amp_to_dB(float amp);

// INT32 (signed)
int32_t lim_i24_audio(int32_t in); // upper & lower bound limit for signed 32b int
int32_t lim_i32(int32_t in, int32_t min,
                int32_t max); // upper & lower bound limit for signed 32b int
int32_t wrap_i32(int32_t in, int32_t min,
                 int32_t max); // wrap within range (circular buf)

// INT16
int16_t min_u16(uint16_t a, uint16_t b); // returns the lower of two 16b
int16_t max_s16(int16_t a, int16_t b);

// UINT8
uint8_t min_u8(uint8_t a, uint8_t b); // returns the lower of two u8
uint8_t max_u8(uint8_t a, uint8_t b); // returns the higher of two u8

//////////////////////
// BLOCK PROCESSING //
//////////////////////

// float
// LOGIC
// 42% @8, 44% @32
void lim_vf_f(float* in, float min, float max, float* out, uint16_t size);
float* lim_vf_audio(float* audio, int size);

// ADD - MARGINALLY SLOWER
void add_vf_vf(float* a, float* b, float* sum, uint16_t size);
void add_vf_f(float* a, float b, float* sum, uint16_t size);
void sub_vf_f(float* a, float b, float* diff, uint16_t size);

// MUL - MARGINALLY SLOWER
void mul_vf_vf(float* a, float* b, float* product, uint16_t size);
// VECT * SCALAR = MODEST IMPROVEMENT!
float* mul_vf_f(float* a, float b, int size);
// mul vectorf by constf then add constf
void muladd_vf_f_f(float* vin, float mul, float add, float* product, uint16_t size);
// mul vectorf by constf then add vectorf
void muladd_vf_f_vf(float* vin, float mul, float* vadd, float* product,
                    uint16_t size);
// MAC: mul vectorf by const then accum into vectorf
void mac_vf_f_vf(float* vmul, float mul, float* vadd, uint16_t size);

// DIV - 20% REDUCTION!
void div_vf_f(float* a, float b, float* divd, uint16_t size);

// marginally slower
void interp_lin_f_v(float* a, float* b, float* c, float* out, uint16_t size);
void interp_lin_f_vvf(float* a, float* b, float c, float* out, uint16_t size);

// int32_t
// LOGIC
void lim_v32_32(int32_t* in, int32_t min, int32_t max, int32_t* out, uint16_t size);
void add_v32_v32_sat24(int32_t* a, int32_t* b, int32_t* out, uint16_t size);
void muladd_v32_f_v32_sat24(int32_t* in, float mul, int32_t* add, int32_t* out,
                            uint16_t size);
// ADD
void add_v32_32(int32_t* a, int32_t b, int32_t* sum, uint16_t size);

// BITWISE
void SHR_v32_32(int32_t* a, uint16_t shift, int32_t* out,
                uint16_t size); // right SHIFT vector by constant
void SHRadd_v32_32(int32_t* a, uint16_t shift, int32_t add, int32_t* out,
                   uint16_t size);
// uint8_t
// SET
void set_v8_v8(uint8_t* b, uint8_t* out, uint16_t size);
void set_v8_8(uint8_t b, uint8_t* out, uint16_t size);

// ADD
void add_v8_v8(uint8_t* a, uint8_t* b, uint8_t* sum, uint16_t size);
void add_v8_8(uint8_t* a, uint8_t b, uint8_t* sum, uint16_t size);

// BITWISE
void OR_v8_8(uint8_t* a, uint8_t mask, uint8_t* out,
             uint16_t size); // OR vector w mask
void AND_v8_8(uint8_t* a, uint8_t mask, uint8_t* out,
              uint16_t size); // AND vector w mask
void SHL_v8_8(uint8_t* a, uint8_t shift, uint8_t* out,
              uint16_t size); // left SHIFT vector by constant
void SHR_v8_8(uint8_t* a, uint8_t shift, uint8_t* out,
              uint16_t size); // right SHIFT vector by constant
