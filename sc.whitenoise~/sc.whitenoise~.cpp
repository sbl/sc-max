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

 sc.whitenoise~
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

typedef struct _whitenoise 
{
	t_pxobject					ob;
    RGen                        rgen;
} t_whitenoise;

t_class *whitenoise_class;

//////////////////////////////////////////////////////////////////////////////////////////////////////

void *whitenoise_new();
void whitenoise_free(t_whitenoise *x);
void whitenoise_assist(t_whitenoise *x, void *b, long m, long a, char *s);

void whitenoise_dsp(t_whitenoise *x, t_signal **sp, short *count);
t_int *whitenoise_perform(t_int *w);

void whitenoise_dsp64	(t_whitenoise *x, t_object *dsp64, short *count, double samplerate,
						 long maxvectorsize, long flags);
void whitenoise_perform64(t_whitenoise*x, t_object *dsp64, double **ins, long numins,
							double **outs, long numouts, long sampleframes, long flags, void *userparam);

//////////////////////////////////////////////////////////////////////////////////////////////////////

int C74_EXPORT main(void){
	t_class *c;
    
	c = class_new("sc.whitenoise~", (method)whitenoise_new, (method)dsp_free, (long)sizeof(t_whitenoise), 0L, NULL, 0);
	
	class_addmethod(c, (method)whitenoise_dsp,		"dsp",		A_CANT, 0);
	class_addmethod(c, (method)whitenoise_dsp64,		"dsp64",		A_CANT, 0);
	class_addmethod(c, (method)whitenoise_assist,    "assist",	A_CANT, 0);
    
	class_dspinit(c);				
	class_register(CLASS_BOX, c);
	whitenoise_class = c;
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void whitenoise_dsp(t_whitenoise *x, t_signal **sp, short *count){
	dsp_add(whitenoise_perform, 3, x, sp[0]->s_vec, sp[0]->s_n);
}

t_int *whitenoise_perform(t_int *w){
    t_whitenoise *x = (t_whitenoise *)(w[1]);	
    t_float *out = (t_float *)(w[2]);
	int n = (int)w[3];
    
    if (x->ob.z_disabled)
        return w + 4;    
    
    RGET
    
	while (n--){
		*out++ = frand2(s1, s2, s3);
    }
    
    RPUT
    
    
	return w + 4;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// 64bit dsp
void whitenoise_dsp64	(t_whitenoise *x, t_object *dsp64, short *count, double samplerate,
						 long maxvectorsize, long flags) {
	object_method(dsp64, gensym("dsp_add64"), x, whitenoise_perform64, 0, NULL);
}


void whitenoise_perform64(t_whitenoise*x, t_object *dsp64, double **ins, long numins,
						  double **outs, long numouts, long sampleframes, long flags, void *userparam) {
	
    t_double *out = outs[0];
	int n = sampleframes;
	//RGen rgen = x->rgen;
    
    if (x->ob.z_disabled)
        return;
    
    //RGET
    
	while (n--){
		//*out++ = frand2(s1, s2, s3);
		*out++ = 2*x->rgen.drand() - 1;
    }
    
    //RPUT
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void whitenoise_assist(t_whitenoise *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_INLET) { //inlet
		sprintf(s, "Ignore this inlet");
	} 
	else {	// outlet
		sprintf(s, "(signal) White Noise"); 			
	}
}

void *whitenoise_new(){
	t_whitenoise *x = NULL;
	x = (t_whitenoise *)object_alloc(whitenoise_class);
    
	if (x) {
		dsp_setup((t_pxobject *)x, 0);
        
        outlet_new((t_object *)x, "signal");
        
        x->rgen.init(sc_randomSeed());
		
	}
	else {
		object_free(x);
		x = NULL;
	}
	
	return (x);
}
