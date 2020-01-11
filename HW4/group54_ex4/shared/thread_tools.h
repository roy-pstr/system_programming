#ifndef THREAD_TOOLS_H
#define THREAD_TOOLS_H

#include <windows.h>
#include "utils.h"
void InitHandels(HANDLE * thread_handels, int size);

HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine, LPVOID p_thread_parameters);

ErrorCode_t HandlerExitCode(HANDLE p_thread_handle);

#endif // SOCKET_SEND_RECV_TOOLS_H


