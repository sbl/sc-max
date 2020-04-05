////////////////////////////////////////
// DEPRECATED. PLEASE USE wrDsp/wrVtl //
////////////////////////////////////////

#pragma once

#include <stdint.h>

typedef struct vactrol {
    float slew; // time constant
    float dest; // target value
    float y; // filtered val
} vactrol_t;

typedef struct vtl_env {
    float dest; // destination level
    float id; // current location
    uint8_t mode; // AR mode
    float vel; // static velocity setting
    float rtime;
    float ftime;
    uint8_t state; // RISE ? FALL etc
} vtl_env_t;

// variable speed
void vac_init(vactrol_t* self);
void vac_time(vactrol_t* self, float slew);
void vac_set(vactrol_t* self, float dest);
float vac_step(vactrol_t* self);
void vac_step_v(vactrol_t* self, float* out, uint16_t size);

// variable speed & ∆rise/fall relationship
void vtl_init(vtl_env_t* self);
void vtl_mode(vtl_env_t* self, uint8_t md);
void vtl_prep(vtl_env_t* self, float slew, float att);
void vtl_dest(vtl_env_t* self, float dest);
void vtl_reset(vtl_env_t* self, float dest);
float vtl_get_id(vtl_env_t* self);
float vtl_step(vtl_env_t* self);
void vtl_step_v(vtl_env_t* self, float* out, uint16_t size);
