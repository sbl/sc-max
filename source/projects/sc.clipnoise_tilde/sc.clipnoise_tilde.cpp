/*
  SuperCollider real time audio synthesis system
    Copyright (c) 2002 James McCartney. All rights reserved.
  http://www.audiosynth.com

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
*/

/*

 sc.clipnoise~
 (c) stephen lumenta under GPL
 http://www.gnu.org/licenses/gpl.html

 part of sc-max http://github.com/sbl/sc-max
 see README
*/

#include "c74_msp.h"
#include "scmax.h"

using namespace c74::max;


static t_class *this_class = nullptr;

struct t_clipnoise {
  t_pxobject ob;
  RGen rgen;
};

void *clipnoise_new(long argc, t_atom *argv) {
    t_clipnoise *self = (t_clipnoise *)object_alloc(this_class);

    dsp_setup((t_pxobject *)self, 0);
    outlet_new((t_object *)self, "signal");
    self->rgen.init(sc_randomSeed());

    return self;
}

void clipnoise_perform64(t_clipnoise* self,
                         t_object* dsp64,
                         double** ins,
                         long numins,
                         double** outs,
                         long numouts,
                         long sampleframes,
                         long flags,
                         void* userparam) {
    double *out = outs[0];
    long n = sampleframes;

    RGET
    while (n--){
        *out++ = fcoin(s1, s2, s3);
    }
    RPUT
}

void clipnoise_dsp64(t_clipnoise *self,
                     t_object* dsp64,
                     short* count,
                     double samplerate,
                     long maxvectorsize,
                     long flags) {
    object_method_direct(void, (t_object*, t_object*, t_perfroutine64, long, void*),
                         dsp64, gensym("dsp_add64"), (t_object*)self, (t_perfroutine64)clipnoise_perform64, 0, NULL);
}

void clipnoise_assist(t_clipnoise *self, void *b, long m, long a, char *s) {
  if (m == ASSIST_INLET) { //inlet
    sprintf(s, "Ignore this inlet");
  }
  else {  // outlet
    sprintf(s, "(signal) Clip Noise");
  }
}

void ext_main(void* r) {
    this_class = class_new("sc.clipnoise~", (method)clipnoise_new, (method)dsp_free, (long)sizeof(t_clipnoise), 0L, A_GIMME, 0);

    class_addmethod(this_class, (method)clipnoise_dsp64, "dsp64", A_CANT, 0);
    class_addmethod(this_class, (method)clipnoise_assist, "assist", A_CANT, 0);

    class_dspinit(this_class);
    class_register(CLASS_BOX, this_class);
}
