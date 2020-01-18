#ifndef _ACCEPT_THREAD_H
#define _ACCEPT_THREAD_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "utils.h"
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

typedef struct {
	SOCKET main;
	SOCKET accepted;
} accept_params_t;
DWORD AcceptThread(LPVOID lpParam);

SOCKET accept_while_checking_stdin_for_exit(SOCKET socket, int timeout_in_seconds);

#endif
