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

 sc.clipnoise~
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

typedef struct _clipnoise 
{
	t_pxobject					ob;
    RGen                        rgen;
} t_clipnoise;

t_class *clipnoise_class;

//////////////////////////////////////////////////////////////////////////////////////////////////////

void *clipnoise_new(long argc, t_atom *argv);
void clipnoise_free(t_clipnoise *x);
void clipnoise_assist(t_clipnoise *x, void *b, long m, long a, char *s);

void clipnoise_dsp(t_clipnoise *x, t_signal **sp, short *count);
t_int *clipnoise_perform(t_int *w);

//////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void){	
	t_class *c;
    
	c = class_new("sc.clipnoise~", (method)clipnoise_new, (method)dsp_free, (long)sizeof(t_clipnoise), 0L, A_GIMME, 0);
	
	class_addmethod(c, (method)clipnoise_dsp,		"dsp",		A_CANT, 0);
	class_addmethod(c, (method)clipnoise_assist,    "assist",	A_CANT, 0);
    
	class_dspinit(c);				
	class_register(CLASS_BOX, c);
	clipnoise_class = c;
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void clipnoise_dsp(t_clipnoise *x, t_signal **sp, short *count){
	dsp_add(clipnoise_perform, 3, x, sp[0]->s_vec, sp[0]->s_n);
}

t_int *clipnoise_perform(t_int *w){
    t_clipnoise *x = (t_clipnoise *)(w[1]);	
    t_float *out = (t_float *)(w[2]);
	int n = (int)w[3];
    
    if (x->ob.z_disabled)
        return w + 4;    
    
    RGET
    
	while (n--){
		*out++ = fcoin(s1, s2, s3);
    }
    
    RPUT
    
	return w + 4;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void clipnoise_assist(t_clipnoise *x, void *b, long m, long a, char *s){
	if (m == ASSIST_INLET) { //inlet
		sprintf(s, "Ignore this inlet");
	} 
	else {	// outlet
		sprintf(s, "(signal) Clip Noise"); 			
	}
}

void *clipnoise_new(long argc, t_atom *argv){
	t_clipnoise *x = NULL;
	x = (t_clipnoise *)object_alloc(clipnoise_class);
    
	if (x) {
		dsp_setup((t_pxobject *)x, 0);
        
        outlet_new((t_object *)x, "signal");
        
        x->rgen.init(sc_randomSeed());
	}
	return (x);
}
