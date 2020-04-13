// really simple fixed-size FIFO queue
//
// should work with any POD data structure.
//
// safe for audio threads, no allocations and reasonable speed.
//
// NOT threadsafe! use e.g. boost::spsc_queue if you need that.
//
/// TODO: simplify by removing `used` flags (after assertion testing)

#include <array>
#include <cassert>

namespace dspkit {

    template<typename T, int capacity>
    class Fifo {
    private:

        // data buffer, accessed circularly
        struct data {
            bool used;
            T value;
        };
        std::array<data, capacity> buf_;
        // write index
        int wrIx_;
        // last write index
        int wrIx0_;
        // read index
        int rdIx_;
        // current count of things
        int size_;

    public:
        Fifo() : wrIx_(0), wrIx0_(0), rdIx_(0), size_(0) {}

        // push a value to the back of the queue
        // @param value
        // @returns false if queue was full, otherwise true
        bool push(T value) {
            if (size_ < (capacity)) {
                buf_[wrIx_].used = true;
                buf_[wrIx_].value = value;
                ++size_;
                wrIx0_ = wrIx_;
                ++wrIx_;
                while (wrIx_ >= capacity) { wrIx_ -= capacity; }
                return true;
            } else {
                return false;
            }
        }

        // consume the first value and copy it by reference
        // if queue is empty, nothing happens
        // @param dst: reference to return value.
        // @returns:  false if queue was empty, otherwise true
        bool  pop(T &dst) {
            if (size_ > 0) {
                dst = buf_[rdIx_].value;
                buf_[rdIx_].used = false;
                --size_;
                ++rdIx_;
                while (rdIx_ >= capacity) { rdIx_ -= capacity; }
                return true;
            } else {
                return false;
            }
        }

        // get a pointer to the first element
        // @returns pointer or nullptr
        const T *front() {
            if (size_ > 0) {
                assert(buf_[rdIx_].used == true);
                return &(buf_[rdIx_].value);
            } else {
                return nullptr;
            }
        }

        // get a pointer to the first element, with an offset
        // NB: there is no check
        // @returns pointer or nullptr
        const T *front(int offset) {
            if (size_ > 0) {
                int ix;
                if (offset >= size_) {
                    ix = rdIx_ + size_ - 1;
                } else {
                    ix = rdIx_ + offset;
                }
                while (ix >= capacity) { ix -= capacity; }
                assert(buf_[ix].used == true);
                return &(buf_[ix].value);
            } else {
                return nullptr;
            }
        }


        // get a pointer to the last element
        // @returns pointer or nullptr
        const T *back() {
            if (size_ > 0) {
                assert(buf_[wrIx0_].used == true);
                return &(buf_[wrIx0_].value);
            } else {
                return nullptr;
            }
        }

        // get the current count of objects in the queue
        int size() { return size_; }

        // clear the queue
        void clear() {
            wrIx_ = 0;
            wrIx0_ = 0;
            rdIx_ = 0;
            size_ = 0;
            // really only need this flag for testing
            for (int i = 0; i < capacity; ++i) {
                buf_[i].used = false;
            }
        }
    };

}
