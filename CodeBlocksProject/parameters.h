#ifndef PARAMETERS_H_INCLUDED
#define PARAMETERS_H_INCLUDED


// simulation parameter definition section
#define NO_OF_SLIDER 16  // number of control sliders; all sliders are defined in function 'void BuildSlidersWindow(void)' from 'aooModule.c'
#define NO_OF_PLOTS 3  // number of presentation plots; all plots are defined in function 'void BuildPlotsWindow(void)' from 'aooModule.c'

#define LOOP_DELAY 50  // time delay (ms) before repeating the next simulation loop; used in function 'void *ThreadProcFunc(void *param)' from 'threadModule.c'

#define NUM_OF_CHUNCKS 4  // number of chunks for calculating mean value of signal and slicing operation; used in function 'void SliceArray(float* inputArray, int nInput, float* outputArray, int nOutput, int iNumChunck)' from 'sigProcModule.c'

#define NUM_OF_BASE_SIGNAL 4  // number of basic signals; bit order + bit number + Tx bits + Rx bits; used in 'void ConfigureSimulation(void)' from 'coreModule.c'
#define NUM_OF_RESAMP_SIGNAL 4  // number of basic signals; signal order + sample number + time + frequency; used in 'void ConfigureSimulation(void)' from 'coreModule.c'
#define NUM_OF_TX_SIGNAL 3  // number of Tx side signals; data + electrical + optical; used in 'void ConfigureSimulation(void)' from 'coreModule.c'
#define NUM_OF_CHANNEL_COEFFS 3 // number of channel coefficients for random fading effects; used in 'void ConfigureSimulation(void)' from 'coreModule.c'
#define NUM_OF_RX_SIGNAL 6  // number of Rx side signals; optical + current + voltage + detected + sampled; used in 'void ConfigureSimulation(void)' from 'coreModule.c'
#define NUM_OF_PROBED_SIGNAL 1  // number of probed signals; fft of Tx signal; used in 'void ConfigureSimulation(void)' from 'coreModule.c'

#define T_TH_FS (2.0/100.0)  // contrast threshold (typical value = 2%); used in function 'float FogLoss(float fVis, float fLambda, float fLinkLen)' from 'FSOModule.c'
#define LAMBDA_0 (550.0)  // reference wavelength (green light) (nm); used in function 'float FogLoss(float fVis, float fLambda, float fLinkLen)' from 'FSOModule.c'
#define TEMP_COH_TURB 5e-3  // temporal coherence of turbulence channel (s); typically 1ms to 10ms; used in function 'void TurbChanCoeff(float *pCoeff, int iNoP, float fDRx, float fCn2, float fLambda, float fLinkLen, float fF_s)' from 'FSOModule.c'
#define TEMP_COH_PE 1/50  // temporal coherence of turbulence channel (s); typically 1/50 s; used in function 'void PEChanCoeff(float *pCoeff, int iNoP, float fDTx, float fTheta, float fLinkLen, float fDRx, float fJitter, float fF_s)' from 'FSOModule.c'
#define MOVING_AVERAGE_WIN_WIDTH 500  // moving average filter window width; used in function 'void SmoothArray_m1(float* inputArray, int nInput, float* outputArray, int nOutput, int iWindowWidth)' from 'sigProcModule.c'
#define FILTER_DELAY 10  // moving average filter delay used in function 'void SmoothArray_m2(float* inputArray, int nInput, float* outputArray, int nOutput, int iDelay)' from 'sigProcModule.c'
#define FILTER_ALFA 0.5  // smoothing filter parameter used in function 'void SmoothArray_m3(float* inputArray, int nInput, float* outputArray, int nOutput, float fFiltPar)' from 'sigProcModule.c'

#define PLOT_WINDOWS_TITLE "Oscilloscope"  // plot windows title; used in function 'void PrepareFigure(void)' from 'grafixModule.c'
#define SLIDER_WINDOW_TITLE "Parameters Sliders"  // sliders parameters title; used in function 'void PrepareWidget(void)' from 'grafixModule.c'
#define HELP_TEXT_SPACER "\n-------------------------------\n"  // help text spacer; used in function 'void PrepareWidget(void)' from 'grafixModule.c'

#define STST_BUTTON_START_TEXT  "   Start    "  // push button label for start mode; used in functions 'void CallBackHub(int id)' and 'void PrepareWidget(void)' from 'appModule.c' and 'grafixModule.c', respectively
#define STST_BUTTON_STOP_TEXT   "   Stop     "  // push button label for stop mode; used in function 'void CallBackHub(int id)' from 'appModule.c'
#define PACN_BUTTON_PAUSE_TEXT  "   Pause    "  // push button label for pause mode; used in functions 'void CallBackHub(int id)' and 'void PrepareWidget(void)' from 'appModule.c' and 'grafixModule.c', respectively
#define PACN_BUTTON_CONT_TEXT   "  Continue  "  // push button label for continue mode; used in function 'void CallBackHub(int id)' from 'appModule.c'
#define RESET_BER_BUTTON_TEXT   " Reset BERT "  // push button label for reset BER; used in function 'void CallBackHub(int id)' from 'appModule.c'
#define SC_BUTTON_SCALE_TEXT    " Autoscale  "  // push button label for auto scale mode; used in functions 'void CallBackHub(int id)' and 'void PrepareWidget(void)' from 'appModule.c' and 'grafixModule.c', respectively
#define MN_BUTTON_SCALE_TEXT    "   Manual   "  // push button label for manual mode; used in function 'void CallBackHub(int id)' from 'appModule.c'

#define PAGE_WIDTH 4000  // plot canvas page width; used in function 'void PrepareFigure(void)' from 'grafixModule.c'
#define PAGE_HEIGHT 2500  // plot canvas page length; used in functions 'void PrepareFigure(void)' and 'void UpdatePlots(void)' from 'grafixModule.c'
#define AXIS_FONT_SIZE 30  // axis label and tick font sizes; used in functions 'void PrepareFigure(void)' and 'void UpdatePlots(void)' from 'grafixModule.c'
#define PLOT_NO_OF_ROW 2  // plot table number of rows; used in functions 'void PrepareFigure(void)' and 'void UpdatePlots(void)' from 'grafixModule.c'
#define PLOT_NO_OF_COLUMN 2  // plot table number of columns; used in functions 'void PrepareFigure(void)' and 'void UpdatePlots(void)' from 'grafixModule.c'
#define AXIS_VERT_GAP_MARGIN  180  // vertical marginal gap between axis; used in macro definition section from 'grafixModule.h'
#define AXIS_HORZ_GAP_MARGIN  180  // horizontal marginal gap between axis; used in macro definition section from 'grafixModule.h'

#define X_MARGIN 20  // x marginal gap; used in function 'void PrepareWidget(void)' from 'grafixModule.c'
#define Y_MARGIN 10  // y marginal gap; used in function 'void PrepareWidget(void)' from 'grafixModule.c'
#define SLIDER_NO_OF_ROW 8  // slide table number of rows; used in function 'void PrepareWidget(void)' from 'grafixModule.c'
#define SLIDER_NO_OF_COLUMN 2  // slide table number of columns; used in function 'void PrepareWidget(void)' from 'grafixModule.c'
#define WIDGET_X_POS 10  // x widget position; used in function 'void PrepareWidget(void)' from 'grafixModule.c'
#define WIDGET_Y_POS 10  // y widget position; used in function 'void PrepareWidget(void)' from 'grafixModule.c'
#define BUTT_WIDTH 100  // button widget width; used in function 'void PrepareWidget(void)' from 'grafixModule.c'
#define BUTT_HEIGHT 27  // button widget height; used in function 'void PrepareWidget(void)' from 'grafixModule.c'
#define SLIDE_WIDTH 300  // button widget width; used in function 'void PrepareWidget(void)' from 'grafixModule.c'
#define SLIDE_HEIGHT 70  // button widget height; used in function 'void PrepareWidget(void)' from 'grafixModule.c'

#define AUTOSCALE_COEFF 10  // auto scale extra vertical margin (%); used in function 'void UpdatePlots(void)' from 'grafixModule.c'

#define BIT_SEQ_LEN 500  // length of the generated burst sequence; used in functions 'void ConfigureSimulation(void)' and 'void IterateSimulation(int iIteration)' from 'coreModule.c'
#define SAMPLES_PER_BIT 40  // samples per bit ratio; used in functions 'void ConfigureSimulation(void)' and 'void IterateSimulation(int iIteration)' from 'coreModule.c'
#define BW_TO_BR_RATIO 1.5  // bandwidth to bit-rate ratio;; used in function 'void IterateSimulation(int iIteration)' from 'coreModule.c'


// header inclusion section
#include <string.h>  // include string header for texts manipulation
#include <stdio.h>  // include header for standard input/output
#include <stdlib.h>  // include header for standard library


#if defined(_WIN32)  // windows operating system
// header inclusion section
#include <windows.h>

#elif defined(__linux__)  // linux operating system
#define BOOL char  // replace BOOL with char
#define TRUE 1  // replace TRUE with '1'
#define FALSE 0  // replace FALSE with '0'

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif
#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif
#endif

#endif // PARAMETERS_H_INCLUDED
