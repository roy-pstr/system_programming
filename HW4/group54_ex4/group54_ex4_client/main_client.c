//Authors � Roy Pasternak(204219273) & Ilay Amar(308520857)
//Project � group54_ex4_client.exe
/*Description � Rock,Paper,Scissors,Lizard,Spock (Game from big bang theory based on Star Trek characters)
Additional to the classic Rock,Paper,Scissors game. The game runs on Server with the following options:
(1) Client Vs. Client, (2) Client Vs. Server. An additional Leaderboard.csv file is created which shows us the 
players rank by the ration Wins/Loses. Enjoy the game! */

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#include "utils.h"
#include "socket_client.h"
#include "socket_tools.h"
#include "user_interface.h" 

SOCKET m_socket = INVALID_SOCKET;
WSADATA wsaData;

ErrorCode_t ParseArgumnets(int argc, char *argv[], char **server_ip, int *server_port, char username[USERNAME_MAX_LEN]) {
	*server_ip = argv[1];
	*server_port = (int)strtol(argv[2],NULL,10);
	strcpy_s(username, USERNAME_MAX_LEN, argv[3]);
	return SUCCESS;
}

int main(int argc, char *argv[]) {

	ErrorCode_t ret_val = SUCCESS;

	//check argv:
	if (argc != CLIENT_ARGUMENTS_NUM)
	{
		printf("Illegal number of arguents! Try again\n");
		return ILLEGAL_NUMBER_OF_ARGV;
	}
	DEBUG_PRINT(printf("============CLIENT============\n"));

	/* parse arguments */

	char *server_ip = NULL;
	int server_port = 0;
	char username[USERNAME_MAX_LEN]; 
	ret_val = ParseArgumnets(argc, argv, &server_ip, &server_port, username);
	GO_TO_EXIT_ON_FAILURE(ret_val, "ParseArgumnets() failed.\n");

	// Initialize Winsock:
	int StartupRes = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (StartupRes != NO_ERROR)
	{
		ret_val = WSASTARTUP_FAILURE;
		printf("error %ld at WSAStartup( ), ending program.\n", WSAGetLastError());
		goto EXIT;
	}
	bool exit = false;
	while (!exit) {
		// Create a socket.
		ret_val = CreateSocket(&m_socket);
		GO_TO_EXIT_ON_FAILURE(ret_val, "CreateSocket() failed.\n");

		//DWORD timeout = 5 * 1000;
		//setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
		/* try to connect to server */
		ret_val = TryToConnectClient(server_ip, server_port, username);
		if (CLIENT_EXIT_TRY_CONNECT == ret_val) {
			goto EXIT;
		}
		GO_TO_EXIT_ON_FAILURE(ret_val, "TryToConnectClient() failed.\n");

		/* client connected */
		printf(ON_CONNECT_TO_SERVER, server_ip, server_port);
		ret_val = StartGameClient(server_ip, server_port, username);
		if (SUCCESS != ret_val) {
			printf(TIMEOUT_FAILURE_FROM_SERVER, server_ip, server_port);
			MenuCode_t menu_code = ReconnectMenu();
			if (CLIENT_TRY_RECONNECT == menu_code) {
				continue; /* try reconnect */
			}
			else if (CLIENT_EXIT_RECONNECT == menu_code) {
				ret_val = CLIENT_EXIT_TRY_CONNECT;
				goto EXIT;
			}
		}
		else{
			break; /* exit loop */
		}
		
	}
	
EXIT:
	if (m_socket != INVALID_SOCKET) {
		if (closesocket(m_socket) == SOCKET_ERROR)
			printf("Failed to close Client socket, error %ld. Ending program\n", WSAGetLastError());
		m_socket = INVALID_SOCKET;
	}
	if (WSACleanup() == SOCKET_ERROR)
		printf("Failed to close Winsocket, error %ld. Ending program.\n", WSAGetLastError());

	return ret_val;
}