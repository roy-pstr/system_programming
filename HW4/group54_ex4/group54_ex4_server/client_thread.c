#include "client_thread.h"
#include "socket_tools.h"
#include "msg_protocol.h"

/*Raffle move for client Vs computer*/
MOVES_ENUM ServerRaffleMove()
{
	srand(time(NULL));
	MOVES_ENUM result = -1;
	result = rand() % 5;
	printf("%d\n", result);
	return result;

}
/*return move as enum*/
MOVES_ENUM StringToEnum(char *move)
{
	if (strcmp(move, "ROCK") == 0)
	{
		return ROCK;
	}
	if (strcmp(move, "PAPER") == 0)
	{
		return PAPER;
	}
	if (strcmp(move, "SCISSORS") == 0)
	{
		return SCISSORS;
	}
	if (strcmp(move, "LIZARD") == 0)
	{
		return LIZARD;
	}
	if (strcmp(move, "SPOCK") == 0)
	{
		return SPOCK;
	}	
}
/*Switch case function to decide who won - return 1 for user one to win, 2 for user two to win, 0 for draw*/

int CheckWhoWon(MOVES_ENUM first_move, MOVES_ENUM second_move)
{
	/*ROCK -> SCISSORS || LIZARD*/
	/*PAPER -> ROCK || SPOCK*/
	/*SCISSORS -> PAPER || LIZARD*/
	/*LIZARD -> PAPER || SPOCK*/
	/*SPOCK -> ROCK || SCISSORS*/
	if (first_move == second_move) //draw
	{
		return 0;
	}
	/*now its first or second only!*/
	if (first_move == ROCK)
	{
		if (second_move == SCISSORS || second_move == LIZARD)
			return 1;
		else
			return 2;
	}
	if (first_move == PAPER)
	{
		if (second_move == ROCK || second_move == SPOCK)
			return 1;
		else
			return 2;
	}
	if (first_move == SCISSORS)
	{
		if (second_move == PAPER || second_move == LIZARD)
			return 1;
		else
			return 2;
	}
	if (first_move == LIZARD)
	{
		if (second_move == PAPER || second_move == SPOCK)
			return 1;
		else
			return 2;
	}
	if (first_move == SPOCK)
	{
		if (second_move == ROCK || second_move == SCISSORS)
			return 1;
		else
			return 2;
	}


}
/*Not done yet*/


ErrorCode_t TestConnectionWithServer(SOCKET *t_socket) {
	ErrorCode_t ret_val = SUCCESS;
	bool client_connected = false;
	protocol_t protocol_msg;
	while (!client_connected) {
		ret_val = RecvData(t_socket, &protocol_msg);
		GO_TO_EXIT_ON_FAILURE(ret_val, "RecvData() failed.\n");
		switch (GetType(&protocol_msg)) {
		case CLIENT_REQUEST:
			ret_val = SendProtcolMsg(t_socket, SERVER_APPROVED);
			GO_TO_EXIT_ON_FAILURE(ret_val, "SendProtcolMsg() failed!\n");
			ret_val = SendProtcolMsg(t_socket, SERVER_MAIN_MENU);
			GO_TO_EXIT_ON_FAILURE(ret_val, "SendProtcolMsg() failed!\n");
			client_connected = true; /* exit while loop */
			break;
		default:
			ret_val = PROTOCOL_MSG_TYPE_ERROR;
			GO_TO_EXIT_ON_FAILURE(ret_val, "Client sent invalid protocol type!");
			break;
		}
	}
EXIT:
	return ret_val;
}

ErrorCode_t ClientVsClient(SOCKET *t_socket) {
	DEBUG_PRINT("ClientVsClient.\n");
	ErrorCode_t ret_val = SUCCESS;
	protocol_t protocol_msg;
	bool quit = false;
	while (!quit) {
		ret_val = RecvData(t_socket, &protocol_msg);
		GO_TO_EXIT_ON_FAILURE(ret_val, "RecvData() failed.\n");
		switch (GetType(&protocol_msg)) {
		//case CLIENT_VERSUS:
		//	break;
		default:
			ret_val = PROTOCOL_MSG_TYPE_ERROR;
			GO_TO_EXIT_ON_FAILURE(ret_val, "Client sent invalid protocol type!");
			break;
		}
	}
EXIT:
	return ret_val;
}

ErrorCode_t ClientVsCpu(SOCKET *t_socket) {
	DEBUG_PRINT("ClientVsCpu.\n");
	ErrorCode_t ret_val = SUCCESS;
	protocol_t protocol_msg;
	bool quit = false;
	while (!quit) {
		ret_val = RecvData(t_socket, &protocol_msg);
		GO_TO_EXIT_ON_FAILURE(ret_val, "RecvData() failed.\n");
		switch (GetType(&protocol_msg)) {
			//case CLIENT_VERSUS:
			//	break;
		default:
			ret_val = PROTOCOL_MSG_TYPE_ERROR;
			GO_TO_EXIT_ON_FAILURE(ret_val, "Client sent invalid protocol type!");
			break;
		}
	}
EXIT:
	return ret_val;
}

ErrorCode_t ClientLeaderboard(SOCKET *t_socket) {
	DEBUG_PRINT("ClientLeaderboard.\n");
	ErrorCode_t ret_val = SUCCESS;
	
EXIT:
	return ret_val;
}

ErrorCode_t ClientMainMenu(SOCKET *t_socket) {
	ErrorCode_t ret_val = SUCCESS;
	protocol_t protocol_msg;
	bool quit = false;
	while (!quit) {
		ret_val = RecvData(t_socket, &protocol_msg);
		GO_TO_EXIT_ON_FAILURE(ret_val, "RecvData() failed.\n");
		switch (GetType(&protocol_msg)) {
		case CLIENT_VERSUS:
			/* client vs client:
			* wait to second client to connect
			* on client2 connect	success - > SERVER_INVITE
									failure - >  SERVER_NO_OPPONENTS */
			ret_val = ClientVsClient(t_socket);
			GO_TO_EXIT_ON_FAILURE(ret_val, "ClientVsClient() failed!\n");
			/*ret_val = SendProtcolMsg(t_socket, SERVER_INVITE);
			GO_TO_EXIT_ON_FAILURE(ret_val, "SendProtcolMsg() failed!\n");
			ret_val = SendProtcolMsg(t_socket, SERVER_NO_OPPONENTS);
			GO_TO_EXIT_ON_FAILURE(ret_val, "SendProtcolMsg() failed!\n");*/
			break;
		case CLIENT_CPU:
			/* client vs cpu */
			ret_val = ClientVsCpu(t_socket);
			GO_TO_EXIT_ON_FAILURE(ret_val, "ClientVsCpu() failed!\n");
			//ret_val = SendProtcolMsg(t_socket, SERVER_APPROVED);
			//GO_TO_EXIT_ON_FAILURE(ret_val, "SendProtcolMsg() failed!\n");
			break;
		case CLIENT_LEADERBOARD:
			/* client leaderboard */
			ret_val = ClientLeaderboard(t_socket);
			GO_TO_EXIT_ON_FAILURE(ret_val, "ClientLeaderboard() failed!\n");
			/*ret_val = SendProtcolMsg(t_socket, SERVER_LEADERBOARD);
			GO_TO_EXIT_ON_FAILURE(ret_val, "SendProtcolMsg() failed!\n");*/
			/* wait for client to decide what to do */
			break;
		case CLIENT_DISCONNECT:
			/* send nothing back to client, close current client thread */
			DEBUG_PRINT("Client request to disconnect from server, closing thread.\n");
			quit = true; /* exit while loop */
			break;
		default:
			ret_val = PROTOCOL_MSG_TYPE_ERROR;
			GO_TO_EXIT_ON_FAILURE(ret_val, "Client sent invalid protocol type!");
			break;
		}
	}
EXIT:
	return ret_val;
}

DWORD ClientThread(SOCKET *t_socket)
{
	ErrorCode_t ret_val = SUCCESS;
	ret_val = TestConnectionWithServer(t_socket);
	GO_TO_EXIT_ON_FAILURE(ret_val, "TestConnectionWithServer() failed!\n");

	ret_val = ClientMainMenu(t_socket);
	GO_TO_EXIT_ON_FAILURE(ret_val, "ClientMainMenu() failed!\n");

EXIT:
	printf("Conversation ended.\n");
	closesocket(*t_socket);
	*t_socket = INVALID_SOCKET;
	return ret_val;
}
