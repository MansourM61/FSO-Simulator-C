// header inclusion section
#include "FSOModule.h"
#include "grafixModule.h"
#include "sigProcModule.h"
#include "sigGenModule.h"
#include <math.h>


// Func: void InitSimPntParam(void)
// Desc: initialise simulation pointer parameters
// In: none
// Out: none
void InitSimPntParam(void) {

    int id;  // slider ID

    for(int index = 0; index < dNumofSliders; index++) {
        id = pSliderParam[index].dSliderID;
        fSimParam[index] = gwgscl(id);  // get current value of the scale
    }

    simParamPointer = (FSOParam*) fSimParam;  // set the structure address to the parameters array address

}


// Func: float GeometricLoss(float fDTx, float fTheta, float fLinkLen, float fDRx)
// Desc: calculate the geometric loss
// In: Tx aperture diameter, beam half-angle divergence angle, link length, Rx aperture diameter
// Out: channel coefficient
float GeometricLoss(float fDTx, float fTheta, float fLinkLen, float fDRx) {

    float fD_B_Rx = fDTx + 2.0*fLinkLen*tan(fTheta*M_PI/180.0);  // beam diameter at Rx side

    return min(POW2(fDRx/fD_B_Rx), 1.0);
}


// Func: float FogLoss(float fVis, float fLambda, float fLinkLen)
// Desc: calculate the fog loss
// In: visibility, lambda, link length
// Out: channel coefficient
float FogLoss(float fVis, float fLambda, float fLinkLen) {

    float q = 0;   // q parameter

    if(fVis > 50.0) {
        q = 1.6;
    }
    else if (fVis <= 50.0 && fVis > 6.0) {
        q = 1.3;
    }
    else if (fVis <= 6.0 && fVis > 1.0) {
        q = 0.67*fVis + 0.34;
    }
    else if (fVis <= 1.0 && fVis > 0.5) {
        q = fVis + 0.5;
    }

    float fbeta_l = (-log(T_TH_FS)/fVis) * pow(fLambda/LAMBDA_0, -q);  // beta_lambda

    return exp(-fbeta_l*fLinkLen/1.0e3);
}


// Func: void TurbChanCoeff(float *pCoeff, int iNoP, float fCn2, float fLambda, float fLinkLen, float fF_s)
// Desc: calculate the turbulence effect
// In: channel coefficient array, number of elements, Rx aperture diameter, the refractive index structure parameter (Cn2), lambda, link length
// Out: none
void TurbChanCoeff(float *pCoeff, int iNoP, float fDRx, float fCn2, float fLambda, float fLinkLen, float fF_s) {

    float k = 2*M_PI/fLambda;  // wave number (rad/m)
    float sig2_R = 1.23*fCn2*pow(k, 7.0/6.0)*pow(fLinkLen, 11.0/6.0);  // Rytov variance
    float fD_AAF = sqrt(k*POW2(fDRx)/(4.0*fLinkLen));  // auxilary parameter for aperture averaging effect
    float fAAF = pow(1.0 + 1.062*POW2(fD_AAF), -7.0/6.0);  // aperture averaging factor

    float sig_x = sqrt(fAAF*sig2_R/4.0);  // sigma parameter weak turbulence
    float mu_x = -fAAF*sig2_R/4.0;  // mu parameter weak turbulence

    float fF_t = 1.0/TEMP_COH_TURB;  // turbulence fading maximum frequency (Hz)

    int iNoT = (int)ceil(fF_t/fF_s*iNoP);  // number of turbulence sample points

    float* pU = (float *)calloc(iNoT*2, sizeof(float));  // allocate memory for turbulence samples

    for(int index = 0; index < iNoT*2; index++) {  // loop through the array
        pU[index] = exp( 2.0*(GaussNormNumGen()*sig_x + mu_x) );  // calculate Log-Normal random sequence
    }

    float* pTempArray_1 = (float *)calloc(iNoP*2, sizeof(float));  // allocate temporary memory 1 for re-sampled turbulence samples
    float* pTempArray_2 = (float *)calloc(iNoP*2, sizeof(float));  // allocate temporary memory 2 for re-sampled turbulence samples

    SampleArray(pU, iNoT, pTempArray_1, iNoP*2, 0);  // re-sample turbulence array

    SmoothArray_m3(pTempArray_1, iNoP*2, pTempArray_2, iNoP*2, FILTER_ALFA);  // smoothen the signal

    int iIndexStart = (int) ceil(iNoP/2.0);  // start index of sampling
    for(int index = 0; index < iNoP; index++) {  // loop through the array
        pCoeff[index] = pTempArray_2[iIndexStart + index];  // copy turbulence coefficients to channel coefficient array
    }

    free(pTempArray_2);  // free temporary turbulence memory
    pTempArray_2 = NULL;
    free(pTempArray_1);  // free temporary turbulence memory
    pTempArray_1 = NULL;
    free(pU);  // free turbulence memory
    pU = NULL;
}


// Func: void PEChanCoeff(float *pCoeff, int iNoE, float fJitter)
// Desc: calculate the pointing error effect
// In: channel coefficient array, number of elements, Tx aperture diameter, beam half-angle divergence angle, link length, Rx aperture diameter, pointing error jitter, sampling frequency
// Out: none
void PEChanCoeff(float *pCoeff, int iNoP, float fDTx, float fTheta, float fLinkLen, float fDRx, float fJitter, float fF_s) {

    float fF_pe = 1.0/TEMP_COH_TURB;  // pointing error fading maximum frequency (Hz)

    float fW_Rx = fDTx/2.0 + fLinkLen*tan(fTheta*M_PI/180.0);  // beam radius at Rx side

    float v_PE = sqrt(M_PI/2.0)*(fDRx/(2.0*fW_Rx));  // aperture to vertical beam size ratio
    float fW2_eq_PE = POW2(fW_Rx)*sqrt(M_PI)*erf(v_PE)/(2.0*v_PE*exp(-POW2(v_PE)));  // equivalent vertical beam size at receiver side

    float r;  // Gaussian distributed random value

    int iNoPE = (int)ceil(fF_pe/fF_s*iNoP);  // number of pointing error sample points

    float* pU = (float *)calloc(iNoPE*2, sizeof(float));  // allocate memory for turbulence samples

    for(int index = 0; index < iNoPE*2; index++) {  // loop through the array
        r = GaussNormNumGen()*fJitter;
        pU[index] = POW2(exp(-2.0*POW2(r)/fW2_eq_PE));  // calculate Rician random sequence
    }

    float* pTempArray_1 = (float *)calloc(iNoP*2, sizeof(float));  // allocate temporary memory 1 for re-sampled turbulence samples
    float* pTempArray_2 = (float *)calloc(iNoP*2, sizeof(float));  // allocate temporary memory 2 for re-sampled turbulence samples

    SampleArray(pU, iNoPE, pTempArray_1, iNoP*2, 0);  // re-sample turbulence array

    SmoothArray_m3(pTempArray_1, iNoP*2, pTempArray_2, iNoP*2, FILTER_ALFA);  // smoothen the signal

    int iIndexStart = (int) ceil(iNoP/2.0);  // start index of sampling
    for(int index = 0; index < iNoP; index++) {  // loop through the array
        pCoeff[index] = pTempArray_2[iIndexStart + index];  // copy turbulence coefficients to channel coefficient array
    }

    free(pTempArray_2);  // free temporary turbulence memory
    pTempArray_2 = NULL;
    free(pTempArray_1);  // free temporary turbulence memory
    pTempArray_1 = NULL;
    free(pU);  // free turbulence memory
    pU = NULL;
}
