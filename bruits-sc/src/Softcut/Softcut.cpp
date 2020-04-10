#include "softcut/Softcut.h"
#include "SC_PlugIn.hpp"
#include "buffer.h"
#include <iostream>

static InterfaceTable* ft;

namespace bruits {

struct SCSC : public SCUnit {
    SCSC() {
        set_calc_function<SCSC, &SCSC::next>();
        soft.setSampleRate(sampleRate());
        soft.reset();

        next(1);
    }

    void next(int nSamples) {
        GET_CPP_BUFFER
        auto* input = in(1);
        auto* output = out(0);

        float fakeBuf[nSamples];
        
        
        if (!bufData) {
            std::cout << "no buffer data" << std::endl;
            for (auto i = 0; i < nSamples; i++) {
                output[i] = input[i];
            }
            return;
        }

        soft.setVoiceBuffer(0, bufData, bufFrames);
        soft.setPlayFlag(0, true);
        soft.setLoopFlag(0, true);
        soft.setRecFlag(0, false);
        
        soft.setPhaseQuant(0, 0);
        soft.setPhaseOffset(0, 0);
        soft.setRecOffset(0, 0.0001);
        
        soft.setPreLevel(0, 0.2);
        soft.setRecLevel(0, 1);
        
        soft.setRate(0, 1);
        soft.setLoopStart(0, 0);
        soft.setLoopEnd(0, 1);
        
        soft.setFadeTime(0, 0.001);

        soft.processBlock(0, input, fakeBuf, nSamples);
        
        for(auto i = 0; i < nSamples; i++) {
            output[i] = fakeBuf[i];
        }
        
    }

private:
    softcut::Softcut<1> soft;
    float m_fbufnum = -1e9f;
    SndBuf* m_buf;
};
}

PluginLoad(BruitsUGens) {
    ft = inTable;
    registerUnit<bruits::SCSC>(ft, "Softcut");
}
