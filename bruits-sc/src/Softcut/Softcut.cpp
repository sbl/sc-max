#include "softcut/Voice.h"
#include "SC_PlugIn.hpp"
#include <iostream>

static InterfaceTable* ft;

using namespace softcut;

namespace bruits {

struct SCSC : public SCUnit {
    SCSC() {
        set_calc_function<SCSC, &SCSC::next>();

        softcut.reset();
        softcut.setSampleRate(sampleRate());



        softcut.setPhaseQuant(1);
        softcut.setPhaseOffset(0);

        // init

        softcut.setPreFilterEnabled(false);
        softcut.setPreLevel(1);
        softcut.setLoopFlag(true);
        softcut.setLoopStart(0);
        softcut.setLoopEnd(1.5);
        softcut.setPhaseOffset(0); // position
        softcut.setRate(1);
        softcut.setPlayFlag(true);

        next(1);
    }

    void next(int nSamples) {
        
        // --------------------------------------------------
#pragma mark - buffer loading
        
        float fbufnum = in0(0);
        if (fbufnum < 0.f) {
            fbufnum = 0.f;
        }
        if (fbufnum != m_fbufnum) {
            uint32 bufnum = (int)fbufnum;
            World* world = this->mWorld;
            if (bufnum >= world->mNumSndBufs) {
                int localBufNum = bufnum - world->mNumSndBufs;
                Graph* parent = this->mParent;
                if (localBufNum <= parent->localBufNum) {
                    this->m_buf = parent->mLocalSndBufs + localBufNum;
                } else {
                    bufnum = 0;
                    this->m_buf = world->mSndBufs + bufnum;
                }
            } else {
                this->m_buf = world->mSndBufs + bufnum;
            }
            this->m_fbufnum = fbufnum;
            LOCK_SNDBUF(m_buf);
        }

        // --------------------------------------------------

        softcut.setBuffer(m_buf->data, m_buf->frames);
        
        
        auto* input = in(1);
        auto* output = out(0);

        if (!m_buf->data || m_buf->frames <= 0) {
            std::cout << "no buffer data" << std::endl;
            for (auto i = 0; i < nSamples; i++) {
                output[i] = 0;
            }
            return;
        }

        // read + write
        softcut.performReads(output, nSamples);
    }

private:
    softcut::Voice softcut;
    float m_fbufnum = -1e9f;
    SndBuf* m_buf;
};
}

PluginLoad(SoftcutUGens) {
    ft = inTable;
    registerUnit<bruits::SCSC>(ft, "Softcut");
}
