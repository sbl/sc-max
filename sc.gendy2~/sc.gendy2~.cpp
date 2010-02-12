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
 
 sc.gendy2~
 (c) stephen lumenta under GPL
 http://www.gnu.org/licenses/gpl.html
 
 part of sc-max http://github.com/sbl/sc-max
 see README

*/

#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"

#include "SC_RGen.h"

# define CONTROL_POINTS 12

//////////////////////////////////////////////////////////////////////////////////////////////////////

// following Hoffmann paper from CMJ- primary and secondary random walks
typedef struct _gendy 
{
	t_pxobject          ob;
    
    // these are set from the outside
    int                 g_ampdist, g_durdist;
    float               g_adparam, g_ddparam;
    float               g_minfreq, g_maxfreq;
    float               g_ampscale, g_durscale;
    int                 g_cps, g_knum;   // defaults to 12
    //random values made using a lehmer number generator xenakis style
    float               g_a, g_c;
    
    double              mPhase;
    float               mFreqMul, mAmp, mNextAmp, mSpeed, mDur;
    int                 mIndex;
    
    float               *mMemoryAmp;
    float               *mMemoryAmpStep;
    float               *mMemoryDur;
    float               *mMemoryDurStep;
    
    // we use sc's random distribution
    RGen                rgen;
    
} t_gendy;

t_class *gendy_class;

//////////////////////////////////////////////////////////////////////////////////////////////////////

// first param is required number of cps rest is used for attributes to set gendyn values
void    *gendy_new              (t_symbol *s, long argc, t_atom *argv);
void    gendy_free              (t_gendy *x);
void    gendy_assist            (t_gendy *x, void *b, long m, long a, char *s);
void    gendy_dsp               (t_gendy *x, t_signal **sp, short *count);
t_int   *gendy_perform          (t_int *w);

float   gendy_distribution      (int which, float a, float f);
float   gendy_mirroring        (float lower, float upper, float in);

//////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void){	
	t_class *c;
        
	c = class_new("sc.gendy2~", (method)gendy_new, (method)gendy_free, (long)sizeof(t_gendy), 0L, A_GIMME, 0);
    
	class_addmethod(c, (method)gendy_dsp,		"dsp",		A_CANT, 0);
	class_addmethod(c, (method)gendy_assist,    "assist",	A_CANT, 0);
    
    // the gendy parameters are set via attributes
    
    CLASS_ATTR_LONG         (c, "ampdist",  ATTR_FLAGS_NONE, t_gendy, g_ampdist);
    CLASS_ATTR_FILTER_CLIP  (c, "ampdist",  0, 6);
    CLASS_ATTR_ORDER        (c, "ampdist",	ATTR_FLAGS_NONE, "1");
    
    CLASS_ATTR_LONG         (c, "durdist",  ATTR_FLAGS_NONE, t_gendy, g_durdist);
    CLASS_ATTR_FILTER_CLIP  (c, "durdist",  0, 6);
    CLASS_ATTR_ORDER        (c, "durdist",	ATTR_FLAGS_NONE, "2");
    
    CLASS_ATTR_FLOAT        (c, "adparam",  ATTR_FLAGS_NONE, t_gendy, g_adparam);
    CLASS_ATTR_ORDER        (c, "adparam",	ATTR_FLAGS_NONE, "3");
    
    CLASS_ATTR_FLOAT        (c, "ddparam",  ATTR_FLAGS_NONE, t_gendy, g_ddparam);
    CLASS_ATTR_ORDER        (c, "ddparam",	ATTR_FLAGS_NONE, "4");
    
    CLASS_ATTR_FLOAT        (c, "minfreq",  ATTR_FLAGS_NONE, t_gendy, g_minfreq);
    CLASS_ATTR_FILTER_MIN   (c, "minfreq",  1.f);
    CLASS_ATTR_ORDER        (c, "minfreq",	ATTR_FLAGS_NONE, "5");
    
    CLASS_ATTR_FLOAT        (c, "maxfreq",  ATTR_FLAGS_NONE, t_gendy, g_maxfreq);
    CLASS_ATTR_FILTER_MIN   (c, "maxfreq",  1.f);
    CLASS_ATTR_ORDER        (c, "maxfreq",	ATTR_FLAGS_NONE, "6");
    
    CLASS_ATTR_FLOAT        (c, "ampscale", ATTR_FLAGS_NONE, t_gendy, g_ampscale);
    CLASS_ATTR_FILTER_CLIP  (c, "ampscale", 0.f, 1.f);
    CLASS_ATTR_ORDER        (c, "ampscale",	ATTR_FLAGS_NONE, "7");
    
    CLASS_ATTR_FLOAT        (c, "durscale", ATTR_FLAGS_NONE, t_gendy, g_durscale);
    CLASS_ATTR_FILTER_CLIP  (c, "durscale", 0.f, 1.f);
    CLASS_ATTR_ORDER        (c, "durscale",	ATTR_FLAGS_NONE, "8");
    
    CLASS_ATTR_LONG         (c, "knum",     ATTR_FLAGS_NONE, t_gendy, g_knum);
    CLASS_ATTR_ORDER        (c, "knum",     ATTR_FLAGS_NONE, "9");

    CLASS_ATTR_FLOAT        (c, "a",        ATTR_FLAGS_NONE, t_gendy, g_a);
    CLASS_ATTR_ORDER        (c, "a",        ATTR_FLAGS_NONE, "10");

    CLASS_ATTR_FLOAT        (c, "c",        ATTR_FLAGS_NONE, t_gendy, g_c);
    CLASS_ATTR_ORDER        (c, "c",        ATTR_FLAGS_NONE, "11");

    
	class_dspinit(c);				
	class_register(CLASS_BOX, c);
	gendy_class = c;
	
	return EXIT_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void gendy_dsp(t_gendy *x, t_signal **sp, short *count){
    x->mFreqMul = (float) 1.f/sys_getsr();
    
	dsp_add(gendy_perform, 3, x, sp[0]->s_vec, sp[0]->s_n);
}

t_int *gendy_perform(t_int *w){
    t_gendy *x      = (t_gendy *)(w[1]);	
    t_float *out    = (t_float *)(w[2]);
	int     n       = (int)w[3];
    
    if (x->ob.z_disabled) return w + 4;    
    
    
    // these are all set via max messages
    int     whichamp    = x->g_ampdist;
    int     whichdur    = x->g_durdist;
    float   aamp        = x->g_adparam;
    float   adur        = x->g_ddparam;
    float   minfreq     = x->g_minfreq;
    float   maxfreq     = x->g_maxfreq;
    float   scaleamp    = x->g_ampscale;
    float   scaledur    = x->g_durscale;
    
    
    float   rate    = x->mDur;
    //phase gives proportion for linear interpolation automatically
    double  phase   = x->mPhase;
    float   amp     = x->mAmp;
    float   nextamp = x->mNextAmp;
    float   speed   = x->mSpeed;
    
    
    while (n--){
        float z;
        
        if (phase >= 1.f) {
            phase -= 1.f;
            
            int index   = x->mIndex;
            int num     = x->g_knum;
            
            if((num>(x->g_cps)) || (num<1)) num=x->g_cps; // clip
                        
            //new code for indexing
            index=(index+1)%num;
            
            //using last amp value as seed
            //random values made using a lehmer number generator xenakis style
            float a= x->g_a;
            float c= x->g_c;
            
            float lehmerxen= fmod(((amp)*a)+c,1.0f);
            
            //printf("lehmer %f \n", lehmerxen);
            
            amp=nextamp;
            
            x->mIndex=index;
            
            //Gendy dist gives value [-1,1], then use scaleamp
            //first term was amp before, now must check new memory slot
            
            float ampstep= (x->mMemoryAmpStep[index])+ gendy_distribution(whichamp, aamp, fabs(lehmerxen));
            ampstep= gendy_mirroring(-1.0,1.0,ampstep);
            
            x->mMemoryAmpStep[index]= ampstep;
            
            nextamp= (x->mMemoryAmp[index])+(scaleamp*ampstep);
            
            nextamp= gendy_mirroring(-1.0,1.0,nextamp);
            
            x->mMemoryAmp[index]= nextamp;
            
            float durstep= (x->mMemoryDurStep[index])+ gendy_distribution(whichdur, adur, x->rgen.frand());
            durstep= gendy_mirroring(-1.0,1.0,durstep);
            
            x->mMemoryDurStep[index]= durstep;
            
            rate= (x->mMemoryDur[index])+(scaledur*durstep);
            
            rate= gendy_mirroring(0.0,1.0,rate);
            
            x->mMemoryDur[index]= rate;
            
            //printf("nextamp %f rate %f \n", nextamp, rate);
            
            //define range of speeds (say between 20 and 1000 Hz)
            //can have bounds as fourth and fifth inputs
            speed=  (minfreq+((maxfreq-minfreq)*rate))*(x->mFreqMul);
            
            //if there are 12 control points in memory, that is 12 per cycle
            //the speed is multiplied by 12
            //(I don't store this because updating rates must remain in range [0,1]
            speed *= num;
        }
        
        //linear interpolation could be changed
        z = ((1.0-phase)*amp) + (phase*nextamp);
        
        phase +=  speed;
        *out++ = z;
    }
    
    x->mPhase = phase;
    x->mAmp =  amp;
    x->mNextAmp = nextamp;
    x->mSpeed = speed;
    x->mDur = rate;
    
    
	return w + 4;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void gendy_assist(t_gendy *x, void *b, long m, long a, char *s){
	if (m == ASSIST_INLET) { //inlet
		sprintf(s, "messages to gendy");
	} 
	else {	// outlet
		sprintf(s, "(signal) Gendy"); 			
	}
}

void gendy_free(t_gendy *x){
    dsp_free((t_pxobject *)x);    

    sysmem_freeptr(x->mMemoryAmp);
    sysmem_freeptr(x->mMemoryDur);
    sysmem_freeptr(x->mMemoryAmpStep);
    sysmem_freeptr(x->mMemoryDurStep);
    
}

void *gendy_new(t_symbol *s, long ac, t_atom *av){
	t_gendy *x = NULL;
    long init_cps;
    
    x = (t_gendy *)object_alloc(gendy_class);
    
	if (x) {
		dsp_setup((t_pxobject *)x, 0);
        
        x->mFreqMul     = (float) 1.f/sys_getsr();
        x->mPhase       = 1.f;	//should immediately decide on new target
        x->mAmp         = 0.0;
        x->mNextAmp     = 0.0;
        x->mSpeed       = 100;
        
        x->mIndex       = 0;
        
        
        // if the first value is a long use it as our number of cps
        // non incremented av always points to the first value in the atom array
        if (attr_args_offset(ac, av) > 0 && atom_gettype(av) == A_LONG) {
            init_cps = atom_getlong(av);

            if(init_cps>0){
                x->g_cps = (int) init_cps;    
                object_post((t_object*) x, "number of cps: %d", x->g_cps);
            } else {
                x->g_cps = CONTROL_POINTS;  
                object_error((t_object*) x, "number of cps too small, setting to default (12)");
            } 
        } else {
            x->g_cps = CONTROL_POINTS;
            object_post((t_object*)x, "no number of cps supplied, using the default (12)");
        }
        
        x->mMemoryAmp       = (float*)sysmem_newptr(x->g_cps * sizeof(float));
        x->mMemoryAmpStep   = (float*)sysmem_newptr(x->g_cps * sizeof(float));
        x->mMemoryDur       = (float*)sysmem_newptr(x->g_cps * sizeof(float));
        x->mMemoryDurStep   = (float*)sysmem_newptr(x->g_cps * sizeof(float));
        
        // defaults
        x->g_ampdist      = 0;
        x->g_durdist      = 0;
        x->g_adparam      = 1.f;
        x->g_ddparam      = 1.f;
        x->g_minfreq      = 440.f;
        x->g_maxfreq      = 660.f;
        x->g_ampscale     = 0.5f;
        x->g_durscale     = 0.5f;
        x->g_a            = 1.17f;
        x->g_c            = 0.31f;
        
        // process the attributes after the defaults have been set
        attr_args_process(x, ac, av);
        
        // set up the random generator
        x->rgen.init(rand());
                
        //initialise to zeroes and separations
        int i=0;
        for(i=0; i < x->g_cps; ++i) {
            x->mMemoryAmp[i]    = 2 * x->rgen.frand() - 1.0;
            x->mMemoryDur[i]    = x->rgen.frand();
            x->mMemoryAmpStep[i]= 2 * x->rgen.frand() - 1.0;
            x->mMemoryDurStep[i]= 2 * x->rgen.frand() - 1.0;
        }
        
        outlet_new((t_object *)x, "signal");
        
	}
	return (x);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

float gendy_mirroring (float lower, float upper, float in) {
    //mirroring for bounds- safe version
    if(in>upper || in<lower) {
    
        float range= (upper-lower);
        
        if(in<lower) in= (2*upper-lower)-in;
            
			in=fmod(in-upper,2*range);
            
			if(in<range) in=upper-in;
                else in=in- (range);
                    }
    
    return in;
}

float gendy_distribution( int which, float a, float f) {
    
    float temp, c;
    
    if(a>1.0) a=1.0;       //a must be in range 0 to 1
    if(a<0.0001) a=0.0001; 	//for safety with some distributions, don't want divide by zero errors
    
    switch (which) {
        case 0: //LINEAR
                //linear
            break;
        case 1: //CAUCHY
                //X has a*tan((z-0.5)*pi)
                //I went back to first principles of the Cauchy distribution and re-integrated with a
                //normalisation constant
            
            //choice of 10 here is such that f=0.95 gives about 0.35 for temp, could go with 2 to make it finer
            c= atan(10*a);		//PERHAPS CHANGE TO a=1/a;
                                //incorrect- missed out divisor of pi in norm temp= a*tan(c*(2*pi*f - 1));
            temp= (1/a)*tan(c*(2*f - 1));	//Cauchy distribution, C is precalculated
            
            //printf("cauchy f %f c %f temp %f out %f \n",f,  c, temp, temp/10);
            
            return temp*0.1; //(temp+100)/200;
            
        case 2: //LOGIST (ic)
                //X has -(log((1-z)/z)+b)/a which is not very usable as is
            
            c=0.5+(0.499*a); //calculate normalisation constant
            c= log((1-c)/c);
            
            //remap into range of valid inputs to avoid infinities in the log
            
            //f= ((f-0.5)*0.499*a)+0.5;
            f= ((f-0.5)*0.998*a)+0.5; //[0,1]->[0.001,0.999]; squashed around midpoint 0.5 by a
                                      //Xenakis calls this the LOGIST map, it's from the range [0,1] to [inf,0] where 0.5->1
                                      //than take natural log. to avoid infinities in practise I take [0,1] -> [0.001,0.999]->[6.9,-6.9]
                                      //an interesting property is that 0.5-e is the reciprocal of 0.5+e under (1-f)/f
                                      //and hence the logs are the negative of each other
            temp= log((1-f)/f)/c;	//n range [-1,1]
                                    //X also had two constants in his- I don't bother
            
            //printf("logist f %f temp %f\n", f, temp);
            
            return temp; //a*0.5*(temp+1.0);	//to [0,1]
            
        case 3: //HYPERBCOS
                //X original a*log(tan(z*pi/2)) which is [0,1]->[0,pi/2]->[0,inf]->[-inf,inf]
                //unmanageable in this pure form
            c=tan(1.5692255*a);    //tan(0.999*a*pi*0.5);    	//[0, 636.6] maximum range
            temp= tan(1.5692255*a*f)/c;	//[0,1]->[0,1]
            temp= log(temp*0.999+0.001)*(-0.1447648);  // multiplier same as /(-6.9077553); //[0,1]->[0,1]
            
            //printf("hyperbcos f %f c %f temp %f\n", f, c, temp);
            
            return 2*temp-1.0;
            
        case 4: //ARCSINE
                //X original a/2*(1-sin((0.5-z)*pi)) aha almost a better behaved one though [0,1]->[2,0]->[a,0]
            c= sin(1.5707963*a); //sin(pi*0.5*a);	//a as scaling factor of domain of sine input to use
            temp= sin(PI*(f-0.5)*a)/c; //[-1,1] which is what I need
            
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