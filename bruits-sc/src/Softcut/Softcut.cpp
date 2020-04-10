#include "SC_PlugIn.hpp"

namespace bruits {

struct Softcut : public SCUnit {
    Softcut() {
        set_calc_function<Softcut, &Softcut::next>();

        next(1);
    }

    void next(int nSamples) {
        const float* input = in(0);
        float* outbuf = out(0);

        // simple gain function
        for (int i = 0; i < nSamples; ++i) {
            outbuf[i] = input[i];
        }
    }

private:
};
}

static InterfaceTable* ft;

PluginLoad(BruitsUGens) {
    ft = inTable;
    registerUnit<bruits::Softcut>(ft, "Softcut");
}
