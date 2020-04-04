#pragma once

#include <string>

namespace bruits {

/**
 * from https://github.com/VCVRack/Rack/blob/v1/include/dsp/ringbuffer.hpp
 *
 * A cyclic buffer which maintains a valid linear array of size S by keeping a copy
 * of the buffer in adjacent memory. S must be a power of 2. Thread-safe for single
 * producers and consumers?
 */
template <typename T, size_t S> struct DoubleRingBuffer {
    T data[S * 2];
    size_t start = 0;
    size_t end = 0;

    size_t mask(size_t i) const {
        return i & (S - 1);
    }

    void push(T t) {
        size_t i = mask(end++);
        data[i] = t;
        data[i + S] = t;
    }

    T shift() {
        return data[mask(start++)];
    }

    void clear() {
        start = end;
    }

    bool empty() const {
        return start == end;
    }

    bool full() const {
        return end - start == S;
    }

    size_t size() const {
        return end - start;
    }

    size_t capacity() const {
        return S - size();
    }

    /** Returns a pointer to S consecutive elements for appending.
    If any data is appended, you must call endIncr afterwards.
    Pointer is invalidated when any other method is called.
    */
    T* endData() {
        return &data[mask(end)];
    }

    void endIncr(size_t n) {
        size_t e = mask(end);
        size_t e1 = e + n;
        size_t e2 = (e1 < S) ? e1 : S;
        // Copy data forward
        std::memcpy(&data[S + e], &data[e], sizeof(T) * (e2 - e));

        if (e1 > S) {
            // Copy data backward from the doubled block to the main block
            std::memcpy(data, &data[S], sizeof(T) * (e1 - S));
        }
        end += n;
    }

    /** Returns a pointer to S consecutive elements for consumption
    If any data is consumed, call startIncr afterwards.
    */
    const T* startData() const {
        return &data[mask(start)];
    }

    void startIncr(size_t n) {
        start += n;
    }
};

}
