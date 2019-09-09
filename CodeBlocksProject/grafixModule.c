// header inclusion section
#include "grafixModule.h"
#include "appModule.h"
#include "coreModule.h"
#include "sigProcModule.h"
#include <math.h>


// Func: void PrepareFigure(void)
// Desc: prepares the plot window to show the curves
// In: none
// Out: none
void PrepareFigure(void) {
    printf("Creating plots guis\n");

    // Level 0
    page(PAGE_WIDTH, PAGE_HEIGHT);  // set the page size
    winapp("CONSOLE");  // defines if the DISLIN program should look like a Windows console
    metafl("XWINS");  // defines a graphics output on the screen. If the screen is a windows display, a graphical window is used that has nearly the size of the screen.
    scrmod("revers");  // background colour is set to 'WHITE' and the foreground colour to 'BLACK'
    x11mod("STORE");  // defines backing store for X graphic windows. Graphical output is send to a pixmap that will be copied to the graphics window

    // Level 1
    disini();  // dislin initialization
    wintit(PLOT_WINDOWS_TITLE);  // windows title
    complx();  // sets the current font to complex font
    winmod("NONE");  // affects the handling of windows in the termination routine DISFIN. The program is not blocked in DISFIN and windows are not deleted
    pagera();  // plots a border around the page

    labdig(2, "x");  // sets the number of digits after the decimal point displayed in labels to be zero (label numbers are set to integer)
    labdig(2, "y");  // sets the number of digits after the decimal point displayed in labels to be zero (label numbers are set to integer)
    ticks(10, "x");  // define the number of ticks between axis labels
    ticks(10, "y");  // define the number of ticks between axis labels

    int ic = intrgb(0.95, 0.95, 0.95);  // creates an explicit colour value from RGB coordinates
    axsbgd(ic);  // defines a background colour for axis systems

    int row = 0;  // plot row index
    int column = 0;  // plot column index

    int dXPos = 0;  // plot x position
    int dYPos = 0;  // plot y position

    for(int index = 0; index < dNumofPlots; index ++) {
        axslen(AXES_WIDTH, AXES_HEIGHT);  // defines the size of an axis system

        height(AXIS_FONT_SIZE);  // defines the character height or axis tick
        hname(AXIS_FONT_SIZE);  // defines the character height of axis label

        name(pPlotProp[index].sXLabel, "x");  // defines 'x' axis titles
        name(pPlotProp[index].sYLabel, "y");  // defines 'y' axis titles

        column = pPlotProp[index].column - 1;  // update column index of the plot
        row = PLOT_NO_OF_ROW - pPlotProp[index].row;  // update row index of the plot

        dXPos = AXES_X_POS + column*(AXES_HORZ_GAP + AXES_WIDTH);  // update x position of the plot
        dYPos = PAGE_HEIGHT - AXES_Y_POS - row*(AXES_VERT_GAP + AXES_HEIGHT);  // update y position of the plot

        axspos(dXPos, dYPos);  // determines the position of an axis system, define the lower left corner of an axis system.
        // Level 2
        graf(-1, +1.0, -1.0, 0.25, -1.0, 1.0, -1.0, 0.5);  // plots a two-dimensional axis system with given x/y lower and upper limits and first label and steps between labels
        setrgb(0.7, 0.7, 0.7);  // defines the foreground colour specified in RGB coordinates
        grid(1, 1);  // overlays a grid on an axis system

        color("fore");  // defines the colours used for plotting text and lines (resets the colours to the default)
        title();  // plots a title over an axis system. The title may contain up to four lines of text designated with TITLIN

        // Level 1
        endgrf();  // terminates an axis system and sets the level back to 1
    }

    return;
}


// Func: void AssignSignalsToPlots(void)
// Desc: assign the plots to associated signals
// In: none
// Out: none
void AssignSignalsToPlots(void) {

    int iXAxisID;  // x axis signal ID
    int iYAxisID;  // y axis signal ID
    int iSigID;  // signal ID
    int iXLen, iYLen;  // length of arrays

    for(int index_plot = 0; index_plot < dNumofPlots; index_plot ++) {  // loop through all plots
        iXAxisID = pPlotProp[index_plot].iXAxisID;  // update x axis ID
        iYAxisID = pPlotProp[index_plot].iYAxisID;  // update y axis ID

        iXLen = 0;  // set x array length to 0
        iYLen = 0;  // set y array length to 0

        for(int index_signal = 0; index_signal < NUM_OF_STORED_SIGNAL; index_signal++) {  // loop through all stored signals
            iSigID = pSignals[index_signal].iSigID;  // update signal ID

            if (iSigID == iXAxisID) {  // if x axis ID is the same as signal ID
                pPlotProp[index_plot].fXArray = pSignals[index_signal].fArray;  // assign signal array pointer to x axis array pointer
                iXLen = pSignals[index_signal].NoS;  // update x axis length
            }

            if (iSigID == iYAxisID) {  // if y axis ID is the same as signal ID
                pPlotProp[index_plot].fYArray = pSignals[index_signal].fArray;  // assign signal array pointer to y axis array pointer
                iYLen = pSignals[index_signal].NoS;  // update y axis length
           }
        }

        if (iXLen == 0 || iYLen == 0 || iXLen != iYLen) {
            printf("Error: the plot axis is not assigned properly, plot number = %d.\n", index_plot);
            exit(-1);
        }

        pPlotProp[index_plot].iNumOfElem = iXLen;  // set number of elements to length of array
    }
}


// Func: void ReassignPlots(void)
// Desc: reassign the plots signals to NULL
// In: none
// Out: none
void ReassignPlots(void) {

    for(int index_plot = 0; index_plot < dNumofPlots; index_plot ++) {  // loop through all plots
        pPlotProp[index_plot].fXArray = NULL;  // set x axis array to NULL
        pPlotProp[index_plot].fYArray = NULL;  // set y axis array to NULL
    }
}


// Func: void UpdatePlots(void)
// Desc: update the plots with assigned signals
// In: none
// Out: none
void UpdatePlots(void) {

    erase();  // erase the plot window

    int row = 0;  // plot row index
    int column = 0;  // plot column index

    int dXPos = 0;  // plot x position
    int dYPos = 0;  // plot y position

    float* fXAxisScaled;  // scaled x axis array
    float* fYAxisScaled;  // scaled y axis array
    float fX0, fX1, fDelX, fXExp;  // x axis limits
    float fY0, fY1, fDelY, fYExp;  // y axis limits
    int iNumOfElem;  // number of elements
    int iPresElem;  // number of presenting elements

    float* fXAxis,* fYAxis;  // x and y axis data pointer

    for(int index_p = 0; index_p < dNumofPlots; index_p++) {

        axslen(AXES_WIDTH, AXES_HEIGHT);  // defines the size of an axis system

        height(AXIS_FONT_SIZE);  // defines the character height or axis tick
        hname(AXIS_FONT_SIZE);  // defines the character height of axis label

        name(pPlotProp[index_p].sXLabel, "x");  // defines 'x' axis titles
        name(pPlotProp[index_p].sYLabel, "y");  // defines 'y' axis titles

        iNumOfElem = pPlotProp[index_p].iNumOfElem;  // update number of elements

        iPresElem = (int)floor(iNumOfElem * fTimeScale/100.0);  // update number of presenting elements

        fXAxis = pPlotProp[index_p].fXArray;  // update x axis data pointer
        fYAxis = pPlotProp[index_p].fYArray;  // update y axis data pointer

        fXAxisScaled = (float*) calloc(iPresElem, sizeof(float));  // allocate memory for scaled x axis array
        fYAxisScaled = (float*) calloc(iPresElem, sizeof(float));  // allocate memory for scaled x axis array

        fXExp = pow(10.0, -pPlotProp[index_p].iXUnitExp);  // exponent
        fYExp = pow(10.0, -pPlotProp[index_p].iYUnitExp);  // exponent

        for(int index_d = 0; index_d < iPresElem; index_d++) {
            fXAxisScaled[index_d] = fXAxis[index_d]*fXExp;  // scale x axis element
            fYAxisScaled[index_d] = fYAxis[index_d]*fYExp;  // scale x axis element
        }

        fX0 = fXAxisScaled[0];  // update lower limit of x axis
        fX1 = fXAxisScaled[iPresElem - 1];  // update lower limit of x axis
        fDelX = (fX1 - fX0)/pPlotProp[index_p].iXDiv;  // x axis step

        if(bScaled == FALSE) {  // if manual scaling is set
            fY0 = pPlotProp[index_p].fOffset - pPlotProp[index_p].fScale*fYExp;  // update lower limit of y axis
            fY1 = pPlotProp[index_p].fOffset + pPlotProp[index_p].fScale*fYExp;  // update lower limit of y axis
        }
        else {  // if autoscaling is set
            float fMin = MinArray(fYAxisScaled, iPresElem);  // minimum value
            float fMax = MaxArray(fYAxisScaled, iPresElem);  // maximum value

            fY0 = (fMin < 0) ? (1.0 + AUTOSCALE_COEFF/100.0)*fMin : (1.0 - AUTOSCALE_COEFF/100.0)*fMin;  // vertical lower limit
            fY1 = (fMax < 0) ? (1.0 - AUTOSCALE_COEFF/100.0)*fMax : (1.0 + AUTOSCALE_COEFF/100.0)*fMax;  // vertical upper limit
        }
        fDelY = (fY1 - fY0)/pPlotProp[index_p].iYDiv;  // y axis step

        // Level 1
        column = pPlotProp[index_p].column - 1;  // update column index of the plot
        row = PLOT_NO_OF_ROW - pPlotProp[index_p].row;  // update row index of the plot

        dXPos = AXES_X_POS + column*(AXES_HORZ_GAP + AXES_WIDTH);  // update x position of the plot
        dYPos = PAGE_HEIGHT - AXES_Y_POS - row*(AXES_VERT_GAP + AXES_HEIGHT);  // update y position of the plot

        axspos(dXPos, dYPos);  // determines the position of an axis system, define the lower left corner of an axis system.
        // Level 2
        color("fore");  // defines the colours used for plotting text and lines (resets the colours to the default)

        graf(fX0, fX1, fX0, fDelX, fY0, fY1, fY0, fDelY);  // plots a two-dimensional axis system with given x/y lower and upper limits and first label and steps between labels
        setrgb(0.7, 0.7, 0.7);  // defines the foreground colour specified in RGB coordinates
        grid(1, 1);  // overlays a grid on an axis system

        color("blue");  // defines the colours used for plotting text and lines (resets the colours to the default)
        curve(fXAxisScaled, fYAxisScaled, iPresElem);  // plot the signal

        title();  // plots a title over an axis system. The title may contain up to four lines of text designated with TITLIN

        // Level 1
        endgrf();  // terminates an axis system and sets the level back to 1

        free(fXAxisScaled);  // free the scaled axis array
    }

    fXAxisScaled = NULL;
}

// Func: void PrepareWidget(void)
// Desc: prepares the widget control window to show the curves
// In:
//      slideParam* pSlideParam - pointer to array of sliders parameters
//      int dSlideNum - number of sliders
// Out: none
void PrepareWidget(void) {
    printf("Creating sliders guis\n");

    //  create the parameters window
    swgtit(SLIDER_WINDOW_TITLE);  // defines a title displayed in the main widget

    swgpop("NOQUIT");  // remove Quit item from menu

    int dSpacerLen = strlen(HELP_TEXT_SPACER);
    int dHelpTextLen = 0;  // length of help text string
    for(int index = 0; index < dNumofSliders; ++index) {
        dHelpTextLen += dSpacerLen + strlen(pSliderParam[index].sHelpText);
    }

    char* sBufText = (char*) calloc((dHelpTextLen + dSpacerLen + ABOT_TEXT_LEN + 1), sizeof(char));  // allocate memory for help text string

    strcpy(sBufText, ABOUT_TEXT);  // add about text
    strcat(sBufText, HELP_TEXT_SPACER);  // add spacer

    if(NO_OF_SLIDER != 0) {  // make sure there are text to add
        strcat(sBufText, pSliderParam[0].sHelpText);
        strcat(sBufText, HELP_TEXT_SPACER);
        for(int index = 1; index < dNumofSliders; ++index) {
            strcat(sBufText, pSliderParam[index].sHelpText);
            strcat(sBufText, HELP_TEXT_SPACER);
        }
    }

    swghlp(sBufText);  // sets a character string that will be displayed if the Help menu is clicked by the user
    swgwth(30);  // sets the default width of horizontal and parent/base widgets

    dSliderWindowsID = wgini("form");  // initializes the widget routines and creates a main widget

    int dPosX = WIDGET_X_POS + ( 0 ) * (BUTT_WIDTH + X_MARGIN);  // widget x position
    int dPosY = WIDGET_Y_POS + ( 0 ) * (BUTT_HEIGHT + Y_MARGIN);  // widget x position

    swgwin(dPosX, dPosY, BUTT_WIDTH, BUTT_HEIGHT);  // set the geometry
    dStStButtonID = wgpbut(dSliderWindowsID, STST_BUTTON_START_TEXT);  // create start/stop button

    dPosX = WIDGET_X_POS + ( 1 ) * (BUTT_WIDTH + X_MARGIN);  // widget x position
    dPosY = WIDGET_Y_POS + ( 0 ) * (BUTT_HEIGHT + Y_MARGIN);  // widget x position
    swgwin(dPosX, dPosY, BUTT_WIDTH, BUTT_HEIGHT);  // set the geometry
    dPaCnButtonID = wgpbut(dSliderWindowsID, PACN_BUTTON_PAUSE_TEXT);  // create pause/continue button

    dPosX = WIDGET_X_POS + ( 2 ) * (BUTT_WIDTH + X_MARGIN);  // widget x position
    dPosY = WIDGET_Y_POS + ( 0 ) * (BUTT_HEIGHT + Y_MARGIN);  // widget x position
    swgwin(dPosX, dPosY, BUTT_WIDTH, BUTT_HEIGHT);  // set the geometry
    dResetBERButtonID = wgpbut(dSliderWindowsID, RESET_BER_BUTTON_TEXT);  // create reset BER button

    dPosX = WIDGET_X_POS + ( 3 ) * (BUTT_WIDTH + X_MARGIN);  // widget x position
    dPosY = WIDGET_Y_POS + ( 0 ) * (BUTT_HEIGHT + Y_MARGIN);  // widget x position
    swgwin(dPosX, dPosY, BUTT_WIDTH, BUTT_HEIGHT);  // set the geometry
    dScaledID = wgpbut(dSliderWindowsID, SC_BUTTON_SCALE_TEXT);  // create scaled/manual button

    dPosX = WIDGET_X_POS + ( 4 ) * (BUTT_WIDTH + X_MARGIN);  // widget x position
    dPosY = WIDGET_Y_POS + ( 0 ) * (BUTT_HEIGHT + Y_MARGIN);  // widget x position
    swgwin(dPosX, dPosY, SLIDE_WIDTH, SLIDE_HEIGHT);  // set the geometry
    swgstp(0.1); // defines a step value for scale widgets
    dTimeScaleSliderID = wgscl(dSliderWindowsID, "Time scale (%)", 0.1, 100.0, 100.0, 1);  // create radio button

    swgatt(dPaCnButtonID, "inactive", "status");  // make the button disabled
    swgatt(dResetBERButtonID, "inactive", "status");  // make the button disabled
    swgatt(dScaledID, "inactive", "status");  // make the button disabled
    swgatt(dTimeScaleSliderID, "inactive", "status");  // make the button disabled

    swgcbk(dStStButtonID, CallBackHub);  // connects the button to the generic callback routine
    swgcbk(dPaCnButtonID, CallBackHub);  // connects the button to the generic callback routine
    swgcbk(dResetBERButtonID, CallBackHub);  // connects the button to the generic callback routine
    swgcbk(dScaledID, CallBackHub);  // connects the scale button to the generic callback routine
    swgcbk(dTimeScaleSliderID, CallBackHub);  // connects the time slider to the generic callback routine

    int iRow = 0;  // button row index
    int iColumn = 0;  // button column index

    for(int index = 0; index < dNumofSliders; ++index) {
        dPosX = WIDGET_X_POS + ( iColumn) * (SLIDE_WIDTH + X_MARGIN);  // widget x position
        dPosY = WIDGET_Y_POS + SLIDE_HEIGHT + Y_MARGIN + ( iRow ) * (SLIDE_HEIGHT + Y_MARGIN);  // widget x position
        swgwin(dPosX, dPosY, SLIDE_WIDTH, SLIDE_HEIGHT);  // set the geometry
        swgstp(pSliderParam[index].fStep); // defines a step value for scale widgets
        pSliderParam[index].dSliderID = wgscl(dSliderWindowsID, pSliderParam[index].sTitle,
                                              pSliderParam[index].fMinVal, pSliderParam[index].fMaxVal, pSliderParam[index].fDefVal, 3);  // creates a scale widget. The widget can be displayed in horizontal or vertical direction

        if(++iRow >= SLIDER_NO_OF_ROW) {  // if the row is filled
            iRow = 0;  // reset row index
            iColumn++;
        }

        swgcbk(pSliderParam[index].dSliderID, pSliderParam[index].fCallback);  // connects the widget with a callback routine
    }

    free(sBufText);  // release help text memory pointer
    sBufText = NULL;
    return;
}
