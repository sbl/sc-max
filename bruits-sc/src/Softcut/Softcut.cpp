#include "SC_PlugIn.hpp"
#include "softcut/Voice.h"
#include <iostream>

static InterfaceTable* ft;

using namespace softcut;

namespace bruits {

struct SCSC : public SCUnit {
    SCSC() {
        set_calc_function<SCSC, &SCSC::next>();

        svc.setSampleRate(sampleRate());
        svc.cutToPos(0);
        
        svc.reset();

        next(1);
    }

    void next(int nSamples) {
        auto* unit = this;
        auto* output = out(0);
        
        GET_BUF
        if (!m_buf->data || m_buf->frames <= 0) {
            for (auto i = 0; i < nSamples; i++) {
                output[i] = 0;
            }
            return ;
        }
        
        svc.setBuffer(bufData, bufFrames);
        
        // parameters

        auto* input = in(1);
        auto rate = in0(2);
        auto position = in0(3);
        auto play = in0(4) > 0.001f;
        auto loopStart = in0(5);
        auto loopEnd = in0(6);
        auto loop = in0(7) > 0.001f;
        auto preLevel = in0(8);
        auto recLevel = in0(9);
        auto recOffset = in0(10);
        auto rec = in0(11) > 0.001f;
        auto fadeTime = in0(12);
        auto recPreSlewTime = in0(13);
        auto rateSlewTime = in0(14);

        svc.setRate(rate);
        svc.cutToPos(position);
        svc.setPlayFlag(play);
        svc.setLoopStart(loopStart);
        svc.setLoopEnd(loopEnd);
        svc.setLoopFlag(loop);
        svc.setPreLevel(preLevel);
        svc.setRecLevel(recLevel);
        svc.setRecOffset(recOffset);
        svc.setRecFlag(rec);
        svc.setFadeTime(fadeTime);
        svc.setRecPreSlewTime(recPreSlewTime);
        svc.setRateSlewTime(rateSlewTime);

        // read + write
        svc.processBlockMono(input, output, nSamples);
    }

private:
    softcut::Voice svc;
    float m_fbufnum = -1e9f;
    SndBuf* m_buf = nullptr;
    float m_position = 0;
};
}

PluginLoad(SoftcutUGens) {
    ft = inTable;
    registerUnit<bruits::SCSC>(ft, "Softcut");
}
