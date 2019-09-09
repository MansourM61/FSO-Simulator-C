// header inclusion section
#include "sigProcModule.h"
#include <gsl/gsl_errno.h>
#include <gsl/gsl_fft_complex.h>


// Func: void SampleArray(float* inputArray, float* outputArray, int nInput, int offset, int nOutput)
// Desc: sample input array with given size into an output array with desired size, start sampling at given offset, if input elements are  finished, zero will be inserted in the output array.
// In: pointer to input array, pointer to output array, number of input elements, number of output elements, offset index of sampling
// Out: none
void SampleArray(float* inputArray, int nInput, float* outputArray, int nOutput, int iOffset) {

    int iInd;  // element index of input array

    if(nOutput >= nInput) {  // if the output array size is bigger than or equal to input array size
        int iRate = round(nOutput/nInput);  // re-sampling rate

        for(int index = 0; index < nOutput; index++) {
            iInd = floor(index/iRate) + iOffset;  // update the input index
            iInd = min(iInd, nInput - 1);  // make sure the index is valid

            outputArray[index] = inputArray[iInd];  // update the output array
        }
    }
    else {  // if output array is smaller than the input array
        int iRate = round(nInput/nOutput);  // re-sampling rate

        for(int index = 0; index < nOutput; index++) {
            iInd = index * iRate + iOffset;  // update the input index
            iInd = min(iInd, nInput - 1);  // make sure the index is valid

            outputArray[index] = inputArray[iInd];  // update the output array
        }

    }
}


// Func: void SliceArray(float* inputArray, int nInput, float* outputArray, int nOutput, int iNumChunck)
// Desc: slice the input signal based on the mean value of given chunks
// In: pointer to input array, pointer to output array, number of input elements, number of output elements, number of chunks
// Out: none
void SliceArray(float* inputArray, int nInput, float* outputArray, int nOutput, int iNumChunck) {

    if(nInput != nOutput) {  // if input and output arrays  are not the same length
        printf("Error: the input and output arrays are not the same length for slicing.\n");
        exit(-1);
    }

    float *pMeanValues = (float*) calloc(iNumChunck, sizeof(float));  // allocate memory for mean values array

    float fSum = 0;  // sample summation
    int dElemNum = ceil(nInput/iNumChunck);  // length of each chunk
    int iInd = 0;  // element index of array
    int iNE = 0;  // number of scanned elements in each chunk

    for(int index = 0; index < nInput; index++) {  // loop through the input array
        fSum += inputArray[index];
        if(++iNE == dElemNum) {
            pMeanValues[iInd++] = fSum/iNE;  // update mean array
            fSum = 0;  // reset the summation
            iNE = 0;  // reset the scanned values counter
        }
    }
    if(iInd != iNumChunck) {  // if the last chunk is not scanned
        pMeanValues[iNumChunck - 1] = fSum/iNE;  // update last item in mean array
    }

    float fThreshLvl;  // threshold level
    int iThreshInd;   // threshold index
    for(int index = 0; index < nInput; index++) {  // loop through the input array
        iThreshInd = min((int) ( round(index/dElemNum) ), iNumChunck - 1);  // update threshold index
        fThreshLvl = pMeanValues[iThreshInd];  // update threshold level

        if(inputArray[index] >= fThreshLvl) {  // if the sampled value is larger than or equal to threshold level
            outputArray[index] = 1;  // set extracted bit to 1
        }
        else {    // if the sampled value is smaller than threshold level
            outputArray[index] = 0;  // set extracted bit to 0
        }
    }

    free(pMeanValues);  // release memory for mean values array
}


// Func: void SampleArray(float* inputArray, float* outputArray, int nInput, int offset, int nOutput)
// Desc: apply a linear map to the input array elements (output = input * A + B)
// In: pointer to input array, pointer to output array, number of input elements, number of output elements, A coefficient, B constant
// Out: none
void LinearMapArray(float* inputArray, int nInput, float* outputArray, int nOutput, float fA, float fB) {

    if(nInput != nOutput) {  // if input and output arrays  are not the same length
        printf("Error: the input and output arrays are not the same length for linear mapping operation.\n");
        exit(-1);
    }

    for(int index = 0; index < nInput; index++) {  // go through the array
        outputArray[index] = inputArray[index]*fA + fB;  // calculate the output array
    }
}


// Func: float SumArray(float* inputArray, int nInput)
// Desc: sum up the elements of the array
// In: pointer to input array, number of input elements
// Out: summation
float SumArray(float* inputArray, int nInput) {

    float fSum = 0;  // sample summation

    for(int index = 0; index < nInput; index++) {  // loop through the input array
        fSum += inputArray[index];  // add the value to the summation variable
    }

    return fSum;  // return the summation
}



// Func: void NormaliseSignal(float* inputArray, int nInput, float* outputArray, int nOutput)
// Desc: sets maximum to 1 and minimum to -1
// In: pointer to un-normalised input array, number of input elements, pointer to normalised output array, number of output elements
// Out: none
void NormaliseSignal(float* inputArray, int nInput, float* outputArray, int nOutput) {

    if(nInput != nOutput) {  // if input and output arrays  are not the same length
        printf("Error: the input and output arrays are not the same length for normalisation operation.\n");
        exit(-1);
    }

    float fMinVal = inputArray[0], fMaxVal = inputArray[0];  // minimum and maximum values of the signal

    for(int index = 1; index < nInput; index++) {  // loop through the input array

        if(inputArray[index] < fMinVal) {  // if a smaller value is found
            fMinVal = inputArray[index];  // replace the minimum value
        }
        if(inputArray[index] > fMaxVal) {  // if a larger value is found
            fMaxVal = inputArray[index];  // replace the maximum value
        }
    }

    for(int index = 0; index < nInput; index++) {  // loop through the input array
        outputArray[index] = (2.0*inputArray[index] - (fMaxVal + fMinVal)) / (fMaxVal - fMinVal);  // normalise output array
    }



}


// Func: float BERTester(float* inputArray, int nInput)
// Desc: calculates the BER based on given bit arrays
// In: pointer to array 1, number of input elements in array 1, pointer to array 2, number of input elements in array 2
// Out: mean value
int BERTester(float* inputArray_1, int nInput_1, float* inputArray_2, int nInput_2) {

    int iNoE = 0;  // number of erroneous bits

    if(nInput_1 != nInput_2) {  // if input and output arrays  are not the same length
        printf("Error: the input and output arrays are not the same length for BER tester.\n");
        exit(-1);
    }

    for(int index = 0; index < nInput_1; index++) {  // loop through the input array
        iNoE += abs(inputArray_1[index] - inputArray_2[index]);  // add the signal elements
    }

    return iNoE;
}


// Func: float MeanArray(float* inputArray, int nInput)
// Desc: calculates the mean value of the signal
// In: pointer to input array, number of input elements
// Out: mean value
float MeanArray(float* inputArray, int nInput) {
    float fSum = 0;

    for(int index = 0; index < nInput; index++) {  // loop through the input array
        fSum += inputArray[index];  // add the signal elements
    }

    return fSum/nInput;
}


// Func: float VarArray(float* inputArray, int nInput)
// Desc: calculates the variance value of the signal
// In: pointer to input array, number of input elements
// Out: mean value
float VarArray(float* inputArray, int nInput) {

    float fMean = MeanArray(inputArray, nInput);

    float fSum = 0;

    for(int index = 0; index < nInput; index++) {  // loop through the input array
        fSum += POW2(inputArray[index] - fMean);  // add the signal elements
    }

    return fSum/nInput;
}


// Func: void SmoothArray_m1(float* inputArray, int nInput, float* outputArray, int nOutput, int iWindowWidth)
// Desc: apply moving average to the input array, method 1
// In: pointer to input array, number of input elements, pointer to output array, number of output elements, moving average window width
// Out: none
void SmoothArray_m1(float* inputArray, int nInput, float* outputArray, int nOutput, int iWindowWidth) {

    if(nInput != nOutput) {  // if input and output arrays  are not the same length
        printf("Error: the input and output arrays are not the same length for smoothen array - method 1.\n");
        exit(-1);
    }

    float fSum;  // dummy summation variable
    int iIndex_s, iIndex_e;  // dummy index variables
    int iSumLen;

    for(int index_1 = 1; index_1 < nOutput; index_1++) {
        iIndex_s = (int)max(0, index_1 - floor(iWindowWidth/2.0));  // start index
        iIndex_e = (int)min(nInput, index_1 + floor(iWindowWidth/2.0));  // end index
        iSumLen = iIndex_e - iIndex_s;
        fSum = 0;

        for(int index_2 = iIndex_s; index_2 < iIndex_e; index_2++) {
            fSum += inputArray[index_2];
        }

        outputArray[index_1] = fSum/iSumLen;
    }

}


// Func: void SmoothArray_m2(float* inputArray, int nInput, float* outputArray, int nOutput, int iDelay)
// Desc: apply moving average to the input array; method 2
// In: pointer to input array, number of input elements, pointer to output array, number of output elements, filter delay parameter
// Out: none
void SmoothArray_m2(float* inputArray, int nInput, float* outputArray, int nOutput, int iDelay) {

    if(nInput != nOutput) {  // if input and output arrays  are not the same length
        printf("Error: the input and output arrays are not the same length for smoothen array - method 2.\n");
        exit(-1);
    }

    int iIndex_e;  // dummy index variables

    outputArray[0] = inputArray[0]/iDelay;  // item i=0

    for(int index = 1; index < nOutput; index++) {
        iIndex_e = (int)min(nInput - 1, index - iDelay);  // end index
        if(iIndex_e < 0) {
            outputArray[index] = outputArray[index - 1] + inputArray[index]/iDelay;
        }
        else {
            outputArray[index] = outputArray[index - 1] + (inputArray[index] - inputArray[iIndex_e])/iDelay;
        }
    }
}


// Func: void SmoothArray_m3(float* inputArray, int nInput, float* outputArray, int nOutput, float fFiltPar)
// Desc: apply moving average to the input array; method 3
// In: pointer to input array, number of input elements, pointer to output array, number of output elements, filter parameter
// Out: none
void SmoothArray_m3(float* inputArray, int nInput, float* outputArray, int nOutput, float fFiltPar) {

    if(nInput != nOutput) {  // if input and output arrays  are not the same length
        printf("Error: the input and output arrays are not the same length for smoothen array - method 3.\n");
        exit(-1);
    }

    outputArray[0] = fFiltPar*inputArray[0];  // item i=0

    for(int index = 1; index < nOutput; index++) {
        outputArray[index] = fFiltPar*inputArray[index] + (1 - fFiltPar)*outputArray[index - 1];
    }
}


// Func: float MinArray(float* inputArray, int nInput)
// Desc: finds the minimum value of the signal
// In: pointer to input array, number of input elements
// Out: minimum value
float MinArray(float* inputArray, int nInput) {

    float fMin = inputArray[0];  // initialised minimum value

    for(int index = 1; index < nInput; index++) {
        if(inputArray[index] < fMin) {
            fMin = inputArray[index];
        }
    }

    return fMin;
}


// Func: float MaxArray(float* inputArray, int nInput)
// Desc: finds the maximum value of the signal
// In: pointer to input array, number of input elements
// Out: maximum value
float MaxArray(float* inputArray, int nInput) {

    float fMax = inputArray[0];  // initialised minimum value

    for(int index = 1; index < nInput; index++) {
        if(inputArray[index] > fMax) {
            fMax = inputArray[index];
        }
    }

    return fMax;
}



// Func: void FFTArray(float* inputArray, int nInput, float* outputArray, int nOutput)
// Desc: calculates fast Fourier transform (FFT) of a signal
// In: pointer to input array, number of input elements, pointer to FFT array, number of FFT elements
// Out: none
void FFTArray(float* inputArray, int nInput, float* outputArray, int nOutput) {

    if(2*nOutput != NEXT_POW_2(nInput)) {
        printf("Error: the input and output arrays are not the same length for FFT operation.\n");
        exit(-1);
    }

    double* pFFTVar = (double *) calloc((nOutput*2) * 2, sizeof(double));  // allocate memory for fft array

    for(int index = 0; index < nInput; index++) {  // go through the input array
        REAL(pFFTVar, index) = inputArray[index];  // fill the input array with the signals
    }

    if(gsl_fft_complex_radix2_forward(pFFTVar, 1, (nOutput * 2)) != GSL_SUCCESS) {
        printf("Error: FFT operation failed.\n");
        exit(-1);
    }

    for(int index = 0; index < nOutput; index++) {  // go through the input array
        outputArray[index] = log10(sqrt(POW2(REAL(pFFTVar, index)) +  POW2(IMAG(pFFTVar, index)))/sqrt(nOutput));  // calculate amplitude of FFT
    }

    free(pFFTVar);  // release fft memory
    pFFTVar = NULL;
}
