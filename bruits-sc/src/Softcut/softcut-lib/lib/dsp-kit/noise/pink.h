//
// Created by emb on 2/29/20.
//
// adapted from code by phil burke
// www.firstpr.com.au/dsp/pink-noise/phil_burk_19990905_patest_pink.c
//
// implementation of algorithm by Voss / Gardne,
// with further optimizations by James McCartney
//
// further reference:
// http://www.firstpr.com.au/dsp/pink-noise/#Voss-McCartney


#ifndef DSP_KIT_PINK_H
#define DSP_KIT_PINK_H

#include <stdint.h>

#define PINK_MAX_RANDOM_ROWS (30)

struct pink {
    long rows[PINK_MAX_RANDOM_ROWS];
    long sum;
    int index;
    int index_mask;
    unsigned long x; // random seed
};

//--- lifecycle
extern struct pink* pink_new(int num_rows);
extern void pink_delete();

//--- process
// @param pink: pink noise structure
// @param x: uniform random value
extern int32_t pink_next(struct pink* pink);
extern void pink_seed(struct pink* pink, unsigned long x);


#endif //DSP_KIT_PINK_H
