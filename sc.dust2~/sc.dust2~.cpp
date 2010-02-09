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

#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"
#include "rgen_helper.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct _dust2 
{
	t_pxobject		m_ob;
    float           m_density;
    float           m_thresh;
    float           m_scale;
    float           m_sr;
    float           m_isr;
    RGen            rgen;
} t_dust2;

t_class *dust2_class;

//////////////////////////////////////////////////////////////////////////////////////////////////////

void *dust2_new(double density);
void dust2_free(t_dust2 *x);
void dust2_assist(t_dust2 *x, void *b, long m, long a, char *s);
void dust2_float(t_dust2 *x, double density);
void dust2_int(t_dust2 *x, long density);
void dust2_calc_density(t_dust2 *x);

void dust2_dsp(t_dust2 *x, t_signal **sp, short *count);
t_int *dust2_perform(t_int *w);

//////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void){	
	t_class *c;
    
	c = class_new("sc.dust2~", (method)dust2_new, (method)dsp_free, (long)sizeof(t_dust2), 0L, A_DEFFLOAT, 0);
	
	class_addmethod(c, (method)dust2_dsp,		"dsp",		A_CANT, 0);
	class_addmethod(c, (method)dust2_assist,     "assist",	A_CANT, 0);
    class_addmethod(c, (method)dust2_float,      "float",    A_FLOAT, 0);
    class_addmethod(c, (method)dust2_int,        "int",      A_LONG, 0);
    
	class_dspinit(c);				
	class_register(CLASS_BOX, c);
	dust2_class = c;
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////


void dust2_float(t_dust2 *x, double density){
    x->m_density = density;
    dust2_calc_density(x);
}

void dust2_int(t_dust2 *x, long density){
    x->m_density = (float) density;
    dust2_calc_density(x);
}

void dust2_dsp(t_dust2 *x, t_signal **sp, short *count){
    x->m_sr = sp[0]->s_sr;
    x->m_isr = 1.0f / x->m_sr;
    
    dust2_calc_density(x);
	dsp_add(dust2_perform, 3, x, sp[0]->s_vec, sp[0]->s_n);
}

void dust2_calc_density(t_dust2 *x){
    x->m_thresh = x->m_density * x->m_isr;
    x->m_scale  = x->m_thresh > 0.f ? 2.f / x->m_thresh : 0.f;
}

t_int *dust2_perform(t_int *w){
    t_dust2 *x = (t_dust2 *)(w[1]);
    t_float *out = (t_float *)(w[2]);
	int n = (int)w[3];
    
    if (x->m_ob.z_disabled)
        return w + 4;
    
    RGET
        
	while (n--){
        float z = frand(s1, s2, s3);
        
        if (z < x->m_thresh) *out = z * x->m_scale - 1.f;
		else  *out = 0.f;

		*out++;
    }
    
    RPUT
        
	return w + 4;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void dust2_assist(t_dust2 *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_INLET) { //inlet
		sprintf(s, "(int/float) density");
	} 
	else {	// outlet
		sprintf(s, "(signal) dust2"); 			
	}
}

void *dust2_new(double density){
	t_dust2 *x = NULL;
	x = (t_dust2 *)object_alloc(dust2_class);
    
	if (x) {
        dsp_setup((t_pxobject *)x, 0);
        outlet_new((t_object *)x, "signal");

        x->m_density = density;
        
        // all calculation have to be done after a dsp chain rebuild
        x->m_sr = 44100.;
        x->m_isr = 1/x->m_sr;
        x->m_scale = 0.;
        x->m_thresh = 0.;

        x->rgen.init(time(NULL));
	}
	return (x);
}
