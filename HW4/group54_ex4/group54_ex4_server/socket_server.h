#ifndef _SOCKET_SERVER_H
#define _SOCKET_SERVER_H
#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include "utils.h"
#include "socket_tools.h"
#include "client_thread.h"

ErrorCode_t SetUpTheServer(SOCKET * p_socket, int port);
ErrorCode_t StartGameServer(int port);
#endif


