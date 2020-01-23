/* this module handles all the setup of the server and starts the game
	those functions are called fro mthe main_thread and this moduel actually manange the cloent_threads */
#ifndef _SOCKET_SERVER_H
#define _SOCKET_SERVER_H
#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include "utils.h"
#include "socket_tools.h"
#include "client_thread.h"

/* defenition of all server denied reasons and their messages */
typedef enum {
	FULL,
	USERNAME_EXIST
} SERVER_DENIED_REASONS_IND;
static const char *SERVER_DENIED_REASONS[] = {
	"Server is full: No slots available for client.",
	"Username is already connected to server."
};

/* create socket, bind and listen */
ErrorCode_t SetUpTheServer(SOCKET * p_socket, int port);

/* set the accept and check client_thread for errors loop */
ErrorCode_t StartGameServer(int port);
#endif


