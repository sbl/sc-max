#ifndef DSPKIT_SVF_H
#define DSPKIT_SVF_H

#include <array>

namespace dspkit {

    class Svf {
    public:
        // c-tor / d-tor
        Svf();

        // initialize internal lookup table for linear pitch control
        // @param sr: sample rate
        // @param baseFreq: base frequency (pitch == 0)
        // @param numOct: how many octaves to compute above base frequency
        void initPitchTable(double sr, double baseFreq = 14.1, double numOct = 10.f);

        // clear the internal filter state
        void clear();

        // process one input sample, returning one output sample
        float processSample(float x);

        // set the corner frequency as linear pitch
        // mapping is dependent on parameters passed to `initPitchTable()`
        // @param pitch: linear pitch in [0, 1]
        void setCutoffPitch(float pitch);

        void setSampleRate(float sr);
        void setCutoff(float fc);
        void setInverseQ(float rq);
        void setLpMix(float mix);
        void setHpMix(float mix);
        void setBpMix(float mix);
        void setBrMix(float mix);

    private:
        void update(float x);
        // recalculate all coefficients for current fc, sr, rq
        void calcCoeffs();
        // recalculate cheaper conefficients for RQ only (no `tan`)
        void calcSecondaryCoeffs();

    private:
        static constexpr size_t gTabSize = 1024;
        // lookup table for primary coefficient (pitch-wise)
        std::array<float, gTabSize> gTab;

        // sample rate
        float sr;
        // corner frequency in hz
        float fc;
        // reciprocal of Q in [0,1]
        float rq;
        // intermediate coefficients
        float g;
        float g1;
        float g2;
        float g3;
        float g4;
        // state variables
        float v0;
        float v1;
        float v2;
        float v0z;
        float v1z;
        float v2z;
        float v3;
        // outputs
        float lp; // lowpass
        float hp; // highpass
        float bp; // bandpass
        float br; // bandreject
        // mix
        float lpMix;
        float hpMix;
        float bpMix;
        float brMix;
    };
}

#endif
