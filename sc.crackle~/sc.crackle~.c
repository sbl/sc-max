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

 sc.crackle~
 (c) stephen lumenta under GPL
 http://www.gnu.org/licenses/gpl.html
 
 part of sc-max http://github.com/sbl/sc-max
 see README
 
 *
 **
 ***		64bit update by volker boehm, august 2016
 **
 *
*/

#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct _crackle 
{
	t_pxobject      ob;
    short           m_connected;
    
    t_double           m_param;
    t_double           m_y1;
    t_double           m_y2;
} t_crackle;

t_class *crackle_class;

//////////////////////////////////////////////////////////////////////////////////////////////////////

void *crackle_new(double param);
void crackle_free(t_crackle *x);
void crackle_assist(t_crackle *x, void *b, long m, long a, char *s);

void crackle_float(t_crackle *x, double param);
void crackle_int(t_crackle *x, long param);
void crackle_dsp(t_crackle *x, t_signal **sp, short *count);
t_int *crackle_perform(t_int *w);

void crackle_dsp64(t_crackle *x, t_object *dsp64, short *count, double samplerate,
				 long maxvectorsize, long flags);
void crackle_perform64(t_crackle *x, t_object *dsp64, double **ins, long numins,
					 double **outs, long numouts, long sampleframes, long flags, void *userparam);

//////////////////////////////////////////////////////////////////////////////////////////////////////

int C74_EXPORT main(void){	
	t_class *c;
    
	c = class_new("sc.crackle~", (method)crackle_new, (method)dsp_free, (long)sizeof(t_crackle), 0L, A_DEFFLOAT, 0);
	
	class_addmethod(c, (method)crackle_dsp,         "dsp",      A_CANT, 0);
	class_addmethod(c, (method)crackle_dsp64,		"dsp64", A_CANT, 0);
	class_addmethod(c, (method)crackle_assist,      "assist",	A_CANT, 0);
    class_addmethod(c, (method)crackle_float,       "float",    A_FLOAT, 0);
    class_addmethod(c, (method)crackle_int,         "int",      A_LONG, 0);
    
	class_dspinit(c);				
	class_register(CLASS_BOX, c);
	crackle_class = c;
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void crackle_float(t_crackle *x, double param){
    x->m_param = param;
}

void crackle_int(t_crackle *x, long param){
    crackle_float(x, (double) param);
}

void crackle_dsp(t_crackle *x, t_signal **sp, short *count){
    x->m_connected = count[0];
	dsp_add(crackle_perform, 4, x, sp[0]->s_vec, sp[1]->s_vec, sp[1]->s_n);
}

t_int *crackle_perform(t_int *w){
    t_crackle *x = (t_crackle *)(w[1]);
    t_float param = x->m_connected ? (*(t_float *)(w[2])) : x->m_param;
    t_float *out = (t_float *)(w[3]);
	int n = (int)w[4];
    
    t_double y1 = x->m_y1;
	t_double y2 = x->m_y2;
	t_double y0;
    
    if (param < 0.) param = 0.;
    if (param > 2.) param = 2.;
    
    if (x->ob.z_disabled)
        return w + 5;    
        
	while (n--){
		*out++ = y0 = fabs(y1 * param - y2 - 0.05);
        y2 = y1; y1 = y0;
    }
    
    x->m_y1 = y1;
	x->m_y2 = y2;

    
	return w + 5;
}



//64-bit dsp method
void crackle_dsp64(t_crackle *x, t_object *dsp64, short *count, double samplerate,
				   long maxvectorsize, long flags) {
	x->m_connected = count[0];
	object_method(dsp64, gensym("dsp_add64"), x, crackle_perform64, 0, NULL);
}

// 64 bit signal input version
void crackle_perform64(t_crackle *x, t_object *dsp64, double **ins, long numins,
					   double **outs, long numouts, long sampleframes, long flags, void *userparam) {
	
	t_double param = x->m_connected ? (*(t_float *)(ins[0])) : x->m_param;
	t_double *out = outs[0];
	int vs = sampleframes;
	
    if (x->ob.z_disabled)
        return;
	
    t_double y1 = x->m_y1;
	t_double y2 = x->m_y2;
	t_double y0;
    
    if (param < 0.) param = 0.;
    if (param > 2.) param = 2.;
    
	while (vs--){
		*out++ = y0 = fabs(y1 * param - y2 - 0.05);
        y2 = y1; y1 = y0;
    }
    
    x->m_y1 = y1;
	x->m_y2 = y2;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void crackle_assist(t_crackle *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_INLET) { //inlet
		sprintf(s, "(signal/float) param");
	} 
	else {	// outlet
		sprintf(s, "(signal) crackle"); 			
	}
}

void *crackle_new(double param){
	t_crackle *x = NULL;
	x = (t_crackle *)object_alloc(crackle_class);
    
	if (x) {
		dsp_setup((t_pxobject *)x, 1);
        
        outlet_new((t_object *)x, "signal");
        
        x->m_param = param;
        x->m_y1 = 0.3;
        x->m_y2 = 0.;
	}
	else {
		object_free(x);
		x = NULL;
	}
	return (x);
}
