#ifndef THREADMODULE_H_INCLUDED
#define THREADMODULE_H_INCLUDED


// header inclusion section
#include "parameters.h"


// macro definition section


// function declaration section
void CreateThreadFunc(void);  // creates the thread process
void PauseThreadFunc(void);  // pauses/continues the thread process
void StopThreadFunc(void);  // stops the thread process


#if defined(_WIN32)  // windows operating system
// https://www.bogotobogo.com/cplusplus/multithreading_win32A.php

// function declaration section
void ThreadProcFunc(void *param);  // thread procedure function


// header inclusion section
#include <windows.h>
#include <process.h>


// macro definition section
#define OBJECT_CHECK_DURATION 10  // time duration (ms) to check an object handle


// variable definition section
HANDLE hThread;  // thread handle
HANDLE hStopCtrl;  // stop event handle
HANDLE hPauseCtrl;  // pause event handle

#elif defined(__unix__)  // linux operating system
// https://www.geeksforgeeks.org/multithreading-c-2/

// function declaration section
void *ThreadProcFunc(void *param);  // thread procedure function


// header inclusion section
#include <unistd.h>  //Header file for sleep(). man 3 sleep for details.
#include <pthread.h>
#include <time.h>

// variable definition section
pthread_t hThread;  // thread handle
pthread_mutex_t pause_mutex;  // mutex for locking pause/continue condition variable
pthread_cond_t pause_cond_var;  // pause/continue condition variable
int dThreadID;  // thread ID
char cThread_msg_pool[1];  // message pool for pause/continue and stop actons

#endif


#endif // THREADMODULE_H_INCLUDED
