#include "SC_PlugIn.hpp"
#include <iostream>

#include "wrDsp/wrFuncGen.h"
#include "wrDsp/wrVtl.h"

namespace bruits {

struct FriendZone : public SCUnit {
    FriendZone() {
        set_calc_function<FriendZone, &FriendZone::next>();
        // vtl_mode(vtl, vtl_mode_transient);
        // vtl_params(vtl, 100, 0.1);

        function_mode(func, fg_cycle);
        next(1);
    }

    ~FriendZone() {
        vtl_deinit(vtl);
        function_deinit(func);
    }

    void next(int nSamples) {
        auto rate = in0(0);
        auto sustain = in0(1);
        auto outbuf = out(0);

        function_rate(func, rate);
        function_mode(func, fg_cycle);

        // calc ramps
        float ctrl_rate = 0.5;
        float audio_rate = 0.5;
        function_ramp_v(func, ctrl_rate, &audio_rate, two_ramps, nSamples);

        // calc something
        float fm_return = 0;
        function_v(func, two_ramps, &fm_return, nSamples);

        // run audio rate
        float fm_in = 0;
        float fm_ix = 0;
        function_fmix_v(func, nSamples, two_ramps, two_ramps, &fm_in, &fm_ix,
                        outbuf);

        // vtl_params(vtl, speed, ratio);
        // vtl_step_v(vtl, outbuf, nSamples);
    }

private:
    float two_ramps[64 * 2] = { 0 };
    vtl_t* vtl = vtl_init();
    func_gen_t* func = function_init(1);
};
}

static InterfaceTable* ft;

PluginLoad(FriendZoneUGens) {
    ft = inTable;
    registerUnit<bruits::FriendZone>(ft, "FriendZone");
}
