
//--------------------------------------

#include <cmath>

// simple one-pole lowpass IIR filter.
class OnePoleSmoother {
    float sr;
    float c;
    float x0;
    float y0;
    float t;

private:
    void calcCoeff() {
        c = expf(-6.9f / (t * sr));
    }

public:
    void init(float samplerate, float time) {
        sr = samplerate;
        t = time;
        calcCoeff();
    }

    static float s(float x, float x0, float c) {
        return x + c * (x0 - x);
    }

    void setSampleRate(float samplerate) {
        sr = samplerate;
        calcCoeff();
    }

    void setTimeConstant(float time) {
        t = time;
        calcCoeff();
    }

    void setTarget(float x) {
        x0 = x;
    }

    // get new value without new input
    float processSample() {
        y0 = smooth(x0, y0, c);
        return y0;
    }

    // get new value with input
    float processSample(float x) {
        setTarget(x);
        return processSample();
    }

    OnePoleSmoother() {
        x0 = 0.f;
        y0 = 0.f;
        c = 0.f;
        sr = 1.f;
        t = 0.f;
    }

};


//--------------------------------------

// generic 3rd-order filter section with poles and zeroes
class ThirdOrderFilter {
    // feedforward coefficients
    float b[3];
    // feedback coefficients
    float a[3];

    // feedforward history
    float xn[3];
    // feedback history
    float yn[3];

private:
    // apply coefficients to history
    float computeOutput() {
        float y = 0;
        for (int i = 0; i < 3; ++i) {
            y += b[i] * xn[i];
            y -= a[i] * yn[i];
        }
        return y;
    }

    void pushInput(float x) {
        for (int i = 0; i < 2; ++i) {
            xn[i + 1] = xn[i];
        }
        xn[0] = x;
    }

    void pushOutput(float y) {
        for (int i = 0; i < 2; ++i) {
            yn[i + 1] = yn[i];
        }
        yn[0] = y;
    }

public:
    // set raw coefficients
    void setCoefficients(const float *ff, const float *fb) {
        for (int i = 0; i < 3; ++i) {
            b[i] = ff[i];
            a[i] = fb[i];
        }
    }

    float processSample() {
        float y = computeOutput();
        pushOutput(y);
        return y;
    }

    float processSample(float x) {
        //pushInput(x);
        float y = processSample();
        // hm...
        pushInput(x);
        return y;
    }

    ThirdOrderFilter() {
        for (int i = 0; i < 3; ++i) {
            b[i] = 0.f;
            a[i] = 0.f;
            xn[i] = 0.f;
            yn[i] = 0.f;
        }
    }
};

//--------------------------------------
// FIXME: needs work, or remove
class ExpLogSmoother {
    ThirdOrderFilter attack;
    OnePoleSmoother decay;
    float x0;
    float y0;
public:
    ExpLogSmoother() {
        x0 = 0.f;
        y0 = 0.f;
        /// test:
        //
        // our theory is that we can implement perceptually equivalent fadein/fadeout
        // by using a decay smoother with exponential impulse response,
        // and attack smoother with (approximate) "inverse exponential" IR.
        //
        // the decay smoother can be first order,
        // attack smoother needs higher order.
        //
        // these coefficient values were found by numerical search,
        // and correspond to a time constant of ~0.233s at 48k samplerate.
        //
        const float sr = 48000.f;
        const float t = 2.3357214690901213;
        const float b[] = {6.204428618741245e-05, -0.0003338094865564586, 9.113963493779792e-05};
        const float a[] = {1.0000591837756803, -0.0001465436571428774, 0.0002057771942725899};

        attack.setCoefficients(b, a);
        decay.init(sr, t);
    }


    // process with input
    float processSample(float x) {
        float y;

#if 1 // testing..
        if (x > y0) {
            y = attack.processSample(x);
        } else {
            y = decay.processSample(x);
        }
#else
        //y = decay.processSample(x);
        y = attack.processSample(x);
#endif
        x0 = x;
        y0 = y;
        return y;
    }


    // process with no input
    float processSample() {
        float y;
        if (x0 > y0) {
            y = attack.processSample();
        } else {
            y = decay.processSample();
        }
        y0 = y;
        return y;
    }
};
