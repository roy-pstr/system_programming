/* this moduel defines both MAIN and EXIT threads which are the core of the server. */
#ifndef _MAIN_THREAD_H
#define _MAIN_THREAD_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "utils.h"

typedef struct {
	int port;
} main_params_t;
DWORD MainThread(LPVOID lpParam);
DWORD MyExitThread();

#endif