#ifndef SIGPROCMODULE_H_INCLUDED
#define SIGPROCMODULE_H_INCLUDED


// header inclusion section
#include "parameters.h"  // include predefined parameters
#include <math.h>

// macro definition section
#define POW2(x) ((x)*(x))  // x to the power of 2
#define REAL(Z, i) ( (Z)[2*(i) + 0] )  // real part of complex number
#define IMAG(Z, i) ( (Z)[2*(i) + 1] )  // imaginary part of complex number
#define NEXT_POW_2(n) ((int) pow(2, ceil(log2(n))) )  // find next power of 2


// function declaration section
void SampleArray(float* inputArray, int nInput, float* outputArray, int nOutput, int iOffset);  // sample input array with given size into an output array with desired size, start sampling at given offset
void SliceArray(float* inputArray, int nInput, float* outputArray, int nOutput, int iNumChunck);  // slice the input signal based on the mean value of given chunks
void LinearMapArray(float* inputArray, int nInput, float* outputArray, int nOutput, float fA, float fB);  // apply a linear map to the input array elements (output = input * A + B)
float SumArray(float* inputArray, int nInput);  // sum up the elements of the array
void NormaliseSignal(float* inputArray, int nInput, float* outputArray, int nOutput);  // sets maximum to 1 and minimum to -1
int BERTester(float* inputArray_1, int nInput_1, float* inputArray_2, int nInput_2);  // calculates the BER based on given bit arrays
float MeanArray(float* inputArray, int nInput);  // calculates the mean value of the signal
float VarArray(float* inputArray, int nInput);  // calculates the variance value of the signal
void SmoothArray_m1(float* inputArray, int nInput, float* outputArray, int nOutput, int iWindowWidth);  // apply moving average to the input array; method 1
void SmoothArray_m2(float* inputArray, int nInput, float* outputArray, int nOutput, int iDelay);  // apply moving average to the input array; method 2
void SmoothArray_m3(float* inputArray, int nInput, float* outputArray, int nOutput, float fFiltPar);  // apply moving average to the input array; method 3
float MinArray(float* inputArray, int nInput);  // finds the minimum value of the signal
float MaxArray(float* inputArray, int nInput);  // finds the maximum value of the signal
void FFTArray(float* inputArray, int nInput, float* outputArray, int nOutput);  // calculates fast Fourier transform (FFT) of a signal

#endif // SIGPROCMODULE_H_INCLUDED
