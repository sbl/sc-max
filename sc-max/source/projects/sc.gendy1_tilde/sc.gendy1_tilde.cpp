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

 Gendyn SuperColluder UGens implemented by Nick Collins (sicklincoln.org)
 Most of the comments are from his original c++ code

 */

/*
 sc.gendy1~
 (c) stephen lumenta under GPL
 http://www.gnu.org/licenses/gpl.html

 part of sc-max http://github.com/sbl/sc-max
 see README
 */

#include "c74_msp.h"
#include "scmax.h"

using namespace c74::max;

#define CONTROL_POINTS 12;

static t_class *gendy_class = nullptr;

// Iannis Xenakis/Marie-Helene Serra GENDYN simulation
struct t_gendy {
    t_pxobject ob;

    // these are set from the outside
    long g_ampdist, g_durdist;
    double g_adparam, g_ddparam;
    double g_minfreq, g_maxfreq;
    double g_ampscale, g_durscale;
    int g_cps, g_knum;   // defaults to 12

    double mPhase;
    double mFreqMul, mAmp, mNextAmp, mSpeed, mDur;
    long mIndex;

    double *mMemoryAmp;
    double *mMemoryDur;

    RGen rgen;
};

void *gendy_new(t_symbol *s, long ac, t_atom *av){
    int init_cps;
    t_gendy *self = (t_gendy *)object_alloc(gendy_class);
    
    if (!self) return nullptr;
    
    dsp_setup((t_pxobject *)self, 0);
    self->mFreqMul = (double) 1.0/sys_getsr();
    self->mPhase = 1.0;  //should immediately decide on new target
    self->mAmp = 0.0;
    self->mNextAmp = 0.0;
    self->mSpeed = 100;
    self->mIndex = 0;
    
    // if the first value is a long use it as our number of cps
    // non incremented av always points to the first value in the atom array
    if (attr_args_offset(ac, av) > 0 && atom_gettype(av) == A_LONG) {
        init_cps = atom_getlong(av);
        
        if(init_cps>0){
            self->g_cps = init_cps;
            object_post((t_object*) self, "number of cps: %d", self->g_cps);
        } else {
            self->g_cps = CONTROL_POINTS;
            object_error((t_object*) self, "number of cps too small, setting to default (12)");
        }
    } else {
        self->g_cps = CONTROL_POINTS;
        object_post((t_object*)self, "no number of cps supplied, using the default (12)");
    }
    
    self->mMemoryAmp = (double*)sysmem_newptr(self->g_cps * sizeof(double));
    self->mMemoryDur = (double*)sysmem_newptr(self->g_cps * sizeof(double));
    
    // defaults
    self->g_ampdist = 0;
    self->g_durdist = 0;
    self->g_adparam = 1.0;
    self->g_ddparam = 1.0;
    self->g_minfreq = 440.0;
    self->g_maxfreq = 660.0;
    self->g_ampscale = 0.5;
    self->g_durscale = 0.5;
    
    // process the attributes after the defaults have been set
    attr_args_process(self, ac, av);
    
    // set up the random generator
    self->rgen.init(sc_randomSeed());
    
    //initialise to zeroes and separations
    int i=0;
    for(i=0; i < self->g_cps; ++i) {
        self->mMemoryAmp[i] = 2 * self->rgen.drand() - 1.0;
        self->mMemoryDur[i] = self->rgen.drand();
    }
    
    outlet_new((t_object *)self, "signal");

    return self;
}

void gendy_free(t_gendy *self) {
    dsp_free((t_pxobject *)self);
    sysmem_freeptr(self->mMemoryAmp);
    sysmem_freeptr(self->mMemoryDur);
}

double gendy_distribution(int which, double a, double f) {
    double temp, c;
    
    if(a>1.0) a=1.0;       //a must be in range 0 to 1
    if(a<0.0001) a=0.0001;  //for safety with some distributions, don't want divide by zero errors
    
    switch (which) {
        case 0: //LINEAR
            //linear
            break;
        case 1: //CAUCHY
            //X has a*tan((z-0.5)*pi)
            //I went back to first principles of the Cauchy distribution and re-integrated with a
            //normalisation constant
            
            //choice of 10 here is such that f=0.95 gives about 0.35 for temp, could go with 2 to make it finer
            c= atan(10*a);    //PERHAPS CHANGE TO a=1/a;
            //incorrect- missed out divisor of pi in norm temp= a*tan(c*(2*pi*f - 1));
            temp= (1/a)*tan(c*(2*f - 1)); //Cauchy distribution, C is precalculated
            
            //printf("cauchy f %f c %f temp %f out %f \n",f,  c, temp, temp/10);
            
            return temp*0.1; //(temp+100)/200;
            
        case 2: //LOGIST (ic)
            //X has -(log((1-z)/z)+b)/a which is not very usable as is
            
            c = 0.5+(0.499*a); //calculate normalisation constant
            c = log((1-c)/c);
            
            //remap into range of valid inputs to avoid infinities in the log
            
            //f= ((f-0.5)*0.499*a)+0.5;
            f = ((f-0.5)*0.998*a)+0.5; //[0,1]->[0.001,0.999]; squashed around midpoint 0.5 by a
            //Xenakis calls this the LOGIST map, it's from the range [0,1] to [inf,0] where 0.5->1
            //than take natural log. to avoid infinities in practise I take [0,1] -> [0.001,0.999]->[6.9,-6.9]
            //an interesting property is that 0.5-e is the reciprocal of 0.5+e under (1-f)/f
            //and hence the logs are the negative of each other
            temp = log((1-f)/f)/c; //n range [-1,1]
            //X also had two constants in his- I don't bother
            
            //printf("logist f %f temp %f\n", f, temp);
            
            return temp; //a*0.5*(temp+1.0);  //to [0,1]
            
        case 3: //HYPERBCOS
            //X original a*log(tan(z*pi/2)) which is [0,1]->[0,pi/2]->[0,inf]->[-inf,inf]
            //unmanageable in this pure form
            c=tan(1.5692255*a);    //tan(0.999*a*pi*0.5);     //[0, 636.6] maximum range
            temp= tan(1.5692255*a*f)/c; //[0,1]->[0,1]
            temp= log(temp*0.999+0.001)*(-0.1447648);  // multiplier same as /(-6.9077553); //[0,1]->[0,1]
            
            //printf("hyperbcos f %f c %f temp %f\n", f, c, temp);
            
            return 2*temp-1.0;
            
        case 4: //ARCSINE
            //X original a/2*(1-sin((0.5-z)*pi)) aha almost a better behaved one though [0,1]->[2,0]->[a,0]
            c= sin(1.5707963*a); //sin(pi*0.5*a); //a as scaling factor of domain of sine input to use
            temp= sin(pi*(f-0.5)*a)/c; //[-1,1] which is what I need
            
            //printf("arcsine f %f c %f temp %f\n", f, c, temp);
            
            return temp;
            
        case 5: //EXPON
            //X original -(log(1-z))/a [0,1]-> [1,0]-> [0,-inf]->[0,inf]
            c= log(1.0-(0.999*a));
            temp= log(1.0-(f*0.999*a))/c;
            
            //printf("expon f %f c %f temp %f\n", f, c, temp);
            
            return 2*temp-1.0;
            
        case 6: //SINUS
            //X original a*sin(smp * 2*pi/44100 * b) ie depends on a second oscillator's value-
            //hmmm, plug this in as a I guess, will automatically accept control rate inputs then!
            return 2*a-1.0;
            
        default:
            break;
    }
    
    return 2*f-1.0;
}

void gendy_perform64(t_gendy* self,
                     t_object* dsp64,
                     double** ins,
                     long numins,
                     double** outs,
                     long numouts,
                     long sampleframes,
                     long flags,
                     void* userparam) {
    double* out = outs[0];
    int n = sampleframes;
    

    if (self->ob.z_disabled) return ;

    // these are all set via max messages
    long whichamp = self->g_ampdist;
    long whichdur = self->g_durdist;
    double aamp = self->g_adparam;
    double adur = self->g_ddparam;
    double minfreq = self->g_minfreq;
    double maxfreq = self->g_maxfreq;
    double scaleamp = self->g_ampscale;
    double scaledur = self->g_durscale;

    double rate = self->mDur;
    //phase gives proportion for linear interpolation automatically
    double phase = self->mPhase;
    double amp = self->mAmp;
    double nextamp = self->mNextAmp;
    double speed = self->mSpeed;

    while (n--){
        double z;

        if (phase >= 1.f) {
            phase -= 1.f;

            long index   = self->mIndex;
            long num     = self->g_knum;

            if((num>(self->g_cps)) || (num<1)) num=self->g_cps; // clip

            index=(index+1) % num;
            amp=nextamp;

            self->mIndex=index;

            //Gendy dist gives value [-1,1], then use scaleamp
            //first term was amp before, now must check new memory slot
            nextamp= (self->mMemoryAmp[index])+(scaleamp*gendy_distribution(whichamp, aamp,self->rgen.frand()));

            //mirroring for bounds- safe version
            if(nextamp>1.0 || nextamp<-1.0) {

                //to force mirroring to be sensible
                if(nextamp<0.0) nextamp=nextamp+4.0;

                nextamp=fmod(nextamp,4.0f);

                if(nextamp>1.0 && nextamp<3.0)
                    nextamp= 2.0-nextamp;

                else if(nextamp>1.0)
                    nextamp=nextamp-4.0;
            };

            self->mMemoryAmp[index]= nextamp;

            //Gendy dist gives value [-1,1]
            rate= (self->mMemoryDur[index])+(scaledur*gendy_distribution(whichdur, adur, self->rgen.frand()));

            if(rate>1.0 || rate<0.0) {
                if(rate<0.0) rate=rate+2.0;
                rate= fmod(rate,2.0f);
                rate= 2.0-rate;
            }

            self->mMemoryDur[index]= rate;

            //define range of speeds (say between 20 and 1000 Hz)
            //can have bounds as fourth and fifth inputs
            speed=  (minfreq+((maxfreq-minfreq)*rate))*(self->mFreqMul);

            //if there are 12 control points in memory, that is 12 per cycle
            //the speed is multiplied by 12
            //(I don't store this because updating rates must remain in range [0,1]
            speed *= num;
        }

        //linear interpolation could be changed
        z = ((1.0-phase)*amp) + (phase*nextamp);

        phase   +=  speed;
        *out++   =  z;
    }

    self->mPhase = phase;
    self->mAmp =  amp;
    self->mNextAmp = nextamp;
    self->mSpeed = speed;
    self->mDur = rate;
}

void gendy_dsp64(t_gendy *self, t_object* dsp64, short* count, double samplerate, long maxvectorsize, long flags) {
    self->mFreqMul = (double) 1.0 / samplerate;
    object_method_direct(void, (t_object*, t_object*, t_perfroutine64, long, void*),
                         dsp64, gensym("dsp_add64"), (t_object*)self, (t_perfroutine64)gendy_perform64, 0, NULL);
}

void gendy_assist(t_gendy *self, void *b, t_assist_function io, long a, char *s) {
    if (io == ASSIST_INLET) { //inlet
        sprintf(s, "messages to gendy");
    }
    else {  // outlet
        sprintf(s, "(signal) Gendy");
    }
}

void ext_main(void *r) {
    t_class *c;
    c = class_new("sc.gendy1~", (method)gendy_new, (method)gendy_free, (long)sizeof(t_gendy), 0L, A_GIMME, 0);
    
    class_addmethod(c, (method)gendy_dsp64, "dsp64", A_CANT, 0);
    class_addmethod(c, (method)gendy_assist, "assist", A_CANT, 0);
    
    // the gendy parameters are set via attributes
    
    CLASS_ATTR_LONG(c, "ampdist", ATTR_FLAGS_NONE, t_gendy, g_ampdist);
    CLASS_ATTR_FILTER_CLIP(c, "ampdist",  0, 6);
    CLASS_ATTR_ORDER(c, "ampdist", ATTR_FLAGS_NONE, "1");
    
    CLASS_ATTR_LONG(c, "durdist", ATTR_FLAGS_NONE, t_gendy, g_durdist);
    CLASS_ATTR_FILTER_CLIP(c, "durdist",  0, 6);
    CLASS_ATTR_ORDER(c, "durdist", ATTR_FLAGS_NONE, "2");
    
    CLASS_ATTR_DOUBLE(c, "adparam", ATTR_FLAGS_NONE, t_gendy, g_adparam);
    CLASS_ATTR_ORDER(c, "adparam", ATTR_FLAGS_NONE, "3");
    
    CLASS_ATTR_DOUBLE(c, "ddparam", ATTR_FLAGS_NONE, t_gendy, g_ddparam);
    CLASS_ATTR_ORDER(c, "ddparam", ATTR_FLAGS_NONE, "4");
    
    CLASS_ATTR_DOUBLE(c, "minfreq", ATTR_FLAGS_NONE, t_gendy, g_minfreq);
    CLASS_ATTR_FILTER_MIN(c, "minfreq",  1.f);
    CLASS_ATTR_ORDER(c, "minfreq", ATTR_FLAGS_NONE, "5");
    
    CLASS_ATTR_DOUBLE(c, "maxfreq", ATTR_FLAGS_NONE, t_gendy, g_maxfreq);
    CLASS_ATTR_FILTER_MIN(c, "maxfreq",  1.f);
    CLASS_ATTR_ORDER(c, "maxfreq", ATTR_FLAGS_NONE, "6");
    
    CLASS_ATTR_DOUBLE(c, "ampscale", ATTR_FLAGS_NONE, t_gendy, g_ampscale);
    CLASS_ATTR_FILTER_CLIP(c, "ampscale", 0.f, 1.f);
    CLASS_ATTR_ORDER(c, "ampscale", ATTR_FLAGS_NONE, "7");
    
    CLASS_ATTR_DOUBLE(c, "durscale", ATTR_FLAGS_NONE, t_gendy, g_durscale);
    CLASS_ATTR_FILTER_CLIP(c, "durscale", 0.f, 1.f);
    CLASS_ATTR_ORDER(c, "durscale", ATTR_FLAGS_NONE, "8");
    
    CLASS_ATTR_LONG(c, "knum",  ATTR_FLAGS_NONE, t_gendy, g_knum);
    CLASS_ATTR_ORDER(c, "knum", ATTR_FLAGS_NONE, "9");
    
    class_dspinit(c);
    class_register(CLASS_BOX, c);
    gendy_class = c;
}
