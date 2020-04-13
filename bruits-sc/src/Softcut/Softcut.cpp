#include "softcut/Softcut.h"
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
        softcut.setVoiceEnabled(0, true);

        auto* v = softcut.voice(0);

        v->setPhaseQuant(1);
        v->setPhaseOffset(0);

        // init

        v->setPreLevel(1);
        v->setLoopFlag(true);
        v->setLoopStart(1);
        v->setLoopEnd(1.5);
        v->setPhaseOffset(0); // position
        v->setRate(1);
        v->setPlayFlag(true);

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
            
            softcut.voice(0)->setPhaseQuant(0);
            softcut.voice(0)->setBuffer(m_buf->data, m_buf->frames);
        }

        // --------------------------------------------------

        auto* input = in(1);
        auto* output = out(0);

        if (!m_buf->data || m_buf->frames <= 0) {
            std::cout << "no buffer data" << std::endl;
            for (auto i = 0; i < nSamples; i++) {
                output[i] = 0;
            }
            return;
        }

        softcut.setInputBus(0, const_cast<float*>(input));
        softcut.setOutputBus(0, output);
        softcut.processBlock(nSamples);
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
