#include "csdl.h"
/*
To Compile:

This is the correct command lol
gcc -O2 -dynamiclib -o blit.dylib BLITopcode.c -DUSE_DOUBLE -I/Library/Frameworks/CsoundLib64.framework/Versions/6.0/Headers -arch x86_64

*/

// ported from https://github.com/supercollider/sc3-plugins/blob/main/source/AntiAliasingOscillators/AntiAliasingOscillators.cpp#L31
//Juhan Nam, Vesa Valimaki, Jonathan S. Abel, and Julius O. Smith
//Efficient Antialiasing Oscillator Algorithms Using Low-Order Fractional Delay Filters
//IEEE Transactions on Audio, Speech, and Language Processing 18(4) May 2010, pp 773--785

//and

//Vesa Valimaki, Juhan Nam, Julius O. Smith and Jonathan S. Abel
//Alias-Suppressed Oscillators Based on Differentiated Polynomial Waveforms
//IEEE Transactions on Audio, Speech, and Language Processing 18(4) May 2010, pp 786--798

typedef struct _blit{
OPDS h;
MYFLT *out;
MYFLT *in1, *in2;
MYFLT freq;
MYFLT amp;
MYFLT phase;
} blit;

/* INITIALISATION*/

int blit_init(CSOUND *csound, blit *p){
p->phase = 0.0f;
return OK;
}

/* PROCESSING */

int blit_process_audio(CSOUND *csound, blit *p){

MYFLT *aout = p->out;
MYFLT freq = *p->in1;
MYFLT amp = *p->in2;
if(freq < 0.000001f){
    freq = 0.000001f;
}
MYFLT sr = csound->GetSr(csound);
float period = sr/freq;
float phase = fmod(p->phase, 1.f);
float t = phase * period;
float x, y;
int inNumSamples = CS_KSMPS;
for(int i = 0; i < inNumSamples; i++){
    if(t >= 4.0f){
        aout[i] = 0.0f;
    } else if(t >= 3.0f){
        x = 4.0f - t;
        aout[i] = 0.16666666666667f*x*x*x;
    } else if(t >= 2.0f){
        x = t - 2.0;
        y = x*x;
        aout[i] = 0.66666666666666f - y + (0.5f*y*x);
    } else if (t >= 1.0f){
        x = t - 2.0;
        y = x*x;
        aout[i] = 0.66666666666666f - y - (0.5f*y*x);
    } else {
        aout[i] = 0.16666666666667f*t*t*t;
    }
    t += 1.f;
    if(t >= period){
        t -= period;
    }
}
float Sampledur = 1/sr; 
p->phase = t * freq * Sampledur;
return OK;
}

/*REGISTRATION*/

static OENTRY localops[] = {
    {
        "blit", sizeof(blit), 0, 7, "a", "ii",
        (SUBR) blit_init, (SUBR) blit_process_audio
    }
};

LINKAGE