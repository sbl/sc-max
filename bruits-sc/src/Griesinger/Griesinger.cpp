#include "SC_InlineBinaryOp.h"
#include "SC_PlugIn.hpp"

#include "reverb.h"

namespace bruits {

struct Griesinger : public SCUnit {
    Griesinger() {
        reverb.Init(reverb_buffer, static_cast<float>(sampleRate()));
        set_calc_function<Griesinger, &Griesinger::next>();

        next(1);
    }

    void next(int nSamples) {
        auto input = in(0);

        auto outL = out(0);
        auto outR = out(1);

        auto amount = sc_clip(in0(1), 0, 1);
        auto time = in0(2);
        auto lp = sc_clip(in0(3), 0, 1);
        auto diffusion = sc_clip(in0(4), 0, 1);

        reverb.set_amount(amount);
        reverb.set_time(time);
        reverb.set_input_gain(0.2);
        reverb.set_lp(lp);
        reverb.set_diffusion(diffusion);

        memcpy(outL, input, nSamples * sizeof(float));
        memcpy(outR, input, nSamples * sizeof(float));

        reverb.Process(outL, outR, nSamples);
    }

private:
    float reverb_buffer[65536];
    rings::Reverb reverb;
};
}

static InterfaceTable* ft;

PluginLoad(GriesingerUGens) {
    ft = inTable;
    registerUnit<bruits::Griesinger>(ft, "Griesinger");
}
