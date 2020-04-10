#include "SC_PlugIn.hpp"
#include "buffer.h"
#include "softcut/SoftCutVoice.h"
#include <iostream>

static InterfaceTable* ft;

namespace bruits {

struct SCSC : public SCUnit {
    SCSC() {
        set_calc_function<SCSC, &SCSC::next>();
        scv.setSampleRate(sampleRate());
        scv.reset();

        next(1);
    }

    void next(int nSamples) {
        GET_CPP_BUFFER

        auto* input = in(1);
        auto* output = out(0);

        if (!bufData) {
            std::cout << "no buffer data" << std::endl;
            for (auto i = 0; i < nSamples; i++) {
                output[i] = input[i];
            }
            return;
        }

        scv.setBuffer(bufData, bufFrames);

        scv.setRate(1);
        scv.setLoopStart(0);
        scv.setLoopEnd(2);
        scv.setLoopFlag(true);

        scv.setFadeTime(0.001);

        scv.setRecLevel(1);
        scv.setPreLevel(0.2);
        scv.setRecFlag(true);
        scv.setPlayFlag(true);

        scv.processBlockMono(input, output, nSamples);
    }

private:
    softcut::SoftCutVoice scv;
    float m_fbufnum = -1e9f;
    SndBuf* m_buf;
};
}

PluginLoad(BruitsUGens) {
    ft = inTable;
    registerUnit<bruits::SCSC>(ft, "Softcut");
}
