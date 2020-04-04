#pragma once

#include "DSPMath.hpp"

namespace lrt {

struct MS20TPT {
    float s = 0;

    inline void process(float in1, float in2) {
        float gx = in1 * in2;

        s = gx + z + gx;
        z = s;
    }

private:
    float z;
};

struct MS20ZDF {
    float y = 0;
    float s = 0;
    MS20TPT tpt;

    inline void process(float in1, float in2) {
        y = in1 * in2 + s;

        tpt.process(in1 - y, in2);
        s = tpt.s;
    }
};

struct MS20zdf {
    explicit MS20zdf(float sr) {
        this->sr = sr;
        rs = new Resampler<1>(OVERSAMPLE, 8);
    }

    void updateSampleRate(float sr) {
        this->sr = sr;
        invalidate();
    }

    void setFrequency(float value) {
        if (freq == value)
            return;

        freq = clampf(value, 0.f, sr / 2);
        invalidate();
    }

    void setDrive(float value) {
        drive = clampf(value, 0.f, 1.1f);
    }

    void setPeak(float value) {
        if (peak == value) {
            return;
        }
        peak = value;
        invalidate();
    }

    float process(float input) {
        rs->doUpsample(0, input);

        float s1, s2;
        float gain = pow2bpol(drive) * DRIVE_GAIN + 1.f;
        float x = 0;

        for (int i = 0; i < rs->getFactor(); i++) {
            x = rs->getUpsampled(0)[i];

            zdf1.process(x - ky, g);
            s1 = zdf1.s;

            zdf2.process(zdf1.y + ky, g);
            s2 = zdf2.s;

            y = 1.f / (g2 * k - g * k + 1.f) * (g2 * x + g * s1 + s2);

            ky = k * atanf(y / 50.f) * 50.f;

            rs->data[0][i] = atanShaper(gain * y / 6.f) * 6.f;
        }

        return rs->getDownsampled(0);
    }

private:
    static constexpr int OVERSAMPLE = 4; // factor of internal oversampling
    static constexpr float DRIVE_GAIN = 20.f; // max drive gain

    float freq = 440, peak = 0, drive = 0;

    float g = 0, g2 = 0, b = 0, k = 0;
    float ky = 0, y = 0;
    float sr = 44100;

    MS20ZDF zdf1, zdf2;
    Resampler<1>* rs;

    void invalidate() {
        b = tanf(freq * (float)M_PI / sr / OVERSAMPLE);
        g = b / (1 + b);

        /* use shifted negative cubic shape for logarithmic like shaping of the peak
         * parameter */
        k = 2.f * cubicShape(peak) * 1.0001f;
        g2 = g * g;
    }
};

}
