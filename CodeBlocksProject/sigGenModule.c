// header inclusion section
#include "sigGenModule.h"
#include <math.h>


// Func: void GenerateBitSeq(float* fseqArray, int nBits)
// Desc: generate a burst of n pseudorandom binary sequence bits
// In: pointer to the bit array, number of generated bits
// Out: none
void GenerateBitSeq(float* fseqArray, int nBits) {
    int bit;

    for(int index = 0; index < nBits; index++) {
        bit = rand( ) % 2;  // generate a random bit in the range of [0, 1]
        fseqArray[index] = bit;  // update the array
    }
}


// Func: void GenerateLinearArray(float fTimeArray, int iLength, float fT0, float fDelT)
// Desc: generate linear array sequence
// In: time array, length of array, start time, time step
// Out: none
void GenerateLinearArray(float* fArray, int iLength, float fX0, float fDelX) {

    for(int index = 0; index < iLength; index++) {
        fArray[index] = fX0 + index*fDelX;
    }
}


// Func: float GaussNormNumGen(void)
// Desc: generate random number based on normal Gaussian distribution
// In: nose
// Out: random number
float GaussNormNumGen(void) {
    static int iGaussGenCounter = {0};  // counter for calculating Gaussian distributed random number
    static float fGaussGenArray[2] = {0, 0};  // generated Gaussian distributed random number

    ++iGaussGenCounter;

    // Marsaglia polar method
    // https://en.wikipedia.org/wiki/Marsaglia_polar_method
    if (iGaussGenCounter%2 != 0) {  // generating random numbers
        float x, y, rsq, f;  // temporary variables
        do {
            x = 2.0 * rand() / (float)RAND_MAX - 1.0;  // x point calculation
            y = 2.0 * rand() / (float)RAND_MAX - 1.0;  // y point calculation
            rsq = x * x + y * y;  // calculate distance
        } while(rsq >= 1.0 || rsq == 0.0);  // repeat the calculation till the points are in the square

        f = sqrt(-2.0*log(rsq)/rsq);  // auxiliary value
        fGaussGenArray[0] = x*f;  // first random value
        fGaussGenArray[1] = y*f;  // second random value

        return fGaussGenArray[0];
    }
    else {  // returning already calculated number
        return fGaussGenArray[1];
    }
}


// Func: void SignalMath(float* X, int XLen, float *Y, int YLen, float *Z, int ZLen, char Oprand)
// Desc: perform mathematical element -by-element operations on the signal
// In: Z = X operand Y; operand: +, -, *, /
// Out: none
void SignalMath(float* X, int XLen, float *Y, int YLen, float *Z, int ZLen, char Operand) {

    if(XLen != YLen || XLen != ZLen) {  // if input and output and output arrays are not the same length
        printf("Error: the input and output arrays are not the same length for mathematical operation.\n");
        exit(-1);
    }

    switch(Operand) {  // select the operand
    case '+':  // add signals
        for(int index = 0; index < XLen; index++) {
            Z[index] = X[index] + Y[index];
        }
        break;

    case '-':  // subtract signals
        for(int index = 0; index < XLen; index++) {
            Z[index] = X[index] - Y[index];
        }
        break;

    case '*':  // multiply signals
        for(int index = 0; index < XLen; index++) {
            Z[index] = X[index] * Y[index];
        }
        break;

    case '/':  // divide signals
        for(int index = 0; index < XLen; index++) {
            Z[index] = X[index] / Y[index];
        }
        break;
    default:
        printf("Error: unknown operand.\n");
        exit(-1);
    }
}
