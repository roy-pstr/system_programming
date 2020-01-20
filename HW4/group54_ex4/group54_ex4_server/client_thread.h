#ifndef _CLIENT_THREAD_H
#define _CLIENT_THREAD_H

#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include "utils.h"
typedef struct {
	SOCKET socket;
	SOCKET main;
	char user_name[USERNAME_MAX_LEN];
	char opponent_name[USERNAME_MAX_LEN];
	bool exit;
} client_params_t;
DWORD ClientThread(LPVOID lpParam);
void InitArgs(client_params_t * args_arr, int size);
#endif


