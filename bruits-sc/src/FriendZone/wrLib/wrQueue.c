#include "wrQueue.h"

#include <stdio.h>
#include <stdlib.h>

queue_t* queue_init(int length) {
    queue_t* self = malloc(sizeof(queue_t));
    if (!self) {
        printf("queue malloc failed\n");
        return NULL;
    }

    self->length = length;
    self->head = 0;
    self->count = 0;
    return self;
}

int queue_enqueue(queue_t* self) {
    if (self->count >= self->length) {
        return -1;
    }
    int eq = self->head + self->count++;
    if (eq >= self->length) {
        eq -= self->length;
    };
    return eq;
}

int queue_front(queue_t* self) {
    if (self->count <= 0) {
        return -1;
    }
    return self->head;
}

int queue_dequeue(queue_t* self) {
    if (self->count <= 0) {
        return -1;
    }
    int dq = self->head++;
    self->count--;
    if (self->head >= self->length) {
        self->head -= self->length;
    };
    return dq;
}
