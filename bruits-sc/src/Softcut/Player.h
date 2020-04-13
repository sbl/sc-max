#pragma once

#include <math.h>
#include <stdint.h>

namespace bruits {

// just a debugger thingy
struct Player {
    void setBuffer(float* b) {
        buf = b;
    }

    void process(float* output, uint64_t numSamples) {
        for (auto i = 0; i < numSamples; i++) {
            auto x = buf[static_cast<uint64_t>(m_phase)];
            output[i] = x;

            m_phase += 1;
            m_phase = fmod(m_phase, 44100);
        }
    }

private:
    float* buf;
    double m_phase = 0;
    static constexpr double increment = (1 / 44100);
};
}
