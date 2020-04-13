namespace dspkit {
    // mega simple lookup table class
    template<typename T>
    class Lut {
    public:
        // look up a value from a table with normalized position
        // NB:
        // - no bounds checking on position! must be in [0, 1] or you get garbage.
        // - table should be "extended" with an extra duplicate or wrapped element as appropriate
        static T lookupLinear(float x, const T *tab, unsigned int size) {
            const unsigned int size_2 = size - 2;
            const float fidx = x * static_cast<float>(size_2);
            const auto idx = static_cast<unsigned int>(fidx);
            const float a = tab[idx];
            const float b = tab[idx + 1];
            const float f = fidx - static_cast<float>(idx);
            return a + (b - a) * f;
        }
    };

    // mega simple bilateral lookup on 2D tables
    // tables are passed as array-of-pointer;
    // IOW it's not assumed that "rows" are contiguous in memory,
    // but it _is_ assumed that they are all the same length
    template <typename T>
    class BiLut {
        // @param x: interpolation position within row (in [0, 1])
        // @param y: interpolation position between rows (in [0, 1])
        // @param tab: data (array of pointers)
        // @param N: size of each row
        // @param M: number of rows
        static T lookupLinear(float x, float y, const T ** tab, unsigned int N, unsigned int M) {
            const unsigned int M_2 = M-2;
            const float fym = x * static_cast<float>(M-2);
            const auto iym = static_cast<unsigned int>(fym);
            const float tab0 = tab[iym];
            const float tab1 = tab[iym+1];
            // FIXME: small optimization is possible here,
            /// by memoizing transformations from `x` to idx/coeff
            const T a = Lut<T>::lookupLinear(x, tab0, N);
            const T b = Lut<T>::lookupLinear(x, tab1, N);
            const float fy = fym - static_cast<float>(iym);
            return a + fy*(b-a);

        }
    };



}
