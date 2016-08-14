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
 
 sc.crackle~
 (c) stephen lumenta under GPL
 http://www.gnu.org/licenses/gpl.html
 
 part of sc-max http://github.com/sbl/sc-max
 see README
 */

#include "c74_msp.h"

using namespace c74::max;


static t_class *crackle_class = nullptr;

struct t_crackle {
    t_pxobject ob;
    short m_connected;
    
    double m_param;
    double m_y1;
    double m_y2;
};

void *crackle_new(double param) {
    t_crackle *x = NULL;
    x = (t_crackle *)object_alloc(crackle_class);
    
    if (x) {
        dsp_setup((t_pxobject *)x, 1);
        outlet_new((t_object *)x, "signal");
        
        x->m_param = param;
        x->m_y1 = 0.3f;
        x->m_y2 = 0.f;
    } else {
        object_free(x);
    }
    return (x);
}

void crackle_float(t_crackle *x, double param){
    x->m_param = param;
}

void crackle_int(t_crackle *x, long param){
    crackle_float(x, (double) param);
}

void crackle_perform64(t_crackle* x,
                       t_object* dsp64,
                       double** ins,
                       long numins,
                       double** outs,
                       long numouts,
                       long sampleframes,
                       long flags,
                       void* userparam) {
    
    double param = x->m_connected ? *ins[0] : x->m_param;
    double *out = outs[0];
    int n = sampleframes;
    
    float y1 = x->m_y1;
    float y2 = x->m_y2;
    float y0;
    
    if (param < 0.f) param = 0.f;
    if (param > 2.f) param = 2.f;
    
    if (x->ob.z_disabled) return;
    
    while (n--){
        *out++ = y0 = fabs(y1 * param - y2 - 0.05f);
        y2 = y1; y1 = y0;
    }
    
    x->m_y1 = y1;
    x->m_y2 = y2;
}

void crackle_dsp64(t_crackle *self,
                   t_object* dsp64,
                   short* count,
                   double samplerate,
                   long maxvectorsize,
                   long flags) {
    self->m_connected = count[0];
    
    object_method_direct(void, (t_object*, t_object*, t_perfroutine64, long, void*),
                         dsp64, gensym("dsp_add64"), (t_object*)self, (t_perfroutine64)crackle_perform64, 0, NULL);
}

void crackle_assist(t_crackle *x, void *b, long m, long a, char *s) {
    if (m == ASSIST_INLET) { //inlet
        sprintf(s, "(signal/float) param");
    }
    else {  // outlet
        sprintf(s, "(signal) crackle");
    }
}

void ext_main(void *r) {
    t_class *c;
    
    c = class_new("sc.crackle~", (method)crackle_new, (method)dsp_free, (long)sizeof(t_crackle), 0L, A_DEFFLOAT, 0);
    
    class_addmethod(c, (method)crackle_dsp64,"dsp64", A_CANT, 0);
    class_addmethod(c, (method)crackle_assist,"assist", A_CANT, 0);
    class_addmethod(c, (method)crackle_float, "float", A_FLOAT, 0);
    class_addmethod(c, (method)crackle_int, "int", A_LONG, 0);
    
    class_dspinit(c);
    class_register(CLASS_BOX, c);
    crackle_class = c;
}
