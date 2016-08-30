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
 
 Original Ugens by Nick Collins
 SLUGens released under the GNU GPL as extensions for SuperCollider 3, by Nick Collins, http://www.informatics.sussex.ac.uk/users/nc81/
 */

/*
 sc.weaklynonlinear~
 (c) stephen lumenta under GPL
 http://www.gnu.org/licenses/gpl.html
 
 part of sc-max http://github.com/sbl/sc-max
 see README
 */

#include "c74_msp.h"

using namespace c74::max;

static t_class *weaklynonlinear_class = nullptr;

struct t_weaklynonlinear {
    t_pxobject ob;
    double x,y,t;
    int reset;
    double pi_sr;
    
    double ratex, ratey, freq, initx, inity, alpha, xexponent, beta, yexponent;
};

void *weaklynonlinear_new(t_symbol *s, long ac, t_atom *av) {
    t_weaklynonlinear *x = (t_weaklynonlinear *)object_alloc(weaklynonlinear_class);

    dsp_setup((t_pxobject *)x, 1);
    
    x->x = 0.0;
    x->y = 0.0;
    x->t = 0.0;
    
    x->pi_sr = 1.0;
    x->reset = 0;
    x->ratex = 1;
    x->ratey = 1;
    x->freq = 440;
    x->initx = 0;
    x->inity = 0;
    x->alpha = 0.;
    x->xexponent = 0;
    x->beta = 0;
    x->yexponent = 0;
    
    attr_args_process(x, ac, av);
    outlet_new((t_object *)x, "signal");

    return x;
}

void weaklynonlinear_perform64(t_weaklynonlinear* self,
                               t_object* dsp64,
                               double** ins,
                               long numins,
                               double** outs,
                               long numouts,
                               long sampleframes,
                               long flags,
                               void* userparam) {
    double *in = ins[0];
    double *out = outs[0];
    long n = sampleframes;
    
    int reset = self->reset;
    double xrate = self->ratex;
    double yrate = self->ratey;
    double w0 = self->freq;
    double alpha = self->alpha;
    double xexponent = self->xexponent;
    double beta = self->beta;
    double yexponent = self->yexponent;
    
    w0 = w0 * self->pi_sr; //convert frequency in Hertz to angular frequency
    w0 = w0 * w0; //constant needed for equation
    
    double x, y, t;
    
    x = self->x;
    y = self->y;
    t = self->t;
    
    if(reset){
        x = self->x;
        y = self->y;
        t=0;
    }
    
    for (int j = 0; j < n; ++j) {
        //the naive Euler update
        double dxdt = xrate * y;
        double nonlinear = 0.0;
        
        if(alpha > 0.000001 || alpha < (-0.000001)) {
            nonlinear = alpha * (pow(x,xexponent)+beta)*(pow(y,yexponent));
        }
        
        double dydt = yrate * (*in++ - w0 * x + nonlinear);
        x += dxdt;
        y += dydt;
        
        if ((x>1.0) || (x<-1.0))
            x=fabs(fmod((x-1.0),4.0)-2.0)-1.0;
        
        *out++ = x;
    }
    
    self->x = x;
    self->y = y;
    self->t =   t;
}

void weaklynonlinear_dsp64(t_weaklynonlinear *self,
                         t_object* dsp64,
                         short* count,
                         double samplerate,
                         long maxvectorsize,
                         long flags) {
    self->pi_sr = M_PI / samplerate;
    object_method_direct(void, (t_object*, t_object*, t_perfroutine64, long, void*),
                         dsp64, gensym("dsp_add64"), (t_object*)self, (t_perfroutine64)weaklynonlinear_perform64, 0, NULL);
}

void weaklynonlinear_assist(t_weaklynonlinear *x, void *b, long m, long a, char *s) {
    if (m == ASSIST_INLET) { //inlet
        sprintf(s, "(signal) Audio Rate input: acts as external force");
    }
    else {	// outlet
        sprintf(s, "(signal) Weakly Nonlinear Oscillator");
    }
}

void ext_main(void *r) {
    t_class *c = class_new("sc.weaklynonlinear~",
                           (method)weaklynonlinear_new,
                           (method)dsp_free,
                           (long)sizeof(t_weaklynonlinear),
                           0,
                           NULL,
                           0);
    
    class_addmethod(c, (method)weaklynonlinear_dsp64, "dsp64", A_CANT, 0);
    class_addmethod(c, (method)weaklynonlinear_assist, "assist", A_CANT, 0);
    
    CLASS_ATTR_LONG  (c, "reset",       ATTR_FLAGS_NONE, t_weaklynonlinear, reset);
    CLASS_ATTR_DOUBLE(c, "ratex",       ATTR_FLAGS_NONE, t_weaklynonlinear, ratex);
    CLASS_ATTR_DOUBLE(c, "ratey",       ATTR_FLAGS_NONE, t_weaklynonlinear, ratey);
    CLASS_ATTR_DOUBLE(c, "freq",        ATTR_FLAGS_NONE, t_weaklynonlinear, freq);
    CLASS_ATTR_DOUBLE(c, "initx",       ATTR_FLAGS_NONE, t_weaklynonlinear, initx);
    CLASS_ATTR_DOUBLE(c, "inity",       ATTR_FLAGS_NONE, t_weaklynonlinear, inity);
    CLASS_ATTR_DOUBLE(c, "alpha",       ATTR_FLAGS_NONE, t_weaklynonlinear, alpha);
    CLASS_ATTR_DOUBLE(c, "beta",        ATTR_FLAGS_NONE, t_weaklynonlinear, beta);
    CLASS_ATTR_DOUBLE(c, "xexponent",   ATTR_FLAGS_NONE, t_weaklynonlinear, xexponent);
    CLASS_ATTR_DOUBLE(c, "yexponent",   ATTR_FLAGS_NONE, t_weaklynonlinear, yexponent);
    
    class_dspinit(c);
    class_register(CLASS_BOX, c);
    
    weaklynonlinear_class = c;
}
