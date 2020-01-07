#ifndef _CLIENT_THREAD_H
#define _CLIENT_THREAD_H

#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include "defines.h"
typedef enum {
	ROCK,
	PAPER,
	SCISSORS,
	LIZARD,
	SPOCK,
}MOVES_ENUM;

DWORD ClientThread(SOCKET * t_socket);
MOVES_ENUM ServerRaffleMove();
#endif


