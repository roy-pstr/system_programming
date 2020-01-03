#include "game_ui.h"
#include <stdio.h>
#include "socket_server.h"
#include "socket_tools.h"
#include "thread_tools.h"

ErrorCode_t StartGameServer(int port) {
	ErrorCode_t ret_val = SUCCESS;

	SOCKET MainSocket = INVALID_SOCKET;
	ret_val = SetUpTheServer(&MainSocket, port);
	GO_TO_EXIT_ON_FAILURE(ret_val, "SetUpTheServer failed. \n");

	SOCKET ClientSockets[NUMBER_OF_CLIENTS];
	//InitSockets(ClientSockets, NUMBER_OF_CLIENTS);
	HANDLE ClientThreadHandles[NUMBER_OF_CLIENTS];
	InitHandels(ClientThreadHandles, NUMBER_OF_CLIENTS);

	ret_val = WaitForClientToConnect(&MainSocket, &ClientSockets[0], &ClientThreadHandles[0]);
	GO_TO_EXIT_ON_FAILURE(ret_val, "WaitForClientToConnect failed. \n");
	//WaitForClient(&MainSocket, &ClientSockets[1], &ClientThreadHandles[1]);

	DWORD Res = WaitForSingleObject(ClientThreadHandles[0], INFINITE);

EXIT:
	if (SUCCESS != CleanupClientThreads(&ClientSockets[0], &ClientThreadHandles[0], NUMBER_OF_CLIENTS)) {
		printf("CleanupClientThreads Failed.\n");
	}
	if (MainSocket != INVALID_SOCKET)
	{
		if (closesocket(MainSocket) == SOCKET_ERROR)
			printf("Failed to close MainSocket, error %ld. Ending program\n", WSAGetLastError());
	}
	if (WSACleanup() == SOCKET_ERROR)
		printf("Failed to close Winsocket, error %ld. Ending program.\n", WSAGetLastError());
	return ret_val;
 }