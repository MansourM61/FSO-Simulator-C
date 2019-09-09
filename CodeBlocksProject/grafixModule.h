#ifndef GRAFIXMODULE_H_INCLUDED
#define GRAFIXMODULE_H_INCLUDED


// header inclusion section
#include "dislin.h"  // include DISLIN header file for plotting figures
#include "parameters.h"  // include predefined parameters
#include "coreModule.h"


// macro definition section
#define AXES_VERT_GAP (int)(4*AXIS_FONT_SIZE + AXIS_VERT_GAP_MARGIN)  // vertical gap between axis
#define AXES_HORZ_GAP (int)(4*AXIS_FONT_SIZE + AXIS_HORZ_GAP_MARGIN)  // horizontal gap between axis
#define AXES_WIDTH (int)(PAGE_WIDTH/PLOT_NO_OF_ROW - AXES_VERT_GAP*(PLOT_NO_OF_ROW + 1)/PLOT_NO_OF_ROW)  // axes width
#define AXES_HEIGHT (int)(PAGE_HEIGHT/PLOT_NO_OF_COLUMN - AXES_HORZ_GAP*(PLOT_NO_OF_COLUMN + 1)/PLOT_NO_OF_COLUMN)  // axes width
#define AXES_X_POS (int)(AXES_VERT_GAP)  // axes x position
#define AXES_Y_POS (int)(AXES_HORZ_GAP)  // axes y position

#define ABOT_TEXT_LEN 1000
#define ABOUT_TEXT "FSO Simulation Program\nWritten by Mojtaba Mansour Abadi\n\
email: mansourabadi.mojtaba@gmail.com\n\nThe code is written in C language using Code::Blocks version 17.12.\n\n\
The code performs a real time simulation by generating psedurandom bit sequence on the fly\
. The bit sequence is changed into electrical signal which modulates a laser later on\
. The output of the laser is launched into a free space optical channel where laser output is exposed to propagation\
, fog/smoke, turbulence, pointing error, and miscellaneous effects\
. The optical beam then is received by an aperture and then converted to electrical signal using a photo-receiver\
. Then received bits are extracted and compared to transmit bits in order to measure bit-error-rate (BER) of the system\
. The turbulence effect is based on Log-Normal model and for fog/smoke, Kim model is utilised.\n\n\
For GUIs and plots DISLIN a free library is used.\nTo perform FFT operation, GSL an open source library is used.\n"

// structure definition section
typedef struct plotProperties {
    char* sXLabel;  // plot x label
    char* sYLabel;  // plot y label
    float fXLabelStep;  // x axis label step
    float fYLabelStep;  // y axis label step
    float* fXArray;  // x axis array
    float* fYArray;  // y axis array
    int iXUnitExp;  // x axis unit exponent
    int iYUnitExp;  // y axis unit exponent
    int iXAxisID;  // x axis ID
    int iYAxisID;  // y axis ID
    int iXDiv;  // number of x axis division
    int iYDiv;  // number of x axis division
    int row;  // row position of the plot in the plot table
    int column;  // column position of the plot in the plot table
    float fOffset;  // offset value of y label
    float fScale;  // scale value of y label
    int iNumOfElem;  // number of elements
} plotProp;  // plot parameters structure

typedef struct sliderParameters {
    char* sTitle;  // slider title
    char* sHelpText;  // slider help text, to be added to help menu
    float fMinVal;  // slider minimum value
    float fMaxVal;  // slider maximum value
    float fDefVal;  // slider default value
    float fStep;  // slider changing step
    void(* fCallback)(int);  // slider callback function pointer
    int dSliderID;  // slider ID
} sliderParam;  // slider parameters structure


// variable definition section
plotProp* pPlotProp;  // plot structure pointer
int dNumofPlots;  // number of plots
sliderParam* pSliderParam;  // slider structure pointer
int dNumofSliders;  // number of sliders
int dSliderWindowsID;  // slider window ID
int dStStButtonID;  // start/stop push button ID
int dPaCnButtonID;  // pause/continue push button ID
int dResetBERButtonID;  // reset BER button push ID
int dScaledID;  // Autoscale ID
int dTimeScaleSliderID;  // time scale slider ID
BOOL bScaled;  // scaled flag (TRUE = Autoscale, FALSE = Manual)
float fTimeScale;  // time scale coefficient

// function declaration section
void PrepareFigure(void);  // prepares the plot window to show the curves
void PrepareWidget(void);  // prepares the widget control window to show the curves
void AssignSignalsToPlots(void);  // assign the plots to associated signals
void ReassignPlots(void);  // reassign the plots signals to NULL
void UpdatePlots(void);  // update the plots with assigned signals

#endif // GRAFIXMODULE_H_INCLUDED
