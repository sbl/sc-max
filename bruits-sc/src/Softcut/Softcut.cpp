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

        next(1);
    }

    void next(int nSamples) {
        GET_CPP_BUFFER
        if (!bufData) {
            std::cout << "no buffer data" << std::endl;
            ft->fClearUnitOutputs(this, nSamples);
            return;
        }

        soft.setVoiceBuffer(0, bufData, bufFrames);

        const float* input = in(1);
        float* outbuf = out(0);

        // simple gain function
        for (int i = 0; i < nSamples; ++i) {
            outbuf[i] = input[i];
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
