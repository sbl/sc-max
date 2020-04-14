#include "softcut/Softcut.h"
#include "SC_PlugIn.hpp"
#include <iostream>

static InterfaceTable* ft;

using namespace softcut;

namespace bruits {

struct SCSC : public SCUnit {
    SCSC() {
        set_calc_function<SCSC, &SCSC::next>();

        softcut.setSampleRate(sampleRate());
        softcut.reset();

        // init

        softcut.setPreLevel(0, 1);
        softcut.setLoopFlag(0, true);
        softcut.setLoopStart(0, 0);
        softcut.setLoopEnd(0, 1.5);
        
        softcut.setPhaseQuant(0, 1.f);
        softcut.setPhaseOffset(0, 0.f);
        softcut.cutToPos(0, 0);
        
        softcut.setRate(0, 1);
        softcut.setPlayFlag(0, true);
                
        next(1);
    }

    void next(int nSamples) {
        auto fbufnum = in0(0);
        auto* output = out(0);
        
        // --------------------------------------------------
        
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

        if (!m_buf->data || m_buf->frames <= 0) {
            std::cout << "no buffer data" << std::endl;
            for (auto i = 0; i < nSamples; i++) {
                output[i] = 0;
            }
            return;
        }
        
        // --------------------------------------------------

        softcut.setVoiceBuffer(0, m_buf->data, m_buf->frames);
        
        auto* input = in(1);
        auto rate = in0(2);
        
        // read + write
        
        softcut.setRate(0, rate);
        
        softcut.processBlock(0, input, output, nSamples);
    }

private:
    softcut::Softcut<1> softcut;
    float m_fbufnum = -1e9f;
    SndBuf* m_buf;
};
}

PluginLoad(SoftcutUGens) {
    ft = inTable;
    registerUnit<bruits::SCSC>(ft, "Softcut");
}
