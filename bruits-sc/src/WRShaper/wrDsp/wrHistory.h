#pragma once

#include "../wrLib/wrQueue.h"

typedef struct {
    int max_stages;
    int max_bsize;
    float* history;
    float** h;
    queue_t* q;
} history_t;

history_t* history_init(int max_stages, int max_block_size);
void history_deinit(history_t* self);

float* history_write_v(history_t* self, float* in, int size);
float* history_read_v(history_t* self, int stages, float* out, int size);
