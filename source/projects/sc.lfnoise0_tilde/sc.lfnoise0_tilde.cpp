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
 sc.lfnoise0~
 (c) stephen lumenta under GPL
 http://www.gnu.org/licenses/gpl.html
 
 part of sc-max http://github.com/sbl/sc-max
 see README
*/

#include "c74_msp.h"
#include "scmax.h"

using namespace c74::max;

static t_class *lfnoise_class = nullptr;

struct t_lfnoise {
    t_pxobject ob;
    
    double m_freq;
    double m_sr;
    short m_connected;
    double m_level;
    int m_counter;
    
    RGen rgen;
};

void *lfnoise_new(double freq) {
    t_lfnoise *x = NULL;
    x = (t_lfnoise *)object_alloc(lfnoise_class);
    
    if (x) {
        // 1 inlet
        dsp_setup((t_pxobject *)x, 1);
        
        x->m_freq       = freq <= 0 ? 500 : freq;
        x->m_counter    = 0;
        x->m_level      = 0.0;
        x->m_sr         = sys_getsr();
        
        
        x->rgen.init(sc_randomSeed());
        
        outlet_new((t_object *)x, "signal");
    }
    return (x);
}

void lfnoise_float(t_lfnoise *x, double freq){
    x->m_freq = (float) freq;
}

void lfnoise_perform64(t_lfnoise* self,
                       t_object* dsp64,
                       double** ins,
                       long numins,
                       double** outs,
                       long numouts,
                       long sampleframes,
                       long flags,
                       void* userparam) {
    double *out = outs[0];
    int remain  = sampleframes;
    
    double freq = self->m_connected ? *ins[0] : self->m_freq;
    double level   = self->m_level;
    int counter = self->m_counter;
    
    if (self->ob.z_disabled) return;
    
    RGET
    do {
        if (counter <= 0) {
            counter = (int)(self->m_sr / sc_max(freq, .001f));
            counter = sc_max(1, counter);
            level = frand2(s1,s2,s3);
        }
        int nsmps = sc_min(remain, counter);
        remain -= nsmps;
        counter -= nsmps;
        
        while(nsmps--){
            *out++ = level;
        }
    } while (remain);
    
    self->m_level = level;
    self->m_counter = counter;
    RPUT
}

void lfnoise_dsp64(t_lfnoise *self,
                   t_object* dsp64,
                   short* count,
                   double samplerate,
                   long maxvectorsize,
                   long flags) {
    self->m_sr         = samplerate;
    self->m_connected  = count[0];
    object_method_direct(void, (t_object*, t_object*, t_perfroutine64, long, void*),
                         dsp64, gensym("dsp_add64"), (t_object*)self, (t_perfroutine64)lfnoise_perform64, 0, NULL);

}

void lfnoise_assist(t_lfnoise *x, void *unused, t_assist_function io, long index, char *s) {
    if (io == ASSIST_INLET) { //inlet
        sprintf(s, "(signal/float) set freq");
    }
    else {	// outlet
        sprintf(s, "(signal) lfnoise");
    }
}

void ext_main(void *r) {
    t_class *c;
    c = class_new("sc.lfnoise0~", (method)lfnoise_new, (method)dsp_free, (long)sizeof(t_lfnoise), 0L, A_DEFFLOAT, 0);
    
    class_addmethod(c, (method)lfnoise_dsp64, "dsp64", A_CANT, 0);
    class_addmethod(c, (method)lfnoise_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)lfnoise_float, "float", A_FLOAT, 0);
    
    class_dspinit(c);
    class_register(CLASS_BOX, c);
    lfnoise_class = c;
}
