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
        svc.reset();
        svc.setRecOffset(-8 / sampleRate());
        svc.cutToPos(0);

        next(1);
    }

    void next(int nSamples) {
        auto* unit = this; // for the GET_BUF macro
        auto* output = out(0);

        GET_BUF
        if (!m_buf->data || m_buf->frames <= 0) {
            for (auto i = 0; i < nSamples; i++) {
                output[i] = 0;
            }
            return;
        }
        svc.setBuffer(bufData, bufFrames);

        auto* input = in(1);
        auto rate = in0(2);
        auto trigger = in0(3);
        auto position = in0(4);
        auto play = in0(5) > 0.001f;
        auto loopStart = in0(6);
        auto loopEnd = in0(7);
        auto loop = in0(8) > 0.001f;
        auto preLevel = in0(9);
        auto recLevel = in0(10);
        auto rec = in0(11) > 0.001f;
        auto preFilterFc = in0(12);
        auto preFilterRq = in0(13);
        auto preFilterLp = in0(14);
        auto preFilterHp = in0(15);
        auto preFilterBp = in0(16);
        auto preFilterBr = in0(17);
        auto preFilterDry = in0(18);
        auto postFilterFc = in0(19);
        auto postFilterRq = in0(20);
        auto postFilterLp = in0(21);
        auto postFilterHp = in0(22);
        auto postFilterBp = in0(23);
        auto postFilterBr = in0(24);
        auto postFilterDry = in0(25);
        auto fadeTime = in0(26);
        auto recPreSlewTime = in0(27);
        auto rateSlewTime = in0(28);

        svc.setRate(rate);

        // require a trigger to cut to new positions
        if (trigger > 0.f && m_prevtrig <= 0.f) {
            svc.cutToPos(position);
        }
        m_prevtrig = trigger;

        // playback controls

        svc.setPlayFlag(play);
        svc.setLoopStart(loopStart);
        svc.setLoopEnd(loopEnd);
        svc.setLoopFlag(loop);
        svc.setPreLevel(preLevel);
        svc.setRecLevel(recLevel);
        svc.setRecFlag(rec);

        // filters

        svc.setPreFilterFc(preFilterFc);
        svc.setPreFilterRq(preFilterRq);
        svc.setPreFilterLp(preFilterLp);
        svc.setPreFilterHp(preFilterHp);
        svc.setPreFilterBp(preFilterBp);
        svc.setPreFilterBr(preFilterBr);
        svc.setPreFilterDry(preFilterDry);
        svc.setPostFilterFc(postFilterFc);
        svc.setPostFilterRq(postFilterRq);
        svc.setPostFilterLp(postFilterLp);
        svc.setPostFilterHp(postFilterHp);
        svc.setPostFilterBp(postFilterBp);
        svc.setPostFilterBr(postFilterBr);
        svc.setPostFilterDry(postFilterDry);

        // fades

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

    float m_prevtrig;
};
}

PluginLoad(SoftcutUGens) {
    ft = inTable;
    registerUnit<bruits::SCSC>(ft, "Softcut");
}
