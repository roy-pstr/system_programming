#include "socket_client.h"
#include "user_interface.h" 

extern SOCKET m_socket;
ErrorCode_t RecvData(SOCKET *t_socket) {
	TransferResult_t RecvRes;

	char *AcceptedStr = NULL;
	printf("Waiting for server to send something...\n");
	RecvRes = ReceiveString(&AcceptedStr, *t_socket);

	if (RecvRes == TRNS_FAILED)
	{
		printf("Socket error while trying to write data to socket\n");
		return SOCKET_ERROR_RECV_DATA;
	}
	else if (RecvRes == TRNS_DISCONNECTED)
	{
		printf("Server closed connection. Bye!\n");
		return SOCKET_ERROR_RECV_DATA;
	}
	printf("Got string from server : %s\n", AcceptedStr);

	free(AcceptedStr);

	return SUCCESS;
}
ErrorCode_t SendData(SOCKET *t_socket) {
	char SendStr[MAX_STR_LEN];
	TransferResult_t SendRes;
	printf("Enter a message to send to the server:\n");
	gets_s(SendStr, sizeof(SendStr)); //Reading a string from the keyboard

	if (STRINGS_ARE_EQUAL(SendStr, "quit")) {
		return CLOSE_SOCKET_CLIENT; //"quit" signals an exit from the client side
	}

	SendRes = SendString(SendStr, *t_socket);

	if (SendRes == TRNS_FAILED)
	{
		printf("Socket error while trying to write data to socket\n");
		return SOCKET_ERROR_SEND_DATA;
	}

	return SUCCESS;
}
void SetClient(char *server_ip, int server_port, char username[]) {
	// Initialize Winsock.
	WSADATA wsaData;
	int StartupRes = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (StartupRes != NO_ERROR)
	{
		printf("error %ld at WSAStartup( ), ending program.\n", WSAGetLastError());
		return;
	}

	// Create a socket.
	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_socket == INVALID_SOCKET)
	{
		printf("Error at socket( ): %ld\n", WSAGetLastError());
		goto server_cleanup_1;
	}

	//Create a sockaddr_in object clientService and set  values.
	SOCKADDR_IN clientService;
	unsigned long Address;
	Address = inet_addr(server_ip);
	if (Address == INADDR_NONE)
	{
		printf("The string \"%s\" cannot be converted into an ip address. ending program.\n",
			SERVER_ADDRESS_STR);
		goto server_cleanup_2;
	}
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = Address;
	clientService.sin_port = htons(server_port); //Setting the port to connect to.

	// Call the connect function, passing the created socket and the sockaddr_in structure as parameters. 
	// Check for general errors.
	if (connect(m_socket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
		DEBUG_PRINT(printf("Error at connect( ): %ld\n", WSAGetLastError()));
		printf(FAILURE_ON_CONNECT_TO_SERVER, server_ip, server_port);
		goto server_cleanup_2;
		return;
	}
	printf(ON_CONNECT_TO_SERVER, server_ip, server_port);

	ErrorCode_t ret_val = SUCCESS;
	while (true) {
		ret_val = RecvData(&m_socket);
		GO_TO_EXIT_ON_FAILURE(ret_val, "RecvData() failed.\n");
		ret_val = SendData(&m_socket);
		if (ret_val == CLOSE_SOCKET_CLIENT) { break; }
		GO_TO_EXIT_ON_FAILURE(ret_val, "SendData() failed.\n");
		
	}

EXIT:
server_cleanup_2:
	if (closesocket(m_socket) == SOCKET_ERROR)
		printf("Failed to close MainSocket, error %ld. Ending program\n", WSAGetLastError());

server_cleanup_1:
	if (WSACleanup() == SOCKET_ERROR)
		printf("Failed to close Winsocket, error %ld. Ending program.\n", WSAGetLastError());
}