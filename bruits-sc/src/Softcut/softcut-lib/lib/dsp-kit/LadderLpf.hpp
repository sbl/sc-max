// a 4th-order lowpass ladder filter
// after v. zavalashin and w. pirkle

#include <array>
#include <cmath>

#include <iostream>

namespace dspkit {
    template<typename T>
    class LadderLpf {
        T t;    // sampling interval
        T g;    // "small-g" single-stage tuning coefficient
        T k;    // scaling coefficient from Q
        T G;    // "big-G" coefficient for each stage

        std::array<T, 4> z1; // storage element per stage

    public:
        void init(T sr) {
            t = 1 / sr;
        }

        void setCutoff(T fc) {
            static constexpr double twopi = 6.2831853071796;
            T wd = twopi * fc;
            T wa = 2 / t * tan(wd * t / 2);
            g = wa * t / 2;
            G = g / (1 + g);
        }

        void setQ(T q) {
            // assuming Q in (0, ~20]
            k = 4 * (q - 0.707) / (25 - 0.707);
            std::cout << k << std::endl;
        }

        T stage(T x, T &z) {
            T v = G * (x - z);
            T y = v + z;
            z = y + v;
            return y;
        }

        T processSample(T x) {
            T g2 = g;
            T g3 = g2 * g;
            T g4 = g2 * g2;
            // sum all stage memories
            T S = g3 * z1[0] + g2 * z1[1] + g * z1[2] + z1[3];
            // scale and warp input as per zavalishin
            T u = (x - k * S) / (1 + k * g4);
            // cascade
            return stage(stage(stage(stage(u, z1[0]), z1[1]), z1[2]), z1[3]);
        }

    };

}