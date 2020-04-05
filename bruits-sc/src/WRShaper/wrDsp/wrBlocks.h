#pragma once

#include <stdint.h>

float* b_cp(float* dest, float src, int size); // memset
float* b_cp_v(float* dest, float* src, int size);
float* b_add(float* io, float add, int size);
float* b_sub(float* io, float sub, int size);
float* b_mul(float* io, float mul, int size);
float* b_accum_v(float* dest, float* src, int size); // += *src
float* b_accum_vv(float** signals, int count, int size); // reduce(+=)

typedef float(b_fn_t)(float in);
float* b_map(b_fn_t fn, float* io, int size);

typedef float*(b_reducer_t)(float* dest, float* src, int size);
float* b_reduce_vv(b_reducer_t reduce, float** signals, int count, int size);
