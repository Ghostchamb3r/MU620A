#include "csdl.h"
#include <math.h>

/*
To Compile:

This is the correct command lol
gcc -O2 -dynamiclib -o hypersaw2.dylib hypersaw2.c -DUSE_DOUBLE -I/Library/Frameworks/CsoundLib64.framework/Versions/6.0/Headers -arch x86_64

*/
/*
Might need to manually define pi
const float pi = 3.141592653589793;
*/
// implement tanh as a table lookup instead of calling tanh every sample
// try macros instead of redefining table values over and over
/*
Hyperbolic Tangent Waveshaping Oscillator sourced from:
Lazzarini, V. & Timoney, J.
 "New Perspectives on Distortion Synthesis for Virtual Analog Oscillators", 
 Computer music journal, vol. 34, no. 1, 2010, pp. 28-40.

*/
#define tanh_table_size 2048
#define tanh_min -1.0
#define tanh_max 1.0

static MYFLT tanh_Table[tanh_table_size];

void generate_tanh_table(){
    for(int i = 0; i < tanh_table_size; i++){
        MYFLT jenerate = tanh_min + (i * (tanh_max - tanh_min) / (tanh_table_size - 1));
        tanh_Table[i] = tanh(jenerate);
    }
}

static MYFLT compute_tanh(MYFLT n){
    if (n <= tanh_min) return tanh_Table[0];
    if (n >= tanh_max) return tanh_Table[tanh_table_size - 1];

    MYFLT base_index = ((n - tanh_min) / (tanh_max - tanh_min)) * (tanh_table_size - 1);
    int index = (int)base_index;
    MYFLT fracture = base_index = index;
    return tanh_Table[index] * (1.0 - fracture) + tanh_Table[index + 1] * fracture;

}


// DATA
typedef struct _hypersaw{
OPDS h;
MYFLT *out;
MYFLT *in1, *in2, *in3, *in4;
MYFLT freq;
MYFLT amp;
MYFLT phase;
MYFLT m;
} hypersaw;

// INITIALIZE 

int hypersaw_init(CSOUND *csound, hypersaw *p){
    p->phase = 0.0f;
    p->m = *p->in4;
    if (p->m < 0.0) p->m = 0.0;
    if (p->m > 1.0) p->m = 1.0;
    generate_tanh_table(); 

return OK;
}


// PROCESS

int hypersaw_process_audio(CSOUND *csound, hypersaw *p){

int inNumSamples = CS_KSMPS;
MYFLT *aout = p->out;
MYFLT freq = *p->in1;
MYFLT amp = *p->in2;
MYFLT phase = *p->in3;
MYFLT m = *p->in4;
MYFLT sr = csound->GetSr(csound);

float phase_increment = (2 * M_PI * freq) / sr;

for(int i = 0; i < inNumSamples; i++){
    float sinewave = amp * sin(p->phase + phase);
    float k = 12000 / (freq * log10(freq));
    float shapedwave = compute_tanh(k * sinewave);
    // apply ring modulation with cosine to shape square into saw
    float ringmodder = shapedwave * cos(p->phase);
    float sawtoothwave = shapedwave * (1 - p->m/2) + ringmodder * p->m;

    aout[i] = sawtoothwave;
    p->phase += phase_increment;
    if (p->phase > 2 * M_PI) p->phase -= 2 * M_PI;

}

return OK;
}

// REGISTER

static OENTRY localops[] = {
{ "hypersaw", sizeof(hypersaw), 0, 7, "a", "iiii",(SUBR) hypersaw_init,
(SUBR) hypersaw_process_audio }
};


LINKAGE