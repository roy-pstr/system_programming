//Authors – Roy Pasternak(204219273) & Ilay Amar(308520857)
//Project – group54_ex4_client.exe
//Description –

#include <stdio.h>
#include <string.h>
<<<<<<< HEAD
#include "defines.h"
#include "socket_client.h"
=======
#include <math.h>
#include <stdlib.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include "defines.h"
#include "socket_tools.h"
#include "csv_handler.h"

>>>>>>> branch_ilay
SOCKET m_socket;

ErrorCode_t ParseArgumnets(int argc, char *argv[], char **server_ip, int *server_port, char username[USERNAME_MAX_LEN]) {
	*server_ip = argv[1];
	*server_port = (int)strtol(argv[2],NULL,10);
	strcpy_s(username, USERNAME_MAX_LEN, argv[3]);
	return SUCCESS;
}

int main(int argc, char *argv[]) {
	ErrorCode_t ret_val = SUCCESS;
	//int ret_val = SUCCESS;
	//char csv_file[88] = "C:\\Users\\Ilay Amar\\Documents\\GitHub\\system_programming\\HW4\\group54_ex4\\Leaderboard.csv"; // ILAY
	//RefreshLeaderboard(csv_file);
	//double x = Round(3.12349, 3); //WORKS with printf("%.3f\n", x);
	//Node *head = NULL;
	//char fornodeone[20] = "Paster"; 
	//char fornodetwo[20] = "Ilay"; 
	//Node *node_one = CreateNode(fornodeone, 3, 4);
	//printf("%s %d %d %f\n", node_one->name, node_one->won, node_one->lost, node_one->ratio);
	//sortedInsert(&head, node_one);
	//Node *node_two = CreateNode(fornodetwo, 2, 3);
	//printf("%s %d %d %f\n", node_two->name, node_two->won, node_two->lost, node_two->ratio);
	//sortedInsert(&head, node_two);
	//printList(head);
	//Node *oops = DetectAndUpdateElement(&head, "Ilay", 1);
	//printf("############\n");
	//sortedInsert(&head, oops);
	//printList(head);

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
	DEBUG_PRINT(printf("============CLIENT============\n"));

	/* parse arguments */

	char *server_ip = NULL;
	int server_port = 0;
	char username[USERNAME_MAX_LEN]; 
	ret_val = ParseArgumnets(argc, argv, &server_ip, &server_port, username);
	GO_TO_EXIT_ON_FAILURE(ret_val, "ParseArgumnets() failed.\n");
	SetClient(server_ip, server_port, username);

EXIT:

	return ret_val;
}