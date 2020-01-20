#include "accept_thread.h"
#include "thread_communication.h"
DWORD AcceptThread(LPVOID lpParam)
{
	ErrorCode_t ret_val = SUCCESS;

	accept_params_t *Args;
	if (NULL == lpParam) {
		printf("error with arguments passed to thread\n");
		return THREAD_PARAMS_CASTING_FAILED;
	}
	Args = (accept_params_t*)lpParam;



	Args->accepted = accept(Args->main, NULL, NULL);
	if (Args->accepted == INVALID_SOCKET)
	{
		printf("Accepting connection with client failed, error %ld\n", WSAGetLastError());
		ret_val = ACCEPT_THREAD_FAILED;
		goto EXIT;
	}
	DEBUG_PRINT(printf("Client Connected.\n"));
	

EXIT:
	return ret_val;
}

#include <conio.h> 
#define STDIN_FILENO 0
extern bool exit_server;
// accept with timeout so we can check stdin for the string exit
// timeout_in_seconds should be small, less than 5 seconds, even 2 is good
// returns SOCKET if accept was succesful, NULL if exit was entered, INVALID_SOCKET if error occured
SOCKET accept_while_checking_stdin_for_exit(SOCKET socket, int timeout_in_seconds) {
	int retval_select = 1;
	char buf[20] = { NULL };
	int numRead;
	fd_set set;
	struct timeval timeout;
	FD_ZERO(&set); /* clear the set */
	FD_SET(socket, &set); /* add our file descriptor to the set */
	timeout.tv_sec = timeout_in_seconds;
	timeout.tv_usec = 0;
	if (timeout_in_seconds != INFINITE) {
		retval_select = select(socket + 1, &set, NULL, NULL, &timeout);
	}
	if (retval_select == SOCKET_ERROR) {
		// select error...
		printf("Error while waiting in accept_with_timeout\n");
		return INVALID_SOCKET;
	}
	while (retval_select == 0) {  // if there is no connection, check for exit
		if (exit_server) { return NULL; }
		FD_ZERO(&set); /* clear the set */
		FD_SET(socket, &set); /* add our file descriptor to the set */
		timeout.tv_sec = timeout_in_seconds;
		timeout.tv_usec = 0;
		if (timeout_in_seconds != INFINITE) {
			retval_select = select(socket + 1, &set, NULL, NULL, &timeout);
		}
		if (retval_select == SOCKET_ERROR) {
			// select error...
			printf("Error while waiting in accept_with_timeout\n");
			return INVALID_SOCKET;
		}
	}
	// we get here only if there is a connect request from the client

	SOCKET AcceptSocket = accept(socket, NULL, NULL);
	return AcceptSocket;
}