
#include <chrono>
#include <random>
#include <iostream>

#include <sndfile.hh>
#include "LadderLpf.hpp"

static constexpr double twopi = 6.2831853071796;

const int sr = 48000.f;
static constexpr int numFrames = sr * 20;

std::default_random_engine randEngine;
std::uniform_real_distribution<float> randDist(0, 1);

dspkit::LadderLpf<float> lpf;
float freqBuf[numFrames];

static void fillModBuffers() {
    float modInc = 4.0 / sr;
    float modPhase = 0;
    float mod = 0;

    for (int fr = 0; fr < numFrames; ++fr) {
        modPhase += modInc;
        while (modPhase > twopi) { modPhase -= twopi; }
        mod = sinf(modPhase) * 0.5f + 0.5f;
        freqBuf[fr] = 14.1 * pow(2, mod * 10.0);
    }
}

// signal buffer
float sndBuf[numFrames];

static void fillSignalBufferNoise() {
    for (int fr = 0; fr < numFrames; ++fr) {
        sndBuf[fr] = randDist(randEngine) - 0.5;
    }
}

void processFreqMod() {
    for (int fr = 0; fr < numFrames; ++fr) {
        lpf.setFc(freqBuf[fr]);
        sndBuf[fr] = lpf.processSample(sndBuf[fr]);
    }
}

int main() {
    const int format = SF_FORMAT_WAV | SF_FORMAT_PCM_24;
    SndfileHandle sndfile("test-lpf.wav", SFM_WRITE, format, 1, sr);

    lpf.init(sr);

    fillModBuffers();
    fillSignalBufferNoise();

    auto start = std::chrono::high_resolution_clock::now();
    processFreqMod();
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    std::cout << "freq modulation: processed " << numFrames << " samples"
              << " in " << microseconds << " usecs" << std::endl;

    // write output soundfile
    for (int fr = 0; fr < numFrames; ++fr) {
        sndfile.writef(sndBuf + fr, 1);
    }
}
