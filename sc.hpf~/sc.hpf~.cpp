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

 sc.hpf~
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

typedef struct _hpf 
{
	t_pxobject					ob;
    short m_connected[2];
    short changed;
    float m_y1, m_y2, m_a0, m_b1, m_b2, m_freq;
    float mRadiansPerSample, mFilterSlope;
    int mFilterLoops, mFilterRemain;
} t_hpf;

t_class *hpf_class;

//////////////////////////////////////////////////////////////////////////////////////////////////////

void *hpf_new   (double freq    );
void hpf_free   (t_hpf *x);
void hpf_assist (t_hpf *x, void *b, long m, long a, char *s);
void hpf_float  (t_hpf *x, double f);
void hpf_int    (t_hpf *x, long i);
void hpf_dsp    (t_hpf *x, t_signal **sp, short *count);
t_int *hpf_perform(t_int *w);

//////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void)
{	
	t_class *c;
    
	c = class_new("sc.hpf~", (method)hpf_new, (method)dsp_free, (long)sizeof(t_hpf), 0L, A_DEFFLOAT, 0);
	
	class_addmethod(c, (method)hpf_dsp,		"dsp",		A_CANT, 0);
	class_addmethod(c, (method)hpf_assist,    "assist",	A_CANT, 0);
    class_addmethod(c, (method)hpf_float, "float", A_FLOAT, 0);
    class_addmethod(c, (method)hpf_int, "int", A_LONG, 0);
    
	class_dspinit(c);				
	class_register(CLASS_BOX, c);
	hpf_class = c;
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void hpf_int(t_hpf *x, long i)
{
    hpf_float(x, i);
}

void hpf_float(t_hpf *x, double f)
{
    long in = proxy_getinlet((t_object *)x);
    if(in==1){
        x->m_freq = f;
        x->changed = 1;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void hpf_dsp(t_hpf *x, t_signal **sp, short *count)
{
    x->mRadiansPerSample = sc_radiansPerSample();
    x->mFilterSlope = sc_filterSlope();
    x->mFilterLoops = sc_filterLoops();
    x->mFilterRemain = sc_filterRemain();
    
    x->m_connected[0] = count[0];
    x->m_connected[1] = count[1];
    
    x->m_a0 = 0.f;
	x->m_b1 = 0.f;
	x->m_b2 = 0.f;
	x->m_y1 = 0.f;
	x->m_y2 = 0.f;
    
    x->changed = 1;
        
    dsp_add(hpf_perform, 4, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec);
}

t_int *hpf_perform(t_int *w)
{
    t_hpf *x = (t_hpf*) w[1];
    t_float *in = (t_float*)w[2];
    t_float freq = x->m_connected[1] ? (*(t_float *)w[3]) : x->m_freq;
    t_float *out = (t_float*)w[4];
    
    float y0;
    float y1 = x->m_y1;
    float y2 = x->m_y2;
    float a0 = x->m_a0;
    float b1 = x->m_b1;
    float b2 = x->m_b2;
    
    if (x->ob.z_disabled) goto ending;
    
    freq = freq< 10.f ? 10.f : freq;
    
	if (freq != x->m_freq || x->changed) {
        
		float pfreq = freq * x->mRadiansPerSample * 0.5;
        
		float C = tan(pfreq);
		float C2 = C * C;
		float sqrt2C = C * sqrt2_f;
		float next_a0 = 1.f / (1.f + sqrt2C + C2);
		float next_b1 = 2.f * (1.f - C2) * next_a0 ;
		float next_b2 = -(1.f - sqrt2C + C2) * next_a0;
        
		float a0_slope = (next_a0 - a0) * x->mFilterSlope;
		float b1_slope = (next_b1 - b1) * x->mFilterSlope;
		float b2_slope = (next_b2 - b2) * x->mFilterSlope;
		LOOP(x->mFilterLoops,
             y0 = ZXP(in) + b1 * y1 + b2 * y2;
             ZXP(out) = a0 * (y0 - 2.f * y1 + y2);
             
             y2 = ZXP(in) + b1 * y0 + b2 * y1;
             ZXP(out) = a0 * (y2 - 2.f * y0 + y1);
             
             y1 = ZXP(in) + b1 * y2 + b2 * y0;
             ZXP(out) = a0 * (y1 - 2.f * y2 + y0);
             
             a0 += a0_slope;
             b1 += b1_slope;
             b2 += b2_slope;
             );
		LOOP(x->mFilterRemain,
             y0 = ZXP(in) + b1 * y1 + b2 * y2;
             ZXP(out) = a0 * (y0 - 2.f * y1 + y2);
             y2 = y1;
             y1 = y0;
             );
        
		x->m_freq = freq;
		x->m_a0 = a0;
		x->m_b1 = b1;
		x->m_b2 = b2;
        x->changed = 0;
	} else {
		LOOP(x->mFilterLoops,
             y0 = ZXP(in) + b1 * y1 + b2 * y2;
             ZXP(out) = a0 * (y0 - 2.f * y1 + y2);
             
             y2 = ZXP(in) + b1 * y0 + b2 * y1;
             ZXP(out) = a0 * (y2 - 2.f * y0 + y1);
             
             y1 = ZXP(in) + b1 * y2 + b2 * y0;
             ZXP(out) = a0 * (y1 - 2.f * y2 + y0);
             );
		LOOP(x->mFilterRemain,
             y0 = ZXP(in) + b1 * y1 + b2 * y2;
             ZXP(out) = a0 * (y0 - 2.f * y1 + y2);
             y2 = y1;
             y1 = y0;
             );
	}
	x->m_y1 = zapgremlins(y1);
	x->m_y2 = zapgremlins(y2);
    
ending:
    return w+5;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void hpf_assist(t_hpf *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_INLET) { //inlet
        switch (a) {
            case 0:
                sprintf(s, "(signal) Something to filter");
                break;
            case 1:
                sprintf(s, "(signal/float) Filter freq");
                break;
        }
		
	} 
	else {	// outlet
		sprintf(s, "(signal) Filtered Signal"); 			
	}
}

void *hpf_new(double freq)
{
	t_hpf *x = NULL;
	x = (t_hpf *)object_alloc(hpf_class);
    
	if (x) {
        // number of inlets
		dsp_setup((t_pxobject *)x, 2);
        
        outlet_new((t_object *)x, "signal");
        
		x->m_freq = freq;
        x->changed = 1;
	}
	return (x);
}
