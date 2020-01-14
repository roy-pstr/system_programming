#ifndef _SOCKET_SERVER_H
#define _SOCKET_SERVER_H
#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include "utils.h"
#include "socket_tools.h"
#include "client_thread.h"
ErrorCode_t CleanupClientThreads(SOCKET * client_connections, HANDLE * client_handles, int number_of_threads);
ErrorCode_t SetUpTheServer(SOCKET * p_socket, int port);
ErrorCode_t WaitForClientToConnect(SOCKET *p_main_socket, client_params_t *client_args, HANDLE *client_handle);
ErrorCode_t StartGameServer(int port);
#endif


