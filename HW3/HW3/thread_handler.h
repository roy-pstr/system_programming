#ifndef _THREAD_HANDLER_H
#define _THREAD_HANDLER_H
#include <windows.h>
#include "guest_thread.h"

HANDLE CreateMutexSimple();

HANDLE CreateSemaphoreSimple(int initial_count, int max_count);

int RunThreadsForAllGuests(int number_of_threads, HANDLE *p_thread_handles, DWORD *p_thread_ids, guest_params_t *p_thread_params);
#endif