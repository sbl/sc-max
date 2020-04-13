
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include <sndfile.h>

#include "lcg.h"
#include "pink.h"
#include "poisson.h"

void test_lcg_speed() {
    float x;
    struct timeval stop, start;

    int n = 1000000;

    // testing speed vs stdlib
    gettimeofday(&start, NULL);

    for (int i = 0; i < n; ++i) {
        x = (float) rand() / (float) RAND_MAX;
        (void) x;
    }
    gettimeofday(&stop, NULL);
    printf("rand(): %lu us\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);

    struct lcg *lcg = lcg_new();
    gettimeofday(&start, NULL);

    for (int i = 0; i < n; ++i) {
        x = lcg_get_float(lcg);
        (void) x;
    }

    gettimeofday(&stop, NULL);
    printf("lcg->get_float(): %lu us\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);
}

void test_lcg_signal(int frames) {
    struct lcg *lcg = lcg_new();
    float x;

    SNDFILE* sf;
    SF_INFO sfinfo;

    sfinfo.channels = 1;
    sfinfo.samplerate = 48000;
    sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_24;
    sf = sf_open ("lcg.wav", SFM_WRITE, &sfinfo);

    float* buf = malloc(sizeof(float) * frames);

    for (int i = 0; i < frames; ++i) {
        buf[i] = lcg_get_float(lcg) * 0.5;
    }
    sf_write_float(sf, buf, frames);
    sf_close(sf);
    free(buf);
}

void test_pink_signal(int frames) {
    struct lcg *lcg = lcg_new();
    struct pink *pink = pink_new(16);
    int z;

    SNDFILE* sf;
    SF_INFO sfinfo;

    sfinfo.channels = 1;
    sfinfo.samplerate = 48000;
    sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_24;
    sf = sf_open ("pink.wav", SFM_WRITE, &sfinfo);

    float* buf = malloc(sizeof(float) * frames);

    for (int i = 0; i < frames; ++i) {
        z = pink_next(pink);
        buf[i] = (float)z / (float)0x7fffffff * 0.5f;
    }
    sf_write_float(sf, buf, frames);
    sf_close(sf);
    free(buf);
    pink_delete(pink);
    lcg_delete(lcg);
}

void test_poisson_signal(int frames) {
    struct lcg *lcg = lcg_new();
    float x, y;
    
    for (int i=0; i<frames; ++i) {
	
    }
}

int main() {
    test_lcg_speed();
    test_lcg_signal(48000 * 8);
    test_pink_signal(48000 * 8);
}
