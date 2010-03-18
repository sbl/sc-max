/*
 sc.impulse~
 
 (c) stephen lumenta under GPL
 http://www.gnu.org/licenses/gpl.html
 
 part of sc-max http://github.com/sbl/sc-max
 see README
*/

#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct _impulse 
{
	t_pxobject      ob;
    short           m_connected[2];
    float           m_freq, m_phase;
    float           m_freqmul, m_phaseoffset; // freqmul -> inverse sample rate
} t_impulse;

t_class *impulse_class;

//////////////////////////////////////////////////////////////////////////////////////////////////////

void    *impulse_new(double freq);
void    impulse_assist(t_impulse *x, void *b, long m, long a, char *s);

void    impulse_dsp(t_impulse *x, t_signal **sp, short *count);
t_int   *impulse_perform(t_int *w);

//////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void){	
	t_class *c;
    
	c = class_new("sc.impulse~", (method)impulse_new, (method)dsp_free, (long) sizeof(t_impulse), 0L, A_DEFFLOAT, 0);
	
	class_addmethod(c, (method)impulse_dsp,     "dsp",		A_CANT, 0);
	class_addmethod(c, (method)impulse_assist,  "assist",	A_CANT, 0);
        
	class_dspinit(c);				
	class_register(CLASS_BOX, c);
	impulse_class = c;
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void impulse_dsp(t_impulse *x, t_signal **sp, short *count){
	x->m_connected[0] = count[0];
	x->m_connected[1] = count[1];
    
    x->m_freqmul = 1/sys_getsr();
    
    // obj, freq, phase, out, frames
    dsp_add(impulse_perform, 5, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[2]->s_n);
}

t_int *impulse_perform(t_int *w){
    t_impulse   *x        = (t_impulse *)(w[1]);	
    t_float     *in     = (t_float *)(w[2]);
    t_float     *out    = (t_float *)(w[3]);
    int         n       = (int) (w[4]);
    
    float freq, phase;
    
    
    
    if (x->ob.z_disabled) return w + 6;    
    
    
    
	while (n--){
        *out++ = 0.f;
    }
    
	return w + 6;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void impulse_assist(t_impulse *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_INLET) { //inlet
        if (a==0) {
            sprintf(s, "(signal/float) impulse freq");
        } else {
            sprintf(s, "(signal/float) impulse phase");
        }

	} 
	else {	// outlet
		sprintf(s, "(signal) impulse"); 			
	}
}

void *impulse_new(double freq){
	t_impulse *x = NULL;

	x = (t_impulse *)object_alloc(impulse_class);
    
	if (x) {
		dsp_setup((t_pxobject *)x, 2);
        
        x->m_freq = freq;
        x->m_phase = 1.f;
        x->m_freqmul = 1/sys_getsr();
        x->m_phaseoffset = 0.f;
        
        outlet_new((t_object *)x, "signal");
	}
	return (x);
}