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
#include "socket_tools.h"
#include "csv_handler.h"

SOCKET m_socket;

int RecvData(SOCKET *t_socket) {
	TransferResult_t RecvRes;

	char *AcceptedStr = NULL;
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
	printf("Got string from client : %s\n", AcceptedStr);

	free(AcceptedStr);

	return SUCCESS;
}
int SendData(SOCKET *t_socket) {
	char SendStr[MAX_STR_LEN];
	TransferResult_t SendRes;

	gets_s(SendStr, sizeof(SendStr)); //Reading a string from the keyboard

	if (STRINGS_ARE_EQUAL(SendStr, "quit")) {
		return SUCCESS; //"quit" signals an exit from the client side
	}
		
	SendRes = SendString(SendStr, *t_socket);

	if (SendRes == TRNS_FAILED)
	{
		printf("Socket error while trying to write data to socket\n");
		return SOCKET_ERROR_SEND_DATA;
	}

	return SUCCESS;
}
void SetClient() {
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
	Address = inet_addr(SERVER_ADDRESS_STR);
	if (Address == INADDR_NONE)
	{
		printf("The string \"%s\" cannot be converted into an ip address. ending program.\n",
			SERVER_ADDRESS_STR);
		goto server_cleanup_2;
	}
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = Address; 
	clientService.sin_port = htons(SERVER_PORT); //Setting the port to connect to.

	// Call the connect function, passing the created socket and the sockaddr_in structure as parameters. 
	// Check for general errors.
	if (connect(m_socket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
		printf("Error at connect( ): %ld\n", WSAGetLastError());
		goto server_cleanup_2;
		return;
	}

server_cleanup_2:
	if (closesocket(m_socket) == SOCKET_ERROR)
		printf("Failed to close MainSocket, error %ld. Ending program\n", WSAGetLastError());

server_cleanup_1:
	if (WSACleanup() == SOCKET_ERROR)
		printf("Failed to close Winsocket, error %ld. Ending program.\n", WSAGetLastError());
}

int main(int argc, char *argv[]) {
	int ret_val = SUCCESS;
	char csv_file[88] = "C:\\Users\\Ilay Amar\\Documents\\GitHub\\system_programming\\HW4\\group54_ex4\\Leaderboard.csv"; // ILAY
	RefreshLeaderboard(csv_file);
	double x = Round(3.12349, 3); //WORKS with printf("%.3f\n", x);
	Node *head = NULL;
	char fornodeone[20] = "Paster"; 
	char fornodetwo[20] = "Ilay"; 
	Node *node_one = CreateNode(fornodeone, 3, 4);
	printf("%s %d %d %f\n", node_one->name, node_one->won, node_one->lost, node_one->ratio);
	sortedInsert(&head, node_one);
	Node *node_two = CreateNode(fornodetwo, 2, 3);
	printf("%s %d %d %f\n", node_two->name, node_two->won, node_two->lost, node_two->ratio);
	sortedInsert(&head, node_two);
	printList(head);
	Node *oops = DetectAndUpdateElement(&head, "Ilay", 1);
	printf("############\n");
	sortedInsert(&head, oops);
	printList(head);

	/*Node* head = NULL;
	Node *new_node = newNode(5);
	sortedInsert(&head, new_node);
	new_node = newNode(10);
	sortedInsert(&head, new_node);
	new_node = newNode(7);
	sortedInsert(&head, new_node);
	new_node = newNode(3);
	sortedInsert(&head, new_node);
	printList(head); */



	//check argv:
	if (argc != CLIENT_ARGUMENTS_NUM)
	{
		printf("Illegal number of arguents! Try again\n");
		return ILLEGAL_NUMBER_OF_ARGV;
	}

	/* parse arguments */
	char *server_ip; /* ip address of the server */
	int server_port; /* ip address of the server */ 
	char username[USERNAME_MAX_LEN]; /* username - is not CASE sensetive*/
	//ParseArgumnets(argc, argv, &server_ip, &server_port, &username);

	//SetClient();

	return ret_val;
}