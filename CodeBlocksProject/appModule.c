// header inclusion section
#include "appModule.h"
#include "grafixModule.h"
#include "threadModule.h"
#include "FSOModule.h"
#include "coreModule.h"
#include <time.h>


// Func: void CallBackHub(void)
// Desc: generic widget callback function
// In: widget id
// Out: none
void CallBackHub(int id) {
    if (id == dStStButtonID) {
        printf("Start/Stop button was pressed\n");
        switch (bStopFlag) {
        case TRUE:  // the button is showing Start
            swgtxt(dStStButtonID, STST_BUTTON_STOP_TEXT);  // change the button text
            swgatt(dPaCnButtonID, "active", "status");  // make the button enabled
            swgatt(dResetBERButtonID, "active", "status");  // make the button enabled
            swgatt(dScaledID, "active", "status");  // make the button disabled
            swgatt(dTimeScaleSliderID, "active", "status");  // make the button disabled

            // TODO: start the simulation
            CreateThreadFunc();  // create the computing thread process

            bStopFlag = FALSE;  // set the start/stop status to 1: 'Stop'
            break;

        case FALSE:  // the button is showing Stop
            swgtxt(dStStButtonID, STST_BUTTON_START_TEXT);  // change the button text
            swgatt(dPaCnButtonID, "inactive", "status");  // make the button enabled
            swgatt(dResetBERButtonID, "inactive", "status");  // make the button enabled
            swgatt(dScaledID, "inactive", "status");  // make the button disabled
            swgatt(dTimeScaleSliderID, "inactive", "status");  // make the button disabled

            // TODO: stop the simulation
            StopThreadFunc();  // stop the computing thread process

            bStopFlag = TRUE;  // set the start/stop status to 1: 'Stop'
            break;
        }
    }
     else if (id == dPaCnButtonID) {
        printf("Pause/Continue button was pressed\n");
        switch (bPauseFlag) {
        case TRUE:  // the button is showing Pause.
            swgtxt(dPaCnButtonID, PACN_BUTTON_CONT_TEXT);  // change the button text

            // TODO: pause the simulation
            PauseThreadFunc();  // pause the computing thread process

            bPauseFlag = FALSE;  // set the pause/continue status to 1: 'Continue'
            break;

        case FALSE:  // the button is showing Continue
            swgtxt(dPaCnButtonID, PACN_BUTTON_PAUSE_TEXT);  // change the button text

            // TODO: continue the simulation
            PauseThreadFunc();  // resume the computing thread process

            bPauseFlag = TRUE;  // set the pause/continue status to 1: 'Stop'
            break;
        }
    }
    else if (id == dResetBERButtonID) {  // reset BER parameter
        printf("Reset BERT button was pressed\n");
        iErrBit = 0;  // reset number of erroneous bits
        iNumBit = 0;  // reset total number of bits
    }
    else if (id == dScaledID) {  // scaled radio button
        printf("Scale button was changed\n");
        switch (bScaled) {
        case FALSE:  // 'Autoscale' is showing
            swgtxt(dScaledID, MN_BUTTON_SCALE_TEXT);  // change the button text
            bScaled = TRUE;  // set to 'Manual'
            break;

        case TRUE:  // 'Manual' is showing
            swgtxt(dScaledID, SC_BUTTON_SCALE_TEXT);  // change the button text
            bScaled = FALSE;  // set to 'Autoscale'
            break;
        }
    }
    else if (id == dTimeScaleSliderID) {  // time scale slider
        fTimeScale = gwgscl(id);  // get current value of the scale
        printf("Time scale slider is changed to %.2f\n", fTimeScale);
    }
    else {
        float val = gwgscl(id);  // get current value of the scale

        char* pSliderName = NULL;

        for(int index = 0; index < dNumofSliders; index++) {
            if(id == pSliderParam[index].dSliderID) {
                pSliderName = pSliderParam[index].sTitle;
                fSimParam[index] = val;  // update the simulation parameter
                break;
            }
        }

        printf("The value of %s is %.2f\n", pSliderName, val);
    }

    return;
}


// Func: void ResetGUI(void)
// Desc: reset the gui widgets
// In: none
// Out: none
void ResetGUI(void) {
    printf("Resetting GUI\n");

    swgtxt(dStStButtonID, STST_BUTTON_START_TEXT);  // change the button text

    swgatt(dPaCnButtonID, "active", "status");  // make the button enabled
    swgtxt(dPaCnButtonID, PACN_BUTTON_PAUSE_TEXT);  // change the button text
    swgatt(dPaCnButtonID, "inactive", "status");  // make the button enabled

    swgatt(dScaledID, "active", "status");  // make the button enabled
    swgtxt(dScaledID, SC_BUTTON_SCALE_TEXT);  // change the button text
    swgatt(dScaledID, "inactive", "status");  // make the button enabled

    swgatt(dTimeScaleSliderID, "active", "status");  // make the button enabled
    swgscl(dTimeScaleSliderID, 100.0);  // change the button text
    swgatt(dTimeScaleSliderID, "inactive", "status");  // make the button enabled

    bStopFlag = TRUE;  // set the start/stop status to 0: 'Start'
    bPauseFlag = TRUE;  // set the pause/continue status to 0: 'Pause'
    bScaled = FALSE;  // set the scaled/manual status to 1: 'Manual'
    fTimeScale = 100.0;  // set time scale coefficient to 100.0%
}


// Func: void Init(void)
// Desc: initialise the application
// In: none
// Out: none
void Init(void) {
    printf("Initialising\n");

    // initialise memory
    pPlotProp = NULL;
    pSliderParam = NULL;

    dNumofPlots = NO_OF_PLOTS;  // update the number of plots
    dNumofSliders = NO_OF_SLIDER;  // update the number of slides

    bStopFlag = TRUE;  // set the start/stop status to 0: 'Start'
    bPauseFlag = TRUE;  // set the pause/continue status to 0: 'Pause'

    bScaled = FALSE;  // set the scaled/manual status to 1: 'Manual'
    fTimeScale = 100.0;  // set time scale coefficient to 100.0%
    return;
}


// Func: void Destroy(void)
// Desc: terminates the application
// In: none
// Out none
void Destroy(void) {
    printf("Shutting down\n");

    if(bStopFlag == FALSE) {  // if simulation is running/pausing
        TerminateSimulation();  // release the resources
    }

    // release memory
    free(pPlotProp);
    pPlotProp = NULL;

    free(pSliderParam);
    pSliderParam = NULL;
}


// Func: void BuildPlotsWindow(void)
// Desc: create plots window.
//       To modify plots:
//            1- change 'NO_OF_PLOTS' value in 'parameters.h'
//            2- change 'PLOT_NO_OF_ROW' and 'PLOT_NO_OF_COLUMN' in 'parameters.h' to modify grid arrangement of plots, if needed
//            3- within the function below, add the properties for each plot.
//            4- the ID for x and y axes are based on the required signal to be plotted. The IDs can be found in function 'void ConfigureSimulation(void)' from 'appModule.c'
// In: none
// Out none
void BuildPlotsWindow(void) {
    printf("Creating plots window\n");

    pPlotProp = (plotProp*) calloc(dNumofPlots, sizeof(plotProp));

    pPlotProp[0].sXLabel = "Time (s)";
    pPlotProp[0].sYLabel = "Data Signal";
    pPlotProp[0].fXLabelStep = 90.0;
    pPlotProp[0].fYLabelStep = 0.5;
    pPlotProp[0].row = 1;
    pPlotProp[0].column = 1;
    pPlotProp[0].iXAxisID = 5;
    pPlotProp[0].iYAxisID = 8;
    pPlotProp[0].fXArray = NULL;
    pPlotProp[0].fYArray = NULL;
    pPlotProp[0].fOffset = 0.0;
    pPlotProp[0].fScale = 2;
    pPlotProp[0].iXDiv = 5;
    pPlotProp[0].iYDiv = 5;
    pPlotProp[0].iXUnitExp = 0;
    pPlotProp[0].iYUnitExp = 0;

    pPlotProp[1].sXLabel = "Frequency (MHz)";
    pPlotProp[1].sYLabel = "Spectrum (dB)";
    pPlotProp[1].fXLabelStep = 10.0;
    pPlotProp[1].fYLabelStep = 0.4;
    pPlotProp[1].row = 1;
    pPlotProp[1].column = 2;
    pPlotProp[1].iXAxisID = 7;
    pPlotProp[1].iYAxisID = 20;
    pPlotProp[1].fXArray = NULL;
    pPlotProp[1].fYArray = NULL;
    pPlotProp[1].fOffset = -2.5;
    pPlotProp[1].fScale = 10.0;
    pPlotProp[1].iXDiv = 5;
    pPlotProp[1].iYDiv = 5;
    pPlotProp[1].iXUnitExp = 6;
    pPlotProp[1].iYUnitExp = 0;

    pPlotProp[2].sXLabel = "Time (s)";
    pPlotProp[2].sYLabel = "Detected Signal";
    pPlotProp[2].fXLabelStep = 45.0;
    pPlotProp[2].fYLabelStep = 0.25;
    pPlotProp[2].row = 2;
    pPlotProp[2].column = 1;
    pPlotProp[2].iXAxisID = 5;
    pPlotProp[2].iYAxisID = 18;
    pPlotProp[2].fXArray = NULL;
    pPlotProp[2].fYArray = NULL;
    pPlotProp[2].fOffset = 0;
    pPlotProp[2].fScale = 0.01;
    pPlotProp[2].iXDiv = 5;
    pPlotProp[2].iYDiv = 5;
    pPlotProp[2].iXUnitExp = 0;
    pPlotProp[2].iYUnitExp = 0;

    PrepareFigure();  // prepare the plots gui

    return;
}


// Func: void BuildSlidersWindow(void)
// Desc: create sliders window
//       To modify sliders:
//            1- change 'NO_OF_SLIDER' value in 'parameters.h'
//            2- change 'SLIDER_NO_OF_ROW' and 'SLIDER_NO_OF_COLUMN' in 'parameters.h' to modify grid arrangement of plots, if needed
//            3- within the function below, add the properties for each plot.
//            4- the parameter is linked to the slider based on the 'dSliderID' parameter.
//               These values are updated in 'void CallBackHub(int id)' within the same file.
//               The IDs for parameters are based on the order of parameters in 'FSOParam' structure from 'FSOModule.h'
//
// In: none
// Out none
void BuildSlidersWindow(void) {
    printf("Creating plots window\n");

    pSliderParam = (sliderParam*) calloc(dNumofSliders, sizeof(sliderParam));

    pSliderParam[0].dSliderID = -1;
    pSliderParam[0].fCallback = &CallBackHub;
    pSliderParam[0].fDefVal = 1.0;
    pSliderParam[0].fMaxVal = 10.0;
    pSliderParam[0].fMinVal = 0.0;
    pSliderParam[0].fStep = 0.1;
    pSliderParam[0].sHelpText = "Tx aperture diameter is in millimetre";
    pSliderParam[0].sTitle = "Tx aperture diameter (mm)";

    pSliderParam[1].dSliderID = -1;
    pSliderParam[1].fCallback = &CallBackHub;
    pSliderParam[1].fDefVal = 0.05;
    pSliderParam[1].fMaxVal = 1.0;
    pSliderParam[1].fMinVal = 0.0;
    pSliderParam[1].fStep = 0.001;
    pSliderParam[1].sHelpText = "Laser beam half-angle divergence angle is in degrees";
    pSliderParam[1].sTitle = "Beam divergence angle (Deg)";

    pSliderParam[2].dSliderID = -1;
    pSliderParam[2].fCallback = &CallBackHub;
    pSliderParam[2].fDefVal = 0.0;
    pSliderParam[2].fMaxVal = 100.0;
    pSliderParam[2].fMinVal = -50.0;
    pSliderParam[2].fStep = 0.1;
    pSliderParam[2].sHelpText = "Laser beam average power is in logarithmic scale";
    pSliderParam[2].sTitle = "Average power (dBm)";

    pSliderParam[3].dSliderID = -1;
    pSliderParam[3].fCallback = &CallBackHub;
    pSliderParam[3].fDefVal = 850.0;
    pSliderParam[3].fMaxVal = 1600.0;
    pSliderParam[3].fMinVal = 400.0;
    pSliderParam[3].fStep = 1.0;
    pSliderParam[3].sHelpText = "Laser beam wavelength is in nanometre";
    pSliderParam[3].sTitle = "Wavelength (nm)";

    pSliderParam[4].dSliderID = -1;
    pSliderParam[4].fCallback = &CallBackHub;
    pSliderParam[4].fDefVal = 5.0;
    pSliderParam[4].fMaxVal = 100.0;
    pSliderParam[4].fMinVal = 1.0;
    pSliderParam[4].fStep = 0.1;
    pSliderParam[4].sHelpText = "Laser extinction ratio is unit-less";
    pSliderParam[4].sTitle = "Extinction ratio";

    pSliderParam[5].dSliderID = -1;
    pSliderParam[5].fCallback = &CallBackHub;
    pSliderParam[5].fDefVal = 10.0;
    pSliderParam[5].fMaxVal = 500.0;
    pSliderParam[5].fMinVal = 1.0;
    pSliderParam[5].fStep = 1.0;
    pSliderParam[5].sHelpText = "Rx aperture diameter is in millimetre";
    pSliderParam[5].sTitle = "Rx aperture diameter (mm)";

    pSliderParam[6].dSliderID = -1;
    pSliderParam[6].fCallback = &CallBackHub;
    pSliderParam[6].fDefVal = 0.5;
    pSliderParam[6].fMaxVal = 50.0;
    pSliderParam[6].fMinVal = 0.0;
    pSliderParam[6].fStep = 0.05;
    pSliderParam[6].sHelpText = "Photodiode responsivity is in amperes per watt";
    pSliderParam[6].sTitle = "Responsivity (A/W)";

    pSliderParam[7].dSliderID = -1;
    pSliderParam[7].fCallback = &CallBackHub;
    pSliderParam[7].fDefVal = 1.0;
    pSliderParam[7].fMaxVal = 10.0;
    pSliderParam[7].fMinVal = 0.001;
    pSliderParam[7].fStep = 0.01;
    pSliderParam[7].sHelpText = "Transimpedance amplifier gain is in kilo Ohms";
    pSliderParam[7].sTitle = "TIA gain (k Ohms)";

    pSliderParam[8].dSliderID = -1;
    pSliderParam[8].fCallback = &CallBackHub;
    pSliderParam[8].fDefVal = 20.0;
    pSliderParam[8].fMaxVal = 1000.0;
    pSliderParam[8].fMinVal = 1.0;
    pSliderParam[8].fStep = 1.0;
    pSliderParam[8].sHelpText = "Noise equivalent power is in pico Watts per square root of bandwidth";
    pSliderParam[8].sTitle = "NEP (pW/sqrt(Hz))";

    pSliderParam[9].dSliderID = -1;
    pSliderParam[9].fCallback = &CallBackHub;
    pSliderParam[9].fDefVal = 50.0;
    pSliderParam[9].fMaxVal = 1000.0;
    pSliderParam[9].fMinVal = 1.0;
    pSliderParam[9].fStep = 50.0;
    pSliderParam[9].sHelpText = "Load impedance is in Ohms";
    pSliderParam[9].sTitle = "Z (Ohms)";

    pSliderParam[10].dSliderID = -1;
    pSliderParam[10].fCallback = &CallBackHub;
    pSliderParam[10].fDefVal = 1.0;
    pSliderParam[10].fMaxVal = 1000.0;
    pSliderParam[10].fMinVal = 0.01;
    pSliderParam[10].fStep = 0.1;
    pSliderParam[10].sHelpText = "Bit-rate is in mega bits per second";
    pSliderParam[10].sTitle = "BR (Mbps)";

    pSliderParam[11].dSliderID = -1;
    pSliderParam[11].fCallback = &CallBackHub;
    pSliderParam[11].fDefVal = 100.0;
    pSliderParam[11].fMaxVal = 1000.0;
    pSliderParam[11].fMinVal = 1.0;
    pSliderParam[11].fStep = 5.0;
    pSliderParam[11].sHelpText = "FSO link length is in meters";
    pSliderParam[11].sTitle = "Link Length (m)";

    pSliderParam[12].dSliderID = -1;
    pSliderParam[12].fCallback = &CallBackHub;
    pSliderParam[12].fDefVal = 10.0;
    pSliderParam[12].fMaxVal = 20.0;
    pSliderParam[12].fMinVal = 0.001;
    pSliderParam[12].fStep = 0.01;
    pSliderParam[12].sHelpText = "Channel visibility in kilo meters";
    pSliderParam[12].sTitle = "Visibility (km)";

    pSliderParam[13].dSliderID = -1;
    pSliderParam[13].fCallback = &CallBackHub;
    pSliderParam[13].fDefVal = -15;
    pSliderParam[13].fMaxVal = -10;
    pSliderParam[13].fMinVal = -20;
    pSliderParam[13].fStep = 0.1;
    pSliderParam[13].sHelpText = "The structure constant of refractive index of turbulence channel is in base 10 logarithm of meter to the power of -2/3";
    pSliderParam[13].sTitle = "log10(Cn2) (m^(-2/3))";

    pSliderParam[14].dSliderID = -1;
    pSliderParam[14].fCallback = &CallBackHub;
    pSliderParam[14].fDefVal = 1.0;
    pSliderParam[14].fMaxVal = 50.0;
    pSliderParam[14].fMinVal = 0.0;
    pSliderParam[14].fStep = 0.05;
    pSliderParam[14].sHelpText = "The jitter of pointing error channel is in millimetre";
    pSliderParam[14].sTitle = "Pointing error jitter (mm)";

    pSliderParam[15].dSliderID = -1;
    pSliderParam[15].fCallback = &CallBackHub;
    pSliderParam[15].fDefVal = 2.0;
    pSliderParam[15].fMaxVal = 20.0;
    pSliderParam[15].fMinVal = 0.0;
    pSliderParam[15].fStep = 0.01;
    pSliderParam[15].sHelpText = "The miscellaneous loss is in decibel";
    pSliderParam[15].sTitle = "Misc loss (dB)";

    PrepareWidget();  // prepare the sliders gui

    InitSimPntParam();  // initialise pointers of simulation parameters

    return;
}
