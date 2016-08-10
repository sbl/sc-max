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

 graynoise~
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
#include "z_dsp.h"
#include "scmax.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct _graynoise 
{
	t_pxobject					ob;
    int                         m_counter;
    RGen                        rgen;
} t_graynoise;

t_class *graynoise_class;

//////////////////////////////////////////////////////////////////////////////////////////////////////

void *graynoise_new(long argc, t_atom *argv);
void graynoise_free(t_graynoise *x);
void graynoise_assist(t_graynoise *x, void *b, long m, long a, char *s);

void graynoise_dsp(t_graynoise *x, t_signal **sp, short *count);
t_int *graynoise_perform(t_int *w);

void graynoise_dsp64(t_graynoise *x, t_object *dsp64, short *count, double samplerate,
				 long maxvectorsize, long flags);
void graynoise_perform64(t_graynoise *x, t_object *dsp64, double **ins, long numins,
					 double **outs, long numouts, long sampleframes, long flags, void *userparam);

//////////////////////////////////////////////////////////////////////////////////////////////////////

int C74_EXPORT main(void){
	t_class *c;
    
	c = class_new("sc.graynoise~", (method)graynoise_new, (method)dsp_free, (long)sizeof(t_graynoise), 0L, A_GIMME, 0);
	
	class_addmethod(c, (method)graynoise_dsp,		"dsp",		A_CANT, 0);
	class_addmethod(c, (method)graynoise_dsp64,		"dsp64",		A_CANT, 0);
	class_addmethod(c, (method)graynoise_assist,    "assist",	A_CANT, 0);
    
	class_dspinit(c);				
	class_register(CLASS_BOX, c);
	graynoise_class = c;
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void graynoise_dsp(t_graynoise *x, t_signal **sp, short *count){
	dsp_add(graynoise_perform, 3, x, sp[0]->s_vec, sp[0]->s_n);
}

t_int *graynoise_perform(t_int *w){
    t_graynoise *x = (t_graynoise *)(w[1]);
    int counter = x->m_counter;
	
    t_float *out = (t_float *)(w[2]);
	int n = (int)w[3];
    
    if (x->ob.z_disabled)
        return w + 4;    
    
    RGET
    
	while (n--){
        counter ^= 1L << (trand(s1,s2,s3) & 31);
		*out++ = counter * 4.65661287308e-10f;
    }
    
    RPUT
    
    x->m_counter = counter;
    
	return w + 4;
}

// 64bit dsp routine
void graynoise_dsp64(t_graynoise *x, t_object *dsp64, short *count, double samplerate,
					 long maxvectorsize, long flags) {
	object_method(dsp64, gensym("dsp_add64"), x, graynoise_perform64, 0, NULL);
}


void graynoise_perform64(t_graynoise*x, t_object *dsp64, double **ins, long numins,
					 double **outs, long numouts, long sampleframes, long flags, void *userparam) {

    t_double *out = outs[0];
	int n = sampleframes;
	
	 int counter = x->m_counter;
    
    if (x->ob.z_disabled)
        return;
    
    RGET
    
	while (n--){
        counter ^= 1L << (trand(s1,s2,s3) & 31);
		*out++ = counter * 4.65661287308e-10;
    }
    
    RPUT
    
    x->m_counter = counter;

}


//////////////////////////////////////////////////////////////////////////////////////////////////////

void graynoise_assist(t_graynoise *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_INLET) { //inlet
		sprintf(s, "Ignore this inlet");
	} 
	else {	// outlet
		sprintf(s, "(signal) Gray Noise"); 			
	}
}

void *graynoise_new(long argc, t_atom *argv){
	t_graynoise *x = NULL;
	x = (t_graynoise *)object_alloc(graynoise_class);
    
	if (x) {
		dsp_setup((t_pxobject *)x, 0);
        
        outlet_new((t_object *)x, "signal");
        
        x->rgen.init(sc_randomSeed());
        x->m_counter = 0;
	}
	else {
		object_free(x);
		x = NULL;
	}
	
	return (x);
}
