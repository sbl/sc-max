#include "wrInterpolate.h"

float interp_linear_2pt(float coeff, float* zeroth) {
    return zeroth[0] + coeff * (zeroth[1] - zeroth[0]);
}

float interp_hermite_4pt(float coeff, float* zeroth) {
    float* y = zeroth;
    float c0 = y[0];
    float c1 = 0.5 * (y[1] - y[-1]);
    float c2 = y[-1] - 2.5 * y[0] + 2 * y[1] - 0.5 * y[2];
    float c3 = 0.5 * (y[2] - y[-1]) + 1.5 * (y[0] - y[1]);
    return ((c3 * coeff + c2) * coeff + c1) * coeff + c0;
}
