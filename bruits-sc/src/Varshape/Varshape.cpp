#include "SC_PlugIn.hpp"

#include "plaits/dsp/engine/engine.h"
#include "plaits/dsp/oscillator/oscillator.h"
#include "plaits/dsp/oscillator/variable_shape_oscillator.h"

namespace bruits {

struct Varshape : public SCUnit {
    Varshape() {
        osc.Init();
        isr = sampleDur();

        set_calc_function<Varshape, &Varshape::next>();

        next(1);
    }

    void next(int nSamples) {
        auto outbuf = out(0);

        auto freq = in0(0) * isr;
        auto pw = in0(1);
        auto shape = in0(2);

        osc.Render<false>(0, freq, pw, shape, outbuf, nSamples);
    }

private:
    plaits::VariableShapeOscillator osc;
    double isr;
};
}

static InterfaceTable* ft;

PluginLoad(VarshapeUGens) {
    ft = inTable;
    registerUnit<bruits::Varshape>(ft, "Varshape");
}
