/*

 sc.whitenoise~
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

typedef struct _whitenoise 
{
	t_pxobject					ob;
    RGen                        rgen;
} t_whitenoise;

t_class *whitenoise_class;

//////////////////////////////////////////////////////////////////////////////////////////////////////

void *whitenoise_new(long argc, t_atom *argv);
void whitenoise_free(t_whitenoise *x);
void whitenoise_assist(t_whitenoise *x, void *b, long m, long a, char *s);

void whitenoise_dsp(t_whitenoise *x, t_signal **sp, short *count);
t_int *whitenoise_perform(t_int *w);

//////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void){	
	t_class *c;
    
	c = class_new("sc.whitenoise~", (method)whitenoise_new, (method)dsp_free, (long)sizeof(t_whitenoise), 0L, A_GIMME, 0);
	
	class_addmethod(c, (method)whitenoise_dsp,		"dsp",		A_CANT, 0);
	class_addmethod(c, (method)whitenoise_assist,    "assist",	A_CANT, 0);
    
	class_dspinit(c);				
	class_register(CLASS_BOX, c);
	whitenoise_class = c;
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void whitenoise_dsp(t_whitenoise *x, t_signal **sp, short *count){
	dsp_add(whitenoise_perform, 3, x, sp[0]->s_vec, sp[0]->s_n);
}

t_int *whitenoise_perform(t_int *w){
    t_whitenoise *x = (t_whitenoise *)(w[1]);	
    t_float *out = (t_float *)(w[2]);
	int n = (int)w[3];
    
    if (x->ob.z_disabled)
        return w + 4;    
    
    RGET
    
	while (n--){
		*out++ = frand2(s1, s2, s3);
    }
    
    RPUT
    
    
	return w + 4;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void whitenoise_assist(t_whitenoise *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_INLET) { //inlet
		sprintf(s, "Ignore this inlet");
	} 
	else {	// outlet
		sprintf(s, "(signal) White Noise"); 			
	}
}

void *whitenoise_new(long argc, t_atom *argv){
	t_whitenoise *x = NULL;
	x = (t_whitenoise *)object_alloc(whitenoise_class);
    
	if (x) {
		dsp_setup((t_pxobject *)x, 0);
        
        outlet_new((t_object *)x, "signal");
        
        x->rgen.init(time(NULL));
	}
	return (x);
}
