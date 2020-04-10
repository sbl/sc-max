#include "SC_PlugIn.hpp"

namespace bruits {

struct First : public SCUnit {
    First() {
        set_calc_function<First, &First::next>();

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

PluginLoad(FirstUGens) {
    ft = inTable;
    registerUnit<bruits::First>(ft, typeid(bruits::First).name());
}
