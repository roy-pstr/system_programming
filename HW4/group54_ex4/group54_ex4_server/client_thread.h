#ifndef _CLIENT_THREAD_H
#define _CLIENT_THREAD_H

#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include "defines.h"
DWORD ClientThread(SOCKET * t_socket);
#endif


