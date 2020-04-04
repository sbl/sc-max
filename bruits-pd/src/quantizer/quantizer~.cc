#include "br_quantizer.h"
#include "m_pd.h"
#include "quantizer_scales.h"

#define CLAMP(v, lower, upper)                                                 \
    if (v < lower) {                                                           \
        v = lower;                                                             \
    } else if (v > upper) {                                                    \
        v = upper;                                                             \
    };

static t_class *quantizer_tilde_class;

typedef struct _quantizer {
    t_object x_obj;
    t_float f; // dummy audio inlet

    braids::Quantizer quant;
} t_quantizer;

extern "C" {
void quantizer_tilde_setup();
void *quantizer_new(void);
void quantizer_set_scale(t_quantizer *x, float scale);
}

void *quantizer_new(void)
{
    t_quantizer *x = (t_quantizer *)pd_new(quantizer_tilde_class);

    outlet_new(&x->x_obj, &s_signal);

    x->quant.Init();
    quantizer_set_scale(x, 1);

    return x;
}

void quantizer_set_scale(t_quantizer *x, float scale_index)
{
    size_t s = static_cast<size_t>(scale_index);
    CLAMP(s, 0, braids::scales.size());

    x->quant.Configure(braids::scales[s]);
}

t_int *quantizer_perform(t_int *w)
{
    t_quantizer *x = (t_quantizer *)(w[1]);
    t_sample *in = (t_sample *)(w[2]);
    t_sample *out = (t_sample *)(w[3]);
    int n = (int)(w[4]);

    while (n--) {
        *out-- = x->quant.Process(*in--);
    }

    return (w + 5);
}

void quantizer_dsp(t_quantizer *x, t_signal **sp)
{
    dsp_add(quantizer_perform, 4, x, sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);
}

void quantizer_tilde_setup()
{
    quantizer_tilde_class =
        class_new(gensym("quantizer~"), (t_newmethod)quantizer_new, nullptr,
                  sizeof(t_quantizer), CLASS_DEFAULT, A_NULL);

    class_addmethod(quantizer_tilde_class, (t_method)quantizer_dsp,
                    gensym("dsp"), A_CANT, 0);
    class_addmethod(quantizer_tilde_class, (t_method)quantizer_set_scale,
                    gensym("scale"), A_FLOAT, 0);

    CLASS_MAINSIGNALIN(quantizer_tilde_class, t_quantizer, f);
}
