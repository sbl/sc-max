#pragma once

#include <stdint.h>

#define LOG_VOL_CONST ((float)(0.1 / 1.1))
#define HPF_COEFF ((float)(0.997))

#define LPGATE_HPF_OFF 0
#define LPGATE_HPF_ON 1
#define LPGATE_VCA 0
#define LPGATE_FILTER 1

typedef struct lpgate {
    float level;
    float* (*lpgate_fnptr)(struct lpgate* self, float* level, float* buffer,
                           int b_size);
    float prev_lpf;
    float prev_out;
    uint8_t hpf;
    uint8_t filter;
} lpgate_t;

lpgate_t* lpgate_init(uint8_t hpf, uint8_t filter);
void lpgate_deinit(lpgate_t* self);

void lpgate_hpf_mode(lpgate_t* self, uint8_t hpf);
void lpgate_filter_mode(lpgate_t* self, uint8_t filter);
// level input expects (0-1)
float lpgate_step(lpgate_t* self, float level, float in);
float* lpgate_v(lpgate_t* self, float* level, float* buffer, int b_size);
