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

#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"
#include "rgen_helper.h"

// we always use 12 control points for the calculation
# define CONTROL_POINTS 12

//////////////////////////////////////////////////////////////////////////////////////////////////////

// Iannis Xenakis/Marie-Helene Serra GENDYN simulation
typedef struct _gendy 
{
	t_pxobject          ob;
    
    // these are set from the outside
    int                 ampdist, durdist;
    float               adparam, ddparam;
    float               minfreq, maxfreq;
    float               ampscale, durscale;
    
    double              mPhase;
    float               mFreqMul, mAmp, mNextAmp, mSpeed, mDur;
    int                 mIndex;
    float               *mMemoryAmp; 	
    float               *mMemoryDur;
    
    // we use sc's random distribution
    RGen                rgen;
    
} t_gendy;

t_class *gendy_class;

//////////////////////////////////////////////////////////////////////////////////////////////////////

void    *gendy_new              (long argc, t_atom *argv);
void    gendy_free              (t_gendy *x);
void    gendy_assist            (t_gendy *x, void *b, long m, long a, char *s);
float   gendy_distribution      (int which, float a, float f);
void    gendy_dsp               (t_gendy *x, t_signal **sp, short *count);
t_int   *gendy_perform          (t_int *w);


// we are explicit here and don't catch everything in a anything method
void    gendy_ampdist           (t_gendy *x, long dist);
void    gendy_durdist           (t_gendy *x, long dist);
void    gendy_adparam           (t_gendy *x, double param);
void    gendy_ddparam           (t_gendy *x, double param);
void    gendy_minfreq           (t_gendy *x, double freq);
void    gendy_maxfreq           (t_gendy *x, double freq);
void    gendy_ampscale          (t_gendy *x, double scale);
void    gendy_durscale          (t_gendy *x, double scale);

//////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void){	
	t_class *c;
    
	c = class_new("sc.gendy1~", (method)gendy_new, (method)gendy_free, (long)sizeof(t_gendy), 0L, A_GIMME, 0);
	
	class_addmethod(c, (method)gendy_dsp,		"dsp",		A_CANT, 0);
	class_addmethod(c, (method)gendy_assist,    "assist",	A_CANT, 0);
    
    class_addmethod(c, (method) gendy_ampdist,  "ampdist",  A_LONG, 0);
    class_addmethod(c, (method) gendy_durdist,  "durdist",  A_LONG, 0);
    class_addmethod(c, (method) gendy_adparam,  "adparam",  A_FLOAT, 0);
    class_addmethod(c, (method) gendy_ddparam,  "ddparam",  A_FLOAT, 0);
    class_addmethod(c, (method) gendy_minfreq,  "minfreq",  A_FLOAT, 0);
    class_addmethod(c, (method) gendy_maxfreq,  "maxfreq",  A_FLOAT, 0);    
    class_addmethod(c, (method) gendy_ampscale, "ampscale", A_FLOAT, 0);    
    class_addmethod(c, (method) gendy_durscale, "durscale",  A_FLOAT, 0);    
    
	class_dspinit(c);				
	class_register(CLASS_BOX, c);
	gendy_class = c;
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void gendy_ampdist(t_gendy *x, long dist){
    
    // clip values
    if(dist<0) dist = 0;
    if(dist>6) dist = 6;
    
    x->ampdist = (int) dist;
}

void gendy_durdist(t_gendy *x, long dist){
    
    // clip values
    if(dist<0) dist = 0;
    if(dist>6) dist = 6;
    
    x->durdist = (int) dist;
}

void gendy_adparam(t_gendy *x, double param){
    x->adparam = (float) param;
}

void gendy_ddparam(t_gendy *x, double param){
    x->ddparam = (float) param;
}


void gendy_minfreq(t_gendy *x, double freq){
    if(freq < 1.0) freq = 1.0; // prevent blow up
    
    x->minfreq = (float) freq;
}

void gendy_maxfreq(t_gendy *x, double freq){
    if(freq < 1.0) freq = 1.0; // prevent blow up
    
    x->maxfreq = (float) freq;
}

void gendy_ampscale(t_gendy *x, double scale){    
    x->ampscale = (float) scale;
}

void gendy_durscale(t_gendy *x, double scale){    
    x->durscale = (float) scale;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////

void gendy_dsp(t_gendy *x, t_signal **sp, short *count){
    x->mFreqMul = (float) 1.f/sys_getsr();
    
	dsp_add(gendy_perform, 3, x, sp[0]->s_vec, sp[0]->s_n);
}

t_int *gendy_perform(t_int *w){
    t_gendy *x = (t_gendy *)(w[1]);	
    t_float *out = (t_float *)(w[2]);
	int n = (int)w[3];
    
    if (x->ob.z_disabled) return w + 4;    
    
        
    float   rate= x->mDur;
    
    //phase gives proportion for linear interpolation automatically
    double  phase = x->mPhase;
    
    float   amp= x->mAmp;
    float   nextamp= x->mNextAmp;
    
    float   speed= x->mSpeed;
    
    while (n--){
		*out++ = 0.f;
        
        float z;
        
        if (phase >= 1.f) {
            phase -= 1.f;
            
            int index= x->mIndex;            

            index=(index+1)%CONTROL_POINTS;
            amp=nextamp;
            
            x->mIndex=index;
            
            //Gendy dist gives value [-1,1], then use scaleamp
            //first term was amp before, now must check new memory slot
            
            nextamp= (x->mMemoryAmp[index])+(x->ampscale * gendy_distribution(x->ampdist, x->adparam,x->rgen.frand()));
            
            //mirroring for bounds- safe version
            if(nextamp>1.0 || nextamp<-1.0) {
                
                //printf("mirroring nextamp %f ", nextamp);
                
                //to force mirroring to be sensible
                if(nextamp<0.0) nextamp=nextamp+4.0;
                
                nextamp=fmod(nextamp,4.0f);
                //printf("fmod  %f ", nextamp);
                
                if(nextamp>1.0 && nextamp<3.0)
                    nextamp= 2.0-nextamp;
                
                else if(nextamp>1.0)
                    nextamp=nextamp-4.0;
                
                //printf("mirrorednextamp %f \n", nextamp);
            };
            
            x->mMemoryAmp[index]= nextamp;
            
            //    Gendy dist gives value [-1,1]
            
            
            
            rate= (x->mMemoryDur[index])+(x->durscale * gendy_distribution(x->durdist, x->ddparam, x->rgen.frand()));
            
            
            if(rate>1.0 || rate<0.0)
            {
                if(rate<0.0) rate=rate+2.0;
                rate= fmod(rate,2.0f);
                rate= 2.0-rate;
            }
            
            x->mMemoryDur[index]= rate;
            
            //printf("nextamp %f rate %f \n", nextamp, rate);
            
            //define range of speeds (say between 20 and 1000 Hz)
            //can have bounds as fourth and fifth inputs
            speed=  (x->minfreq+((x->maxfreq-x->minfreq)*rate))*(x->mFreqMul);
            
            //if there are 12 control points in memory, that is 12 per cycle
            //the speed is multiplied by 12
            //(I don't store this because updating rates must remain in range [0,1]
            speed *= CONTROL_POINTS;
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
		sprintf(s, "Ignore this inlet");
	} 
	else {	// outlet
		sprintf(s, "(signal) Gendy"); 			
	}
}

void gendy_free(t_gendy *x){
    dsp_free((t_pxobject *)x);    

    sysmem_freeptr(x->mMemoryAmp);
    sysmem_freeptr(x->mMemoryDur);
}

void *gendy_new(long argc, t_atom *argv){
	t_gendy *x = NULL;
	x = (t_gendy *)object_alloc(gendy_class);
    
	if (x) {
		dsp_setup((t_pxobject *)x, 0);
        
        x->mFreqMul     = (float) 1.f/sys_getsr();
        x->mPhase       = 1.f;	//should immediately decide on new target
        x->mAmp         = 0.0;
        x->mNextAmp     = 0.0;
        x->mSpeed       = 100;
        
        x->mIndex=0;
        
        x->mMemoryAmp= (float*)sysmem_newptr(CONTROL_POINTS * sizeof(float));
        x->mMemoryDur= (float*)sysmem_newptr(CONTROL_POINTS * sizeof(float));
        
        
        /* defaults */
        x->ampdist      = 0;
        x->durdist      = 0;
        x->adparam      = 1.f;
        x->ddparam      = 1.f;
        x->minfreq      = 440.f;
        x->maxfreq      = 660.f;
        x->ampscale     = 0.5f;
        x->durscale     = 0.5f; 
        
        x->rgen.init(rand());
                
        //initialise to zeroes and separations
        int i=0;
        for(i=0; i<CONTROL_POINTS;++i) {
            x->mMemoryAmp[i] = 2 * x->rgen.frand() - 1.0;
            x->mMemoryDur[i] = x->rgen.frand();
        }
        
        outlet_new((t_object *)x, "signal");
        
	}
	return (x);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////


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