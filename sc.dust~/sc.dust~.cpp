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
 sc.dust~
 (c) stephen lumenta under GPL
 http://www.gnu.org/licenses/gpl.html
 
 part of sc-max http://github.com/sbl/sc-max
 see README
*/

#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"

#include "SC_RGen.h"

#define RGET RGen& rgen = x->rgen; uint32 s1 = rgen.s1; uint32 s2 = rgen.s2; uint32 s3 = rgen.s3;
#define RPUT rgen.s1 = s1; rgen.s2 = s2; rgen.s3 = s3;

//////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct _dust 
{
	t_pxobject		m_ob;
    short           m_connected;
    
    float           m_density;
    float           m_thresh;
    float           m_scale;
    float           m_sr;
    float           m_isr;
    RGen            rgen;
} t_dust;

t_class *dust_class;

//////////////////////////////////////////////////////////////////////////////////////////////////////

void *dust_new(double density);
void dust_free(t_dust *x);
void dust_assist(t_dust *x, void *b, long m, long a, char *s);
void dust_float(t_dust *x, double density);
void dust_int(t_dust *x, long density);
void dust_calc_density(t_dust *x);

void dust_dsp(t_dust *x, t_signal **sp, short *count);
t_int *dust_perform(t_int *w);

//////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void){	
	t_class *c;
    
	c = class_new("sc.dust~", (method)dust_new, (method)dsp_free, (long)sizeof(t_dust), 0L, A_DEFFLOAT, 0);
	
	class_addmethod(c, (method)dust_dsp,		"dsp",		A_CANT, 0);
	class_addmethod(c, (method)dust_assist,     "assist",	A_CANT, 0);
    class_addmethod(c, (method)dust_float,      "float",    A_FLOAT, 0);
    class_addmethod(c, (method)dust_int,        "int",      A_LONG, 0);
    
	class_dspinit(c);				
	class_register(CLASS_BOX, c);
	dust_class = c;
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////


void dust_float(t_dust *x, double density){
    x->m_density = density;
    dust_calc_density(x);
}

void dust_int(t_dust *x, long density){
    dust_float(x, (double) density);
}


void dust_calc_density(t_dust *x){
    x->m_thresh = x->m_density * x->m_isr;
    x->m_scale  = x->m_thresh > 0.f ? 1.f / x->m_thresh : 0.f;
}

void dust_dsp(t_dust *x, t_signal **sp, short *count){
    x->m_connected = count[0];
    // t_dust, in_density, out_density, n
	dsp_add(dust_perform, 4, x, sp[0]->s_vec, sp[1]->s_vec, sp[1]->s_n);
}

t_int *dust_perform(t_int *w){
    t_dust *x = (t_dust *)(w[1]);
    t_float density = x->m_connected ? (*(t_float *)(w[2])) : x->m_density;
    t_float *out = (t_float *)(w[3]);
	int n = (int)w[4];
    
    if (x->m_ob.z_disabled)
        return w + 5;
    
    if (density != x->m_density) {
        x->m_density = density;
        dust_calc_density(x);
    }
    
    RGET
        
	while (n--){
        float z = frand(s1, s2, s3);
        
        if (z < x->m_thresh) *out = z * x->m_scale;
		else  *out = 0.f;

		*out++;
    }
    
    RPUT
        
	return w + 5;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void dust_assist(t_dust *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_INLET) { //inlet
		sprintf(s, "(signal/float) density");
	} 
	else {	// outlet
		sprintf(s, "(signal) Dust"); 			
	}
}

void *dust_new(double density){
	t_dust *x = NULL;
	x = (t_dust *)object_alloc(dust_class);
    
	if (x) {
        dsp_setup((t_pxobject *)x, 1);
        outlet_new((t_object *)x, "signal");

        x->rgen.init(time(NULL));
        x->m_density = density;
        
        x->m_sr = sys_getsr();
        x->m_isr = 1/x->m_sr;
        x->m_scale = 0.;
        x->m_thresh = 0.;
        
        dust_calc_density(x);
	}
	return (x);
}
