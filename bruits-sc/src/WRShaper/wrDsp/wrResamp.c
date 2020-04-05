#include "wrResamp.h"
#include "wrMath.h" // _Abs

static void interp_fast(float* codec, float* tape, float coeff, float* speed,
                        int b_size) {
    for (int i = 0; i < b_size; i++) {
        // this does *not* result in equal power with varying coeff
        float isp = 1.0 / _Abs(*speed);
        float iRcB = isp * coeff;
        float c1 = iRcB - isp + 1.0;
        float c2 = 1.0 - iRcB;

        // variable width linear interp impulse response _/\_
        tape[-1] += *codec * lim_f_0_1(c2 - isp);
        tape[0] += *codec * c2;
        tape[1] += *codec * c1;
        tape[2] += *codec * lim_f_0_1(c1 - isp);

        codec++;
        coeff += *speed++;
        while (coeff < 0.0) {
            coeff += 1.0;
            tape--;
        }
        while (coeff >= 1.0) {
            coeff -= 1.0;
            tape++;
        }
    }
}

static void interp_normal(float* codec, float* tape, float coeff, float* speed,
                          int b_size) {
    for (int i = 0; i < b_size; i++) {
        tape[0] += *codec * (1.0 - coeff);
        tape[1] += *codec++ * coeff;

        coeff += *speed++;
        while (coeff < 0.0) {
            coeff += 1.0;
            tape--;
        }
        while (coeff >= 1.0) {
            coeff -= 1.0;
            tape++;
        }
    }
}

static void interp_slow(float* codec, float* tape, float coeff, float* speed,
                        int b_size) {
    for (int i = 0; i < b_size; i++) {
        // volume scale (== impulse shaping)
        float voila = *codec++ * _Abs(*speed);
        tape[0] += voila * (1.0 - coeff);
        tape[1] += voila * coeff;

        coeff += *speed++;
        while (coeff < 0.0) {
            coeff += 1.0;
            tape--;
        }
        while (coeff >= 1.0) {
            coeff -= 1.0;
            tape++;
        }
    }
}

IO_block_t* resamp_to(float* speed, IO_block_t* codec, IO_block_t* tapeio,
                      int s_origin, float s_interp
                      //                     , int         b_size
) {
    float aspd = _Abs(speed[0]);
    if (aspd > 1.0) {
        interp_fast(codec->audio, &(tapeio->audio[s_origin]), s_interp, speed,
                    codec->size);
    } else if (aspd == 1.0) {
        interp_normal(codec->audio, &(tapeio->audio[s_origin]), s_interp, speed,
                      codec->size);
    } else {
        interp_slow(codec->audio, &(tapeio->audio[s_origin]), s_interp, speed,
                    codec->size);
    }
    return tapeio;
}

IO_block_t* resamp_from(float* speed, IO_block_t* tapeio, int s_origin,
                        float s_interp, IO_block_t* codec) {
    float* buf = codec->audio;
    float* hb = &(tapeio->audio[s_origin - 1]); // wide for lagrange
    float co = s_interp;
    for (int i = 0; i < (codec->size); i++) {
        float coeff[4];
        // shifted from the textbook by +1 to co (shift range to 0-1)
        // Julius Smith III on 3rd order lagrange interpolation
        coeff[0] = -((co) * (co - 1.0) * (co - 2.0)) / 6.0;
        coeff[1] = ((co + 1.0) * (co - 1.0) * (co - 2.0)) / 2.0;
        coeff[2] = -((co + 1.0) * (co) * (co - 2.0)) / 2.0;
        coeff[3] = ((co + 1.0) * (co) * (co - 1.0)) / 6.0;

        *buf = 0.0;
        for (int s = 0; s < 4; s++) {
            *buf += hb[s] * coeff[s];
        }
        buf++;
        co += *speed++;
        while (co < 0.0) {
            co += 1.0;
            hb--;
        }
        while (co >= 1.0) {
            co -= 1.0;
            hb++;
        }
    }
    return codec;
}
