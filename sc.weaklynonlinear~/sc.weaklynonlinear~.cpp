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
 
 Original Ugens by Nick Collins 
 SLUGens released under the GNU GPL as extensions for SuperCollider 3, by Nick Collins, http://www.informatics.sussex.ac.uk/users/nc81/
 */

/*
 
 sc.weaklynonlinear~
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

typedef struct _weaklynonlinear 
{
	t_pxobject		ob;
  double           x,y,t;
  int             reset;
  double           ratex, ratey,freq,initx, inity, alpha, xexponent, beta, yexponent;
	double		m_sr;
} t_weaklynonlinear;

t_class *weaklynonlinear_class;

//////////////////////////////////////////////////////////////////////////////////////////////////////

void *weaklynonlinear_new(t_symbol *s, long ac, t_atom *av);
void weaklynonlinear_free(t_weaklynonlinear *x);
void weaklynonlinear_assist(t_weaklynonlinear *x, void *b, long m, long a, char *s);

void weaklynonlinear_dsp(t_weaklynonlinear *x, t_signal **sp, short *count);
t_int *weaklynonlinear_perform(t_int *w);

void weaklynonlinear_dsp64	(t_weaklynonlinear *x, t_object *dsp64, short *count, double samplerate,
						 long maxvectorsize, long flags);
void weaklynonlinear_perform64(t_weaklynonlinear *x, t_object *dsp64, double **ins, long numins,
							double **outs, long numouts, long sampleframes, long flags, void *userparam);

//////////////////////////////////////////////////////////////////////////////////////////////////////

int C74_EXPORT main(void){
	t_class *c;
  
	c = class_new("sc.weaklynonlinear~", (method)weaklynonlinear_new, (method)dsp_free, (long)sizeof(t_weaklynonlinear), 0L, NULL, 0);
	
	class_addmethod(c, (method)weaklynonlinear_dsp,       "dsp",	A_CANT, 0);
	class_addmethod(c, (method)weaklynonlinear_dsp64,       "dsp64",	A_CANT, 0);
	class_addmethod(c, (method)weaklynonlinear_assist,    "assist",	A_CANT, 0);
  
  CLASS_ATTR_LONG         (c, "reset",       ATTR_FLAGS_NONE, t_weaklynonlinear, reset);
  CLASS_ATTR_DOUBLE        (c, "ratex",       ATTR_FLAGS_NONE, t_weaklynonlinear, ratex);
  CLASS_ATTR_DOUBLE        (c, "ratey",       ATTR_FLAGS_NONE, t_weaklynonlinear, ratey);
  CLASS_ATTR_DOUBLE        (c, "freq",        ATTR_FLAGS_NONE, t_weaklynonlinear, freq);
  CLASS_ATTR_DOUBLE        (c, "initx",       ATTR_FLAGS_NONE, t_weaklynonlinear, initx);
  CLASS_ATTR_DOUBLE        (c, "inity",       ATTR_FLAGS_NONE, t_weaklynonlinear, inity);
  CLASS_ATTR_DOUBLE        (c, "alpha",       ATTR_FLAGS_NONE, t_weaklynonlinear, alpha);
  CLASS_ATTR_DOUBLE        (c, "beta",        ATTR_FLAGS_NONE, t_weaklynonlinear, beta);
  CLASS_ATTR_DOUBLE        (c, "xexponent",   ATTR_FLAGS_NONE, t_weaklynonlinear, xexponent);
  CLASS_ATTR_DOUBLE        (c, "yexponent",   ATTR_FLAGS_NONE, t_weaklynonlinear, yexponent);
  
	class_dspinit(c);				
	class_register(CLASS_BOX, c);
	weaklynonlinear_class = c;
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void weaklynonlinear_dsp(t_weaklynonlinear *x, t_signal **sp, short *count){
  
	dsp_add(weaklynonlinear_perform, 4, x, sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);
}

t_int *weaklynonlinear_perform(t_int *w){
  t_weaklynonlinear *unit = (t_weaklynonlinear *)(w[1]);	
  
  t_float *in = (t_float *)(w[2]);
  t_float *out = (t_float *)(w[3]);
	long n = (int)w[4];
	
  int reset = unit->reset;
  float xrate = unit->ratex;
  float yrate = unit->ratey;
  float w0 = unit->freq;
  float alpha = unit->alpha;
  float xexponent = unit->xexponent;
  float beta = unit->beta;
  float yexponent = unit->yexponent;
  
  
  if (unit->ob.z_disabled) goto ending;
  
  
	w0 = w0 * TWOPI/(sys_getsr()); //convert frequency in Hertz to angular frequency
	w0 = w0*w0; //constant needed for equation
              //float eta= (float)ZIN0(5);
	
	float x,y,t; 
	
	x = unit->x;
	y = unit->y;
	t = unit->t;
	
	if(reset){
		x= unit->x;
		y= unit->y;
		t=0;
	}
  
	//Runge Kutta? would require four cos calls
	for (int j=0; j<n;++j) {
		
		//the naive Euler update
		float dxdt= xrate*y;
		
		float nonlinear= 0.0;
		
		if(alpha>0.000001 || alpha<(-0.000001)) {
			nonlinear= alpha * (pow(x,xexponent)+beta)*(pow(y,yexponent));
		}
		
		float dydt= yrate*(ZXP(in) - w0*x + nonlinear);
		
		//t=t+1;
		x+=dxdt;
		y+=dydt;
		
		if ((x>1.0) || (x<-1.0)) 
      x=fabs(fmod((x-1.0),4.0)-2.0)-1.0;
		
		ZXP(out) = x;
	}
	
	unit->x=x;
	unit->y=y;
	unit->t=t;
  
ending:
	return w + 5;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
// 64bit dsp
void weaklynonlinear_dsp64	(t_weaklynonlinear *x, t_object *dsp64, short *count, double samplerate,
							 long maxvectorsize, long flags) {
	x->m_sr = samplerate;
	object_method(dsp64, gensym("dsp_add64"), x, weaklynonlinear_perform64, 0, NULL);
}

void weaklynonlinear_perform64(t_weaklynonlinear *unit, t_object *dsp64, double **ins, long numins,
							   double **outs, long numouts, long sampleframes, long flags, void *userparam) {
	
	t_double *in = ins[0];
	t_double *out = outs[0];
	long n = sampleframes;
	
	int reset = unit->reset;
	double xrate = unit->ratex;
	double yrate = unit->ratey;
	double w0 = unit->freq;
	double alpha = unit->alpha;
	double xexponent = unit->xexponent;
	double beta = unit->beta;
	double yexponent = unit->yexponent;
	
	
	if (unit->ob.z_disabled) return;
	
	
	w0 = w0 * TWOPI/unit->m_sr; //convert frequency in Hertz to angular frequency
	w0 = w0*w0; //constant needed for equation
	
	double x,y,t;
	
	x = unit->x;
	y = unit->y;
	t = unit->t;
	
	if(reset){
		x= unit->x;
		y= unit->y;
		t=0;
	}
	
	//Runge Kutta? would require four cos calls
	for (int j=0; j<n;++j) {
		
		//the naive Euler update
		double dxdt= xrate*y;
		
		double nonlinear= 0.0;
		
		if(alpha>0.000001 || alpha<(-0.000001)) {
			nonlinear= alpha * (pow(x,xexponent)+beta)*(pow(y,yexponent));
		}
		
		double dydt= yrate*(*(in)++ - w0*x + nonlinear);
		

		x+=dxdt;
		y+=dydt;
		
		if ((x>1.0) || (x<-1.0))
			x=fabs(fmod((x-1.0),4.0)-2.0)-1.0;
		
		*out++ = x;
	}
	
	unit->x=x;
	unit->y=y;
	unit->t=t;
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void weaklynonlinear_assist(t_weaklynonlinear *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_INLET) { //inlet
		sprintf(s, "(signal) Audio Rate input: acts as external force");
	} 
	else {	// outlet
		sprintf(s, "(signal) Weakly Nonlinear Oscillator"); 			
	}
}

void *weaklynonlinear_new(t_symbol *s, long ac, t_atom *av){
	t_weaklynonlinear *x = NULL;
	x = (t_weaklynonlinear *)object_alloc(weaklynonlinear_class);
  
	if (x) {
		dsp_setup((t_pxobject *)x, 1);
    
    x->x = 0.0;
    x->y = 0.0;
    x->t = 0.0;
    x->reset=0; 
    x->ratex=1.;
    x->ratey=1.;
    x->freq=440.;
		x->initx = 0.;
    x->inity = 0.;
    x->alpha=0.;
    x->xexponent=0.;
    x->beta=0.;
    x->yexponent=0.;
    
    attr_args_process(x, ac, av);
    outlet_new((t_object *)x, "signal");
	}
	else {
		object_free(x);
		x = NULL;
	}
	
	return (x);
}
