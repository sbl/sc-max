//
// Created by ezra on 4/21/18.
//

/*
 * this class implements one half of a crossfaded read/write sch.
 */

#ifndef Softcut_SUBHEAD_H
#define Softcut_SUBHEAD_H

#include "Resampler.h"
#include "SoftClip.h"
#include "Types.h"
#include "FadeCurves.h"

namespace softcut {

    typedef enum { Playing=0, Stopped=1, FadeIn=2, FadeOut=3 } State;
    typedef enum { None, Stop, LoopPos, LoopNeg } Action ;

    class SubHead {
        friend class ReadWriteHead;

    public:
        void init(FadeCurves *fc);
        void setSampleRate(float sr);

    private:
        sample_t peek4();
        unsigned int wrapBufIndex(int x);

    protected:
        static constexpr int blockSize = 2048;
        sample_t peek();
        //! poke
        //! @param in: input value
        //! @param pre: scaling level for previous buffer content
        //! @param rec: scaling level for new content
        void poke(sample_t in, float pre, float rec);
        Action updatePhase(phase_t start, phase_t end, bool loop);
        void updateFade(float inc);

        // getters
        phase_t phase() { return phase_; }
        float fade() { return fade_; }
        float trig() { return trig_; }
        State state() { return state_; }
        
        // setters
        void setState(State state);
        void setPhase(phase_t phase);

        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // **NB** buffer size must be a power of two!!!!
        void setBuffer(sample_t *buf, unsigned int frames);
        void setRate(rate_t rate);
        FadeCurves *fadeCurves;

    private:
        Resampler resamp_;
        SoftClip clip_;

        sample_t* buf_; // output buffer
        unsigned int wrIdx_ = 0; // write index
        unsigned int bufFrames_ = 0;
        unsigned int bufMask_ = 0;

        State state_ = Stopped;
        rate_t rate_ = 1;
        int inc_dir_ = 0;
        phase_t phase_ = 0;
        float fade_ = 0;
        float trig_ = 0; // output trigger value
        bool active_ = false;
        int recOffset_ = 0;

        float preFade_ = 0;
        float recFade_ = 0;

        void setRecOffsetSamples(int d);
    };

}


#endif //Softcut_SUBHEAD_H
