#include "SC_PlugIn.hpp"

#include "bruits/ring.h"

#include "plaits/dsp/voice.h"
#include "stmlib/dsp/sample_rate_converter.h"

static InterfaceTable* ft;

namespace bruits {

struct Zopf : SCUnit {
    Zopf() {
        memset(shared_buffer, 0, sizeof(shared_buffer));
        stmlib::BufferAllocator allocator(shared_buffer, sizeof(shared_buffer));
        voice.Init(&allocator);

        memset(&patch, 0, sizeof(patch));
        memset(&modulations, 0, sizeof(modulations));

        patch.engine = 0;
        patch.lpg_colour = 0.5f;
        patch.decay = 0.5f;

        if (isAudioRateIn(kTriggerIn)) {
            modulations.trigger_patched = true;
        }

        set_calc_function<Zopf, &Zopf::next>();
        next(1);
    }

    void next(int nSamples) {
        float* mainOut = out(0);
        float* auxOut = out(1);

        // params
        float pitch = in0(0);
        float harmonics = in0(1);
        float timbre = in0(2);
        float morph = in0(3);
        int engine = in0(4);

        const float* audioTrigger = in(kTriggerIn);
        float lpgColor = in0(6);
        float lpgDecay = in0(7);

        float fmAmount = in0(8);

        for (int i = 0; i < nSamples; ++i) {
            if (mainBuffer.empty()) {
                constexpr int blockSize = plaits::kBlockSize;

                // Model buttons
                patch.engine = engine;

                pitch += log2f(48000.f * sampleDur());
                patch.note = 60.f + pitch * 12.f;

                patch.harmonics = harmonics;

                patch.timbre = timbre;
                patch.morph = morph;
                patch.lpg_colour = lpgColor;
                patch.decay = lpgDecay;

                // influences frequency behaviour in trigger mode
                patch.frequency_modulation_amount = fmAmount;

                if (modulations.trigger_patched) {
                    modulations.trigger = audioTrigger[i];
                }

                // Render frames
                plaits::Voice::Frame output[blockSize];
                voice.Render(patch, modulations, output, blockSize);

                for (auto j = 0; j < blockSize; j++) {
                    mainBuffer.push(output[j].out / 32768.f);
                    auxBuffer.push(output[j].aux / 32768.f);
                }
            }

            // Set output
            if (!mainBuffer.empty()) {
                auto main = mainBuffer.shift();
                auto aux = auxBuffer.shift();

                mainOut[i] = -main;
                auxOut[i] = -aux;
            }
        }
    };

private:
    static constexpr int kTriggerIn = 5;

    DoubleRingBuffer<float, 256> mainBuffer;
    DoubleRingBuffer<float, 256> auxBuffer;

    plaits::Voice voice;
    plaits::Patch patch;
    plaits::Modulations modulations;
    char shared_buffer[16384];
};
}

PluginLoad(ZopfUGens) {
    ft = inTable;
    registerUnit<bruits::Zopf>(ft, "Zopf");
}
