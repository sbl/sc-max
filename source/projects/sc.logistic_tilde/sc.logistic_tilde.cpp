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
 sc.logistic~
 (c) stephen lumenta under GPL
 http://www.gnu.org/licenses/gpl.html

 part of sc-max http://github.com/sbl/sc-max
 see README
 */

#include "c74_msp.h"
#include "scmax.h"

using namespace c74::max;

static t_class *logistic_class = nullptr;

struct t_logistic {
    t_pxobject ob;

    short m_connected[2];
    double m_chaos_param;
    double m_freq;
    double m_sr;

    double m_y1; // init value
    long m_counter;
};

void *logistic_new(t_symbol *s, long argc, t_atom *argv) {
    t_logistic *x = NULL;
    x = (t_logistic *)object_alloc(logistic_class);
    
    if (!x) return x;
    
    dsp_setup((t_pxobject *)x, 2);
    outlet_new((t_object *)x, "signal");
    
    double chaos_param = atom_getfloat(&argv[0]);
    double freq = atom_getfloat(&argv[1]);
    
    x->m_chaos_param = chaos_param ? chaos_param : 3.0;
    x->m_freq = (short) freq ? freq : 1000.0;
    x->m_y1 = 0.5;
    x->m_counter = 0;
    x->m_sr = sys_getsr();

    return x;
}

void logistic_float(t_logistic *x, double f){
    switch (proxy_getinlet((t_object *)x)) {
        case 0:
            x->m_chaos_param = f;
            break;
        case 1:
            x->m_freq = f;
            break;
    }
}

void logistic_int(t_logistic *x, long l) {
    logistic_float(x, (double) l);
}

void logistic_perform64(t_logistic* self,
                        t_object* dsp64,
                        double** ins,
                        long numins,
                        double** outs,
                        long numouts,
                        long sampleframes,
                        long flags,
                        void* userparam) {
    double *out = outs[0];
    long i;
    
    double paramf = self->m_connected[0] ? *ins[0] : self->m_chaos_param;
    double freq = self->m_connected[1] ? *ins[1] : self->m_freq;
    
    double  y1 = self->m_y1;
    long counter = self->m_counter;

    if (self->ob.z_disabled) return ;

    long remain = sampleframes;
    if (freq > self->m_sr) freq = self->m_sr;
    if (freq < 0) freq = 0;

    do {
        if (counter<=0) {
            counter = (long)(self->m_sr  / sc_max(freq, 0.001));
            counter = sc_max(1, counter);
            y1 = paramf * y1 * (1.0 - y1);  // chaotic equation
        }
        long nsmps = sc_min(counter, remain);
        counter -= nsmps;
        remain  -= nsmps;

        for(i=0; i<nsmps;i++){
            *out++ = y1;
        }

    } while(remain);

    self->m_y1 = y1;
    self->m_counter = counter;
}

void logistic_dsp64(t_logistic *self, t_object* dsp64, short* count, double samplerate, long maxvectorsize, long flags) {
    self->m_connected[0] = count[0];
    self->m_connected[1] = count[1];
    
    object_method_direct(void, (t_object*, t_object*, t_perfroutine64, long, void*),
                         dsp64, gensym("dsp_add64"), (t_object*)self, (t_perfroutine64)logistic_perform64, 0, NULL);
}

void logistic_assist(t_logistic *x, void *b, t_assist_function io, long a, char *s) {
    if (io == ASSIST_INLET) { //inlet
        switch (a) {
            case 0:
                sprintf(s, "(signal/float) chaos param");
                break;
            case 1:
                sprintf(s, "(signal/float) freq");
                break;
            default:
                break;
        }
    }
    else {  // outlet
        sprintf(s, "(signal) Logistic");
    }
}

void ext_main(void *r) {
    t_class *c;
    c = class_new("sc.logistic~", (method)logistic_new, (method)dsp_free, (long)sizeof(t_logistic), 0, A_GIMME, 0);
    
    class_addmethod(c, (method)logistic_dsp64, "dsp64", A_CANT, 0);
    class_addmethod(c, (method)logistic_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)logistic_float, "float", A_FLOAT, 0);
    class_addmethod(c, (method)logistic_int, "int", A_LONG, 0);
    
    class_dspinit(c);
    class_register(CLASS_BOX, c);
    logistic_class = c;
}
