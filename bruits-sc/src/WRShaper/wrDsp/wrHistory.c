#include "wrHistory.h"

#include <stdio.h>
#include <stdlib.h>

#include "wrBlocks.h"

// typedef struct{
//    int max_stages;
//    int max_bsize;
//    float* history;
//    float** h;
//} history_t;

history_t* history_init(int max_stages, int max_block_size) {
    history_t* self = malloc(sizeof(history_t));
    if (!self) {
        printf("history couldn't malloc.\n");
        return NULL;
    }

    self->max_stages = max_stages;
    self->max_bsize = max_block_size;

    // sample buffer
    self->history = malloc(sizeof(float) * self->max_stages * self->max_bsize);
    if (!self->history) {
        printf("history->history couldn't malloc.\n");
        free(self);
        self = NULL;
        return NULL;
    }

    // pointers to each stage
    self->h = malloc(sizeof(float) * self->max_stages);
    if (!self->h) {
        printf("history->h couldn't malloc.\n");
        free(self->history);
        self->history = NULL;
        free(self);
        self = NULL;
        return NULL;
    }

    // link pointers to buffer
    for (int i = 0; i < max_stages; i++) {
        self->h[i] = &(self->history[i * self->max_bsize]);
    }

    self->q = queue_init(self->max_stages);
    if (!self->q) {
        printf("history->q couldn't init.\n");
        free(self->h);
        self->h = NULL;
        free(self->history);
        self->history = NULL;
        free(self);
        self = NULL;
        return NULL;
    }

    for (int i = 0; i < max_stages; i++) {
        b_cp(self->h[i], 0.0, self->max_bsize); // zero history
        queue_enqueue(self->q); // tell the queue it's full
    }

    return self;
}

void history_deinit(history_t* self) {
    queue_deinit(self->q);
    free(self->h);
    self->h = NULL;
    free(self->history);
    self->history = NULL;
    free(self);
    self = NULL;
}

float* history_write_v(history_t* self, float* in, int size) {
    queue_dequeue(self->q); // make room in the queue
    int ix = queue_enqueue(self->q);
    b_cp_v(self->h[ix], in, size);
    return in;
}

float* history_read_v(history_t* self, int stage, float* out, int size) {
    int oldest = queue_front(self->q); // front of queue is longest delay
    int s = oldest - 1 - stage; // -1 goes to oldest, then -stage to step through
    while (s < 0) {
        s += self->max_stages;
    }
    return self->h[s];
}
