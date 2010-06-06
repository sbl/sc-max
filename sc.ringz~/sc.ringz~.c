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

 sc.ringz~
 (c) stephen lumenta under GPL
 http://www.gnu.org/licenses/gpl.html
 
 part of sc-max http://github.com/sbl/sc-max
 see README
*/

#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"

// from SCBOUNDSMacrosH.h
#define sc_max(a,b) (((a) > (b)) ? (a) : (b))
#define sc_min(a,b) (((a) < (b)) ? (a) : (b))

//////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct _ringz 
{
	t_pxobject      ob;
    
    short           m_connected[2];
    float           m_chaos_param;
    float           m_freq;
    float           m_sr;
    
    double          m_y1; // init value
	long            m_counter;
} t_ringz;

t_class *ringz_class;

//////////////////////////////////////////////////////////////////////////////////////////////////////

void    *ringz_new       (t_symbol *s, long argc, t_atom *argv);
void    ringz_assist     (t_ringz *x, void *b, long m, long a, char *s);
void    ringz_float      (t_ringz *x, double f);
void    ringz_int        (t_ringz *x, long l);
void    ringz_dsp        (t_ringz *x, t_signal **sp, short *count);
t_int   *ringz_perform   (t_int *w);

//////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void){	
	t_class *c;
    
	c = class_new("sc.ringz~", (method)ringz_new, (method)dsp_free, (long)sizeof(t_ringz), 0L, A_GIMME, 0);
	
	class_addmethod(c, (method)ringz_dsp,		"dsp",		A_CANT, 0);
	class_addmethod(c, (method)ringz_assist,     "assist",	A_CANT, 0);
    class_addmethod(c, (method)ringz_float,      "float",    A_FLOAT, 0);
    class_addmethod(c, (method)ringz_int,        "int",      A_LONG, 0);
    
	class_dspinit(c);				
	class_register(CLASS_BOX, c);
	ringz_class = c;
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void ringz_float(t_ringz *x, double f){
    switch (proxy_getinlet((t_object *)x)) {
        case 0:
            x->m_chaos_param = f;
            break;
        case 1:
            x->m_freq = f;
            break;
    }
}

void ringz_int(t_ringz *x, long l){
    ringz_float(x, (double) l);
}

void ringz_dsp(t_ringz *x, t_signal **sp, short *count){
    x->m_connected[0] = count[0];
    x->m_connected[1] = count[1];
    // class, param, freq, out, n
	dsp_add(ringz_perform, 5, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[2]->s_n);
}

#define OFFSET 6

t_int *ringz_perform(t_int *w){
    t_ringz  *x          = (t_ringz *)(w[1]);
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
			counter = (long)(x->m_sr  / sc_max(freq, .001f));
			counter = sc_max(1, counter);
			y1 = paramf * y1 * (1.0 - y1);	// chaotic equation
		}
		long nsmps = sc_min(counter, remain);
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

void ringz_assist(t_ringz *x, void *b, long m, long a, char *s)
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
		sprintf(s, "(signal) ringz"); 			
	}
}

void *ringz_new(t_symbol *s, long argc, t_atom *argv){
	t_ringz *x = NULL;
	x = (t_ringz *)object_alloc(ringz_class);
    
	if (x) {
		dsp_setup((t_pxobject *)x, 2);
        
        float chaos_param   = atom_getfloatarg(0,argc,argv);
        float freq          = atom_getfloatarg(1,argc,argv);
        
        x->m_chaos_param    = (short) chaos_param ? chaos_param : 3.f;
        x->m_freq           = (short) freq ? freq : 1000.f;
        x->m_y1             = 0.5f;
        x->m_counter        = 0L;
        x->m_sr             = sys_getsr();
        
        outlet_new((t_object *)x, "signal");
	}
	return (x);
}
