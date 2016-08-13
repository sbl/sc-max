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

 sc.lfnoise2~
 (c) stephen lumenta under GPL
 http://www.gnu.org/licenses/gpl.html
 
 part of sc-max http://github.com/sbl/sc-max
 see README
*/

#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"
#include "scmax.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct _lfnoise 
{
	t_pxobject      ob;
    
    float           m_freq;
    short           m_connected;
    float           m_level, m_slope, m_curve;
    float           m_nextvalue, m_nextmidpt;
    float           m_sr;
    int             m_counter;
    
    RGen            rgen;
} t_lfnoise;

t_class *lfnoise_class;

//////////////////////////////////////////////////////////////////////////////////////////////////////

void *lfnoise_new       (double freq);
void lfnoise_free       (t_lfnoise *x);
void lfnoise_assist     (t_lfnoise *x, void *b, long m, long a, char *s);

void lfnoise_float      (t_lfnoise *x, double freq);

void lfnoise_dsp        (t_lfnoise *x, t_signal **sp, short *count);
t_int *lfnoise_perform  (t_int *w);

//////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void){	
	t_class *c;
    
	c = class_new("sc.lfnoise2~", (method)lfnoise_new, (method)dsp_free, (long)sizeof(t_lfnoise), 0L, A_DEFFLOAT, 0);
	
	class_addmethod(c, (method)lfnoise_dsp,         "dsp",      A_CANT, 0);
	class_addmethod(c, (method)lfnoise_assist,      "assist",	A_CANT, 0);
    class_addmethod(c, (method)lfnoise_float,       "float",	A_FLOAT, 0);
    
	class_dspinit(c);				
	class_register(CLASS_BOX, c);
	lfnoise_class = c;
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void lfnoise_float(t_lfnoise *x, double freq){
    
    x->m_freq = (float) freq;
}

void lfnoise_dsp(t_lfnoise *x, t_signal **sp, short *count){
    x->m_sr         = sp[0]->s_sr;
    x->m_connected  = count[0];
    // class, in, out, n
	dsp_add(lfnoise_perform, 4, x, sp[0]->s_vec, sp[1]->s_vec, sp[1]->s_n);
}

t_int *lfnoise_perform(t_int *w){
    t_lfnoise       *x      = (t_lfnoise *) w[1];	
    t_float         *out    = (t_float *)       w[3];
	int             remain  = (int)  w[4];
    
    t_float         freq    = x->m_connected ? (*(t_float *)(w[2])) : x->m_freq;
    float           level   = x->m_level;
	float           slope   = x->m_slope;
    float           curve   = x->m_curve;
    int32            counter = x->m_counter;
    
    
    if (x->ob.z_disabled) return w + 5;
    
	
	RGET
    
	do {
		if (counter<=0) {
			float value = x->m_nextvalue;
			x->m_nextvalue = frand2(s1,s2,s3);
			level = x->m_nextmidpt;
			x->m_nextmidpt = (x->m_nextvalue + value) * 0.5;
            
			counter = (int32)(x->m_sr / sc_max(freq, 0.001f));
			counter = sc_max(2, counter);
			float fseglen = (float)counter;
			curve = 2.f * (x->m_nextmidpt - level - fseglen * slope) / (fseglen * fseglen + fseglen);
		}
        
		int nsmps = sc_min(remain, counter);
		remain -= nsmps;
		counter -= nsmps;
        
        while (nsmps--) {
            *out++ = level;
            slope += curve;
			level += slope;
        }

	} while (remain);

	x->m_level  = level;
	x->m_slope  = slope;
    x->m_curve  = curve;
	x->m_counter= counter;
	
    RPUT
    
	return w + 5;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void lfnoise_assist(t_lfnoise *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_INLET) { //inlet
		sprintf(s, "(signal/float) set freq");
	} 
	else {	// outlet
		sprintf(s, "(signal) lfnoise"); 			
	}
}

void *lfnoise_new(double freq){
	t_lfnoise *x = NULL;
	x = (t_lfnoise *)object_alloc(lfnoise_class);
    
	if (x) {
        // 1 inlet
		dsp_setup((t_pxobject *)x, 1);
        
        x->rgen.init(sc_randomSeed());
        
        x->m_freq           = freq <= 0 ? 500 : freq;
        x->m_counter        = 0;
        x->m_slope          = 0.f;
        x->m_level          = 0.f;
        x->m_nextvalue      = x->rgen.frand2();
        x->m_nextmidpt      = x->m_nextvalue * .5f;
        x->m_sr             = sys_getsr();
        
        
        outlet_new((t_object *)x, "signal");
	}
	return (x);
}
