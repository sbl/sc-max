#pragma once

#include "SC_PlugIn.hpp"

// clutch to re-use some SC functionality - this is rather ugly...
#define GET_CPP_BUFFER                                                             \
    float fbufnum = in0(0);                                                        \
    if (fbufnum < 0.f) {                                                           \
        fbufnum = 0.f;                                                             \
    }                                                                              \
    if (fbufnum != m_fbufnum) {                                                    \
        uint32 bufnum = (int)fbufnum;                                              \
        World* world = this->mWorld;                                               \
        if (bufnum >= world->mNumSndBufs) {                                        \
            int localBufNum = bufnum - world->mNumSndBufs;                         \
            Graph* parent = this->mParent;                                         \
            if (localBufNum <= parent->localBufNum) {                              \
                this->m_buf = parent->mLocalSndBufs + localBufNum;                 \
            } else {                                                               \
                bufnum = 0;                                                        \
                this->m_buf = world->mSndBufs + bufnum;                            \
            }                                                                      \
        } else {                                                                   \
            this->m_buf = world->mSndBufs + bufnum;                                \
        }                                                                          \
        this->m_fbufnum = fbufnum;                                                 \
    }                                                                              \
    SndBuf* buf = this->m_buf;                                                     \
    LOCK_SNDBUF(buf);                                                              \
    float* bufData __attribute__((__unused__)) = buf->data;                        \
    uint32 bufChannels __attribute__((__unused__)) = buf->channels;                \
    uint32 bufSamples __attribute__((__unused__)) = buf->samples;                  \
    uint32 bufFrames = buf->frames;                                                \
    int mask __attribute__((__unused__)) = buf->mask;                              \
    int guardFrame __attribute__((__unused__)) = bufFrames - 2;
