#include "softcut/SoftCutVoice.h"
#include "softcut/FadeCurves.h"
#include "SC_PlugIn.hpp"
#include "buffer.h"
#include <iostream>

static InterfaceTable* ft;

using namespace softcut;

namespace bruits {

struct SCSC : public SCUnit {
    SCSC() {
        set_calc_function<SCSC, &SCSC::next>();
        
        initCurves();
        softcut.reset();
        
        softcut.setSampleRate(sampleRate());
        softcut.setPhaseQuant(1);
        softcut.setPhaseOffset(0);
        
        next(1);
    }

    
    void initCurves() {
        /// FIXME: the fade curve data is static, shared among all instances
        // this is fine in the context of norns,
        /// but here each instance should probably own a copy
        /// in any case, this is the wrong place to set these magic numbers!
        FadeCurves::setPreShape(FadeCurves::Shape::Linear);
        FadeCurves::setRecShape(FadeCurves::Shape::Raised);
        FadeCurves::setMinPreWindowFrames(0);
        FadeCurves::setMinRecDelayFrames(0);
        FadeCurves::setPreWindowRatio(1.f/8);
        FadeCurves::setRecDelayRatio(1.f/(8*16));
    }
    
    
    void next(int nSamples) {
        float fbufnum = in0(0);
        if (fbufnum < 0.f) {
            fbufnum = 0.f;
        }
        if (fbufnum != m_fbufnum) {
            has_changes = true;

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
        auto position = in0(2);
        
        auto* output = out(0);

        if (!bufData) {
            std::cout << "no buffer data" << std::endl;
            for (auto i = 0; i < nSamples; i++) {
                output[i] = input[i];
            }
            return;
        }

        for (auto i = 0; i < nSamples; i++) {
            inBuf[i] = input[i];
        }

        // magic
        
        if (has_changes) {
            softcut.setBuffer(bufData, bufFrames);
            softcut.setPreLevel(1);
            softcut.setLoopFlag(true);
            softcut.setLoopStart(0);
            softcut.setLoopEnd(2);
            softcut.setPhaseOffset(0); // position
            softcut.setRate(1);
            softcut.setPlayFlag(true);

            has_changes = false;
        }

        softcut.processBlockMono(inBuf, outBuf, SOFTCUT_IO_BUF_FRAMES);

        // magic out

        for (auto i = 0; i < nSamples; i++) {
            output[i] = outBuf[i];
        }
    }

private:
    SoftCutVoice softcut;
    float m_fbufnum = -1e9f;
    SndBuf* m_buf;
    

    bool has_changes = false;

    static constexpr int SOFTCUT_IO_BUF_FRAMES = 1024;

    float inBuf[SOFTCUT_IO_BUF_FRAMES] = { 0 };
    float outBuf[SOFTCUT_IO_BUF_FRAMES] = { 0 };
};
}

PluginLoad(BruitsUGens) {
    ft = inTable;
    registerUnit<bruits::SCSC>(ft, "Softcut");
}
