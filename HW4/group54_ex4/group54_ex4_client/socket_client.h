#ifndef _SOCKET_SERVER_H
#define _SOCKET_SERVER_H
#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include "defines.h"
#include "socket_tools.h"
void SetClient();
ErrorCode_t TryToConnectClient(char * server_ip, int server_port, char username[]);
ErrorCode_t StartGameClient(char * server_ip, int server_port, char username[]);
#endif


