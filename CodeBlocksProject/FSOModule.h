#ifndef FSOMODULE_H_INCLUDED
#define FSOMODULE_H_INCLUDED


// header inclusion section
#include "dislin.h"  // include DISLIN header file for plotting figures
#include "parameters.h"  // include predefined parameters


// macro definition section


// structure definition section
typedef struct FSOParameters {
    float fD_Tx;  // Tx aperture diameter (mm)
    float fTheta;  // Tx half-angle divergence angle (Deg)
    float fP_avg;  // Tx laser average power (dBm)
    float fLambda;  // Tx laser wavelength (nm)
    float fExtRatio;  // Tx extinction ratio ()

    float fD_Rx;  // Rx aperture diameter (mm)
    float fResp;  // Rx photodiode responsivity (A/W)
    float fG_TIA;  // Rx transimpedance apmlifer gain (kOhms)
    float fNEP;  // Rx noise equivalent power (pW/sqrt(Hz))
    float fImp;  // Rx impedance (Ohms)

    float fBR;  // signal bandwidth (MHz)
    float fLinkLen;  // FSO link length  (m)

    float fVis;  // channel visibility (km)
    float fCn2;  // turbulence structure refractive index * 1e-15 (m^-2/3)
    float fJit;  // pointing error jitter (mm)
    float fMisc;  // miscellaneous loss (dB)
} FSOParam;  // FSO parameters structure

// variable definition section
FSOParam* simParamPointer;  // simulation parameters
float fSimParam[NO_OF_SLIDER];   // pointer to the simulation parameters

// function declaration section
void InitSimPntParam(void);  // initialise simulation pointer parameters
float GeometricLoss(float fDTx, float fTheta, float fLinkLen, float fDRx);  // calculate the geometric loss
float FogLoss(float fVis, float fLambda, float fLinkLen);  // calculate the fog loss
void TurbChanCoeff(float *pCoeff, int iNoP, float fDRx, float fCn2, float fLambda, float fLinkLen, float fF_s);  // calculate the turbulence effect
void PEChanCoeff(float *pCoeff, int iNoP, float fDTx, float fTheta, float fLinkLen, float fDRx, float fJitter, float fF_s);  // calculate the pointing error effect

/*
http://www.hongliangjie.com/2012/12/19/how-to-generate-gamma-random-variables/
https://www.gnu.org/software/gsl/
*/
#endif // FSOMODULE_H_INCLUDED
