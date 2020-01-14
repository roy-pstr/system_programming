#include "socket_server.h"
#include "thread_tools.h"
#include "csv_handler.h"
#include "thread_communication.h"
HANDLE ThreadHandles[NUMBER_OF_CLIENTS];
SOCKET ThreadInputs[NUMBER_OF_CLIENTS];

ErrorCode_t CleanupClientThreads(SOCKET *client_connections, HANDLE *client_handles, int number_of_threads)
{
	int Ind;
	ErrorCode_t ret_val = SUCCESS;
	for (Ind = 0; Ind < number_of_threads; Ind++)
	{
		if (client_handles[Ind] != NULL)
		{
			/* CLOSE SOCKET */
			if (client_connections[Ind] != INVALID_SOCKET) {
				if (closesocket(client_connections[Ind]) == SOCKET_ERROR) {
					printf("Failed to close one of the clients connection sockets, error %ld.\n", WSAGetLastError());
				}
			}
			/* handle the thread exit code */
			if (SUCCESS != HandlerExitCode(client_handles[Ind])) {
				printf("HandlerExitCode failed.\n");
			}
			/* close handle */
			if (FALSE == CloseHandle(client_handles[Ind]))
			{
				printf("Error when closing thread: %d\n", GetLastError());
				return CLIENT_THREAD_CLOSE_ERROR;
			}
			client_handles[Ind] = NULL;
			client_connections[Ind] = INVALID_SOCKET;
		}
	}
	return ret_val;
}
ErrorCode_t SetUpTheServer(SOCKET *p_socket, int port) {
	DEBUG_PRINT(printf("Starting set up the server\n"));
	ErrorCode_t ret_val = SUCCESS;
	// Initialize Winsock.
	WSADATA wsaData;
	int StartupRes = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (StartupRes != NO_ERROR)
	{
		printf("error %ld at WSAStartup( ), ending program.\n", WSAGetLastError());
		return WSASTARTUP_FAILURE;
	}

	// Create a socket.
	
	*p_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (*p_socket == INVALID_SOCKET)
	{
		printf("Error at socket( ): %ld\n", WSAGetLastError());
		//goto server_cleanup_1;
	}
	DEBUG_PRINT(printf("Server create socket succeed\n"));
	// Create a sockaddr_in object and set its values.
	SOCKADDR_IN service;
	unsigned long Address;
	Address = inet_addr(SERVER_ADDRESS_STR);
	if (Address == INADDR_NONE)
	{
		printf("The string \"%s\" cannot be converted into an ip address. ending program.\n",
			SERVER_ADDRESS_STR);
		//goto server_cleanup_2;
	}
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = Address;
	service.sin_port = htons(port);

	// Bind the socket.
	int bindRes = bind(*p_socket, (SOCKADDR*)&service, sizeof(service));
	if (bindRes == SOCKET_ERROR)
	{
		printf("bind( ) failed with error %ld. Ending program\n", WSAGetLastError());
		//goto server_cleanup_2;
	}
	DEBUG_PRINT(printf("Server bind with port: %d succeed\n", port));

	// Listen on the Socket.
	int ListenRes = listen(*p_socket, MAX_CONN_LISTEN);
	if (ListenRes == SOCKET_ERROR)
	{
		printf("Failed listening on socket, error %ld.\n", WSAGetLastError());
		//goto server_cleanup_2;
	}
	return ret_val;
}
ErrorCode_t WaitForClientToConnect(SOCKET *p_main_socket, client_params_t *client_args, HANDLE *client_handle) {
	ErrorCode_t ret_val = SUCCESS;
	DEBUG_PRINT(printf("Waiting for a client to connect...\n"));
	SOCKET AcceptSocket = accept(*p_main_socket, NULL, NULL);
	if (AcceptSocket == INVALID_SOCKET)
	{
		printf("Accepting connection with client failed, error %ld\n", WSAGetLastError());
		goto EXIT;
	}
	DEBUG_PRINT(printf("Client Connected.\n"));

	/* warp the client socket connection via ClientThread */
	client_args->socket = AcceptSocket; // shallow copy: close client_socket when the time comes.
	if (NULL == (*client_handle = CreateThreadSimple((LPTHREAD_START_ROUTINE)ClientThread, client_args)))
	{
		printf("Error when creating client thread: %d\n", GetLastError());
		ret_val = THREAD_CREATE_FAILED;
		goto EXIT;
	}

EXIT:
	return ret_val;
}

/*StartGame functions */
Node *Leaderboard_head = NULL;
void InitLeaderboard(Node **head, char * file_path) {
	if (true == IsFileExists(file_path)) {
		RefreshLeaderboard(file_path, head);
	}
	else {
		printf("no leaderboard.csv exist yet, play soem games!\n");
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

	InitThreadCommunicationModule();

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
void CleanupWorkerThreads() /* update this function with all the failure checks needed !!! */
{
	int Ind;
	for (Ind = 0; Ind < NUMBER_OF_CLIENTS; Ind++)
	{
		if (ThreadHandles[Ind] != NULL)
		{
			// poll to check if thread finished running:
			DWORD Res = WaitForSingleObject(ThreadHandles[Ind], INFINITE);
			if (Res == WAIT_OBJECT_0)
			{
				closesocket(ThreadInputs[Ind]);
				CloseHandle(ThreadHandles[Ind]);
				ThreadHandles[Ind] = NULL;
				break;
			}
			else
			{
				printf("Waiting for thread failed. Ending program\n");
				return;
			}
		}
	}
}
