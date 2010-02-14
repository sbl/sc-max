/*
 sc.range~
 
 Not really a SC Ugen. A convenience to scale a signal to determined values.
 Scaling can be for polar (0 - 1) or bipolar (-1 - 1) values.
 
 We use attributes and only floats to set the boundaries since this is mostly not modulated.

 (c) stephen lumenta under GPL
 http://www.gnu.org/licenses/gpl.html
 
 part of sc-max http://github.com/sbl/sc-max
 see README
*/

#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct _range 
{
	t_pxobject      ob;
    short           m_connected;
    char            m_mode; // 0: bipolar (default) 1: unipolar
    float           m_high, m_low; // boundaries
    
} t_range;

t_class *range_class;

//////////////////////////////////////////////////////////////////////////////////////////////////////

void    *range_new(t_symbol *s, long ac, t_atom *av);
void    range_assist(t_range *x, void *b, long m, long a, char *s);

void    range_dsp(t_range *x, t_signal **sp, short *count);
t_int   *range_perform(t_int *w);

//////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void){	
	t_class *c;
    
	c = class_new("sc.range~", (method)range_new, (method)dsp_free, (long)sizeof(t_range), 0L, A_GIMME, 0);
	
	class_addmethod(c, (method)range_dsp,		"dsp",		A_CANT, 0);
	class_addmethod(c, (method)range_assist,    "assist",	A_CANT, 0);
    
    CLASS_ATTR_FLOAT        (c, "low",      ATTR_FLAGS_NONE, t_range, m_low);
    CLASS_ATTR_FLOAT        (c, "high",     ATTR_FLAGS_NONE, t_range, m_high);
    
    CLASS_ATTR_CHAR         (c, "mode",     ATTR_FLAGS_NONE, t_range, m_mode);
    CLASS_ATTR_STYLE_LABEL  (c, "mode",     ATTR_FLAGS_NONE, "onoff", "mode");
    
	class_dspinit(c);				
	class_register(CLASS_BOX, c);
	range_class = c;
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void range_dsp(t_range *x, t_signal **sp, short *count){
	x->m_connected = count[0];
    
    dsp_add(range_perform, 4, x, sp[0]->s_vec, sp[1]->s_vec, sp[1]->s_n);
}

t_int *range_perform(t_int *w){
    t_range *x      = (t_range *)(w[1]);	
    t_float *in     = (t_float *)(w[2]);
    t_float *out    = (t_float *)(w[3]);
	int     n       = (int)w[4];
    float   hi      = x->m_high; 
    float   lo      = x->m_low;

    if (x->ob.z_disabled) return w + 5;    
    
    
	while (n--){
        float   mul, add;
        
        // bipolar 
        if(x->m_mode == 0){
            mul = (hi - lo) * 0.5f;
            add = mul + lo;
        }
        // unipolar
        else {
            mul = hi - lo;
            add = lo;
        }
        
        *out++ = (t_float) (*in++ * mul) + add;
    }
    
	return w + 5;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void range_assist(t_range *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_INLET) { //inlet
		sprintf(s, "(signal) something to scale");
	} 
	else {	// outlet
		sprintf(s, "(signal) scaled signal"); 			
	}
}

void *range_new(t_symbol *s, long ac, t_atom *av){
	t_range *x = NULL;
	x = (t_range *)object_alloc(range_class);
    
	if (x) {
		dsp_setup((t_pxobject *)x, 1);
        
        x->m_low    = 0.f;
        x->m_high   = 1.f;
        x->m_mode   = 0; // bipolar
        
        attr_args_process(x, ac, av);
        
        outlet_new((t_object *)x, "signal");
	}
	return (x);
}