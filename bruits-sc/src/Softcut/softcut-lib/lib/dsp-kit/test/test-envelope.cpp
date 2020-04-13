//
// Created by emb on 2/3/20.
//

#include <iostream>
#include <vector>

#include "lib/cnpy/cnpy.h"

#include "Envelope.hpp"

constexpr float samplerate = 48000.f;
constexpr size_t numFrames = samplerate * 4.1;
//float output[numFrames];

std::vector<float> output;

int main() {
    Envelope env;
    env.setSampleRate(samplerate);
    env.setValue(0);
    env.setRiseShape(easing::function::in_exponential);
    env.setFallShape(easing::function::out_exponential);
    env.addStage(1.0, 0.25);
    env.addStage(0.0, 0.25);
    env.addStage(0.5, 0.25, easing::function::in_out_sine);
    // env.setSequenceMode(Envelope::SequenceMode::queue);
    env.setSequenceMode(Envelope::SequenceMode::loop);

    for (int i=0; i<numFrames; ++i) {
        float y= env.processSample();
        //std::cout << y << std::endl;
        output.push_back(y);
    }

    cnpy::npy_save("../workbench/test-env.npy",&output[0],{1, 1, numFrames},"w");
}