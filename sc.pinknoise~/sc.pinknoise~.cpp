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

 sc.pinknoise~
 
 (c) stephen lumenta under GPL
 http://www.gnu.org/licenses/gpl.html
 
 part of sc-max http://github.com/sbl/sc-max
 see README
*/

#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"
#include "clz.h"
#include "rgen_helper.h"

#define N_DICE 16

//////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct _pinknoise {
	t_pxobject					ob;
    uint32                      m_dice[N_DICE];
    int32                       m_total;
    RGen                        rgen;
} t_pinknoise;

t_class *pinknoise_class;

//////////////////////////////////////////////////////////////////////////////////////////////////////

void *pinknoise_new(long argc, t_atom *argv);
void pinknoise_free(t_pinknoise *x);
void pinknoise_assist(t_pinknoise *x, void *b, long m, long a, char *s);

void pinknoise_dsp(t_pinknoise *x, t_signal **sp, short *count);
t_int *pinknoise_perform(t_int *w);

//////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void){	
	t_class *c;
    
	c = class_new("sc.pinknoise~", (method)pinknoise_new, (method)dsp_free, (long)sizeof(t_pinknoise), 0L, A_GIMME, 0);
	
	class_addmethod(c, (method)pinknoise_dsp,		"dsp",		A_CANT, 0);
	class_addmethod(c, (method)pinknoise_assist,    "assist",	A_CANT, 0);
    
	class_dspinit(c);				
	class_register(CLASS_BOX, c);
	pinknoise_class = c;
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void pinknoise_dsp(t_pinknoise *x, t_signal **sp, short *count){
	dsp_add(pinknoise_perform, 3, x, sp[0]->s_vec, sp[0]->s_n);
}

t_int *pinknoise_perform(t_int *w){
    t_pinknoise *x = (t_pinknoise *)(w[1]);	
    t_float *out = (t_float *)(w[2]);
	int n = (int)w[3];
    
    if (x->ob.z_disabled)
        return w + 4;    
    
    RGET
    
    uint32 total = x->m_total;
	uint32 *dice = x->m_dice;
    
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
    
    x->m_total = total;
    
    RPUT
    
	return w + 4;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void pinknoise_assist(t_pinknoise *x, void *b, long m, long a, char *s){
	if (m == ASSIST_INLET) { //inlet
		sprintf(s, "Ignore this inlet");
	} 
	else {	// outlet
		sprintf(s, "(signal) Pink Noise"); 			
	}
}

void *pinknoise_new(long argc, t_atom *argv){
	t_pinknoise *x = NULL;
	x = (t_pinknoise *)object_alloc(pinknoise_class);
    
	if (x) {
		dsp_setup((t_pxobject *)x, 0);
        
        outlet_new((t_object *)x, "signal");
        x->rgen.init(time(NULL));
        
        // set up 
        RGET
        
        uint32 *dice = x->m_dice;
        int32 total = 0;
        for (int i=0; i<N_DICE; ++i) {
            uint32 newrand = trand(s1,s2,s3) >> 13;
            total += newrand;
            dice[i] = newrand;
        }
        x->m_total = total;
        
        RPUT
	}
	return (x);
}
