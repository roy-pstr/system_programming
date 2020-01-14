#include "game_ui.h"
#include <stdio.h>
#include "socket_server.h"
#include "socket_tools.h"
#include "thread_tools.h"
#include "csv_handler.h"
#include "client_thread.h"

Node *Leaderboard_head = NULL;
void InitLeaderboard(Node **head, char * file_path) {
	if (SUCCESS == IsFileExists(file_path)) {
		RefreshLeaderboard(file_path, head);
	}
	else {
		LinkedListToCsv(Leaderboard_head, CSV_NAME);
		RefreshLeaderboard(file_path, head);
	}
}

ErrorCode_t StartGameServer(int port) {
	ErrorCode_t ret_val = SUCCESS;

	SOCKET MainSocket = INVALID_SOCKET;
	ret_val = SetUpTheServer(&MainSocket, port);
	GO_TO_EXIT_ON_FAILURE(ret_val, "SetUpTheServer failed. \n");

	/* initilaze the linked list of the leaderboards (if exists) */
	InitLeaderboard(&Leaderboard_head, CSV_NAME);
	
	SOCKET ClientSockets[NUMBER_OF_CLIENTS];
	client_params_t ClientThreadArgs[NUMBER_OF_CLIENTS];
	//InitSockets(ClientSockets, NUMBER_OF_CLIENTS);
	HANDLE ClientThreadHandles[NUMBER_OF_CLIENTS];
	InitHandels(ClientThreadHandles, NUMBER_OF_CLIENTS);

	ret_val = WaitForClientToConnect(&MainSocket, &ClientThreadArgs[0], &ClientThreadHandles[0]);
	GO_TO_EXIT_ON_FAILURE(ret_val, "WaitForClientToConnect failed. \n");
	ret_val = WaitForClientToConnect(&MainSocket, &ClientThreadArgs[1], &ClientThreadHandles[1]);
	GO_TO_EXIT_ON_FAILURE(ret_val, "WaitForClientToConnect failed. \n");

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