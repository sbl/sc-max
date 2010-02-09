/*

 graynoise~
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

typedef struct _graynoise 
{
	t_pxobject					ob;
    int                         m_counter;
    RGen                        rgen;
} t_graynoise;

t_class *graynoise_class;

//////////////////////////////////////////////////////////////////////////////////////////////////////

void *graynoise_new(t_symbol *s, long argc, t_atom *argv);
void graynoise_free(t_graynoise *x);
void graynoise_assist(t_graynoise *x, void *b, long m, long a, char *s);

void graynoise_dsp(t_graynoise *x, t_signal **sp, short *count);
t_int *graynoise_perform(t_int *w);

//////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void){	
	t_class *c;
    
	c = class_new("sc.graynoise~", (method)graynoise_new, (method)dsp_free, (long)sizeof(t_graynoise), 0L, A_GIMME, 0);
	
	class_addmethod(c, (method)graynoise_dsp,		"dsp",		A_CANT, 0);
	class_addmethod(c, (method)graynoise_assist,    "assist",	A_CANT, 0);
    
	class_dspinit(c);				
	class_register(CLASS_BOX, c);
	graynoise_class = c;
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void graynoise_dsp(t_graynoise *x, t_signal **sp, short *count){
	dsp_add(graynoise_perform, 3, x, sp[0]->s_vec, sp[0]->s_n);
}

t_int *graynoise_perform(t_int *w){
    t_graynoise *x = (t_graynoise *)(w[1]);
    int counter = x->m_counter;
	
    t_float *out = (t_float *)(w[2]);
	int n = (int)w[3];
    
    RGET
    
	while (n--){
        counter ^= 1L << (trand(s1,s2,s3) & 31);
		*out++ = counter * 4.65661287308e-10f;
    }
    
    RPUT
    
    x->m_counter = counter;
    
	return w + 4;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void graynoise_assist(t_graynoise *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_INLET) { //inlet
		sprintf(s, "Ignore this inlet");
	} 
	else {	// outlet
		sprintf(s, "(signal) Gray Noise"); 			
	}
}

void *graynoise_new(t_symbol *s, long argc, t_atom *argv){
	t_graynoise *x = NULL;
	x = (t_graynoise *)object_alloc(graynoise_class);
    
	if (x) {
		dsp_setup((t_pxobject *)x, 0);
        
        outlet_new((t_object *)x, "signal");
        
        x->rgen.init(time(NULL));
        x->m_counter = 0;
	}
	return (x);
}
