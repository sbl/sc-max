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

 sc.lfnoise0~
 (c) stephen lumenta under GPL
 http://www.gnu.org/licenses/gpl.html
 
 part of sc-max http://github.com/sbl/sc-max
 see README
 
 *
 **
 ***		64bit update by vb, august 2016 -- http://vboehm.net
*/


#include "ext.h"
#include "ext_obex.h"
#include "ext_common.h"
#include "z_dsp.h"
#include "scmax.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct _lfnoise 
{
	t_pxobject      ob;
    
    double			m_freq;
    short			m_connected;
    double			m_level;
    double			m_sr;
    long				m_counter;
    
    RGen			rgen;
} t_lfnoise;

t_class *lfnoise_class;

//////////////////////////////////////////////////////////////////////////////////////////////////////

void *lfnoise_new       (double freq);
void lfnoise_free       (t_lfnoise *x);
void lfnoise_assist     (t_lfnoise *x, void *b, long m, long a, char *s);

void lfnoise_float      (t_lfnoise *x, double freq);
void lfnoise_bang		(t_lfnoise *x);

void lfnoise_dsp        (t_lfnoise *x, t_signal **sp, short *count);
t_int *lfnoise_perform  (t_int *w);

void lfnoise_dsp64	(t_lfnoise *x, t_object *dsp64, short *count, double samplerate,
						 long maxvectorsize, long flags);
void lfnoise_perform64(t_lfnoise *x, t_object *dsp64, double **ins, long numins,
						   double **outs, long numouts, long sampleframes, long flags, void *userparam);

//////////////////////////////////////////////////////////////////////////////////////////////////////

int C74_EXPORT main(void){
	t_class *c;
    
	c = class_new("sc.lfnoise0~", (method)lfnoise_new, (method)dsp_free, (long)sizeof(t_lfnoise), 0L, A_DEFFLOAT, 0);
	
	class_addmethod(c, (method)lfnoise_dsp,         "dsp",      A_CANT, 0);
	class_addmethod(c, (method)lfnoise_dsp64,		"dsp64",      A_CANT, 0);
	class_addmethod(c, (method)lfnoise_assist,      "assist",	A_CANT, 0);
    class_addmethod(c, (method)lfnoise_float,       "float",	A_FLOAT, 0);
	class_addmethod(c, (method)lfnoise_bang,       "bang", 0);
    
	class_dspinit(c);				
	class_register(CLASS_BOX, c);
	lfnoise_class = c;
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void lfnoise_float(t_lfnoise *x, double freq){    
    x->m_freq = freq;
}

void lfnoise_bang	(t_lfnoise *x) {
	x->m_counter = 0;
}

void lfnoise_dsp(t_lfnoise *x, t_signal **sp, short *count){
    x->m_sr         = sp[0]->s_sr;
    x->m_connected  = count[0];
    // class, in, out, n
	dsp_add(lfnoise_perform, 4, x, sp[0]->s_vec, sp[1]->s_vec, sp[1]->s_n);
}

t_int *lfnoise_perform(t_int *w){
    t_lfnoise   *x      = (t_lfnoise *) w[1];	
    t_float         *out    = (t_float *)       w[3];
	int             remain  = (int)  w[4];
    
    t_float         freq    = x->m_connected ? (*(t_float *)(w[2])) : x->m_freq;
    float           level   = x->m_level;
	long            counter = x->m_counter;
    
    
    if (x->ob.z_disabled) return w + 5;
    
    
    
    RGET
    
	do {
		if (counter<=0) {
			counter = (int)(x->m_sr / sc_max(freq, .001f));
			counter = sc_max(1, counter);
			level = frand2(s1,s2,s3);
		}
		int nsmps = sc_min(remain, counter);
		remain -= nsmps;
		counter -= nsmps;
        
        while(nsmps--){
            *out++ = level;
        }
	} while (remain);
    
    RPUT
    
    x->m_level = level;
	x->m_counter = counter;
    
	return w + 5;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
// 64bit dsp
void lfnoise_dsp64	(t_lfnoise *x, t_object *dsp64, short *count, double samplerate,
					 long maxvectorsize, long flags) {
	x->m_sr         = samplerate;
    x->m_connected  = count[0];
	object_method(dsp64, gensym("dsp_add64"), x, lfnoise_perform64, 0, NULL);
}


void lfnoise_perform64(t_lfnoise *x, t_object *dsp64, double **ins, long numins,
					   double **outs, long numouts, long sampleframes, long flags, void *userparam) {
	
    t_double		*out = outs[0];
	int				remain  = sampleframes;
    
    t_double		freq    = x->m_connected ? (*(t_double *)(ins[0])) : x->m_freq;
    double			level   = x->m_level;
	long				counter = x->m_counter;
    
    
    if (x->ob.z_disabled)
		return;
    
    RGET
    
	do {
		if (counter<=0) {
			counter = (int)(x->m_sr / MAX(freq, .001));
			counter = MAX(1, counter);
			level = frand2(s1,s2,s3);
		}
		int nsmps = MIN(remain, counter);
		remain -= nsmps;
		counter -= nsmps;
        
        while(nsmps--){
            *out++ = level;
        }
	} while (remain);
    
    RPUT
    
    x->m_level = level;
	x->m_counter = counter;
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
        
        x->m_freq       = freq <= 0 ? 500 : freq;
        x->m_counter    = 0;
        x->m_level      = 0.0;
        x->m_sr         = sys_getsr();
        
        
        x->rgen.init(sc_randomSeed());
        
        outlet_new((t_object *)x, "signal");
	}
	else {
		object_free(x);
		x = NULL;
	}
	
	return (x);
}
