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
 sc.mantissamask~
 (c) stephen lumenta under GPL
 http://www.gnu.org/licenses/gpl.html

 part of sc-max http://github.com/sbl/sc-max
 see README
 */


#include "c74_msp.h"

using namespace c74::max;

static t_class *mantissamask_class = nullptr;

struct t_mantissamask {
    t_pxobject ob;
    int m_bits;
};

void *mantissamask_new(long bits) {
    t_mantissamask *x = (t_mantissamask *)object_alloc(mantissamask_class);
    
    if (!x) return x;
    
    dsp_setup((t_pxobject *)x, 2);
    outlet_new((t_object *)x, "signal");
    
    x->m_bits = bits;
    
    return x;
}

void mantissamask_int(t_mantissamask *self, long bits) {
    if(proxy_getinlet((t_object *)self) == 1) {
        self->m_bits = bits;
    }
}

void mantissamask_float(t_mantissamask *self, double bits) {
    mantissamask_int(self, (long)bits);
}

void mantissamask_perform64(t_mantissamask* self,
                            t_object* dsp64,
                            double** ins,
                            long numins,
                            double** outs,
                            long numouts,
                            long sampleframes,
                            long flags,
                            void* userparam) {
    double *in = ins[0];
    long bits = self->m_bits;

    double *out = (outs[0]);
    long mask = -1 << (53 - bits);
    int n = sampleframes;

    if (self->ob.z_disabled) return ;

    while (n--)
        *out++ = mask & (long)(*(in)++);
}

void mantissamask_dsp64(t_mantissamask *self, t_object* dsp64, short* count, double samplerate, long maxvectorsize, long flags) {
    object_method_direct(void, (t_object*, t_object*, t_perfroutine64, long, void*),
                         dsp64, gensym("dsp_add64"), (t_object*)self, (t_perfroutine64)mantissamask_perform64, 0, NULL);
}

void mantissamask_assist(t_mantissamask *self, void *b, t_assist_function io, long a, char *s) {
    if (io == ASSIST_INLET) { //inlet
        switch (a) {
            case 0:
                sprintf(s, "(signal) signal to mask");
                break;
            case 1:
                sprintf(s, "(float/int) bit precision");
                break;
        }
    }
    else {  // outlet
        sprintf(s, "(signal) masked signal");
    }
}

void ext_main(void *r) {
    t_class *c;
    c = class_new("sc.mantissamask~", (method)mantissamask_new, (method)dsp_free, (long)sizeof(t_mantissamask), 0L, A_LONG, 0);
    
    class_addmethod(c, (method)mantissamask_dsp64, "dsp64", A_CANT, 0);
    class_addmethod(c, (method)mantissamask_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)mantissamask_float, "float", A_FLOAT, 0);
    class_addmethod(c, (method)mantissamask_int, "int", A_LONG, 0);
    
    class_dspinit(c);
    class_register(CLASS_BOX, c);
    mantissamask_class = c;
}
