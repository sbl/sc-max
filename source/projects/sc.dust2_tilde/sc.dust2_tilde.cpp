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
 sc.dust2~
 (c) stephen lumenta under GPL
 http://www.gnu.org/licenses/gpl.html
 
 part of sc-max http://github.com/sbl/sc-max
 see README
 */

#include "c74_msp.h"
#include "scmax.h"

using namespace c74::max;

static t_class *dust2_class = nullptr;
struct t_dust2 {
    t_pxobject m_ob;
    short  m_connected;
    
    double m_density;
    double m_thresh;
    double m_scale;
    double m_sr;
    double m_isr;
    RGen rgen;
};

void dust2_calc_density(t_dust2 *x) {
    x->m_thresh = x->m_density * x->m_isr;
    x->m_scale  = x->m_thresh > 0.f ? 2.f / x->m_thresh : 0.f;
}

void *dust2_new(double density) {
    t_dust2 *x = NULL;
    x = (t_dust2 *)object_alloc(dust2_class);
    
    if (x) {
        dsp_setup((t_pxobject *)x, 1);
        outlet_new((t_object *)x, "signal");
        
        x->rgen.init(sc_randomSeed());
        x->m_density = density;
        
        x->m_sr = sys_getsr();
        x->m_isr = 1/x->m_sr;
        x->m_scale = 0.;
        x->m_thresh = 0.;
        
        dust2_calc_density(x);
    }
    return (x);
}

void dust2_float(t_dust2 *x, double density) {
    x->m_density = density;
    dust2_calc_density(x);
}

void dust2_int(t_dust2 *x, long density) {
    dust2_float(x, (double) density);
}

void dust2_perform64(t_dust2* self,
                     t_object* dsp64,
                     double** ins,
                     long numins,
                     double** outs,
                     long numouts,
                     long sampleframes,
                     long flags,
                     void* userparam) {
    double density = self->m_connected ? *ins[0] : self->m_density;
    double *out = outs[0];
    int n = sampleframes;
    
    if (self->m_ob.z_disabled) return;
    
    if (density != self->m_density) {
        self->m_density = density;
        dust2_calc_density(self);
    }
    
    RGET
    while (n--){
        float z = frand(s1, s2, s3);
        
        if (z < self->m_thresh) *out++ = z * self->m_scale - 1.f;
        else  *out++ = 0.f;
    }
    RPUT
}

void dust2_dsp64(t_dust2 *self,
                 t_object* dsp64,
                 short* count,
                 double samplerate,
                 long maxvectorsize,
                 long flags) {
    self->m_connected = count[0];
    object_method_direct(void, (t_object*, t_object*, t_perfroutine64, long, void*),
                         dsp64, gensym("dsp_add64"), (t_object*)self, (t_perfroutine64)dust2_perform64, 0, NULL);
}

void dust2_assist(t_dust2 *x, void *b, long m, long a, char *s) {
    if (m == ASSIST_INLET) { //inlet
        sprintf(s, "(signal/float) density");
    }
    else {  // outlet
        sprintf(s, "(signal) dust2 out");
    }
}

void ext_main(void *r) {
    t_class *c;
    c = class_new("sc.dust2~", (method)dust2_new, (method)dsp_free, (long)sizeof(t_dust2), 0L, A_DEFFLOAT, 0);
    
    class_addmethod(c, (method)dust2_dsp64, "dsp64", A_CANT, 0);
    class_addmethod(c, (method)dust2_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)dust2_float, "float", A_FLOAT, 0);
    class_addmethod(c, (method)dust2_int, "int", A_LONG, 0);
    
    class_dspinit(c);
    class_register(CLASS_BOX, c);
    dust2_class = c;
}
