#ifndef _SOCKET_SERVER_H
#define _SOCKET_SERVER_H
#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include "utils.h"
#include "socket_tools.h"
ErrorCode_t CleanupClientThreads(SOCKET * client_connections, HANDLE * client_handles, int number_of_threads);
ErrorCode_t SetUpTheServer(SOCKET * p_socket, int port);
ErrorCode_t WaitForClientToConnect(SOCKET * p_socket, SOCKET * client_socket, HANDLE *client_handle);
void SetServer();
#endif


