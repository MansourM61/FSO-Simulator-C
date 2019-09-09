// header inclusion section
#include "threadModule.h"
#include "appModule.h"
#include "coreModule.h"
#include "grafixModule.h"
#include <stdlib.h>
#include <time.h>


#if defined(_WIN32)  // windows operating system
// Func: void ThreadProcFunc(void *param)
// Desc: thread procedure function
// In: parameter list
// Out: none
void ThreadProcFunc(void *param) {
    printf("Entering computing thread\n");

    HANDLE hStopFlag;  // stop event handle
    HANDLE hPauseFlag;  // pause event handle

    hStopFlag = OpenEvent(EVENT_ALL_ACCESS, FALSE, "Stop");  // create stop event
    hPauseFlag = OpenEvent(EVENT_ALL_ACCESS, FALSE, "Pause");  // create pause event

    int Index = 0;  // initialise the counter

    srand(time(NULL));  // start value (seed) for generating a sequence of pseudo-random integer values based on the current time

    ConfigureSimulation();  // initialise the simulation

    AssignSignalsToPlots();  // assign signals to plots

    DWORD dwResult;  // event handle definition

    do {

        // TODO: call computing function
        IterateSimulation(Index);  // perform one simulation iteration

        UpdatePlots();  // update plots

        Sleep(LOOP_DELAY);  // wait for 500 ms
        printf("Iteration index = %d\n", Index++);
        dwResult = WaitForSingleObject(hStopFlag, OBJECT_CHECK_DURATION);  //  check for stop event signal
    } while(dwResult == WAIT_TIMEOUT);

    // release the resources
    CloseHandle(hStopFlag);
    CloseHandle(hPauseFlag);

    ReassignPlots();  // reassign the plots

    TerminateSimulation();  // release the resources

    printf("Leaving computing thread\n");
	_endthread();
}


// Func: void CreateThreadFunc(void)
// Desc: creates the thread
// In: none
// Out: none
void CreateThreadFunc(void) {
    printf("Creating computing thread\n");

    hStopCtrl = CreateEvent(NULL, TRUE, FALSE, TEXT("Stop"));  // create a manual reset, initially not set event for stop control
    hPauseCtrl = CreateEvent(NULL, TRUE, FALSE, TEXT("Pause"));  // create a manual reset, initially not set event for pause control

    // check if resources are taken
    if ((hStopCtrl == NULL) || (hPauseCtrl == NULL)) {
        printf("Error: createMutex error: %ld.\n", GetLastError());
        exit(-1);
    }

    hThread = (HANDLE) _beginthread(ThreadProcFunc, 0, NULL);  // create thread

    // check if resources are taken
    if (hThread == NULL) {
        CloseHandle(hStopCtrl);
        CloseHandle(hPauseCtrl);
        printf("Error: CreateThread error: %ld.\n", GetLastError());
        exit(-1);
    }

	return;
}


// Func: void PauseThreadFunc(void)
// Desc: pauses/continues the thread process
// In: none
// Out: none
void PauseThreadFunc(void) {

    if (bPauseFlag == TRUE) {
        printf("Pausing computing thread\n");
        SetEvent(hPauseCtrl);  // send pause signal to the computing thread process
        SuspendThread(hThread);  // pause the computing process
    }
    else {
        printf("Resuming computing thread\n");
        ResumeThread(hThread);  // resume the computing process
        ResetEvent(hPauseCtrl);  // send continue signal to the computing thread process
   }

    return;
}


// Func: void StopThreadFunc(void)
// Desc: stops the thread process
// In: none
// Out: none
void StopThreadFunc(void) {
    printf("Stopping computing thread\n");

    if (bPauseFlag == FALSE) {
        ResumeThread(hThread);  // resume the computing process
        ResetEvent(hPauseCtrl);  // send continue signal to the computing thread process
    }

    SetEvent(hStopCtrl);  // send stop signal to the computing thread process

    WaitForSingleObject(hThread, INFINITE);  // wait till the thread is finished

    // release the resources
	CloseHandle(hThread);
    CloseHandle(hStopCtrl);
    CloseHandle(hPauseCtrl);

    ResetGUI();  // reset the gui widgets

    return;
}

#elif defined(__linux__)  // linux operating system

// Func: void ThreadProcFunc(void *param)
// Desc: thread procedure function
// In: parameter list
// Out: none
void *ThreadProcFunc(void *param) {
    printf("Entering computing thread\n");

    char *msg = (char*) param;

    int Index = 0;  // initialise the counter

    srand(time(NULL));  // start value (seed) for generating a sequence of pseudo-random integer values based on the current time

    ConfigureSimulation();  // initialise the simulation

    AssignSignalsToPlots();  // assign signals to plots

    do {

        // TODO: call computing function
        IterateSimulation(Index);  // perform one simulation iteration

        UpdatePlots();  // update plots

        usleep(LOOP_DELAY*1000);  // wait for 500 ms
        printf("Iteration index = %d\n", Index++);
        if (*msg == 'l') {
            pthread_mutex_lock(&pause_mutex);
            pthread_cond_wait(&pause_cond_var, &pause_mutex);
            pthread_mutex_unlock(&pause_mutex);
        }
    } while(*msg != 'E');

    ReassignPlots();  // reassign the plots

    TerminateSimulation();  // release the resources

    printf("Leaving computing thread\n");
    pthread_exit(NULL);
}


// Func: void CreateThreadFunc(void)
// Desc: creates the thread
// In: none
// Out: none
void CreateThreadFunc(void) {
    printf("Creating computing thread\n");

    pthread_mutex_init(&pause_mutex, NULL);  // mutex for locking pause/continue condition variable
    pthread_cond_init(&pause_cond_var, NULL);  // pause/continue condition variable

    *cThread_msg_pool = 'r';  // initialise the message

    dThreadID = pthread_create(&hThread, NULL, ThreadProcFunc, cThread_msg_pool);  // create thread

    // check if resources are taken
    if (dThreadID != 0) {
        printf("Error: pthread_create error: %d.\n", dThreadID);
        exit(-1);
    }

	return;
}


// Func: void PauseThreadFunc(void)
// Desc: pauses/continues the thread process
// In: none
// Out: none
void PauseThreadFunc(void) {

    if (bPauseFlag == TRUE) {
        printf("Pausing computing thread\n");

        *cThread_msg_pool = 'l';  // set the message
    }
    else {
        printf("Resuming computing thread\n");

        *cThread_msg_pool = 'r';  // set the message

        pthread_mutex_lock(&pause_mutex);
        pthread_cond_signal(&pause_cond_var);
        pthread_mutex_unlock(&pause_mutex);
   }

    return;
}


// Func: void StopThreadFunc(void)
// Desc: stops the thread process
// In: none
// Out: none
void StopThreadFunc(void) {
    printf("Stopping computing thread\n");

    if (bPauseFlag == FALSE) {
        *cThread_msg_pool = 'r';  // set the message

        pthread_mutex_lock(&pause_mutex);
        pthread_cond_signal(&pause_cond_var);
        pthread_mutex_unlock(&pause_mutex);
   }

    *cThread_msg_pool = 'e';  // set the message

    pthread_join(hThread, NULL);  // wait for the thread to terminate

    pthread_cond_destroy(&pause_cond_var);  // destroy the condition variable

    ResetGUI();  // reset the gui widgets

    return;
}

#endif
