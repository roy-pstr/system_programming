#ifndef _THREAD_HANDLER_H
#define _THREAD_HANDLER_H
#include <windows.h>

HANDLE CreateMutexSimple();

HANDLE CreateSemaphoreSimple(int initial_count, int max_count);

HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine,
	LPVOID p_thread_parameters,
	LPDWORD p_thread_id);
#endif