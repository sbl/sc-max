#include <iostream>

#include "Smoothing.hpp"


ExpLogSmoother smooth;

void do_section(float val, float nsamps) {
    for (int i = 0; i < nsamps; ++i) {
        float y = smooth.processSample(val);
        std::cout << y << ", ";
    }
}

int main() {
    int dur = 48000;

    std::cout << "output = [ ";

    do_section(0, 100);
    do_section(1, dur);
    do_section(0, dur);
    do_section(1, dur);
    do_section(0, dur);

    std::cout << " ]; " << std::endl << std::endl;

}
