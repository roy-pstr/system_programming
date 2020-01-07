//Authors – Roy Pasternak(204219273) & Ilay Amar(308520857)
//Project – group54_ex4_client.exe
//Description –

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#include "defines.h"
#include "socket_client.h"
#include "socket_tools.h"
#include "csv_handler.h"
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

	//int ret_val = SUCCESS;
	//char csv_file[MAX_STR_LEN] = "C:\\Users\\Ilay Amar\\Documents\\GitHub\\system_programming\\HW4\\group54_ex4\\Leaderboard.csv"; // ILAY
	//if (FILE_ERROR == IsFileExists(csv_file))
	//{
	//	printf("Leaderboard doesn't exist, play some games in order to fill it!!\n");
	//	return FILE_ERROR; //DEBUG return to main menu 
	//}
	//Node *head = NULL;
	//RefreshLeaderboard(csv_file, &head);
	////Next following lines are creating string to print leadearboard
	//char *strlead = NULL;
	//int buff = LengthOfLinkedList(head); 
	//buff = buff * LINE_MAX_LEN + SPACES_MAX_LEN;
	//int ret = AllocateString(&strlead, buff);
	//LinkedListToStr(head, &strlead ,buff);
	//printf("%s\n", strlead);


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
	// Create a socket.
	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_socket == INVALID_SOCKET)
	{
		printf("Error at socket( ) in client: %ld\n", WSAGetLastError());
		ret_val = SOCKET_CREATE_ERROR;
		goto EXIT;
	}

	/* try to connect to server */
	ret_val = TryToConnectClient(server_ip, server_port, username);
	if (CLIENT_EXIT_TRY_CONNECT == ret_val) {
		goto EXIT;
	}
	GO_TO_EXIT_ON_FAILURE(ret_val, "TryToConnectClient() failed.\n");

	/* client connected */
	printf(ON_CONNECT_TO_SERVER, server_ip, server_port);
	ret_val = StartGameClient(server_ip, server_port, username);
	GO_TO_EXIT_ON_FAILURE(ret_val, "StartGameClient() failed.\n");

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