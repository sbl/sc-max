
#include <chrono>
#include <random>
#include <iostream>

#include <sndfile.hh>

#include "Svf.hpp"

static constexpr double twopi = 6.2831853071796;

const int sr = 48000.f;
static constexpr int numFrames = sr * 20;

std::default_random_engine randEngine;
std::uniform_real_distribution<float> randDist(0, 1);

dspkit::Svf svf;

// store modulated frequency/pitch in own buffer for good profiling comparison
float freqBuf[numFrames];
float pitchBuf[numFrames];

static void fillModBuffers() {
    float modInc = 4.0 / sr;
    float modPhase = 0;
    float mod = 0;


    for (int fr = 0; fr < numFrames; ++fr) {
        modPhase += modInc;
        while (modPhase > twopi) { modPhase -= twopi; }
        mod = sinf(modPhase) * 0.5f + 0.5f;
        pitchBuf[fr] = mod;
        // these magic numbers are from Svf pitch table setup,
        // with default values for base freq and octave count
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

void processPitchMod() {
    for (int fr = 0; fr < numFrames; ++fr) {
        svf.setFcPitch(pitchBuf[fr]);
        sndBuf[fr] = svf.processSample(sndBuf[fr]);
    }
}

void processFreqMod() {
    for (int fr = 0; fr < numFrames; ++fr) {
        svf.setFc(freqBuf[fr]);
        sndBuf[fr] = svf.processSample(sndBuf[fr]);
    }
}

int main() {
    const int format = SF_FORMAT_WAV | SF_FORMAT_PCM_24;
    SndfileHandle sndfile("test-svf.wav", SFM_WRITE, format, 1, sr);

    svf.initPitchTable(sr);

    svf.setSr(sr);
    svf.setFc(1000);
    svf.setRq(0.5);

    svf.setLpMix(1.f);
    svf.setHpMix(0.f);
    svf.setBpMix(0.f);
    svf.setBrMix(0.f);

    fillModBuffers();
    fillSignalBufferNoise();

    // test execution speed with pitch control (using primary coefficient LUT)
    auto start = std::chrono::high_resolution_clock::now();
    processPitchMod();
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    std::cout << "pitch modulation: processed " << numFrames << " samples"
              << " in " << microseconds << " usecs" << std::endl;


    start = std::chrono::high_resolution_clock::now();
    processFreqMod();
    elapsed = std::chrono::high_resolution_clock::now() - start;
    microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    std::cout << "freq modulation: processed " << numFrames << " samples"
              << " in " << microseconds << " usecs" << std::endl;

    // write output soundfile
    for (int fr = 0; fr < numFrames; ++fr) {
        sndfile.writef(sndBuf + fr, 1);
    }
}
