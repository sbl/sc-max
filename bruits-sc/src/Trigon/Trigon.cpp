// SC port of Tides 2 by Mutable Instruments
// SC Port by Stephen Lumenta (stephen.lumenta@gmail.com)
//
// Copyright Tides 2 2017 Emilie Gillet.
// Author: Emilie Gillet (emilie.o.gillet@gmail.com)

#include "SC_PlugIn.hpp"

#include "stmlib/dsp/hysteresis_quantizer.h"
#include "stmlib/dsp/units.h"
#include "tides2/io_buffer.h"
#include "tides2/ramp_extractor.h"
#include "tides2/ramp_generator.h"

namespace bruits {

struct Trigon : public SCUnit {
    Trigon() {
        ramp_generator.Init();

        set_calc_function<Trigon, &Trigon::next>();
        next(1);
    }

    void next(int nSamples) {
        auto* output = out(0);

        auto frequency = in0(0);
        // auto shape = in0(1);
        // auto slope = in0(2);
        // auto smoothness = in0(3);

        ramp_generator.frequency(frequency);

        frequency =
            kFrequencyScaled / sampleRate() * stmlib::SemitonesToRatio(frequency);

        for (int i = 0; i < nSamples; i++) {
            // ramp_generator.Step(0);
            output[i] = 0;
        }
    }

private:
    tides::RampGenerator<1> ramp_generator;

    static constexpr float kTriggerThreshold = 0.1F;
    static constexpr float kFrequencyScaled = 8.1757989156437F;
};
}

static InterfaceTable* ft;
PluginLoad(TrigonGens) {
    ft = inTable;
    registerUnit<bruits::Trigon>(ft, "Trigon");
}
