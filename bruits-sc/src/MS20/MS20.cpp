#include "SC_PlugIn.hpp"

#include "MS20zdf.hpp"

namespace bruits {
struct MS20 : public SCUnit {
    MS20() {
        audioRate = isAudioRateIn(1);

        set_calc_function<MS20, &MS20::next>();
        next(1);
    }

    void next(int nSamples) {
        auto input = in(0);

        auto freqBuffer = in(1);
        auto freq = in0(1);

        auto peak = in0(2);
        auto drive = in0(3);

        float* outbuf = out(0);

        ms20zdf.setPeak(peak);
        ms20zdf.setDrive(drive);

        for (int i = 0; i < nSamples; ++i) {
            if (audioRate) {
                freq = freqBuffer[i];
            }
            ms20zdf.setFrequency(freq);
            outbuf[i] = ms20zdf.process(input[i]);
        }
    }

private:
    lrt::MS20zdf ms20zdf { static_cast<float>(sampleRate()) };
    bool audioRate = false;
};
}

static InterfaceTable* ft;

PluginLoad(MS20UGens) {
    ft = inTable;
    registerUnit<bruits::MS20>(ft, "MS20");
}
