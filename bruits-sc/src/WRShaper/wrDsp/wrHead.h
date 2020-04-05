#pragma once

#include "wrFilter.h" // filter_lp
#include "wrResamp.h"

#define HEAD_SLEW_RECORD ((float)(0.05))
#define HEAD_SLEW_FEEDBACK ((float)(0.05))
#define HEAD_SLEW_MONITOR ((float)(0.05))

#define INVALID_SAMP 0x7FFFFFFF
#define BIT_HEADROOM 0
#define F_TO_TAPE_SCALE ((float)(MAX24b >> BIT_HEADROOM))

typedef struct {
    filter_lp1_t* record;
    filter_lp1_t* feedback;
    filter_lp1_t* monitor;
} rhead_t;

// tapehead mode, refactored from tapedeck.h
typedef enum {
    READONLY,
    OVERDUB,
    OVERWRITE,
    LOOPER // overwrite, but returns old tape content (external fb)
    ,
    tape_mode_count
} tape_mode_t;

typedef enum {
    HEAD_Inactive = -1,
    HEAD_Active,
    HEAD_Fadein,
    HEAD_Fadeout
} HEAD_Action_t; // motion_block_t

rhead_t* RH_Init(void);
void RH_DeInit(rhead_t* self);

void RH_set_rw(rhead_t* self, tape_mode_t rmode);

void RH_set_record_level(rhead_t* self, float level);
void RH_set_record_params(rhead_t* self, float feedback, float monitor);

IO_block_t* RH_rw_process(rhead_t* self, IO_block_t* headbuf, HEAD_Action_t action,
                          int32_t** access, int count, int* dirty,
                          int write_offset);
float RH_rw_process_cv(rhead_t* self, float input, uint8_t action // HEAD_Action_t
                       ,
                       int16_t* cv_read, int* dirty, int write_offset);
