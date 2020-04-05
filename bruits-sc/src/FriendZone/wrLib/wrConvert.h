#pragma once

#include "wrGlobals.h"
#include <stdint.h>

// pickling
int16_t _sf_to_s12(float cv);
float _s12_to_sf(int16_t cv);