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

#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"
#include "scmax.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct _lfclipnoise 
{
	t_pxobject      ob;
    
    float           m_freq;
    short           m_connected;
    float           m_level;
    float           m_sr;
    int             m_counter;
    
    RGen            rgen;
} t_lfclipnoise;

t_class *lfclipnoise_class;

//////////////////////////////////////////////////////////////////////////////////////////////////////

void *lfclipnoise_new       (double freq);
void lfclipnoise_free       (t_lfclipnoise *x);
void lfclipnoise_assist     (t_lfclipnoise *x, void *b, long m, long a, char *s);

void lfclipnoise_float      (t_lfclipnoise *x, double freq);

void lfclipnoise_dsp        (t_lfclipnoise *x, t_signal **sp, short *count);
t_int *lfclipnoise_perform  (t_int *w);

//////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void){	
	t_class *c;
    
	c = class_new("sc.lfclipnoise~", (method)lfclipnoise_new, (method)dsp_free, (long)sizeof(t_lfclipnoise), 0L, A_DEFFLOAT, 0);
	
	class_addmethod(c, (method)lfclipnoise_dsp,         "dsp",      A_CANT, 0);
	class_addmethod(c, (method)lfclipnoise_assist,      "assist",	A_CANT, 0);
    class_addmethod(c, (method)lfclipnoise_float,       "float",	A_FLOAT, 0);
    
	class_dspinit(c);				
	class_register(CLASS_BOX, c);
	lfclipnoise_class = c;
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void lfclipnoise_float(t_lfclipnoise *x, double freq){    
    x->m_freq = (float) freq;
}

void lfclipnoise_dsp(t_lfclipnoise *x, t_signal **sp, short *count){
    x->m_sr         = sp[0]->s_sr;
    x->m_connected  = count[0];
    // class, in, out, n
	dsp_add(lfclipnoise_perform, 4, x, sp[0]->s_vec, sp[1]->s_vec, sp[1]->s_n);
}

t_int *lfclipnoise_perform(t_int *w){
    t_lfclipnoise   *x      = (t_lfclipnoise *) w[1];	
    t_float         *out    = (t_float *)       w[3];
	int             remain  = (int)  w[4];
    
    t_float         freq    = x->m_connected ? (*(t_float *)(w[2])) : x->m_freq;
    float           level   = x->m_level;
	int32           counter = x->m_counter;
    
    
    if (x->ob.z_disabled) return w + 5;
    
    
    
    RGET
    
	do {
		if (counter<=0) {
            // otherwise not working
            if(freq < 0.0001) freq = 0.0001;
            
			counter = (int) x->m_sr / sc_max(freq, 0.001f);
			counter = sc_max(1, counter);
			level = fcoin(s1,s2,s3);
		}
		int nsmps = sc_min(remain, counter);
		remain -= nsmps;
		counter -= nsmps;
        
        int i;
        for(i=0; i<nsmps;i++){
            *out++ = level;
        }
	} while (remain);
    
    RPUT
    
    x->m_level = level;
	x->m_counter = counter;
    
	return w + 5;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void lfclipnoise_assist(t_lfclipnoise *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_INLET) { //inlet
		sprintf(s, "(signal/float) set freq");
	} 
	else {	// outlet
		sprintf(s, "(signal) lfclipnoise"); 			
	}
}

void *lfclipnoise_new(double freq){
	t_lfclipnoise *x = NULL;
	x = (t_lfclipnoise *)object_alloc(lfclipnoise_class);
    
	if (x) {
        // 1 inlet
		dsp_setup((t_pxobject *)x, 1);
        
        x->m_freq       = freq <= 0 ? 500 : freq;
        x->m_counter    = 0;
        x->m_level      = 0.0;
        x->m_sr         = sys_getsr();
        x->rgen.init(sc_randomSeed());
        
        outlet_new((t_object *)x, "signal");
	}
	return (x);
}
