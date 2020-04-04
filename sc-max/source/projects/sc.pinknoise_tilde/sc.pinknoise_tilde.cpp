/*
 euperCollider real time audio synthesis system
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
 sc.pinknoise~

 (c) stephen lumenta under GPL
 http://www.gnu.org/licenses/gpl.html

 part of sc-max http://github.com/sbl/sc-max
 see README
 */

#include "c74_msp.h"
#include "scmax.h"
#include "clz.h"

#define N_DICE 16

using namespace c74::max;

static t_class *pinknoise_class = nullptr;

struct t_pinknoise {
    t_pxobject ob;
    uint32 m_dice[N_DICE];
    int32 m_total;
    RGen rgen;
};

void *pinknoise_new(long argc, t_atom *argv) {
    t_pinknoise *self = (t_pinknoise *)object_alloc(pinknoise_class);
    
    if (!self) return self;
        
    dsp_setup((t_pxobject *)self, 0);
    
    outlet_new((t_object *)self, "signal");
    self->rgen.init(sc_randomSeed());
    
    RGET
    uint32 *dice = self->m_dice;
    int32 total = 0;
    for (int i=0; i < N_DICE; ++i) {
        uint32 newrand = trand(s1,s2,s3) >> 13;
        total += newrand;
        dice[i] = newrand;
    }
    self->m_total = total;
    RPUT
    
    return self;
}

void pinknoise_perform64(t_pinknoise* self,
                         t_object* dsp64,
                         double** ins,
                         long numins,
                         double** outs,
                         long numouts,
                         long sampleframes,
                         long flags,
                         void* userparam) {
    double *out = outs[0];
    int n = sampleframes;

    if (self->ob.z_disabled) return;

    RGET
    uint32 total = self->m_total;
    uint32 *dice = self->m_dice;

    while (n--){
        uint32 counter = trand(s1,s2,s3); // Magnus Jonsson's suggestion.
        uint32 newrand = counter >> 13;
        int k = (CTZ(counter)) & 15;
        uint32 prevrand = dice[k];
        dice[k] = newrand;
        total += (newrand - prevrand);
        newrand = trand(s1,s2,s3) >> 13;
        elem32 val; // ensure write before read <sk>
        val.u = (total + newrand) | 0x40000000;
        *out++ = val.f - 3.0f;
        counter ++;
    }

    self->m_total = total;
    RPUT
}

void pinknoise_dsp64(t_pinknoise *self, t_object* dsp64, short* count, double samplerate, long maxvectorsize, long flags) {
    object_method_direct(void, (t_object*, t_object*, t_perfroutine64, long, void*),
                         dsp64, gensym("dsp_add64"), (t_object*)self, (t_perfroutine64)pinknoise_perform64, 0, NULL);
}

void pinknoise_assist(t_pinknoise *self, void *b, t_assist_function io, long a, char *s) {
    if (io == ASSIST_INLET) {
        sprintf(s, "Ignore this inlet");
    }
    else {
        sprintf(s, "(signal) Pink Noise");
    }
}

void ext_main(void *r) {
    t_class *c;
    c = class_new("sc.pinknoise~", (method)pinknoise_new, (method)dsp_free, (long)sizeof(t_pinknoise), 0L, A_GIMME, 0);
    
    class_addmethod(c, (method)pinknoise_dsp64, "dsp64", A_CANT, 0);
    class_addmethod(c, (method)pinknoise_assist, "assist", A_CANT, 0);
    
    class_dspinit(c);
    class_register(CLASS_BOX, c);
    pinknoise_class = c;
}
