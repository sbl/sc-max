#include "SC_PlugIn.hpp"
#include "buffer.h"
#include "softcut/Softcut.h"
#include <iostream>

static InterfaceTable* ft;

using namespace softcut;

namespace bruits {

struct SCSC : public SCUnit {
    SCSC() {
        set_calc_function<SCSC, &SCSC::next>();

        softcut.reset();
        softcut.setSampleRate(sampleRate());

        softcut.setPhaseQuant(0, 1);
        softcut.setPhaseOffset(0, 0);

        // init
        
        softcut.setPreLevel(0, 1);
        softcut.setLoopFlag(0, true);
        softcut.setLoopStart(0, 0 );
        softcut.setLoopEnd(0, 1);
        softcut.setPhaseOffset(0, 0); // position
        softcut.setRate(0, 1);
        softcut.setPlayFlag(0, true);
                
        // end init
        
        next(1);
    }

    void next(int nSamples) {
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
            
            
            softcut.setPhaseQuant(0, 0);
        }
        SndBuf* buf = this->m_buf;
        LOCK_SNDBUF(buf);
        float* bufData __attribute__((__unused__)) = buf->data;
        uint32 bufChannels __attribute__((__unused__)) = buf->channels;
        uint32 bufSamples __attribute__((__unused__)) = buf->samples;
        uint32 bufFrames = buf->frames;
        int mask __attribute__((__unused__)) = buf->mask;
        int guardFrame __attribute__((__unused__)) = bufFrames - 2;

        // --------------------------------------------------

        auto* input = in(1);
        
        auto* output = out(0);

        if (!bufData) {
            std::cout << "no buffer data" << std::endl;
            for (auto i = 0; i < nSamples; i++) {
                output[i] = input[i];
            }
            return;
        }


        // magic
        softcut.setVoiceBuffer(0, bufData, bufFrames);
        softcut.processBlock(0, input, output, nSamples);
    }

private:
    softcut::Softcut<1> softcut;
    float m_fbufnum = -1e9f;
    SndBuf* m_buf;
};
}

PluginLoad(BruitsUGens) {
    ft = inTable;
    registerUnit<bruits::SCSC>(ft, "Softcut");
}
