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

#include "c74_msp.h"

using namespace c74::max;

static t_class *range_class = nullptr;

struct t_range {
    t_pxobject ob;
    short m_connected;
    char m_mode; // 0: bipolar (default) 1: unipolar
    double m_high, m_low; // boundaries
};

void *range_new(t_symbol *s, long ac, t_atom *av) {
    t_range *self = NULL;
    self = (t_range *)object_alloc(range_class);
    
    if (!self) return self;
    
    dsp_setup((t_pxobject *)self, 1);
    outlet_new((t_object *)self, "signal");
    
    self->m_low    = 0.0;
    self->m_high   = 1.0;
    self->m_mode   = 0; // bipolar
    
    attr_args_process(self, ac, av);
    
    return self;
}

void range_perform64(t_range* self,
                     t_object* dsp64,
                     double** ins,
                     long numins,
                     double** outs,
                     long numouts,
                     long sampleframes,
                     long flags,
                     void* userparam) {
    double *in = ins[0];
    double *out = outs[0];
    int n = sampleframes;
    double hi = self->m_high;
    double lo = self->m_low;

    if (self->ob.z_disabled) return ;

    while (n--){
        double mul, add;

        // bipolar
        if(self->m_mode == 0){
            mul = (hi - lo) * 0.5;
            add = mul + lo;
        }
        // unipolar
        else {
            mul = hi - lo;
            add = lo;
        }

        *out++ = (double)(*in++ * mul) + add;
    }
}

void range_dsp64(t_range *self, t_object* dsp64, short* count, double samplerate, long maxvectorsize, long flags) {
    self->m_connected = count[0];
    object_method_direct(void, (t_object*, t_object*, t_perfroutine64, long, void*),
                         dsp64, gensym("dsp_add64"), (t_object*)self, (t_perfroutine64)range_perform64, 0, NULL);
}

void range_assist(t_range *self, void *b, t_assist_function io, long a, char *s) {
    if (io == ASSIST_INLET) {
        sprintf(s, "(signal) something to scale");
    }
    else {
        sprintf(s, "(signal) scaled signal");
    }
}

void ext_main(void *r) {
    t_class *c;
    c = class_new("sc.range~", (method)range_new, (method)dsp_free, (long)sizeof(t_range), 0L, A_GIMME, 0);
    
    class_addmethod(c, (method)range_dsp64, "dsp64", A_CANT, 0);
    class_addmethod(c, (method)range_assist, "assist", A_CANT, 0);
    
    CLASS_ATTR_DOUBLE(c, "low", ATTR_FLAGS_NONE, t_range, m_low);
    CLASS_ATTR_DOUBLE(c, "high", ATTR_FLAGS_NONE, t_range, m_high);
    CLASS_ATTR_CHAR(c, "mode", ATTR_FLAGS_NONE, t_range, m_mode);
    CLASS_ATTR_STYLE_LABEL(c, "mode", ATTR_FLAGS_NONE, "onoff", "mode");
    
    class_dspinit(c);
    class_register(CLASS_BOX, c);
    range_class = c;
}
