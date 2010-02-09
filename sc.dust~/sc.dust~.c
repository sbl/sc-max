/*

 dust~
 (c) stephen lumenta under GPL
 http://www.gnu.org/licenses/gpl.html
 
 part of sc-max
 see README
*/

#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct _dust 
{
	t_pxobject					ob;	
	float						offset; 
} t_dust;

void *dust_class;

//////////////////////////////////////////////////////////////////////////////////////////////////////

void *dust_new(t_symbol *s, long argc, t_atom *argv);
void dust_free(t_dust *x);
void dust_assist(t_dust *x, void *b, long m, long a, char *s);

void dust_float(t_dust *x, double f);

void dust_dsp(t_dust *x, t_signal **sp, short *count);
t_int *dust_perform(t_int *w);

//////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void)
{	
	// object initialization, note the use of dsp_free for the freemethod, which is required
	// unless you need to free allocated memory, in which case you should call dsp_free from
	// your custom free function.

	t_class *c;
	
	c = class_new("sc.dust~", (method)dust_new, (method)dsp_free, (long)sizeof(t_dust), 0L, A_GIMME, 0);
	
	class_addmethod(c, (method)dust_float,		"float",	A_FLOAT, 0);
	class_addmethod(c, (method)dust_dsp,		"dsp",		A_CANT, 0);
	class_addmethod(c, (method)dust_assist,     "assist",	A_CANT, 0);
	
	class_dspinit(c);				// new style object version of dsp_initclass();
	class_register(CLASS_BOX, c);	// register class as a box class
	dust_class = c;
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void dust_float(t_dust *x, double f){
	x->offset = f;
}

void dust_dsp(t_dust *x, t_signal **sp, short *count){
	post("my sample rate is: %f", sp[0]->s_sr);	
	dsp_add(dust_perform, 4, x, sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);
}

t_int *dust_perform(t_int *w){
	t_dust *x = (t_dust *)(w[1]);
	t_float *inL = (t_float *)(w[2]);
	t_float *outL = (t_float *)(w[3]);
	int n = (int)w[4];
	
	while (n--)
		*outL++ = *inL++ + x->offset;
		
	// you have to return the NEXT pointer in the array OR MAX WILL CRASH
	return w + 5;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void dust_assist(t_dust *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_INLET) { //inlet
		sprintf(s, "I am inlet %ld", a);
	} 
	else {	// outlet
		sprintf(s, "I am outlet %ld", a); 			
	}
}

// NOT CALLED!, we use dsp_free for a generic free function
void dust_free(t_dust *x) {
	;
}

void *dust_new(t_symbol *s, long argc, t_atom *argv){
	t_dust *x = NULL;
	
	if (x = (t_dust *)object_alloc(dust_class)) {
		dsp_setup((t_pxobject *)x, 1);	// MSP inlets: arg is # of inlets and is REQUIRED! 
										// use 0 if you don't need inlets
		outlet_new(x, "signal"); // signal outlet (note "signal" rather than NULL)
		x->offset = 0.;
	}
	return (x);
}
