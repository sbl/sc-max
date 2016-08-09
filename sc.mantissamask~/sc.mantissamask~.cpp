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

 sc.mantissamask~
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
#include "SC_Types.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct _mantissamask {
	t_pxobject	ob;
    
    int         m_bits;
} t_mantissamask;

t_class *mantissamask_class;

//////////////////////////////////////////////////////////////////////////////////////////////////////

void *mantissamask_new      (long bits);
void mantissamask_assist    (t_mantissamask *x, void *b, long m, long a, char *s);
void mantissamask_float     (t_mantissamask *x, double bits);
void mantissamask_int       (t_mantissamask *x, long bits);
void mantissamask_dsp       (t_mantissamask *x, t_signal **sp, short *count);
t_int *mantissamask_perform (t_int *w);

void mantissamask_dsp64	(t_mantissamask *x, t_object *dsp64, short *count, double samplerate,
					 long maxvectorsize, long flags);
void mantissamask_perform64(t_mantissamask*x, t_object *dsp64, double **ins, long numins,
						double **outs, long numouts, long sampleframes, long flags, void *userparam);

//////////////////////////////////////////////////////////////////////////////////////////////////////

int C74_EXPORT main(void){
	t_class *c;
    
	c = class_new("sc.mantissamask~", (method)mantissamask_new, (method)dsp_free, (long)sizeof(t_mantissamask), 0L, A_LONG, 0);
	
	class_addmethod(c, (method)mantissamask_dsp,		"dsp",		A_CANT, 0);
	class_addmethod(c, (method)mantissamask_dsp64,		"dsp64",		A_CANT, 0);
	class_addmethod(c, (method)mantissamask_assist,     "assist",	A_CANT, 0);
    
    class_addmethod(c, (method)mantissamask_float,      "float",    A_FLOAT, 0);
    class_addmethod(c, (method)mantissamask_int,        "int",      A_LONG, 0);
    
	class_dspinit(c);				
	class_register(CLASS_BOX, c);
	mantissamask_class = c;
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void mantissamask_int(t_mantissamask *x, long bits){
    if(proxy_getinlet((t_object *)x) == 1) {
        x->m_bits = bits;
    }
}

void mantissamask_float(t_mantissamask *x, double bits){
    mantissamask_int(x, (long) bits);
}

void mantissamask_dsp(t_mantissamask *x, t_signal **sp, short *count){     
	// class, in, bits, out, n
    dsp_add(mantissamask_perform, 5, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[0]->s_n);
}

t_int *mantissamask_perform(t_int *w){
    t_mantissamask  *x      = (t_mantissamask *)(w[1]);	
    int32           *in     = (int32*)(w[2]);    
    int32           bits    = x->m_bits;
    
    int32           *out    = (int32*)(w[4]);
   	int32           mask    = -1 << (23 - bits);    
    int             n       = (int)w[5];
    
    
    if (x->ob.z_disabled) return w + 6;    
    
	while (n--) *out++ = mask & (*(in)++);
    
	return w + 6;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////
// 64bit dsp
void mantissamask_dsp64	(t_mantissamask *x, t_object *dsp64, short *count, double samplerate,
					 long maxvectorsize, long flags) {
	// class, in, bits, out, n
    object_method(dsp64, gensym("dsp_add64"), x, mantissamask_perform64, 0, NULL);
}


void mantissamask_perform64(t_mantissamask*x, t_object *dsp64, double **ins, long numins,
							double **outs, long numouts, long sampleframes, long flags, void *userparam) {
	
	int64           *in     =  (int64*)ins[0];
	int64           *out    = (int64*)(outs[0]);
    int64           bits    = x->m_bits;
	int64           mask    = (int64_t)-1 << (52 - bits);
    int				n       = sampleframes;
    
    if (x->ob.z_disabled) return;
	
	while (n--)
		*out++ = mask & (*(in)++);
		
}


//////////////////////////////////////////////////////////////////////////////////////////////////////

void mantissamask_assist(t_mantissamask *x, void *b, long m, long a, char *s) {
	if (m == ASSIST_INLET) { //inlet
        switch (a) {
            case 0:
                sprintf(s, "(signal) signal to mask");
                break;
            case 1:
                sprintf(s, "(float/int) bit precision");
                break;
        }
		
	} 
	else {	// outlet
		sprintf(s, "(signal) masked signal"); 			
	}
}

void *mantissamask_new(long bits){
	t_mantissamask *x = NULL;
	x = (t_mantissamask *)object_alloc(mantissamask_class);
    
	if (x) {
		dsp_setup((t_pxobject *)x, 2);
                
        x->m_bits = (int) bits;
        outlet_new((t_object *)x, "signal");
		
	}
	else {
		object_free(x);
		x = NULL;
	}
	
	return (x);
}
