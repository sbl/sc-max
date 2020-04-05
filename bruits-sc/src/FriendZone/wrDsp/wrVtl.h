#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { vtl_mode_transient, vtl_mode_sustain, vtl_mode_cycle } vtl_mode_t;

typedef struct {
    float dest;
    float level;
    float vel;
    float rtime;
    float ftime;
    vtl_mode_t mode;
    uint8_t state; // RISE ? FALL etc
} vtl_t;

// variable speed & ∆rise/fall relationship
vtl_t* vtl_init(void);
void vtl_deinit(vtl_t* self);
void vtl_mode(vtl_t* self, vtl_mode_t mode);
void vtl_params(vtl_t* self, float speed, float ARratio);
void vtl_dest(vtl_t* self, float dest);
float vtl_get_level(vtl_t* self);
float vtl_step(vtl_t* self);

float* vtl_step_v(vtl_t* self, float* out, int b_size);

#ifdef __cplusplus
}
#endif
