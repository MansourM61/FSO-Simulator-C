/*
* FSO Simulation Program
* Written by Mojtaba Mansour Abadi
* email: mansourabadi.mojtaba@gmail.com
*
* The code is written in C language using Code::Blocks version 17.12.
* The code performs a real time simulation by generating psedurandom bit sequence
* on the fly. The bit sequence is changed into electrical signal which modulates
* a laser later on.
* The output of the laser is launched into a free space optical channel where laser output
* is exposed to propagation, fog/smoke, turbulence, pointing error, and miscellaneous effects.
* The optical beam then is received by an aperture and then converted to electrical signal using
* a photo-receiver. Then received bits are extracted and compared to transmit bits in order to measure
* bit-error-rate (BER) of the system.
*
* The turbulence effect is based on Log-Normal model and for fog/smoke, Kim model is utilised.
*
* For GUIs and plots DISLIN a free library is used.
* To perform FFT operation, GSL an open source library is used.
*/

// header inclusion section
#include "parameters.h"
#include "appModule.h"
#include "grafixModule.h"
#include <stdio.h>


int main(void) {

    Init();  // initialise the application

    BuildPlotsWindow();  // build plots Window

    BuildSlidersWindow();  // build sliders Window

    wgfin();  // terminates the widget routines. The widgets will be displayed on the screen. After choosing OK in the Exit menu, all widgets are deleted and the program is continued after WGFIN. After choosing Quit in the Exit menu, the program is terminated
    disfin();  // dislin termination, gives control to the rest of the program

    Destroy();  // terminates the application

    return 0;
}
