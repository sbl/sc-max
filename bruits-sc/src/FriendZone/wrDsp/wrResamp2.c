#include "wrResamp2.h"

#include "wrInterpolate.h"

// read the ssample at phase from a given buffer of known length
float buffer_peek(float* buf, int buf_len, float phase) {
    // find sample indices
    int p0 = (int)phase;
    int pn1 = p0 - 1;
    int p1 = p0 + 1;
    int p2 = p0 + 2;

    // bounds check
    if (pn1 < 0) {
        pn1 += buf_len;
    }
    if (p1 >= buf_len) {
        p1 -= buf_len;
    }
    if (p2 >= buf_len) {
        p2 -= buf_len;
    }

    // calc coefficient
    float coeff = phase - (float)p0;

    // interpolate array to find value
    float samps[4] = { buf[pn1], buf[p0], buf[p1], buf[p2] };
    return interp_hermite_4pt(coeff, &samps[1]);
}

float buffer_peek_unsafe(float* buf, float phase) {
    return interp_hermite_4pt(phase - (float)(int)phase, buf);
}

float* buffer_peek_v(float* io, float* buf, int buf_len,
                     float phase // initial sample
                     ,
                     float* rate // step through buf with this array
                     ,
                     int size) {
    // working buffers
    float maxrate = (rate[0] > rate[size - 1]) ? rate[0] : rate[size - 1];
    int sampsize = maxrate * size + 4; // maybe +3?
    float s[sampsize];
    int p0 = (int)phase;

    { // build contiguous source array
        int ix[sampsize];
        int pn1 = p0 - 1;
        for (int i = 0; i < sampsize; i++) {
            // TODO can this index builder be optimized out?
            ix[i] = pn1 + i;
        }
        { // edge checking from both ends until first in range
            int i;
            i = 0;
            while (ix[i] < 0) {
                ix[i++] += buf_len;
            }
            i = sampsize - 1;
            while (ix[i] >= buf_len) {
                ix[i--] -= buf_len;
            }
        }
        for (int i = 0; i < sampsize; i++) {
            s[i] = buf[ix[i]];
        }
    }

    // interpolate
    float* o = io;
    float coeff = phase - (float)p0;
    int ix = 1; // allow for leading samp
    for (int i = 0; i < size; i++) {
        *o++ = interp_hermite_4pt(coeff, &s[ix]);
        coeff += *rate++;
        while (coeff >= 1.0) {
            coeff -= 1.0;
            ix++;
        }
    }
    return io;
}
