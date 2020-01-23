/*this module handles the set of the socket for the client side*/
#ifndef _SOCKET_SERVER_H
#define _SOCKET_SERVER_H
#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include "utils.h"
#include "socket_tools.h"
/* this function tries to create a connection between the client and the server */
ErrorCode_t TryToConnectClient(char * server_ip, int server_port, char username[]);
/* this function handles all the game flow */
ErrorCode_t StartGameClient(char * server_ip, int server_port, char username[]);
#endif


