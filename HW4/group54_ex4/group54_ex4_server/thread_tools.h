/*thread_tools module definition of help function for handling threads operations */
#ifndef THREAD_TOOLS_H
#define THREAD_TOOLS_H

#include <windows.h>
#include "utils.h"

ErrorCode_t MyResetEvent(HANDLE event);
ErrorCode_t WaitForMutex(HANDLE mutex);
ErrorCode_t ReleaseMutexSimp(HANDLE mutex);
HANDLE CreateEventSimple();
HANDLE CreateMutexSimple();

/* init array of handles with NULL */
void InitHandels(HANDLE * thread_handels, int size);

HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine, LPVOID p_thread_parameters);

/* return SUCCESS if thread handler exit with SUCCESS */
ErrorCode_t HandlerExitCode(HANDLE p_thread_handle);

/* close array of threads one by one (terminate if fail to close!) */
ErrorCode_t CloseThreads(HANDLE * p_threads, int num_of_threads);

#endif 


