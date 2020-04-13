#include <cmath>
//#include <iostream>

#include "Lut.hpp"
#include "Svf.hpp"

using dspkit::Svf;
using dspkit::Lut;

Svf::Svf() {
    clear();
}

void Svf::initPitchTable(double sr, double baseFreq, double numOct) {
    this->sr = static_cast<float>(sr);
    auto sz_1 = gTabSize -1;
    // base-2 logarithmic pitch increment per entry
    double inc = numOct / static_cast<double>(sz_1);
    double exp = 0.0;
    double f;
    for (int i = 0; i < sz_1; ++i) {
        f = baseFreq * pow(2.0, exp);
        // std::cout << f << std::endl;
        exp += inc;
        gTab[i] = static_cast<float>(tan(M_PI * f / sr));
    }
    // extra element for "extended" lookup
    gTab[sz_1] = gTab[sz_1 - 1];
}

void Svf::setCutoffPitch(float pitch) {
    this->g = Lut<float>::lookupLinear(pitch, gTab.data(), gTabSize);
    calcSecondaryCoeffs();
}

void Svf::clear() {
    v0z = 0;
    v1 = 0;
    v2 = 0;
}

void Svf::calcCoeffs() {
    g = static_cast<float>(tan(M_PI * fc / sr));
    calcSecondaryCoeffs();
}

float Svf::processSample(float in) {
    update(in);
    return lp*lpMix + bp*bpMix + hp*hpMix + br*brMix;
}

void Svf::update(float in) {
    // update
    v0 = in;
    v1z = v1;
    v2z = v2;
    v3 = v0 + v0z - 2.f * v2z;
    v1 += g1 * v3 - g2 * v1z;
    v2 += g3 * v3 + g4 * v1z;
    v0z = v0;
    // output
    lp = v2;
    bp = v1;
    hp = v0 - rq * v1 - v2;
    br = v0 - rq * v1;
}


void Svf::setSampleRate(float sr) {
    this->sr = sr;
    calcCoeffs();
}

void Svf::setCutoff(float fc) {
    this->fc = (fc > sr/2) ? sr/2 : fc;
    calcCoeffs();
}

void Svf::setInverseQ(float rq) {
    this->rq = rq;
    calcSecondaryCoeffs();
}

void Svf::setLpMix(float mix) {
    lpMix = mix;
}

void Svf::setHpMix(float mix) {
    hpMix = mix;
}

void Svf::setBpMix(float mix) {
    bpMix = mix;
}

void Svf::setBrMix(float mix) {
    brMix = mix;
}

// calculate only secondary coefficients, given primary (e.g. from LUT)
void Svf::calcSecondaryCoeffs() {
    g1 = g / (1.f + g * (g + rq));
    g2 = 2.f * (g + rq) * g1;
    g3 = g * g1;
    g4 = 2.f * g1;
}