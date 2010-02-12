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

 sc.brownnoise~
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

typedef struct _brownnoise 
{
	t_pxobject		ob;
    float           m_level;    
    RGen            rgen;
} t_brownnoise;

t_class *brownnoise_class;

//////////////////////////////////////////////////////////////////////////////////////////////////////

void *brownnoise_new(long argc, t_atom *argv);
void brownnoise_free(t_brownnoise *x);
void brownnoise_assist(t_brownnoise *x, void *b, long m, long a, char *s);

void brownnoise_dsp(t_brownnoise *x, t_signal **sp, short *count);
t_int *brownnoise_perform(t_int *w);

//////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void){	
	t_class *c;
    
	c = class_new("sc.brownnoise~", (method)brownnoise_new, (method)dsp_free, (long)sizeof(t_brownnoise), 0L, A_GIMME, 0);
	
	class_addmethod(c, (method)brownnoise_dsp,		"dsp",		A_CANT, 0);
	class_addmethod(c, (method)brownnoise_assist,   "assist",	A_CANT, 0);
    
	class_dspinit(c);				
	class_register(CLASS_BOX, c);
	brownnoise_class = c;
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void brownnoise_dsp(t_brownnoise *x, t_signal **sp, short *count){
	dsp_add(brownnoise_perform, 3, x, sp[0]->s_vec, sp[0]->s_n);
}

t_int *brownnoise_perform(t_int *w){
    t_brownnoise *x = (t_brownnoise *)(w[1]);	
    t_float *out = (t_float *)(w[2]);
	int n = (int)w[3];
    
    if (x->ob.z_disabled)
        return w + 4;    
    
    RGET
    
    float z = x->m_level;
    
	while (n--){
        z += frand8(s1, s2, s3);
		if (z > 1.f) z = 2.f - z;
		else if (z < -1.f) z = -2.f - z;

		*out++ = z;
    }
    
    x->m_level = z;
    
    RPUT
    
    
	return w + 4;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void brownnoise_assist(t_brownnoise *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_INLET) { //inlet
		sprintf(s, "Ignore this inlet");
	} 
	else {	// outlet
		sprintf(s, "(signal) Brown Noise"); 			
	}
}

void *brownnoise_new(long argc, t_atom *argv){
	t_brownnoise *x = NULL;
	x = (t_brownnoise *)object_alloc(brownnoise_class);
    
	if (x) {
		dsp_setup((t_pxobject *)x, 0);
        
        outlet_new((t_object *)x, "signal");
        
        x->rgen.init(time(NULL));
        
        RGET
        
        x->m_level = frand2(s1, s2, s3);
        
        RPUT
	}
	return (x);
}