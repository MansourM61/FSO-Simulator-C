// header inclusion section
#include "coreModule.h"
#include "grafixModule.h"
#include "FSOModule.h"


// Func: void ConfigureSimulation(void)
// Desc: configure the parameters and initialise arrays for simulation
// In: none
// Out: none
void ConfigureSimulation(void) {
    printf("Initialising/configuring simulation\n");

    pSignals = (signalBund*) calloc(NUM_OF_STORED_SIGNAL, sizeof(signalBund));  // allocate memory for signals

    for(int index = 0; index < NUM_OF_BASE_SIGNAL; index++) {
        pSignals[index].fArray = (float*) calloc(BIT_SEQ_LEN, sizeof(float));  // allocate memory for array
        pSignals[index].NoS = BIT_SEQ_LEN;  // number of samples
        pSignals[index].iSigID = index;  // ID of signal
    }

    for(int index = NUM_OF_BASE_SIGNAL; index < NUM_OF_STORED_SIGNAL; index++) {
        if(index == 7 || index == 19 || index == 20) {
            continue;
        }
        pSignals[index].fArray = (float*) calloc(BIT_SEQ_LEN*SAMPLES_PER_BIT, sizeof(float));  // allocate memory for array
        pSignals[index].NoS = BIT_SEQ_LEN*SAMPLES_PER_BIT;  // number of samples
        pSignals[index].iSigID = index;  // ID of signal
    }

    // array for Rx sampled signal
    pSignals[19].fArray = (float*) calloc(BIT_SEQ_LEN, sizeof(float));  // allocate memory for array
    pSignals[19].NoS = BIT_SEQ_LEN;  // number of samples
    pSignals[19].iSigID = 19;  // ID of signal

    int iFFTSize = NEXT_POW_2(BIT_SEQ_LEN*SAMPLES_PER_BIT)/2;
    // array for frequency signal
    pSignals[7].fArray = (float*) calloc(iFFTSize, sizeof(float));  // allocate memory for array
    pSignals[7].NoS = iFFTSize;  // number of samples
    pSignals[7].iSigID = 7;  // ID of signal
    // array for spectrum signal
    pSignals[20].fArray = (float*) calloc(iFFTSize, sizeof(float));  // allocate memory for array
    pSignals[20].NoS = iFFTSize;  // number of samples
    pSignals[20].iSigID = 20;  // ID of signal

    pBitOrderSignal = (pSignals + 0);  // update bit order signal pointer; order signal ID = 0
    pBitNumSignal = (pSignals + 1);  // update bit number signal pointer; order signal ID = 1
    pTxBitSignal = (pSignals + 2);  // update Tx bits signal pointer; Tx bits signal ID = 2
    pRxBitSignal = (pSignals + 3);  // update Rx bits signal pointer; Rx bits signal ID = 3

    pOrderSignal = (pSignals + 4);  // update order signal pointer; order signal ID = 4
    pSampNumSignal = (pSignals + 5);  // update sample number signal pointer; order signal ID = 5
    pTimeSignal = (pSignals + 6);  // update time signal pointer; time signal ID = 6
    pFreqSignal = (pSignals + 7);  // update frequency signal pointer; frequency signal ID = 7

    pDataSignal = (pSignals + 8);  // update data signal pointer; data signal ID = 8
    pElecSignal = (pSignals + 9);  // update electrical signal pointer; electrical signal ID = 9
    pTxOptSignal = (pSignals + 10);  // Tx optical power signal pointer; Tx optical signal signal ID = 10

    pTurbCoeff = (pSignals + 11);  // turbulence channel coefficient pointer; turbulence channel coefficient signal ID = 11
    pPECoeff = (pSignals + 12);  // pointing error channel coefficient pointer; pointing error channel coefficient signal ID = 12
    pChannelCoeff = (pSignals + 13);  // channel coefficient pointer; channel coefficient signal ID = 13

    pRxOptSignal = (pSignals + 14);  // Rx optical power signal pointer; Rx optical signal ID = 14
    pRxCurSignal = (pSignals + 15);  // Rx detected current signal pointer; Rx detected current signal ID = 15
    pRxVoltSignal = (pSignals + 16);  // Rx amplified voltage signal pointer; Rx amplified voltage signal ID = 16
    pNoiseSignal = (pSignals + 17);  // Rx additive white Gaussian noise pointer; Rx AWGN signal ID = 17
    pRxDetSignal = (pSignals + 18);  // Rx detected signal pointer; Rx detected signal ID = 18
    pRxSampledSignal = (pSignals + 19);  // Rx sampled signal pointer; Rx sampled signal ID = 19
    pTxFFTSignal = (pSignals + 20);  // Tx data spectrum signal; Tx data spectrum signal ID = 20

    iErrBit = 0;  // set number of erogenous bits to 0
    iNumBit = 0;  // set number of total bits to 0
    fBER = 0.0;  // initialise BER
    fT0 = 0.0;  // initialise the time

    ORDER_GENERATOR(pBitOrderSignal, 0);  // fill bit order array
    ORDER_GENERATOR(pOrderSignal, 0);  // fill order signal array
}


// Func: void TerminateSimulation(void)
// Desc: terminate the parameters and release arrays
// In: none
// Out: none
void TerminateSimulation(void) {
    printf("Terminating simulation\n");

    // release signal memories
    for(int index = 0; index < NUM_OF_STORED_SIGNAL; index++) {
        free(pSignals[index].fArray);
        pSignals[index].fArray = NULL;
    }
    pSignals = NULL;
}


// Func: void IterateSimulation(void)
// Desc: perform one iteration of the simulation
// In: iteration number
// Out: none
void IterateSimulation(int iIteration) {

    float fBitDuration = 1.0/(simParamPointer->fBR*1.0e6);  // bit duration (s)
    delt_T = fBitDuration/SAMPLES_PER_BIT;  // sampling time (s)

    FREQUENCY_GENERATOR(pFreqSignal, 0.5/delt_T);  // update frequency signal array

    // pseudorandom bit sequence (PRBS) generation
    ORDER_GENERATOR(pBitNumSignal, iIteration);  // update bit order signal array
    BIT_GENERATOR(pTxBitSignal);  //  update random bit signal array

    // time generation
    ORDER_GENERATOR(pSampNumSignal, iIteration);  // update order signal array
    TIME_GENERATOR(pTimeSignal, iIteration, fT0, delt_T)  // update time signal array
    fT0 = pTimeSignal->fArray[pTimeSignal->NoS - 1];  // update start time

    // PRBS to data signal conversion
    RECT_PULSE_SHAPPER(pTxBitSignal, pDataSignal);  // sample the generated bits and create data signal using rectangular pulse shaping

    // FFT calculation
    SPECTRUM_CALC(pDataSignal, pTxFFTSignal);  // calculate FFT signal of the Tx data signal

    // data signal to electrical signal conversion
    SIGNAL_CONV(pDataSignal, pElecSignal, 5.0, -2.5);  // convert data signal to bipolar NRZ-OOK electrical signal (amplitude and offset)

    // change electrical signal to Tx optical power signal
    float fP_avg = pow(10.0, simParamPointer->fP_avg/10.0)*1.0e-3;  // laser average power (W)
    float P_low = 2.0*fP_avg/(1.0 + simParamPointer->fExtRatio);  // output power for high state
    float P_high = 2.0*fP_avg/(1.0 + 1.0/simParamPointer->fExtRatio);  // output power for low state
    float fDelP = P_high - P_low;  // swing power of the laser
    NORM_SIGNAL(pElecSignal, pTxOptSignal);  // NORMALISE SIGNAL
    SIGNAL_CONV(pTxOptSignal, pTxOptSignal, fDelP/2.0, fP_avg);  // convert electrical signal to Tx optical signal (amplitude and average)

    // calculate geometrical loss
    fGeoLossChCoeff = GEOMETRIC_LOSS_CALC(simParamPointer->fD_Tx*1.0e-3, simParamPointer->fTheta, simParamPointer->fLinkLen, simParamPointer->fD_Rx*1.0e-3);  // calculate geometric loss channel coefficient; loss after propagation

    // calculate fog loss
    fFogLossChCoeff = FOG_LOSS_CALC(simParamPointer->fVis, simParamPointer->fLambda, simParamPointer->fLinkLen);  // optical loss due to fog/smoke

    // calculate miscellaneous loss
    fMiscLossChCoeff = MISC_LOSS_CALC(simParamPointer->fMisc);  // loss due to miscellaneous effects

    // calculate turbulence effect
    TURB_EFFECT_CALC(pTurbCoeff, simParamPointer->fD_Rx*1.0e-3, pow(10, simParamPointer->fCn2), simParamPointer->fLambda*1.0e-9, simParamPointer->fLinkLen, 1.0/delt_T);  // turbulence effect

    // calculate pointing error effect
    PE_EFFECT_CALC(pPECoeff, simParamPointer->fD_Tx*1.0e-3, simParamPointer->fTheta, simParamPointer->fLinkLen, simParamPointer->fD_Rx*1.0e-3, simParamPointer->fJit*1.0e-3, 1.0/delt_T);  // pointing error effect

    float fTxSignalPower = POW_MEAS(pTxOptSignal);  // measure power of Tx optical signal
    float fConstantLoss = fGeoLossChCoeff*fFogLossChCoeff*fMiscLossChCoeff;  // channel constant loss

    // apply channel effects
    CHANNEL_EFFECT(pTxOptSignal, pRxOptSignal, fConstantLoss, pTurbCoeff, pPECoeff); // apply channel effects to the optical signal

    // optical signal to current signal conversion
    SIGNAL_CONV(pRxOptSignal, pRxCurSignal, simParamPointer->fResp, 0.0);  // convert Rx optical power signal Rx electrical current signal
    AC_COUPLING(pRxCurSignal, pRxCurSignal);  // remove DC part of the signal

    // current signal to voltage signal conversion
    SIGNAL_CONV(pRxCurSignal, pRxVoltSignal, simParamPointer->fG_TIA*1.0e3, 0.0);  // convert Rx current signal Rx voltage signal

    float fRxSignalPower = (fConstantLoss*simParamPointer->fResp*simParamPointer->fG_TIA*1.0e3)*
                           (fConstantLoss*simParamPointer->fResp*simParamPointer->fG_TIA*1.0e3)*
                           (fTxSignalPower/simParamPointer->fImp);  // measure power of Rx optical signal

    // add white Gaussian noise
    float fBW = BW_TO_BR_RATIO*simParamPointer->fBR*1.0e6;  // signal bandwidth (Hz)
    float fNoisePower = simParamPointer->fNEP*1.0e-12*sqrt(fBW);  // power of the noise
    float fNoiseVoltageRMS = sqrt(fNoisePower*simParamPointer->fImp);  // noise voltage rms
    GENERATE_AWGN(pNoiseSignal, fNoiseVoltageRMS);  // generate additive white Gaussian noise array
    ADD_SIGNALS(pRxVoltSignal, pNoiseSignal, pRxDetSignal);  // add noise to the signal

    // extract Rx bits
    BIT_SAMPLE_EXTRACTOR(pRxDetSignal, (int)(SAMPLES_PER_BIT/2.0), pRxSampledSignal); //  extract Rx bit samples from received electrical signal
    BIT_SAMPLE_QUANTISER(pRxSampledSignal, pRxBitSignal);  // quantise the sampled received signal

    // BER calculation
    iNumBit += BIT_SEQ_LEN;  // update the number of total bits
    iErrBit += BERT(pTxBitSignal, pRxBitSignal);  // calculate number of erroneous bits
    fBER = 1.0*iErrBit/iNumBit;  // calculate BER
    fSNR = fRxSignalPower/fNoisePower;  // SNR calculation
    printf("SNR = %.3f dB, BER = %.2e\n", 10.0*log10(fSNR), fBER);  // print SNR and BER to the console
}
