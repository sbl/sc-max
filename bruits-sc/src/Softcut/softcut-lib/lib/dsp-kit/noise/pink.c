//
// Created by emb on 2/29/20.
//
// see pink.h comments for attribution

#include <malloc.h>
#include "pink.h"

#define PINK_RANDOM_BITS       (24)
#define PINK_RANDOM_SHIFT      ((sizeof(long)*8)-PINK_RANDOM_BITS)

static long pink_rand(struct pink* pink) {
    pink->x = (pink->x * 196314165) +  907633515;
    return ((long)pink->x) >> PINK_RANDOM_SHIFT;
}
struct pink* pink_new(int num_rows) {
    struct pink* pink = malloc(sizeof(struct pink));
    pink->index = 0;
    pink->index_mask = (1<<num_rows)- 1;
    for (int i=0; i<num_rows; ++i) {
        pink->rows[i] = 0;
    }
    pink->sum = 0;
    return pink;
}

void pink_delete(struct pink* pink) {
    free(pink);
}

 void pink_seed(struct pink* pink, unsigned long x) {
    pink->x = x;
}

int32_t pink_next(struct pink* pink) {
    long rand;
    pink->index = (pink->index + 1) & pink->index_mask;

    if (pink->index != 0) {
        unsigned int num_zeros = 0;
        unsigned int n = pink->index;
        while ((n & 1) == 0) {
            n = n >> 1;
            ++num_zeros;
        }
        pink->sum -= pink->rows[num_zeros];
        rand = pink_rand(pink);
        pink->sum += rand;
        pink->rows[num_zeros] = rand;
    }
    long sum = pink->sum;
    // add white noise
    sum += pink_rand(pink);
    return sum;
}