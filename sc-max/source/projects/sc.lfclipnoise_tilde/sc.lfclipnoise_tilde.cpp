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
 
 sc.lfclipnoise~
 (c) stephen lumenta under GPL
 http://www.gnu.org/licenses/gpl.html
 
 part of sc-max http://github.com/sbl/sc-max
 see README
*/

#include "c74_msp.h"
#include "scmax.h"

using namespace c74::max;

static t_class *lfclipnoise_class = nullptr;

struct t_lfclipnoise {
    t_pxobject ob;
    
    double m_freq;
    short m_connected;
    double m_level;
    double m_sr;
    int m_counter;
    
    RGen            rgen;
};

void *lfclipnoise_new(double freq) {
    t_lfclipnoise *x = NULL;
    x = (t_lfclipnoise *)object_alloc(lfclipnoise_class);
    
    if (!x) return x;
    
    dsp_setup((t_pxobject *)x, 1);
    
    x->m_freq       = freq <= 0 ? 500 : freq;
    x->m_counter    = 0;
    x->m_level      = 0.0;
    x->m_sr         = sys_getsr();
    x->rgen.init(sc_randomSeed());
    
    outlet_new((t_object *)x, "signal");

    return x;
}

void lfclipnoise_float(t_lfclipnoise *x, double freq) {
    x->m_freq = freq;
}

void lfclipnoise_perform64(t_lfclipnoise* self,
                           t_object* dsp64,
                           double** ins,
                           long numins,
                           double** outs,
                           long numouts,
                           long sampleframes,
                           long flags,
                           void* userparam) {
    
    double *out = outs[0];
    int remain = sampleframes;
    
    double freq = self->m_connected ? *ins[0] : self->m_freq;
    double level = self->m_level;
    long counter = self->m_counter;
    
    if (self->ob.z_disabled) return ;
    
    RGET
    do {
        if (counter<=0) {
            // otherwise not working
            if(freq < 0.0001) freq = 0.0001;
            
            counter = self->m_sr / sc_max(freq, 0.001f);
            counter = sc_max(1, counter);
            level = fcoin(s1,s2,s3);
        }
        int nsmps = sc_min(remain, counter);
        remain -= nsmps;
        counter -= nsmps;
        
        int i;
        for(i=0; i<nsmps; i++) {
            *out++ = level;
        }
    } while (remain);
    RPUT
    
    self->m_level = level;
    self->m_counter = counter;
}

void lfclipnoise_dsp64(t_lfclipnoise *self,
                       t_object* dsp64,
                       short* count,
                       double samplerate,
                       long maxvectorsize,
                       long flags) {
    self->m_sr = samplerate;
    self->m_connected = count[0];
    
    object_method_direct(void, (t_object*, t_object*, t_perfroutine64, long, void*),
                         dsp64, gensym("dsp_add64"), (t_object*)self, (t_perfroutine64)lfclipnoise_perform64, 0, NULL);
}

void lfclipnoise_assist(t_lfclipnoise *x, void *b, long m, long a, char *s) {
    if (m == ASSIST_INLET) { //inlet
        sprintf(s, "(signal/float) set freq");
    }
    else {  // outlet
        sprintf(s, "(signal) lfclipnoise");
    }
}

void ext_main(void *r){
    t_class *c;
    
    c = class_new("sc.lfclipnoise~", (method)lfclipnoise_new, (method)dsp_free, (long)sizeof(t_lfclipnoise), 0L, A_DEFFLOAT, 0);
    class_addmethod(c, (method)lfclipnoise_dsp64, "dsp64", A_CANT, 0);
    class_addmethod(c, (method)lfclipnoise_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)lfclipnoise_float, "float",  A_FLOAT, 0);
    
    class_dspinit(c);
    class_register(CLASS_BOX, c);
    lfclipnoise_class = c;
}
