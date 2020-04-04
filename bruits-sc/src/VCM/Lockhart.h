#pragma once

#include <cmath>

#include "utl/LambertW.h"

namespace bruits {

inline float sign(float v) {
    return v > 0 ? 1 : v < 0 ? -1 : 0;
}

/**
 * See "Virtual Analog Models of the Lockhart and Serge Wavefolders
 *
 * Esqueda, F, Pöntynen, H, Parker, JD & Bilbao, S 2017, 'Virtual analog models of
 * the Lockhart and Serge wavefolders', Applied Sciences (Switzerland), vol. 7, no.
 * 12, 1328. https://doi.org/10.3390/app7121328
 */
struct Lockhart {
    inline double operator()(double input) {
        double out = 0;

        double xn;
        // Compute Antiderivative
        double lambda = sign(input);
        double u = kDelta * exp(lambda * kBeta * input);
        double ln = utl::LambertW<0>(u);
        double fn = (0.5 * kVT / kBeta) * (ln * (ln + 2)) - 0.5 * input * input;

        // Check for ill-conditioning
        if (abs(input - pxn1) < kErrorThreshold) {
            // Compute Averaged Wavefolder Output
            xn = 0.5 * (input + pxn1);
            u = kDelta * exp(lambda * kBeta * xn);
            ln = utl::LambertW<0>(u);
            out = lambda * kVT * ln - xn;

        } else {
            // Apply AA Form
            out = (fn - pfn1) / (input - pxn1);
        }

        // Update States
        pfn1 = fn;
        pxn1 = input;

        return out;
    }

private:
    static constexpr double kRL = 7.5e3;
    static constexpr double kR = 15e3;
    static constexpr double kIS = 10e-16;
    static constexpr double kVT = 26e-3;
    static constexpr double kErrorThreshold = 10e-10;

    static constexpr double kBeta = (kR + 2 * kRL) / (kVT * kR);
    static constexpr double kDelta = (kRL * kIS) / kVT;

    double pfn1;
    double pxn1;
};

}
