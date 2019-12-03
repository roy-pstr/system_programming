#ifndef _THREAD_HANDLER_H
#define _THREAD_HANDLER_H
#include <windows.h>
#include "getGrade_thread.h"
#include "defines.h"
int RunMultiplayThreads(int number_of_threads, HANDLE *p_thread_handles, DWORD *p_thread_ids, GetGrade_params *p_thread_params);
#endif