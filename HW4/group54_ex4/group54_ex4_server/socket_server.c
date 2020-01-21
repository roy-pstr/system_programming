#include "socket_server.h"
#include "thread_tools.h"
#include "csv_handler.h"
#include "thread_communication.h"

client_params_t ClientThreadArgs[NUMBER_OF_CLIENTS];
HANDLE ClientThreadHandles[NUMBER_OF_CLIENTS];
Node *Leaderboard_head = NULL;
param_node *Leader_param = NULL;
SOCKET MainSocket = INVALID_SOCKET;
extern bool exit_server;

void InitLeaderboard(Node **head, char * file_path) {
	if (true == IsFileExists(file_path)) {
		RefreshLeaderboard(file_path, head);
	}
	else {
		printf("no leaderboard.csv exist yet, play some games!\n");
	}
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
ErrorCode_t GetUserNameFromClient(SOCKET *p_socket, char *user_name) {
	ErrorCode_t ret_val = SUCCESS;
	protocol_t protocol_msg;
	ret_val = RecvData_WithTimeout(p_socket, &protocol_msg, SERVER_RECIVE_TIMEOUT); /* add timeout */
	GO_TO_EXIT_ON_FAILURE(ret_val, "RecvData_WithTimeout() failed.\n");
	switch (GetType(&protocol_msg)) {
	case CLIENT_REQUEST:
		strcpy_s(user_name, USERNAME_MAX_LEN, GetParam(protocol_msg.param_list_head,0));
		break;
	default:
		ret_val = PROTOCOL_MSG_TYPE_ERROR;
		GO_TO_EXIT_ON_FAILURE(ret_val, "Client sent invalid protocol type!");
		break;
	}
EXIT:
	FreeProtocol(&protocol_msg);
	return ret_val;
}

ErrorCode_t PutClientInThread( client_params_t *client_args, HANDLE *client_handle) {
	ErrorCode_t ret_val = SUCCESS;
	ret_val = SendProtcolMsgNoParams(&client_args->socket, SERVER_APPROVED);
	GO_TO_EXIT_ON_FAILURE(ret_val, "SendProtcolMsg() failed!\n");
	/* client is good to go : create thread for the client */
	if (NULL == (*client_handle = CreateThreadSimple((LPTHREAD_START_ROUTINE)ClientThread, client_args)))
	{
		printf("Error when creating client thread: %d\n", GetLastError());
		ret_val = THREAD_CREATE_FAILED;
		goto EXIT;
	}

EXIT:
	return ret_val;
}
ErrorCode_t SendServerDenied(SOCKET *t_socket, SERVER_DENIED_REASONS_IND reason_ind) {
	ErrorCode_t ret_val = SUCCESS;
	/* send server denied! */
	ret_val = SendProtcolMsgWithParams(t_socket, SERVER_DENIED, &SERVER_DENIED_REASONS[reason_ind], 1);
	GO_TO_EXIT_ON_FAILURE(ret_val, "SendProtcolMsgWithParams() failed!\n");
	DEBUG_PRINT(printf("No slots available for client, dropping the connection.\n"));
	closesocket(*t_socket); //Closing the socket, dropping the connection.
EXIT:
	return ret_val;
}
bool CheckIfUsernameExists(char new_user_name[USERNAME_MAX_LEN]) {
	for (int i = 0; i < NUMBER_OF_CLIENTS; i++)
	{
		if (ClientThreadHandles[i] != NULL) {
			if (STRINGS_ARE_EQUAL(new_user_name, ClientThreadArgs[i].user_name)) {
				return !ALLOW_SAME_USER_NAME;
			}
		}
	}
	return false;
}
int FindFirstNullHandler(HANDLE *client_handles, int size) {
	int i;
	for (i = 0; i < size; i++)
	{
		if (client_handles[i] == NULL)
			break;
		else
		{
			// poll to check if thread finished running:
			DWORD Res = WaitForSingleObject(client_handles[i], 0);

			if (Res == WAIT_OBJECT_0) // this thread finished running
			{
				CloseHandle(client_handles[i]);
				client_handles[i] = NULL;
				break;
			}
		}
	}
	return i;
}
ErrorCode_t TerminateAllClients() {
	ErrorCode_t ret_val = SUCCESS;
	DWORD wait_code;
	for (int i = 0; i < NUMBER_OF_CLIENTS; i++)
	{
		if (ClientThreadHandles[i] != NULL) {
			if (WAIT_TIMEOUT == (wait_code = WaitForSingleObject(ClientThreadHandles[i], 0)))
			{
				ShutDownAndCloseSocket(&ClientThreadArgs[i].socket);
				if (FALSE == TerminateThread(ClientThreadHandles[i], THREAD_TREMINATE)) {
					printf("TerminateThread() failed with: %d \n", GetLastError());
					ret_val = THREAD_TERMINATE_FAILED;
				}
				ClientThreadHandles[i] = NULL;
			}
		}
	}
EXIT:
	return ret_val;
}
ErrorCode_t CleanupClientThreads(client_params_t *client_args, HANDLE *client_handles, int number_of_threads)
{
	int Ind;
	ErrorCode_t ret_val = SUCCESS;
	for (Ind = 0; Ind < number_of_threads; Ind++)
	{
		if (client_handles[Ind] != NULL)
		{
			ShutDownAndCloseSocket(&client_args[Ind].socket);
			/* handle the thread exit code */
			if (SUCCESS != HandlerExitCode(client_handles[Ind])) {
				printf("HandlerExitCode failed.\n");
			}
			/* close handle */
			if (FALSE == CloseHandle(client_handles[Ind]))
			{
				printf("Error when closing thread: %d\n", GetLastError());
				printf("Trying to terminate thread...\n");
				if (FALSE == TerminateThread(client_handles[Ind], THREAD_TREMINATE)) {
					printf("TerminateThread() failed with: %d \n", GetLastError());
				}
			}
			client_handles[Ind] = NULL;
		}
	}
	return ret_val;
}

ErrorCode_t SetUpTheConnection(SOCKET *accepted_socket) {
	ErrorCode_t ret_val = SUCCESS;
	char new_user_name[USERNAME_MAX_LEN];
	int ind = 0;

	ret_val = GetUserNameFromClient(accepted_socket, &new_user_name[0]);
	GO_TO_EXIT_ON_FAILURE(ret_val, "GetUserNameFromClient failed. \n");

	ind = FindFirstNullHandler(ClientThreadHandles, NUMBER_OF_CLIENTS);
	if (ind == NUMBER_OF_CLIENTS) {
		ret_val = SendServerDenied(accepted_socket, FULL); /*close the socket*/
		GO_TO_EXIT_ON_FAILURE(ret_val, "ServerIsFull failed. \n");
	}
	else if (CheckIfUsernameExists(new_user_name)) {
		ret_val = SendServerDenied(accepted_socket, USERNAME_EXIST); /*close the socket*/
		GO_TO_EXIT_ON_FAILURE(ret_val, "ServerIsFull failed. \n");
	}
	else {
		ClientThreadArgs[ind].socket = *accepted_socket; // shallow copy: close client_socket when the time comes.
		strcpy_s(ClientThreadArgs[ind].user_name, USERNAME_MAX_LEN, new_user_name);
		ret_val = PutClientInThread(&ClientThreadArgs[ind], &ClientThreadHandles[ind]);
		GO_TO_EXIT_ON_FAILURE(ret_val, "PutClientInThread failed. \n");
	}
EXIT:
	return ret_val;
}

// accept with timeout so we can check if 'exit_server' flag in on.
SOCKET AcceptWithTimeout(SOCKET socket, int timeout_in_seconds) {
	int retval_select = 1;
	fd_set set;
	struct timeval timeout;
	FD_ZERO(&set); /* clear the set */
	FD_SET(socket, &set); /* add our file descriptor to the set */
	timeout.tv_sec = timeout_in_seconds;
	timeout.tv_usec = 0;
	if (timeout_in_seconds != INFINITE) {
		retval_select = select(0, &set, NULL, NULL, &timeout);
	}
	if (retval_select == SOCKET_ERROR) {
		// select error...
		printf("Error while waiting in AcceptWithTimeout\n");
		return INVALID_SOCKET;
	}
	while (retval_select == 0) {  
		/* check the exit flag */
		if (exit_server) { return NULL; }
		FD_ZERO(&set); /* clear the set */
		FD_SET(socket, &set); /* add our file descriptor to the set */
		timeout.tv_sec = timeout_in_seconds;
		timeout.tv_usec = 0;
		if (timeout_in_seconds != INFINITE) {
			retval_select = select(0, &set, NULL, NULL, &timeout);
		}
		if (retval_select == SOCKET_ERROR) {
			// select error...
			printf("Error while waiting in AcceptWithTimeout\n");
			return INVALID_SOCKET;
		}
	}


	SOCKET AcceptSocket = accept(socket, NULL, NULL);
	if (AcceptSocket == INVALID_SOCKET)
	{
		printf("Accepting connection with client failed, error %ld\n", WSAGetLastError());
	}
	return AcceptSocket;
}
ErrorCode_t CheckClientsStatus() {
	ErrorCode_t ret_val = SUCCESS;
	for (int i = 0; i < NUMBER_OF_CLIENTS; i++)
	{
		if (ClientThreadHandles[i] != NULL) {
			DWORD wait_code = WaitForSingleObject(ClientThreadHandles[i], 0);
			if (wait_code == WAIT_OBJECT_0) {
				/* client quit */
				if (CLIENT_THREAD_FAILED == HandlerExitCode(ClientThreadHandles[i])) {
					/*client thread failed -> treminate all other clients and exit server */
					TerminateAllClients();
					exit_server = true;
				}
			}
		}
	}
EXIT:
	return ret_val;
}
/* this function create the server socket and then handles the clients threads creation and waiting */
ErrorCode_t StartGameServer(int port) {
	ErrorCode_t ret_val = SUCCESS;
	HANDLE accept_thread = NULL;
	ret_val = SetUpTheServer(&MainSocket, port); /* create, bind and listen */
	GO_TO_EXIT_ON_FAILURE(ret_val, "SetUpTheServer failed. \n");

	/* initilaze the linked list of the leaderboards (if exists) */
	InitLeaderboard(&Leaderboard_head, CSV_NAME);

	/* initilaze sockets and thread handles to NULL */
	InitArgs(ClientThreadArgs, NUMBER_OF_CLIENTS);
	InitHandels(ClientThreadHandles, NUMBER_OF_CLIENTS);

	/* init the thread_communication module */
	InitThreadCommunicationModule();
	
	SOCKET accepted = INVALID_SOCKET;

	while (!exit_server) {
		if (NULL == (accepted = AcceptWithTimeout(MainSocket, ACCEPT_LIESNTER_INTERVALS_SEC))) {
			continue;
		}
		else if (accepted != INVALID_SOCKET) {
			SetUpTheConnection(&accepted);
			accepted = INVALID_SOCKET;
		}

		/*check client status */
		ret_val = CheckClientsStatus();
		GO_TO_EXIT_ON_FAILURE(ret_val, "CheckClientsStatus failed. \n");
	}

EXIT:
	DEBUG_PRINT(printf("StartGameServer: \"exit.\"\n"));
	if (NULL != accept_thread) {
		if (FALSE == CloseHandle(accept_thread))
		{
			printf("Error when closing thread: %d\n", GetLastError());
			ret_val = THREAD_CLOSE_ERROR;
		}
		accept_thread = NULL;
	}
	if (SUCCESS != CleanupClientThreads(&ClientThreadArgs[0], &ClientThreadHandles[0], NUMBER_OF_CLIENTS)) {
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


