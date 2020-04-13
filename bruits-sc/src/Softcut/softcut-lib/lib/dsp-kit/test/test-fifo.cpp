#include <iostream>

#include "Fifo.hpp"

using namespace dspkit;

int main() {
    constexpr int n = 100;
    Fifo<int, n> fifo;

    for (int i=0; i<n+1; ++i) {
        if( fifo.push(i) ) {
            std::cout << "pushed " << i << "; ok" << std::endl;
        } else {
            std::cout << "pushed " << i << "; failed" << std::endl;
        }
    }

    int out;
    for (int i=0; i<n+1; ++i) {
        if( fifo.pop(out) ) {
            std::cout << "popped " << out << "; ok" << std::endl;
        } else {
            std::cout << "popped " << out << "; failed" << std::endl;
        }
    }
}

    
