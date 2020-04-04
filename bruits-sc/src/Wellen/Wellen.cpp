// SC port of Tides 2 by Mutable Instruments
// SC Port by Stephen Lumenta (stephen.lumenta@gmail.com)
//
// Copyright Tides 2 2017 Emilie Gillet.
// Author: Emilie Gillet (emilie.o.gillet@gmail.com)

#include "SC_PlugIn.hpp"

#include "stmlib/dsp/hysteresis_quantizer.h"
#include "stmlib/dsp/units.h"
#include "tides2/poly_slope_generator.h"
#include "tides2/ramp_extractor.h"
#include "tides2/io_buffer.h"

namespace bruits {

struct Wellen : public SCUnit {
    Wellen() {
        polySlopeGenerator.Init();

        isAudioTrigger = isAudioRateIn(4);
        set_calc_function<Wellen, &Wellen::next>();
        next(1);
    }

    void next(int nSamples) {
        float* out0 = out(0);
        float* out1 = out(1);
        float* out2 = out(2);
        float* out3 = out(3);

        float frequency = in0(0);
        float shape = in0(1);
        float slope = in0(2);
        float smoothness = in0(3);
        float shift = in0(4);

        const float* aTriggerIn = in(5);
        float kTriggerIn = in0(5);

        rampMode = (tides::RampMode)((int)in0(6) % 3);
        outputMode = (tides::OutputMode)((int)in0(7) % 4);

        frequency =
            kFrequencyScaled / sampleRate() * stmlib::SemitonesToRatio(frequency);

        for (int i = 0; i < nSamples; i++) {
            bool current = false;
            if (isAudioTrigger) {
                current = aTriggerIn[i] >= kTriggerThreshold;
            } else {
                current = kTriggerIn >= kTriggerThreshold;
            }
            trigFlags[frame] = stmlib::ExtractGateFlags(previousTrigFlag, current);
            previousTrigFlag = trigFlags[frame];

            if (++frame >= tides::kBlockSize) {
                frame = 0;
                polySlopeGenerator.Render(rampMode, outputMode, tides::RANGE_AUDIO,
                                          frequency, slope, shape, smoothness,
                                          shift, trigFlags, nullptr, polyOut,
                                          tides::kBlockSize);
            }

            out0[i] = polyOut[frame].channel[0] * 0.2;
            out1[i] = polyOut[frame].channel[1] * 0.2;
            out2[i] = polyOut[frame].channel[2] * 0.2;
            out3[i] = polyOut[frame].channel[3] * 0.2;
        }
    }

private:
    static constexpr float kTriggerThreshold = 0.1f;
    static constexpr float kFrequencyScaled = 8.1757989156437f;

    bool isAudioTrigger;

    uint8_t frame = 0;

    tides::PolySlopeGenerator polySlopeGenerator;

    // State
    tides::RampMode rampMode = tides::RAMP_MODE_LOOPING;
    tides::OutputMode outputMode = tides::OUTPUT_MODE_GATES;

    // Buffers
    tides::PolySlopeGenerator::OutputSample polyOut[tides::kBlockSize] = {};
    stmlib::GateFlags trigFlags[tides::kBlockSize] = {};
    stmlib::GateFlags previousTrigFlag = stmlib::GATE_FLAG_LOW;
};

}

static InterfaceTable* ft;
PluginLoad(WellenGens) {
    ft = inTable;
    registerUnit<bruits::Wellen>(ft, "Wellen");
}
