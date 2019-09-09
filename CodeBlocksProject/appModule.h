#ifndef APPMODULE_H_INCLUDED
#define APPMODULE_H_INCLUDED


// header inclusion section
#include "parameters.h"  // include predefined parameters


// macro definition section


// variable definition section
BOOL bStopFlag;  // start/stop flag (true = the button is showing Start)
BOOL bPauseFlag;  // pause/continue flag (true = the button is showing Pause)


// function declaration section
void CallBackHub(int id);  // generic widget callback function
void Init(void);  // initialise the application
void Destroy(void);  // terminates the application
void BuildPlotsWindow(void);  // create plots window
void BuildSlidersWindow(void);  // creates sliders Window
void ResetGUI(void);  // reset the gui widgets

#endif // APPMODULE_H_INCLUDED
