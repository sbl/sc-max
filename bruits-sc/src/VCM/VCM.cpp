
#include "SC_PlugIn.hpp"

#include "Lockhart.h"

namespace bruits {

inline float tanh(float x) {
    return (-.67436811832e-5
            + (.2468149110712040
               + (.583691066395175e-1 + .3357335044280075e-1 * x) * x)
                * x)
        / (.2464845986383725
           + (.609347197060491e-1
              + (.1086202599228572 + .2874707922475963e-1 * x) * x)
               * x);
}

struct VCM : public SCUnit {
    VCM() {
        set_calc_function<VCM, &VCM::next>();

        next(1);
    }

    void next(int nSamples) {
        const float* input = in(0);
        float* outbuf = out(0);

        for (auto i = 0; i < nSamples; ++i) {
            double x = input[i] * 0.3;

            x = lh1(x);
            x = lh2(x);
            x = lh3(x);
            x = lh4(x);

            x = tanh(x) * 3;

            outbuf[i] = x;
        }
    }

private:
    Lockhart lh1, lh2, lh3, lh4;
};
}

static InterfaceTable* ft;

PluginLoad(VCMUGens) {
    ft = inTable;
    registerUnit<bruits::VCM>(ft, "VCM");
}
