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

 sc.logistic~
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

// from SCBOUNDSMacrosH.h
//#define sc_max(a,b) (((a) > (b)) ? (a) : (b))
//#define sc_min(a,b) (((a) < (b)) ? (a) : (b))

//////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct _logistic 
{
	t_pxobject      ob;
    
    short			m_connected[2];
    double			m_chaos_param;
    double			m_freq;
    double			m_sr;
    
    double			m_y1; // init value
	long				m_counter;
} t_logistic;

t_class *logistic_class;

//////////////////////////////////////////////////////////////////////////////////////////////////////

void    *logistic_new       (t_symbol *s, long argc, t_atom *argv);
void    logistic_assist     (t_logistic *x, void *b, long m, long a, char *s);
void    logistic_float      (t_logistic *x, double f);
void    logistic_int        (t_logistic *x, long l);
void		logistic_bang		(t_logistic *x);
void    logistic_dsp        (t_logistic *x, t_signal **sp, short *count);
t_int   *logistic_perform   (t_int *w);

void logistic_dsp64	(t_logistic *x, t_object *dsp64, short *count, double samplerate,
					 long maxvectorsize, long flags);
void logistic_perform64(t_logistic*x, t_object *dsp64, double **ins, long numins,
					   double **outs, long numouts, long sampleframes, long flags, void *userparam);

//////////////////////////////////////////////////////////////////////////////////////////////////////

int C74_EXPORT main(void){
	t_class *c;
    
	c = class_new("sc.logistic~", (method)logistic_new, (method)dsp_free, (long)sizeof(t_logistic), 0L, A_GIMME, 0);
	
	class_addmethod(c, (method)logistic_dsp,		"dsp",		A_CANT, 0);
	class_addmethod(c, (method)logistic_dsp64,		"dsp64",		A_CANT, 0);
	class_addmethod(c, (method)logistic_assist,     "assist",	A_CANT, 0);
    class_addmethod(c, (method)logistic_float,      "float",    A_FLOAT, 0);
    class_addmethod(c, (method)logistic_int,        "int",      A_LONG, 0);
	class_addmethod(c, (method)logistic_bang,        "bang", 0);
    
	class_dspinit(c);				
	class_register(CLASS_BOX, c);
	logistic_class = c;
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void logistic_float(t_logistic *x, double f){
    switch (proxy_getinlet((t_object *)x)) {
        case 0:
            x->m_chaos_param = f;
            break;
        case 1:
            x->m_freq = f;
            break;
    }
}

void logistic_int(t_logistic *x, long l){
    logistic_float(x, (double) l);
}

//reset function
void logistic_bang(t_logistic *x) {
    x->m_counter = 0;
}

void logistic_dsp(t_logistic *x, t_signal **sp, short *count){
    x->m_connected[0] = count[0];
    x->m_connected[1] = count[1];
    // class, param, freq, out, n
	dsp_add(logistic_perform, 5, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[2]->s_n);
}

#define OFFSET 6

t_int *logistic_perform(t_int *w){
    t_logistic  *x          = (t_logistic *)(w[1]);
    t_float     paramf      = x->m_connected[0] ? (*(t_float *)(w[2])) : x->m_chaos_param;
    t_float     freq        = x->m_connected[1] ? (*(t_float *)(w[3])) : x->m_freq;
    double      y1          = x->m_y1;
    long        counter     = x->m_counter;
    t_float     *out        = (t_float *)(w[4]);
	int         n           = (int)w[5];
    long        i;
    
    if (x->ob.z_disabled) return w + OFFSET;    
    
    long remain = n;
    if (freq > x->m_sr) freq = x->m_sr;
    if (freq < 0) freq = 0;
    
    do{
        if (counter<=0) {
			counter = (long)(x->m_sr  / MAX(freq, .001f));
			counter = MAX(1, counter);
			y1 = paramf * y1 * (1.0 - y1);	// chaotic equation
		}
		long nsmps = MIN(counter, remain);
		counter -= nsmps;
		remain  -= nsmps;
        
        for(i=0; i<nsmps;i++){
            *out++ = y1;
        }
        
    } while(remain);
    
    x->m_y1 = y1;
	x->m_counter = counter;
            
        
	return w + OFFSET;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
// 64bit dsp
void logistic_dsp64	(t_logistic *x, t_object *dsp64, short *count, double samplerate,
					 long maxvectorsize, long flags) {
	x->m_connected[0] = count[0];
    x->m_connected[1] = count[1];
	object_method(dsp64, gensym("dsp_add64"), x, logistic_perform64, 0, NULL);
}


void logistic_perform64(t_logistic*x, t_object *dsp64, double **ins, long numins,
						double **outs, long numouts, long sampleframes, long flags, void *userparam) {
	
	t_double		*out        = outs[0];
    t_double     paramf      = x->m_connected[0] ? (*(t_double *)(ins[0])) : x->m_chaos_param;
    t_double     freq        = x->m_connected[1] ? (*(t_double *)(ins[1])) : x->m_freq;
	
    double			y1			= x->m_y1;
    long				counter	= x->m_counter;
	long				remain 	= sampleframes;
    long		i;
    
    if (x->ob.z_disabled)
		return;

    //if (freq > x->m_sr) freq = x->m_sr;
    //if (freq < 0) freq = 0;
	CLIP_ASSIGN(freq, 0.01, x->m_sr);
	CLIP_ASSIGN(paramf, 0.0, 4.0);
    
    do{
        if (counter<=0) {
			counter = (long)(x->m_sr / freq);		//MAX(freq, .001)
			//counter = MAX(1, counter);
			y1 = paramf * y1 * (1.0 - y1);			// chaotic equation
		}
		long nsmps = MIN(counter, remain);
		counter -= nsmps;
		remain  -= nsmps;
        
        for(i=0; i<nsmps;i++){
            *out++ = y1;
        }
        
    } while(remain);
    
    x->m_y1 = y1;
	x->m_counter = counter;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////

void logistic_assist(t_logistic *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_INLET) { //inlet
        switch (a) {
            case 0:
                sprintf(s, "(signal/float) chaos param");
                break;
            case 1:
                sprintf(s, "(signal/float) freq");
                break;
            default:
                break;
        }
	} 
	else {	// outlet
		sprintf(s, "(signal) Logistic"); 			
	}
}

void *logistic_new(t_symbol *s, long argc, t_atom *argv){
	t_logistic *x = NULL;
	x = (t_logistic *)object_alloc(logistic_class);
    
	if (x) {
		dsp_setup((t_pxobject *)x, 2);
        
        float chaos_param   = atom_getfloatarg(0,argc,argv);
        float freq          = atom_getfloatarg(1,argc,argv);
        
        x->m_chaos_param    = chaos_param ? chaos_param : 3.;
        x->m_freq           =  freq ? freq : 1000.;
        x->m_y1             = 0.5;
        x->m_counter        = 0L;
        x->m_sr             = sys_getsr();
        
        outlet_new((t_object *)x, "signal");
	}
	else {
		object_free(x);
		x = NULL;
	}
	return (x);
}
