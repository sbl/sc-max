#include "SC_PlugIn.hpp"

#include "plaits/dsp/engine/engine.h"
#include "plaits/dsp/oscillator/variable_saw_oscillator.h"

namespace bruits {

struct Warsaw : public SCUnit {
    Warsaw() {
        saw.Init();
        isr = sampleDur();

        set_calc_function<Warsaw, &Warsaw::next>();

        next(1);
    }

    void next(int nSamples) {
        auto outbuf = out(0);

        auto freq = in0(0) * isr;
        auto pw = in0(1);
        auto shape = in0(2);

        saw.Render(freq, pw, shape, outbuf, nSamples);
    }

private:
    plaits::VariableSawOscillator saw;
    double isr;
};
}

static InterfaceTable* ft;

PluginLoad(WarsawUGens) {
    ft = inTable;
    registerUnit<bruits::Warsaw>(ft, "Warsaw");
}
