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

#include <cmath>

// rate conversions

#undef SAMPLERATE
#define SAMPLERATE sys_getsr()

#undef BUFLENGTH
#define BUFLENGTH sys_getblksize()

float sc_radiansPerSample(){
    return TWOPI / sys_getsr();
}

float sc_bufDuration(){
	return sys_getblksize() / sys_getsr();
}

float sc_bufrate(){
	return 1 / sc_bufDuration();
}

float sc_slopeFactor(){
	return 1 / sys_getblksize();
}

int sc_filterLoops(){
	return sys_getblksize() / 3;
}

int sc_filterRemain(){
	return sys_getblksize() % 3;
}

float sc_filterSlope(){
	float loops = sc_filterLoops();
	if (loops == 0)
	    return 0;
	else
	    return 1. / loops;
}

// DSP loops

#define ZXP(z) (*(z)++)


#define LOOP(length, stmt)			\
{	\
for(int _tmp_i=0;_tmp_i!=length; _tmp_i++) {				\
stmt;					\
}							\
}

// math
const double log001 = std::log(0.001);
const double log01  = std::log(0.01);
const double log1   = std::log(0.1);
const double rlog2  = 1./std::log(2.);
const double sqrt2  = std::sqrt(2.);
const double rsqrt2 = 1. / sqrt2;
const float pi_f    = std::acos(-1.f);
const float pi2_f   = pi_f * 0.5f;
const float pi32_f  = pi_f * 1.5f;
const float twopi_f = pi_f * 2.f;
const float sqrt2_f = std::sqrt(2.f);
const float rsqrt2_f= 1.f/std::sqrt(2.f);


typedef float float32;

inline float32 zapgremlins(float32 x)
{
	float32 absx = abs(x);
	// very small numbers fail the first test, eliminating denormalized numbers
	// (zero also fails the first test, but that is OK since it returns zero.)
	// very large numbers fail the second test, eliminating infinities
	// Not-a-Numbers fail both tests and are eliminated.
	return (absx > (float32)1e-15 && absx < (float32)1e15) ? x : (float32)0.;
}

#endif 