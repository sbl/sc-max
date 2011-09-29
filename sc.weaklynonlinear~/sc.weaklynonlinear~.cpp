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
 */

#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"
#include "scmax.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct _weaklynonlinear 
{
	t_pxobject		ob;
  float           x,y,t;
  int             reset;
  float           ratex, ratey,freq,initx, inity, alpha, xexponent, beta, yexponent;
} t_weaklynonlinear;

t_class *weaklynonlinear_class;

//////////////////////////////////////////////////////////////////////////////////////////////////////

void *weaklynonlinear_new(t_symbol *s, long ac, t_atom *av);
void weaklynonlinear_free(t_weaklynonlinear *x);
void weaklynonlinear_assist(t_weaklynonlinear *x, void *b, long m, long a, char *s);

void weaklynonlinear_dsp(t_weaklynonlinear *x, t_signal **sp, short *count);
t_int *weaklynonlinear_perform(t_int *w);

//////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void){	
	t_class *c;
  
	c = class_new("sc.weaklynonlinear~", (method)weaklynonlinear_new, (method)dsp_free, (long)sizeof(t_weaklynonlinear), 0L, NULL, 0);
	
	class_addmethod(c, (method)weaklynonlinear_dsp,       "dsp",	A_CANT, 0);
	class_addmethod(c, (method)weaklynonlinear_assist,    "assist",	A_CANT, 0);
  
  CLASS_ATTR_LONG         (c, "reset",       ATTR_FLAGS_NONE, t_weaklynonlinear, reset);
  CLASS_ATTR_FLOAT        (c, "ratex",       ATTR_FLAGS_NONE, t_weaklynonlinear, ratex);
  CLASS_ATTR_FLOAT        (c, "ratey",       ATTR_FLAGS_NONE, t_weaklynonlinear, ratey);
  CLASS_ATTR_FLOAT        (c, "freq",        ATTR_FLAGS_NONE, t_weaklynonlinear, freq);
  CLASS_ATTR_FLOAT        (c, "initx",       ATTR_FLAGS_NONE, t_weaklynonlinear, initx);
  CLASS_ATTR_FLOAT        (c, "inity",       ATTR_FLAGS_NONE, t_weaklynonlinear, inity);
  CLASS_ATTR_FLOAT        (c, "alpha",       ATTR_FLAGS_NONE, t_weaklynonlinear, alpha);
  CLASS_ATTR_FLOAT        (c, "beta",        ATTR_FLAGS_NONE, t_weaklynonlinear, beta);
  CLASS_ATTR_FLOAT        (c, "xexponent",   ATTR_FLAGS_NONE, t_weaklynonlinear, xexponent);    
  CLASS_ATTR_FLOAT        (c, "yexponent",   ATTR_FLAGS_NONE, t_weaklynonlinear, yexponent);    
  
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
    x->ratex=1.f; 
    x->ratey=1.f; 
    x->freq=440.f;
    x->initx = 0.f;
    x->inity = 0.f;
    x->alpha=0.f; 
    x->xexponent=0.f;
    x->beta=0.f;
    x->yexponent=0.f;
    
    attr_args_process(x, ac, av);
    outlet_new((t_object *)x, "signal");
	}
	return (x);
}
