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

#include "scmax.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct _ringz 
{
	t_pxobject      ob;
    
    short           m_connected[3];
    float           m_y1, m_y2, m_b1, m_b2, m_freq, m_decayTime;
    float           m_sr, m_rps;
    int             m_floops, m_fremains; 
    double          m_fslope;
} t_ringz;

t_class *ringz_class;

//////////////////////////////////////////////////////////////////////////////////////////////////////

void    *ringz_new       (t_symbol *s, long argc, t_atom *argv);
void    ringz_assist     (t_ringz *x, void *b, long m, long a, char *s);
void    ringz_float      (t_ringz *x, double f);
void    ringz_int        (t_ringz *x, long l);
void    ringz_dsp        (t_ringz *x, t_signal **sp, short *count);
t_int   *ringz_perform   (t_int *w);

void    calcRates        (t_ringz *x, t_signal *sp);
//////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void)
{	
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
        case 1:
            x->m_freq = f;
            post("frequency: %f", x->m_freq);
            break;
        case 2:
            x->m_decayTime = f;
            post("decay: %f", x->m_decayTime);
            break;
    }
}

void ringz_int(t_ringz *x, long l){
    ringz_float(x, (double) l);
}

void ringz_dsp(t_ringz *x, t_signal **sp, short *count)
{
    x->m_connected[0] = count[0];
    x->m_connected[1] = count[1];
    x->m_connected[2] = count[2];
    
    calcRates(x, sp[0]);
    post("loops: %d", x->m_floops);
    post("remains: %d", x->m_fremains);
    post("n: %d", sp[0]->s_n);
    
    // in, freq, decay, out, n
	dsp_add(ringz_perform, 5, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec);    
}

#define OFFSET 6

t_int *ringz_perform(t_int *w)
{
    t_ringz     *x          = (t_ringz *)(w[1]);
        
    // no processing when disabled
    // if (x->ob.z_disabled) return w + OFFSET;    

    t_float   *in           = (t_float *)(w[2]);
    t_float   *out          = (t_float *)(w[5]);
    float     freq          = x->m_connected[1] ? (*(t_float *)(w[3])) : x->m_freq;
    float     decayTime     = x->m_connected[2] ? (*(t_float *)(w[4])) : x->m_decayTime;
    
    float y0;
	float y1 = x->m_y1;
	float y2 = x->m_y2;
	float a0 = 0.5f;
	float b1 = x->m_b1;
	float b2 = x->m_b2;
    
    // on coeff change
    if (freq != x->m_freq || decayTime != x->m_decayTime) {        
		float ffreq = freq * x->m_rps;

		float R = decayTime == 0.f ? 0.f : exp(log(0.001)/(decayTime * x->m_sr));
		float twoR = 2.f * R;
		float R2 = R * R;
		float cost = (twoR * cos(ffreq)) / (1.f + R2);
		float b1_next = twoR * cost;
		float b2_next = -R2;
		float b1_slope = (b1_next - b1) * x->m_fslope;
		float b2_slope = (b2_next - b2) * x->m_fslope;
        
		LOOP(x->m_floops,
             y0 = ZXP(in) + b1 * y1 + b2 * y2;
             ZXP(out) = a0 * (y0 - y2);
             
             y2 = ZXP(in) + b1 * y0 + b2 * y1;
             ZXP(out) = a0 * (y2 - y1);
             
             y1 = ZXP(in) + b1 * y2 + b2 * y0;
             ZXP(out) = a0 * (y1 - y0);
             
             b1 += b1_slope;
             b2 += b2_slope;
             );
		LOOP(x->m_fremains,
             y0 = ZXP(in) + b1 * y1 + b2 * y2;
             ZXP(out) = a0 * (y0 - y2);
             y2 = y1;
             y1 = y0;
             );
        
        
		x->m_freq = freq;
		x->m_decayTime = decayTime;
		x->m_b1 = b1_next;
		x->m_b2 = b2_next;
	} 
    // no coeff change
    else {
		LOOP(x->m_floops,
             y0 = ZXP(in) + b1 * y1 + b2 * y2;
             ZXP(out) = a0 * (y0 - y2);
             
             y2 = ZXP(in) + b1 * y0 + b2 * y1;
             ZXP(out) = a0 * (y2 - y1);
             
             y1 = ZXP(in) + b1 * y2 + b2 * y0;
             ZXP(out) = a0 * (y1 - y0);
             );
		LOOP(x->m_fremains,
             y0 = ZXP(in) + b1 * y1 + b2 * y2;
             ZXP(out) = a0 * (y0 - y2);
             y2 = y1;
             y1 = y0;
             );
	}
    
            
//	x->m_y1 = zapgremlins(y1);
//	x->m_y2 = zapgremlins(y2);
        
	return w + OFFSET;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void ringz_assist(t_ringz *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_INLET) { //inlet
        switch (a) {
            case 0:
                sprintf(s, "(signal) the signal to ring");
                break;
            case 1:
                sprintf(s, "(signal/float) frequency");
                break;
            case 2:
                sprintf(s, "(signal/float) decay time ");
                break;
            default:
                break;
        }
	} 
	else {	// outlet
		sprintf(s, "(signal) ringz"); 			
	}
}

void *ringz_new(t_symbol *s, long argc, t_atom *argv)
{
	t_ringz *x = NULL;
	x = (t_ringz *)object_alloc(ringz_class);
    
	if (x) {
		dsp_setup((t_pxobject *)x, 3);
        
        // TODO fix me
        float freq          = atom_getfloatarg(0,argc,argv);
        float decayTime     = atom_getfloatarg(1,argc,argv);
        
        x->m_b1 = 0.f;
        x->m_b2 = 0.f;
        x->m_y1 = 0.f;
        x->m_y2 = 0.f;
        x->m_freq = freq;
        x->m_decayTime = decayTime;
        
        
        outlet_new((t_object *)x, "signal");
	}
	return (x);
}

void calcRates(t_ringz *x, t_signal *sp)
{
    x->m_sr         = sp->s_sr;
    x->m_rps        = TWOPI / x->m_sr;
    x->m_floops     = sp->s_n / 3;
    x->m_fremains   = sp->s_n % 3;
    
    if (x->m_floops == 0) {
        x->m_fslope = 0.;
    } else {
        x->m_fslope = 1. / x->m_floops;
    }

}