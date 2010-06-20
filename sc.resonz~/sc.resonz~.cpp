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

 sc.resonz~
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

typedef struct _resonz 
{
	t_pxobject					ob;
    short m_connected[3];
    short changed;
    
    float m_y1, m_y2, m_a0, m_b1, m_b2, m_freq, m_rq;
    float mRadiansPerSample, mFilterSlope;
    int mFilterLoops, mFilterRemain;
} t_resonz;

t_class *resonz_class;

//////////////////////////////////////////////////////////////////////////////////////////////////////

void *resonz_new   (double freq, double rq);
void resonz_free   (t_resonz *x);
void resonz_assist (t_resonz *x, void *b, long m, long a, char *s);
void resonz_float  (t_resonz *x, double f);
void resonz_int    (t_resonz *x, long i);
void resonz_dsp    (t_resonz *x, t_signal **sp, short *count);
t_int *resonz_perform(t_int *w);

//////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void)
{	
	t_class *c;
    
	c = class_new("sc.resonz~", (method)resonz_new, (method)dsp_free, (long)sizeof(t_resonz), 0L, A_DEFFLOAT, A_DEFFLOAT, 0);
	
	class_addmethod(c, (method)resonz_dsp,		"dsp",		A_CANT, 0);
	class_addmethod(c, (method)resonz_assist,    "assist",	A_CANT, 0);
    class_addmethod(c, (method)resonz_float, "float", A_FLOAT, 0);
    class_addmethod(c, (method)resonz_int, "int", A_LONG, 0);
    
	class_dspinit(c);				
	class_register(CLASS_BOX, c);
	resonz_class = c;
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void resonz_int(t_resonz *x, long i)
{
    resonz_float(x, i);
}

void resonz_float(t_resonz *x, double f)
{
    switch (proxy_getinlet((t_object *)x)) {
        case 1:
            x->m_freq = f;
            x->changed = 1;
            break;
        case 2:
            x->m_rq = f;
            x->changed = 1;
            break;
    
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void resonz_dsp(t_resonz *x, t_signal **sp, short *count)
{
    x->mRadiansPerSample = sc_radiansPerSample();
    x->mFilterSlope = sc_filterSlope();
    x->mFilterLoops = sc_filterLoops();
    x->mFilterRemain = sc_filterRemain();
    
    x->m_connected[0] = count[0];
    x->m_connected[1] = count[1];
    x->m_connected[2] = count[2];
    
    x->m_a0 = 0.f;
	x->m_b1 = 0.f;
	x->m_b2 = 0.f;
	x->m_y1 = 0.f;
	x->m_y2 = 0.f;
        
    x->changed = 1;
    
    dsp_add(resonz_perform, 5, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec);
}

t_int *resonz_perform(t_int *w)
{
    t_resonz *x = (t_resonz*) w[1];
    t_float *out = (t_float*)w[5];
    t_float *in = (t_float*)w[2];
	t_float freq = x->m_connected[1] ? (*(t_float *)w[3]) : x->m_freq;
	t_float rq = x->m_connected[2] ? (*(t_float *)w[4]) : x->m_rq;
    
    
	float y0;
	float y1 = x->m_y1;
	float y2 = x->m_y2;
	float a0 = x->m_a0;
	float b1 = x->m_b1;
	float b2 = x->m_b2;
    
    if (x->ob.z_disabled) goto ending;
    
    if (freq != x->m_freq ||rq != x->m_rq || x->changed){
		float ffreq = freq * x->mRadiansPerSample;
		float B = ffreq * rq;
		float R = 1.f - B * 0.5f;
		float twoR = 2.f * R;
		float R2 = R * R;
		float cost = (twoR * cos(ffreq)) / (1.f + R2);
		float b1_next = twoR * cost;
		float b2_next = -R2;
		float a0_next = (1.f - R2) * 0.5f;
		float a0_slope = (a0_next - a0) * x->mFilterSlope;
		float b1_slope = (b1_next - b1) * x->mFilterSlope;
		float b2_slope = (b2_next - b2) * x->mFilterSlope;            

        LOOP(x->mFilterLoops,
             y0 = ZXP(in) + b1 * y1 + b2 * y2;
             ZXP(out) = a0 * (y0 - y2);
             
             y2 = ZXP(in) + b1 * y0 + b2 * y1;
             ZXP(out) = a0 * (y2 - y1);
             
             y1 = ZXP(in) + b1 * y2 + b2 * y0;
             ZXP(out) = a0 * (y1 - y0);
             
             a0 += a0_slope;
             b1 += b1_slope;
             b2 += b2_slope;
             );
        
        LOOP(x->mFilterRemain,
             y0 = ZXP(in) + b1 * y1 + b2 * y2;
             ZXP(out) = a0 * (y0 - y2);
             y2 = y1;
             y1 = y0;
             );
        
		x->m_freq = freq;
		x->m_rq = rq;
		x->m_a0 = a0_next;
		x->m_b1 = b1_next;
		x->m_b2 = b2_next;
        x->changed = 0;
    }
    else{
		LOOP(x->mFilterLoops,
             y0 = ZXP(in) + b1 * y1 + b2 * y2;
             ZXP(out) = a0 * (y0 - y2);
             
             y2 = ZXP(in) + b1 * y0 + b2 * y1;
             ZXP(out) = a0 * (y2 - y1);
             
             y1 = ZXP(in) + b1 * y2 + b2 * y0;
             ZXP(out) = a0 * (y1 - y0);
             );
		LOOP(x->mFilterRemain,
             y0 = ZXP(in) + b1 * y1 + b2 * y2;
             ZXP(out) = a0 * (y0 - y2);
             y2 = y1;
             y1 = y0;
             );
    }

    x->m_y1 = zapgremlins(y1);
    x->m_y2 = zapgremlins(y2);
    
ending:
    return w+6;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void resonz_assist(t_resonz *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_INLET) { //inlet
        switch (a) {
            case 0:
                sprintf(s, "(signal) Something to filter");
                break;
            case 1:
                sprintf(s, "(signal/float) resonant frequency in hz");
                break;
            case 2:
                sprintf(s, "(signal/float) bandwidth ratio (reciprocal of Q)");
                break;
        }
		
	} 
	else {	// outlet
		sprintf(s, "(signal) LPFed Signal"); 			
	}
}

void *resonz_new(double freq, double rq)
{
	t_resonz *x = NULL;
	x = (t_resonz *)object_alloc(resonz_class);
    
	if (x) {
        // number of inlets
		dsp_setup((t_pxobject *)x, 3);
        
        outlet_new((t_object *)x, "signal");
        
		x->m_freq = freq;
        x->m_rq = rq;
	}
	return (x);
}
