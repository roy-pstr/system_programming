//Authors – Roy Pasternak(204219273) & Ilay Amar(308520857)
//Project – group54_ex4_server.exe
//Description – 

#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include "defines.h"
#include "socket_tools.h"

int CommunicateWithClient(SOCKET *t_socket) {
	char SendStr[MAX_STR_LEN];
	TransferResult_t SendRes;
	TransferResult_t RecvRes;
	char *AcceptedStr = NULL;

	/* get msg from client */
	RecvRes = ReceiveString(&AcceptedStr, *t_socket);
	if (RecvRes == TRNS_FAILED)
	{
		printf("Service socket error while reading, closing thread.\n");
		closesocket(*t_socket);
		return 1;
	}
	else if (RecvRes == TRNS_DISCONNECTED)
	{
		printf("Connection closed while reading, closing thread.\n");
		closesocket(*t_socket);
		return 1;
	}
	else
	{
		printf("Got string from client : %s\n", AcceptedStr);
	}

	/* if succeded send an answer to client */
	strcpy(SendStr, "Answer from server");
	SendRes = SendString(SendStr, *t_socket);
	if (SendRes == TRNS_FAILED)
	{
		printf("Service socket error while writing, closing thread.\n");
		closesocket(*t_socket);
		return 1;
	}

	free(AcceptedStr);
	return SUCCESS;
}
void SetServer() {

	// Initialize Winsock.
	WSADATA wsaData;
	int StartupRes = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (StartupRes != NO_ERROR)
	{
		printf("error %ld at WSAStartup( ), ending program.\n", WSAGetLastError());                                 
		return;
	}

	// Create a socket.
	SOCKET MainSocket = INVALID_SOCKET;
	MainSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (MainSocket == INVALID_SOCKET)
	{
		printf("Error at socket( ): %ld\n", WSAGetLastError());
		goto server_cleanup_1;
	}

	// Create a sockaddr_in object and set its values.
	SOCKADDR_IN service;
	unsigned long Address;
	Address = inet_addr(SERVER_ADDRESS_STR);
	if (Address == INADDR_NONE)
	{
		printf("The string \"%s\" cannot be converted into an ip address. ending program.\n",
			SERVER_ADDRESS_STR);
		goto server_cleanup_2;
	}
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = Address;
	service.sin_port = htons(SERVER_PORT);

	// Bind the socket.
	int bindRes = bind(MainSocket, (SOCKADDR*)&service, sizeof(service));
	if (bindRes == SOCKET_ERROR)
	{
		printf("bind( ) failed with error %ld. Ending program\n", WSAGetLastError());
		goto server_cleanup_2;
	}

	// Listen on the Socket.
	int ListenRes = listen(MainSocket, MAX_CONN_LISTEN);
	if (ListenRes == SOCKET_ERROR)
	{
		printf("Failed listening on socket, error %ld.\n", WSAGetLastError());
		goto server_cleanup_2;
	}

	printf("Waiting for a client to connect...\n");

	//int Ind;
	for (int Loop = 0; Loop < SERVER_MAX_LOOPS; Loop++)
	{
		SOCKET AcceptSocket = accept(MainSocket, NULL, NULL);
		if (AcceptSocket == INVALID_SOCKET)
		{
			printf("Accepting connection with client failed, error %ld\n", WSAGetLastError());
			goto server_cleanup_3;
		}

		printf("Client Connected.\n");

		CommunicateWithClient(&AcceptSocket);
		//Ind = FindFirstUnusedThreadSlot();
		//if (Ind == NUM_OF_WORKER_THREADS) //no slot is available
		//{
		//	printf("No slots available for client, dropping the connection.\n");
		//	closesocket(AcceptSocket); //Closing the socket, dropping the connection.
		//}
		//else
		//{
		//	ThreadInputs[Ind] = AcceptSocket; // shallow copy: don't close 
		//									  // AcceptSocket, instead close 
		//									  // ThreadInputs[Ind] when the
		//									  // time comes.
		//	ThreadHandles[Ind] = CreateThread(
		//		NULL,
		//		0,
		//		(LPTHREAD_START_ROUTINE)ServiceThread,
		//		&(ThreadInputs[Ind]),
		//		0,
		//		NULL
		//	);
		//}
	}

server_cleanup_3:

	//CleanupWorkerThreads();

server_cleanup_2:
	if (closesocket(MainSocket) == SOCKET_ERROR)
		printf("Failed to close MainSocket, error %ld. Ending program\n", WSAGetLastError());

server_cleanup_1:
	if (WSACleanup() == SOCKET_ERROR)
		printf("Failed to close Winsocket, error %ld. Ending program.\n", WSAGetLastError());
}
int main(int argc, char *argv[]) {
	int ret_val = SUCCESS;

	//check argv:
	if (argc != SERVER_ARGUMENTS_NUM)
	{
		printf("Illegal number of arguents! Try again\n");
		return ILLEGAL_NUMBER_OF_ARGV;
	}

	/* get port */
	//int port = argv[1]; /* string to int.... */
	
	SetServer();
	
	return ret_val;
}