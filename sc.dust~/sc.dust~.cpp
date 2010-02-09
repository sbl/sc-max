/*

 sc.dust~
 (c) stephen lumenta under GPL
 http://www.gnu.org/licenses/gpl.html
 
 part of sc-max http://github.com/sbl/sc-max
 see README
*/

#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"
#include "rgen_helper.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct _dust 
{
	t_pxobject		m_ob;
    float           m_density;
    float           m_thresh;
    float           m_scale;
    float           m_sr;
    float           m_isr;
    RGen            rgen;
} t_dust;

t_class *dust_class;

//////////////////////////////////////////////////////////////////////////////////////////////////////

void *dust_new(double density);
void dust_free(t_dust *x);
void dust_assist(t_dust *x, void *b, long m, long a, char *s);
void dust_float(t_dust *x, double density);
void dust_int(t_dust *x, long density);
void dust_calc_density(t_dust *x);

void dust_dsp(t_dust *x, t_signal **sp, short *count);
t_int *dust_perform(t_int *w);

//////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void){	
	t_class *c;
    
	c = class_new("sc.dust~", (method)dust_new, (method)dsp_free, (long)sizeof(t_dust), 0L, A_DEFFLOAT, 0);
	
	class_addmethod(c, (method)dust_dsp,		"dsp",		A_CANT, 0);
	class_addmethod(c, (method)dust_assist,     "assist",	A_CANT, 0);
    class_addmethod(c, (method)dust_float,      "float",    A_FLOAT, 0);
    class_addmethod(c, (method)dust_int,        "int",      A_LONG, 0);
    
	class_dspinit(c);				
	class_register(CLASS_BOX, c);
	dust_class = c;
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////


void dust_float(t_dust *x, double density){
    x->m_density = density;
    dust_calc_density(x);
}

void dust_int(t_dust *x, long density){
    x->m_density = (float) density;
    dust_calc_density(x);
}

void dust_dsp(t_dust *x, t_signal **sp, short *count){
    x->m_sr = sp[0]->s_sr;
    x->m_isr = 1.0f / x->m_sr;
    
    dust_calc_density(x);
	dsp_add(dust_perform, 3, x, sp[0]->s_vec, sp[0]->s_n);
}

void dust_calc_density(t_dust *x){
    x->m_thresh = x->m_density * x->m_isr;
    x->m_scale  = x->m_thresh > 0.f ? 1.f / x->m_thresh : 0.f;
}

t_int *dust_perform(t_int *w){
    t_dust *x = (t_dust *)(w[1]);
    t_float *out = (t_float *)(w[2]);
	int n = (int)w[3];
    
    if (x->m_ob.z_disabled)
        return w + 4;
    
    RGET
        
	while (n--){
        float z = frand(s1, s2, s3);
        
        if (z < x->m_thresh) *out = z * x->m_scale;
		else  *out = 0.f;

		*out++;
    }
    
    RPUT
        
	return w + 4;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void dust_assist(t_dust *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_INLET) { //inlet
		sprintf(s, "(int/float) density");
	} 
	else {	// outlet
		sprintf(s, "(signal) Dust"); 			
	}
}

void *dust_new(double density){
	t_dust *x = NULL;
	x = (t_dust *)object_alloc(dust_class);
    
	if (x) {
        dsp_setup((t_pxobject *)x, 0);
        outlet_new((t_object *)x, "signal");

        x->m_density = density;
        
        // all calculation have to be done after a dsp chain rebuild
        x->m_sr = 44100.;
        x->m_isr = 1/x->m_sr;
        x->m_scale = 0.;
        x->m_thresh = 0.;

        x->rgen.init(time(NULL));
	}
	return (x);
}
