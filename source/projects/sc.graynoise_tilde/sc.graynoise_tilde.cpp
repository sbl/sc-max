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
 graynoise~
 (c) stephen lumenta under GPL
 http://www.gnu.org/licenses/gpl.html

 part of sc-max http://github.com/sbl/sc-max
 see README
*/

#include "c74_msp.h"
#include "scmax.h"

using namespace c74::max;


static t_class *graynoise_class = nullptr;

struct t_graynoise {
    t_pxobject          ob;
    int                         m_counter;
    RGen                        rgen;
};

void *graynoise_new(long argc, t_atom *argv) {
    t_graynoise *x = NULL;
    x = (t_graynoise *)object_alloc(graynoise_class);
    
    if (x) {
        dsp_setup((t_pxobject *)x, 0);
        
        outlet_new((t_object *)x, "signal");
        
        x->rgen.init(sc_randomSeed());
        x->m_counter = 0;
    }
    return (x);
}

void graynoise_perform64(t_graynoise *self,
                         t_object* dsp64,
                         double** ins,
                         long numins,
                         double** outs,
                         long numouts,
                         long sampleframes,
                         long flags,
                         void* userparam) {
    int counter = self->m_counter;
    double *out = outs[0];
    int n = sampleframes;

    if (self->ob.z_disabled) return ;

    RGET
    while (n--){
        counter ^= 1L << (trand(s1,s2,s3) & 31);
        *out++ = counter * 4.65661287308e-10f;
    }
    RPUT
    
    self->m_counter = counter;
}

void graynoise_dsp64(t_graynoise *self,
                     t_object* dsp64,
                     short* count,
                     double samplerate,
                     long maxvectorsize,
                     long flags) {
    object_method_direct(void, (t_object*, t_object*, t_perfroutine64, long, void*),
                         dsp64, gensym("dsp_add64"), (t_object*)self, (t_perfroutine64)graynoise_perform64, 0, NULL);
}

void graynoise_assist(t_graynoise *x, void *b, long m, long a, char *s) {
    if (m == ASSIST_INLET) { //inlet
        sprintf(s, "Ignore this inlet");
    }
    else {  // outlet
        sprintf(s, "(signal) Gray Noise");
    }
}

void ext_main(void *r) {
    t_class *c;
    c = class_new("sc.graynoise~", (method)graynoise_new, (method)dsp_free, (long)sizeof(t_graynoise), 0L, A_GIMME, 0);

    class_addmethod(c, (method)graynoise_dsp64, "dsp64", A_CANT, 0);
    class_addmethod(c, (method)graynoise_assist, "assist", A_CANT, 0);
    
    class_dspinit(c);
    class_register(CLASS_BOX, c);
    
    graynoise_class = c;
}
