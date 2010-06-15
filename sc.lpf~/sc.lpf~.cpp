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

 sc.lpf~
 (c) stephen lumenta under GPL
 http://www.gnu.org/licenses/gpl.html
 
 part of sc-max http://github.com/sbl/sc-max
 see README
*/

#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct _lpf 
{
	t_pxobject					ob;
} t_lpf;

t_class *lpf_class;

//////////////////////////////////////////////////////////////////////////////////////////////////////

void *lpf_new();
void lpf_free(t_lpf *x);
void lpf_assist(t_lpf *x, void *b, long m, long a, char *s);

void lpf_dsp(t_lpf *x, t_signal **sp, short *count);
t_int *lpf_perform(t_int *w);

//////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void){	
	t_class *c;
    
	c = class_new("sc.lpf~", (method)lpf_new, (method)dsp_free, (long)sizeof(t_lpf), 0L, NULL, 0);
	
	class_addmethod(c, (method)lpf_dsp,		"dsp",		A_CANT, 0);
	class_addmethod(c, (method)lpf_assist,    "assist",	A_CANT, 0);
    
	class_dspinit(c);				
	class_register(CLASS_BOX, c);
	lpf_class = c;
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void lpf_dsp(t_lpf *x, t_signal **sp, short *count){
	dsp_add(lpf_perform, 4, x, sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);
}

t_int *lpf_perform(t_int *w){
    t_lpf *x = (t_lpf *)(w[1]);
    t_float *in = (t_float *)(w[2]);
    t_float *out = (t_float *)(w[3]);
	int n = (int)w[4];
    
    if (x->ob.z_disabled)
        return w + 5;    
    
    while (n--) {
        *out++ = *in++;
    }
    
	return w + 5;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void lpf_assist(t_lpf *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_INLET) { //inlet
		sprintf(s, "Something to filter");
	} 
	else {	// outlet
		sprintf(s, "(signal) LPFed Signal"); 			
	}
}

void *lpf_new(){
	t_lpf *x = NULL;
	x = (t_lpf *)object_alloc(lpf_class);
    
	if (x) {
        // number of inlets
		dsp_setup((t_pxobject *)x, 1);
        
        outlet_new((t_object *)x, "signal");
        
	}
	return (x);
}
