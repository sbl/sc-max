#pragma once

#include <stdint.h>

#include "wrFilter.h"

typedef struct VU_meter {
    filter_lp1_t* smooth;
} VU_meter_t;

// variable speed
VU_meter_t* VU_init(void);
void VU_deinit(VU_meter_t* self);
void VU_time(VU_meter_t* self, float slew);
float VU_step(VU_meter_t* self, float in);
