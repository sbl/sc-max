#pragma once

typedef struct IO_block {
    int size;
    float* audio;
} IO_block_t; // dsp_block.h, tape.h, rhead.h

IO_block_t* resamp_to(float* speed, IO_block_t* codec, IO_block_t* tapeio,
                      int s_origin, float s_interp);
IO_block_t* resamp_from(float* speed, IO_block_t* tapeio, int s_origin,
                        float s_interp, IO_block_t* codec
                        //                                , int         b_size
);
