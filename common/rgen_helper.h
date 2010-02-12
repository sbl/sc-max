/*
 *  rgen_helper.h
 *
 *  rewritten rget rput for max
 */

#include "SC_RGen.h"

// macros to get pseudo-random number generator, and put its state in registers
#define RGET RGen& rgen = x->rgen; uint32 s1 = rgen.s1; uint32 s2 = rgen.s2; uint32 s3 = rgen.s3;
#define RPUT rgen.s1 = s1; rgen.s2 = s2; rgen.s3 = s3;