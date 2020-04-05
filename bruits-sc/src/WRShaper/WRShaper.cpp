#include "SC_PlugIn.hpp"

#include "wrDsp/wrShaper.h"

namespace bruits {

struct WRShaper : public SCUnit {
    WRShaper() {
        set_calc_function<WRShaper, &WRShaper::next>();

        next(1);
    }

    void next(int nSamples) {
        auto inbuf = in(0);
        auto shape = in0(1);
        auto outbuf = out(0);

        // saad
        float* vbuf = const_cast<float*>(inbuf);;
        shaper_apply_v(shape, vbuf, outbuf, nSamples);
    }

};
}

static InterfaceTable* ft;

PluginLoad(WRShaperUGens) {
    ft = inTable;
    registerUnit<bruits::WRShaper>(ft, "WRShaper");
}
