#ifndef SIGGENMODULE_H_INCLUDED
#define SIGGENMODULE_H_INCLUDED


// header inclusion section
#include "parameters.h"  // include predefined parameters


// macro definition section


// variable definition section


// function declaration section
void GenerateBitSeq(float* fseqArray, int nBits);  // generate a burst of n pseudo-random binary sequence bits
void GenerateLinearArray(float* fTimeArray, int iLength, float fT0, float fDelT);  // generate linear array
float GaussNormNumGen(void) ;  // generate random number based on normal Gaussian distribution; adopted from 'https://rosettacode.org/wiki/Statistics/Normal_distribution#C'
void SignalMath(float* X, int XLen, float *Y, int YLen, float *Z, int ZLen, char Oprand);  // perform mathematical element -by-element operations on the signal

#endif // SIGGENMODULE_H_INCLUDED
