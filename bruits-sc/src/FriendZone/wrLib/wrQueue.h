#pragma once

// queue library which just handles array indices
// handle your own data and just use this system to manage over/underflow

typedef struct {
    int head;
    int count;
    int length;
} queue_t;

queue_t* queue_init(int length);

// returns valid array index for success
// returns -1 for over/underflow
int queue_enqueue(queue_t* self);
int queue_front(queue_t* self); // non-destructive query
int queue_dequeue(queue_t* self);
