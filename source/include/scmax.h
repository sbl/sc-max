/*
 *  scmax.h
 * (c) stephen lumenta under GPL
 * 
 * conveniences for commonly used functions
 * some parts are friendly derived from tim blechmanns sc4pd project
 *
 * http://www.gnu.org/licenses/gpl.html
 * part of sc-max http://github.com/sbl/sc-max
 * see README
 */

#ifndef SCMAX_H_XHP91X4G
#define SCMAX_H_XHP91X4G

#include "SC_Constants.h"
#include "SC_RGen.h"

using namespace c74::max;

// rate conversions

#undef SAMPLERATE
#define SAMPLERATE sys_getsr()

#undef BUFLENGTH
#define BUFLENGTH sys_getblksize()

#undef SAMPLEDUR
#define SAMPLEDUR sc_sampleDur()

double sc_radiansPerSample(){
    return twopi / sys_getsr();
}

double sc_sampleDur(){
    return 1 / sys_getsr();
}

double sc_bufDuration(){
	return sys_getblksize() / sys_getsr();
}

double sc_bufrate(){
	return 1 / sc_bufDuration();
}

double sc_slopeFactor(){
	return 1 / sys_getblksize();
}

int sc_filterLoops(){
	return sys_getblksize() / 3;
}

int sc_filterRemain(){
	return sys_getblksize() % 3;
}

double sc_filterSlope(){
	float loops = sc_filterLoops();
	if (loops == 0)
	    return 0;
	else
	    return 1. / loops;
}

// http://www.musicdsp.org/archive.php?classid=1#59
unsigned long sc_randomSeed( void )
{
    static unsigned long randSeed = 22222;
    randSeed = (randSeed * 196314165) + 907633515;
    return randSeed;
}

// DSP loops

#define ZXP(z) (*(z)++)


#define LOOP(length, stmt)			\
{	\
    for(int _tmp_i=0;_tmp_i!=length; _tmp_i++) {				\
        stmt;					\
    }							\
}

#define sc_abs(a) std::abs(a)
#define sc_max(a,b) (((a) > (b)) ? (a) : (b))
#define sc_min(a,b) (((a) < (b)) ? (a) : (b))

typedef int int32;
typedef unsigned int uint32;

typedef short int16;
typedef unsigned short uint16;

typedef signed char int8;
typedef unsigned char uint8;

typedef float float32;
typedef double float64;

inline float32 zapgremlins(float32 x)
{
	float32 absx = std::abs(x);
	// very small numbers fail the first test, eliminating denormalized numbers
	// (zero also fails the first test, but that is OK since it returns zero.)
	// very large numbers fail the second test, eliminating infinities
	// Not-a-Numbers fail both tests and are eliminated.
	return (absx > (float32)1e-15 && absx < (float32)1e15) ? x : (float32)0.;
}

#define RGET RGen& rgen = self->rgen; uint32 s1 = rgen.s1; uint32 s2 = rgen.s2; uint32 s3 = rgen.s3;
#define RPUT rgen.s1 = s1; rgen.s2 = s2; rgen.s3 = s3;


#endif 