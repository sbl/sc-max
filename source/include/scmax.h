/*
 *  scmax.h
 * (c) stephen lumenta under GPL
 *
 * http://www.gnu.org/licenses/gpl.html
 * part of sc-max http://github.com/sbl/sc-max
 * see README
 */

#ifndef SCMAX_H_XHP91X4G
#define SCMAX_H_XHP91X4G

#include "SC_Constants.h"
#include "SC_RGen.h"


// http://www.musicdsp.org/archive.php?classid=1#59
unsigned long sc_randomSeed( void ) {
    static unsigned long randSeed = 22222;
    randSeed = (randSeed * 196314165) + 907633515;
    return randSeed;
}

// DSP loops

#define ZXP(z) (*(z)++)

#define sc_max(a,b) (((a) > (b)) ? (a) : (b))
#define sc_min(a,b) (((a) < (b)) ? (a) : (b))

#define RGET \
    RGen& rgen = self->rgen; \
    uint32 s1 = rgen.s1; \
    uint32 s2 = rgen.s2; \
    uint32 s3 = rgen.s3;
#define RPUT \
    rgen.s1 = s1; \
    rgen.s2 = s2; \
    rgen.s3 = s3;


inline double zapgremlins(double x) {
    double absx = std::abs(x);
    // very small numbers fail the first test, eliminating denormalized numbers
    // (zero also fails the first test, but that is OK since it returns zero.)
    // very large numbers fail the second test, eliminating infinities
    // Not-a-Numbers fail both tests and are eliminated.
    return (absx > (double)1e-15 && absx < (double)1e15) ? x : (double)0.;
}

#endif