#ifndef COREMODULE_H_INCLUDED
#define COREMODULE_H_INCLUDED


// header inclusion section
#include "parameters.h"  // include predefined parameters
#include "sigGenModule.h"  // signal generators header file
#include "sigProcModule.h"  // signal processors header file
#include <math.h>  // math.h header file


// macro definition section
#define NUM_OF_STORED_SIGNAL (NUM_OF_BASE_SIGNAL + NUM_OF_RESAMP_SIGNAL + NUM_OF_TX_SIGNAL + NUM_OF_CHANNEL_COEFFS + NUM_OF_RX_SIGNAL + NUM_OF_PROBED_SIGNAL)  // total number of stored signals

// bit generator MARCO
// In: bit array
// Out: none
#define BIT_GENERATOR(ARRAY) { \
    GenerateBitSeq(ARRAY->fArray, ARRAY->NoS); \
}

// time generator MARCO
// In: time array, simulation iteration index, start time, time step
// Out: none
#define TIME_GENERATOR(ARRAY, ITERRATION, T0, DELT) { \
    GenerateLinearArray(ARRAY->fArray, ARRAY->NoS, T0, DELT); \
}

// order generator MARCO
// In: order array, iteration index
// Out: none
#define ORDER_GENERATOR(ARRAY, ITERRATION) { \
    for(int index = 0; index < ARRAY->NoS; index++) { \
        ARRAY->fArray[index] = index + ARRAY->NoS*ITERRATION; \
    } \
}

// frequency generator MARCO
// In: frequency array, frequency step
// Out: none
#define FREQUENCY_GENERATOR(ARRAY, DELF) { \
    GenerateLinearArray(ARRAY->fArray, ARRAY->NoS, 0, DELF/ARRAY->NoS); \
}

// rectangular pulse shaper MARCO
// In: input signal with 'm' elements, output shaped signal with 'n' elements
// Out: none
#define RECT_PULSE_SHAPPER(ARRAY_IN, ARRAY_OUT) { \
    SampleArray(ARRAY_IN->fArray, ARRAY_IN->NoS, ARRAY_OUT->fArray, ARRAY_OUT->NoS, 0); \
}

// array copier MARCO
// In: input array with 'm' elements, output array with 'm' elements
#define COPY_ARRAY(ARRAY_IN, ARRAY_OUT) { \
    SampleArray(ARRAY_IN->fArray, min(ARRAY_IN->NoS, ARRAY_OUT->NoS), ARRAY_OUT->fArray, min(ARRAY_IN->NoS, ARRAY_OUT->NoS), 0); \
}

// bit sample extractor MACRO
// In: input signal with 'm' elements, offset index of the sampled element from the first sample in a bit, output samples with 'n' elements
// Out: none
#define BIT_SAMPLE_EXTRACTOR(ARRAY_IN, OFFSET, ARRAY_OUT) { \
    SampleArray(ARRAY_IN->fArray, ARRAY_IN->NoS, ARRAY_OUT->fArray, ARRAY_OUT->NoS, OFFSET); \
}

// bit sample quantiser MACRO
// In: input samples with 'm' elements, output bits with 'm' elements
// Out: none
#define BIT_SAMPLE_QUANTISER(ARRAY_IN, ARRAY_OUT) { \
    SliceArray(ARRAY_IN->fArray, ARRAY_IN->NoS, ARRAY_OUT->fArray, ARRAY_OUT->NoS, NUM_OF_CHUNCKS); \
}

// bit sample quantiser MACRO
// In: input signal with 'm' elements, output converted signal with 'm' elements
// Out: none
#define SIGNAL_CONV(ARRAY_IN, ARRAY_OUT, AMP, BIAS) { \
    LinearMapArray(ARRAY_IN->fArray, ARRAY_IN->NoS, ARRAY_OUT->fArray, ARRAY_OUT->NoS, AMP, BIAS); \
}

// normalise signal MACRO
// In: input signal with 'm' elements, output converted signal with 'm' elements
// Out: none
#define NORM_SIGNAL(ARRAY_IN, ARRAY_OUT) { \
    NormaliseSignal(ARRAY_IN->fArray, ARRAY_IN->NoS, ARRAY_OUT->fArray, ARRAY_OUT->NoS); \
}

// bit error rate calculator MACRO
// In: input bit sequence 1 with 'm' elements, input bit sequence 2 with 'm' elements
// Out: BER
#define BERT(ARRAY_1, ARRAY_2) \
    BERTester(ARRAY_1->fArray, ARRAY_1->NoS, ARRAY_2->fArray, ARRAY_2->NoS)

// AWGN generator MACRO
// In: AWGN array, noise rms value
// Out: none
#define GENERATE_AWGN(ARRAY, NOISE_RMS) { \
    for(int index = 0; index < ARRAY->NoS; index++) { \
        ARRAY->fArray[index] = GaussNormNumGen(); \
    } \
    LinearMapArray(ARRAY->fArray, ARRAY->NoS, ARRAY->fArray, ARRAY->NoS, NOISE_RMS, 0.0); \
}

// element wise adding signals MACRO
// In: input array with 'm' elements, input array with 'm' elements, output array with 'm' elements
// Out: none
#define ADD_SIGNALS(X, Y, SUM) \
    SignalMath(X->fArray, X->NoS, Y->fArray, Y->NoS, SUM->fArray, SUM->NoS, '+');

// geometric loss calculator MACRO
// In: Tx aperture diameter, beam half-angle divergence angle, link length, Rx aperture diameter
// Out: geometric loss
#define GEOMETRIC_LOSS_CALC(D_TX, DIV_ANG, LINK_LEN, D_RX) \
    GeometricLoss(D_TX, DIV_ANG, LINK_LEN, D_RX);

// fog/smoke loss calculator MACRO
// In: visibility, beam wavelength, link length
// Out: fog/smoke loss
#define FOG_LOSS_CALC(VIS, LAMBDA, LINK_LEN) \
    FogLoss(VIS, LAMBDA, LINK_LEN);

// miscellaneous loss calculator MACRO
// In: miscellaneous loss in dB
// Out: miscellaneous loss
#define MISC_LOSS_CALC(ATT_DB) \
    pow(10.0, -ATT_DB/10.0);

// turbulence channel coefficient calculator MACRO
// In: turbulence channel coefficient array, Rx aperture diameter, the structure refractive index (Cn2), beam wavelength, link length, sampling frequency
// Out: none
#define TURB_EFFECT_CALC(ARRAY, DRX, CN2, LAMBDA, LINK_LEN, FS) \
    TurbChanCoeff(ARRAY->fArray, ARRAY->NoS, DRX, CN2, LAMBDA, LINK_LEN, FS);

// pointing error channel coefficient calculator MACRO
// In: pointing error channel coefficient array, Tx aperture diameter, beam half-angle divergence angle, link length, Rx aperture diameter, jitter, sampling frequency
// Out: none
#define PE_EFFECT_CALC(ARRAY, D_TX, DIV_ANG, LINK_LEN, D_RX, JITTER, FS) \
    PEChanCoeff(ARRAY->fArray, ARRAY->NoS, D_TX, DIV_ANG, LINK_LEN, D_RX, JITTER, FS);

// apply channel effects to optical signal MACRO
// In: input optical signal array, output optical signal array, constant channel coefficient, turbulence channel coefficient array, pointing error channel coefficient array
// Out: none
#define CHANNEL_EFFECT(ARRAY_IN, ARRAY_OUT, CONSTANT_EFFECT, TURB_EFFECT, PE_EFFECT) \
    LinearMapArray(ARRAY_IN->fArray, ARRAY_IN->NoS, ARRAY_OUT->fArray, ARRAY_OUT->NoS, CONSTANT_EFFECT, 0); \
    SignalMath(ARRAY_OUT->fArray, ARRAY_OUT->NoS, TURB_EFFECT->fArray, TURB_EFFECT->NoS, ARRAY_OUT->fArray, ARRAY_OUT->NoS, '*'); \
    SignalMath(ARRAY_OUT->fArray, ARRAY_OUT->NoS, PE_EFFECT->fArray, PE_EFFECT->NoS, ARRAY_OUT->fArray, ARRAY_OUT->NoS, '*');

// remove DC voltage of the signal MACRO
// In: input signal with DC component array, output optical signal without DC component array
// Out: none
#define AC_COUPLING(ARRAY_IN, ARRAY_OUT) \
    LinearMapArray(ARRAY_IN->fArray, ARRAY_IN->NoS, ARRAY_OUT->fArray, ARRAY_OUT->NoS, 1.0, -MeanArray(ARRAY_IN->fArray, ARRAY_IN->NoS));

// measure the power of the signal MACRO
// In: input signal array, output optical signal array
// Out: signal power
#define POW_MEAS(ARRAY) \
    VarArray(ARRAY->fArray, ARRAY->NoS);

// calculate FFT of the signal MACRO
// In: input signal with 'm' elements, spectrum signal 'n' elements
// Out: signal power
#define SPECTRUM_CALC(ARRAY_IN, ARRAY_OUT) \
    FFTArray(ARRAY_IN->fArray, ARRAY_IN->NoS, ARRAY_OUT->fArray, ARRAY_OUT->NoS);

// structure definition section
typedef struct signalBundleStruct {
    int iSigID;  // signal ID
    float* fArray;  // values array
    int NoS;  // number of samples
} signalBund;  // signal bundle structure


// variable definition section
signalBund* pBitOrderSignal;  // bit order signal pointer [0 .. number of generated bits in one iteration]
signalBund* pBitNumSignal;  // bit number signal pointer [first bit index .. last bit index]
signalBund* pTxBitSignal;  // Tx bit signal pointer; output of PRBS generator
signalBund* pRxBitSignal;  // Rx bit signal pointer

signalBund* pOrderSignal;  // order signal pointer
signalBund* pSampNumSignal;  // sample number signal pointer
signalBund* pTimeSignal;  // time signal pointer
signalBund* pFreqSignal;  // frequency signal pointer

signalBund* pDataSignal;  // data signal pointer; output of rectangular pulse shaper
signalBund* pElecSignal;  // electrical signal pointer; output of modulator
signalBund* pTxOptSignal;  // Tx optical signal pointer; output of the laser

signalBund* pTurbCoeff;  // turbulence channel coefficient; LOG-NORMAL model for weak and Gamma-Gamma from moderate to strong
signalBund* pPECoeff;  // pointing error channel coefficient
signalBund* pChannelCoeff;  // channel coefficient; including: geometric/fog/miscellaneous loss + turbulence + pointing error

signalBund* pRxOptSignal;  // Rx optical signal pointer; output of rectangular pulse shaper
signalBund* pRxCurSignal;  // Rx detected current signal pointer; output of photodiode
signalBund* pRxVoltSignal;  // Rx amplified voltage signal pointer; output of TIA
signalBund* pNoiseSignal;  // Rx additive Gaussian noise; added by photo-receiver
signalBund* pRxDetSignal;  // Rx detected signal pointer; output of photo-receiver
signalBund* pRxSampledSignal;  // Rx sampled signal pointer; output of sampler

signalBund* pTxFFTSignal;  // Tx data spectrum signal

signalBund* pSignals;  // signals pointer

float fT0;  // first element in the time array
float delt_T;  // sampling time
float fBER;  // bit error rate (BER)
float fSNR;  // signal to noise ratio (SNR) (dB)
unsigned long iErrBit;  // number of erroneous bits
unsigned long iNumBit;  // number of total bits bits
float fGeoLossChCoeff;  // geometric loss channel coefficient; loss after propagation
float fFogLossChCoeff;  // fog/smoke loss channel coefficient; loss due to fog/smoke
float fMiscLossChCoeff;  // miscellaneous loss channel coefficient; loss due to miscellaneous


// function declaration section
void ConfigureSimulation(void);  // configure the parameters and initialise arrays for simulation
void TerminateSimulation(void);  // terminate the parameters and release arrays
void IterateSimulation(int iIteration);  // perform one iteration of the simulation


#endif // COREMODULE_H_INCLUDED
