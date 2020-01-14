#include "client_thread.h"
#include "socket_tools.h"
#include "msg_protocol.h"
#include "csv_handler.h"
#include "game_engine.h"
//#include "socket_server.h"
#include "thread_communication.h"

extern Node *Leaderboard_head;
ErrorCode_t UpdateLeaderboard(char **game_results, char *username) {
	Node *update_lb = NULL; /*, *update_server = NULL;*/
	ErrorCode_t ret_val = SUCCESS;
	if (strcmp(game_results[3], "NONE") != 0)
	{
		if (strcmp(game_results[3], game_results[0]) == 0)
		{
			//update_server = DetectAndUpdateElement(&Leaderboard_head, game_results[0], 1); ILAY - NOT MANDATORY
			update_lb = DetectAndUpdateElement(&Leaderboard_head, username, 0);
		}
		else {
			update_lb = DetectAndUpdateElement(&Leaderboard_head, username, 1);
			//update_server = DetectAndUpdateElement(&Leaderboard_head, game_results[0], 0);
		}
	}
	if (NULL != update_lb)
	{
		sortedInsert(&Leaderboard_head, update_lb);
		//sortedInsert(&Leaderboard_head, update_server);
		LinkedListToCsv(Leaderboard_head, CSV_NAME);

	}
	return ret_val;
}

/* Conenct functions */
ErrorCode_t TestConnectionWithServer(client_params_t *Args) {
	ErrorCode_t ret_val = SUCCESS;
	bool client_connected = false;
	protocol_t protocol_msg;
	while (!client_connected) {
		ret_val = RecvData(&Args->socket, &protocol_msg);
		GO_TO_EXIT_ON_FAILURE(ret_val, "RecvData() failed.\n");
		switch (GetType(&protocol_msg)) {
		case CLIENT_REQUEST:
			strcpy_s(Args->user_name, USERNAME_MAX_LEN, protocol_msg.param_list[0]);
			ret_val = SendProtcolMsgNoParams(&Args->socket, SERVER_APPROVED);
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

/* ClientVsClient functions */
extern HANDLE second_client_connected_event;
extern client_params_t ClientThreadArgs[NUMBER_OF_CLIENTS];
ErrorCode_t GetOpponentName(client_params_t *curr_client) {
	ErrorCode_t ret_val = SUCCESS;
	int opponent_ind = -1;
	if (ClientThreadArgs[0].socket == curr_client->socket) {
		opponent_ind = 1;
	}
	else if (ClientThreadArgs[1].socket == curr_client->socket) {
		opponent_ind = 0;
	}
	else {
		return GET_OPPENONT_NAME_FAILED;
	}
	strcpy_s(curr_client->opponent_name, USERNAME_MAX_LEN, ClientThreadArgs[opponent_ind].user_name);
	return ret_val;
}
ErrorCode_t PlayClientVsClient(client_params_t *Args, bool first_player) {
	DEBUG_PRINT(printf("PlayClientVsClient.\n"));
	ErrorCode_t ret_val = SUCCESS;
	protocol_t recv_protocol;
	bool exit = false;
	MOVES_ENUM other_user_move, user_move;
	char **game_results;
	AllocateFullParamList(&game_results);

	while (!exit) {

		/* send SERVER_PLAYER_MOVE_REQUEST to client */
		ret_val = SendProtcolMsgNoParams(&Args->socket, SERVER_PLAYER_MOVE_REQUEST);
		GO_TO_EXIT_ON_FAILURE(ret_val, "SendProtcolMsgNoParams() failed!\n");

		/* wait for response frrom client : with user move */
		ret_val = RecvData(&Args->socket, &recv_protocol);
		GO_TO_EXIT_ON_FAILURE(ret_val, "RecvData() failed.\n");

		if (CLIENT_PLAYER_MOVE == GetType(&recv_protocol)) {

			/* get user move from protocol */
			user_move = StringToEnum(recv_protocol.param_list[0]);

			/* send my move and get oppent move and name */
			WriteAndReadMoves(user_move, &other_user_move, first_player);

			GetGameResults(game_results, user_move, Args->user_name, other_user_move, Args->opponent_name);
			//GetGameResults(game_results, user_move, Args->user_name, other_user_move, other_user_name);
			//DEBUG_PRINT(printf("GetGameResults: %s,%s,%s,%s\n", game_results[0], game_results[1], game_results[2], game_results[3]));

			/* update leaderboard wtih result */
			UpdateLeaderboard(game_results, Args->user_name);

			/* send SERVER_GAME_RESULTS to client : with results!*/
			ret_val = SendProtcolMsgWithParams(&Args->socket, SERVER_GAME_RESULTS, game_results, 4);
			GO_TO_EXIT_ON_FAILURE(ret_val, "SendProtcolMsgWithParams() failed!\n");

			/* send SERVER_GAME_OVER_MENU to client */
			ret_val = SendProtcolMsgNoParams(&Args->socket, SERVER_GAME_OVER_MENU);
			GO_TO_EXIT_ON_FAILURE(ret_val, "SendProtcolMsgNoParams() failed!\n");
			exit = true; /* exit loop */

		}
		else {
			ret_val = PROTOCOL_MSG_TYPE_ERROR;
			GO_TO_EXIT_ON_FAILURE(ret_val, "Server sent invalid protocol type!\n");
		}
	}
EXIT:
	FreeFullParamList(&game_results);
	return ret_val;
}
ErrorCode_t ClientVsClient(client_params_t *Args) {
	DEBUG_PRINT(printf("ClientVsClient.\n"));
	ErrorCode_t ret_val = SUCCESS;
	protocol_t recv_protocol;
	
	bool created_session_file = false;
	/* try to create the game session file */
	ret_val = TryCreateSessionFile(&created_session_file);
	GO_TO_EXIT_ON_FAILURE(ret_val, "CreateSession() failed.\n");

	/* check if it is the second player */
	if (false == created_session_file) {
		ret_val = SignleSecondPlayerConnected();
		GO_TO_EXIT_ON_FAILURE(ret_val, "SignleSecondPlayerConnected() failed!\n");
		DEBUG_PRINT(printf("User: %s is the second user connected\n", Args->user_name));
	}
	else {
		DEBUG_PRINT(printf("Waiting for second user (from user: %s)\n", Args->user_name));
	}

	/* wait until the second player connect and signle the event */
	ret_val = WaitForSecondPlayer();
	GO_TO_EXIT_ON_FAILURE(ret_val, "WaitForSecondPlayer() failed.\n");

	/* send SERVER_INVITE to client */
	ret_val = GetOpponentName(Args);
	char * opponent_name = Args->opponent_name;
	ret_val = SendProtcolMsgWithParams(&Args->socket, SERVER_INVITE, &opponent_name, 1);
	GO_TO_EXIT_ON_FAILURE(ret_val, "SendProtcolMsgNoParams() failed!\n");

	bool exit = false;
	while (!exit) {
		/* start playing */
		ret_val = PlayClientVsClient(Args, created_session_file);
		GO_TO_EXIT_ON_FAILURE(ret_val, "PlayClientVsClient() failed!\n");

		/* wait to client to decide: play again or back to main menu */
		ret_val = RecvData(&Args->socket, &recv_protocol);
		GO_TO_EXIT_ON_FAILURE(ret_val, "RecvData() failed.\n");

		switch (GetType(&recv_protocol))
		{
		case CLIENT_REPLAY:
			continue; /* go to start of the loop and play again */
		case CLIENT_MAIN_MENU:
			exit = true; /* exit loop */
			break;
		default:
			ret_val = PROTOCOL_MSG_TYPE_ERROR;
			GO_TO_EXIT_ON_FAILURE(ret_val, "Server sent invalid protocol type!\n");
		}
	}
EXIT:
	ret_val = ResetSecondPlayerEvent();
	if (true == created_session_file) {
		DeleteGameSessionFile();
	}
	GO_TO_EXIT_ON_FAILURE(ret_val, "MyResetEvent() failed!\n");
	return ret_val;
}

/* ClientVsCpu functions */
ErrorCode_t PlayClientVsCpu(client_params_t *Args) {
	DEBUG_PRINT(printf("PlayClientVsCpu.\n"));
	ErrorCode_t ret_val = SUCCESS;
	protocol_t recv_protocol;
	bool exit = false;
	MOVES_ENUM server_move, user_move;
	char **game_results;
	AllocateFullParamList(&game_results);
	while (!exit) {

		server_move = ServerRaffleMove();

		/* send SERVER_PLAYER_MOVE_REQUEST to client */
		ret_val = SendProtcolMsgNoParams(&Args->socket, SERVER_PLAYER_MOVE_REQUEST);
		GO_TO_EXIT_ON_FAILURE(ret_val, "SendProtcolMsgNoParams() failed!\n");

		/* wait for response frrom client : with user move */
		ret_val = RecvData(&Args->socket, &recv_protocol);
		GO_TO_EXIT_ON_FAILURE(ret_val, "RecvData() failed.\n");

		if (CLIENT_PLAYER_MOVE == GetType(&recv_protocol)) {
			/* get game results */
			user_move = StringToEnum(recv_protocol.param_list[0]);
			GetGameResults(game_results, user_move, Args->user_name, server_move, "server");
			//DEBUG_PRINT(printf("GetGameResults: %s,%s,%s,%s\n", game_results[0], game_results[1], game_results[2], game_results[3]));

			/* update leaderboard wtih result */
			UpdateLeaderboard(game_results, Args->user_name);

			/* send SERVER_GAME_RESULTS to client : with results!*/
			ret_val = SendProtcolMsgWithParams(&Args->socket, SERVER_GAME_RESULTS, game_results,4);
			GO_TO_EXIT_ON_FAILURE(ret_val, "SendProtcolMsgWithParams() failed!\n");

			/* send SERVER_GAME_OVER_MENU to client */
			ret_val = SendProtcolMsgNoParams(&Args->socket, SERVER_GAME_OVER_MENU);
			GO_TO_EXIT_ON_FAILURE(ret_val, "SendProtcolMsgNoParams() failed!\n");
			exit = true; /* exit loop */

		}
		else {
			ret_val = PROTOCOL_MSG_TYPE_ERROR;
			GO_TO_EXIT_ON_FAILURE(ret_val, "Server sent invalid protocol type!\n");
		}
	}
EXIT:
	FreeFullParamList(&game_results);
	return ret_val;
}
ErrorCode_t ClientVsCpu(client_params_t *Args) {
	DEBUG_PRINT(printf("ClientVsCpu.\n"));
	ErrorCode_t ret_val = SUCCESS;
	protocol_t recv_protocol;
	bool exit = false;
	while (!exit) {
		ret_val = PlayClientVsCpu(Args);
		GO_TO_EXIT_ON_FAILURE(ret_val, "ClientVsCpu() failed!\n");

		/* wait to client to decide: play again or back to main menu */
		ret_val = RecvData(&Args->socket, &recv_protocol);
		GO_TO_EXIT_ON_FAILURE(ret_val, "RecvData() failed.\n");

		switch (GetType(&recv_protocol))
		{
		case CLIENT_REPLAY:
			continue; /* go to start of the loop and play again */
		case CLIENT_MAIN_MENU:
			exit = true; /* exit loop */
			break;
		default:
			ret_val = PROTOCOL_MSG_TYPE_ERROR;
			GO_TO_EXIT_ON_FAILURE(ret_val, "Server sent invalid protocol type!\n");
		}
	}
EXIT:
	return ret_val;
}

/* Leaderboard functions */
ErrorCode_t ClientLeaderboard(client_params_t *Args) {
	DEBUG_PRINT(printf("ClientLeaderboard.\n"));
	ErrorCode_t ret_val = SUCCESS;
	protocol_t recv_protocol;
	char *leaderboard_str = NULL;
	int linkedlist_depth = 0, string_size = 0;
	bool exit = false;
	while (!exit) {
		linkedlist_depth = LengthOfLinkedList(Leaderboard_head);
		string_size = linkedlist_depth * LINE_MAX_LEN + SPACES_MAX_LEN +100;
		if (NULL != leaderboard_str) {
			free(leaderboard_str);
		}
		ret_val = AllocateString(&leaderboard_str, string_size);
		GO_TO_EXIT_ON_FAILURE(ret_val, "AllocateString() failed!\n");
		LinkedListToStr(Leaderboard_head, &leaderboard_str, string_size);
		/* send leaderboard to client */
		ret_val = SendProtcolMsgWithParams(&Args->socket, SERVER_LEADERBOARD, &leaderboard_str, 1);
		GO_TO_EXIT_ON_FAILURE(ret_val, "SendProtcolMsgWithParams() failed!\n");

		/* send show leaderboard menu to client */
		ret_val = SendProtcolMsgNoParams(&Args->socket, SERVER_LEADERBORAD_MENU);
		GO_TO_EXIT_ON_FAILURE(ret_val, "SendProtcolMsgWithParams() failed!\n");

		/* wait for response frrom client */
		ret_val = RecvData(&Args->socket, &recv_protocol);
		GO_TO_EXIT_ON_FAILURE(ret_val, "RecvData() failed.\n");

		switch (GetType(&recv_protocol)) {
			{
		case CLIENT_REFRESH:
			RefreshLeaderboard(CSV_NAME, &Leaderboard_head);
			continue;
		case CLIENT_MAIN_MENU:
			exit = true; /* exit loop! */
			continue;
		default:
			ret_val = PROTOCOL_MSG_TYPE_ERROR;
			GO_TO_EXIT_ON_FAILURE(ret_val, "Invalid protocol type!\n");
			break;
			}
		}
	}

EXIT:
	if (NULL != leaderboard_str) {
		free(leaderboard_str);
		leaderboard_str = NULL;
	}
	return ret_val;
}

/* MainMenu functions */
ErrorCode_t ClientMainMenu(client_params_t *Args) {
		ErrorCode_t ret_val = SUCCESS;
		protocol_t protocol_msg;
		bool quit = false;
		while (!quit) {
			ret_val = SendProtcolMsgNoParams(&Args->socket, SERVER_MAIN_MENU);
			GO_TO_EXIT_ON_FAILURE(ret_val, "SendProtcolMsg() failed!\n");
			ret_val = RecvData(&Args->socket, &protocol_msg);
			GO_TO_EXIT_ON_FAILURE(ret_val, "RecvData() failed.\n");
			switch (GetType(&protocol_msg)) {
			case CLIENT_VERSUS:
				/* client vs client:
				* wait to second client to connect
				* on client2 connect	success - > SERVER_INVITE
										failure - >  SERVER_NO_OPPONENTS */
				ret_val = ClientVsClient(Args);
				GO_TO_EXIT_ON_FAILURE(ret_val, "ClientVsClient() failed!\n");
				/*ret_val = SendProtcolMsg(Args->socket, SERVER_INVITE);
				GO_TO_EXIT_ON_FAILURE(ret_val, "SendProtcolMsg() failed!\n");
				ret_val = SendProtcolMsg(Args->socket, SERVER_NO_OPPONENTS);
				GO_TO_EXIT_ON_FAILURE(ret_val, "SendProtcolMsg() failed!\n");*/
				break;
			case CLIENT_CPU:
				/* client vs cpu */
				ret_val = ClientVsCpu(Args);
				GO_TO_EXIT_ON_FAILURE(ret_val, "ClientVsCpu() failed!\n");
				break;
			case CLIENT_LEADERBOARD:
				/* client leaderboard */
				ret_val = ClientLeaderboard(Args);
				GO_TO_EXIT_ON_FAILURE(ret_val, "ClientLeaderboard() failed!\n");
				/*ret_val = SendProtcolMsg(Args->socket, SERVER_LEADERBOARD);
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



/* Thread definition */
DWORD ClientThread(LPVOID lpParam)
{
	ErrorCode_t ret_val = SUCCESS;

	/* Handle arguments passed */
	client_params_t *Args;
	if (NULL == lpParam) {
		printf("error with arguments passed to thread\n");
		return CLIENT_PARAMS_CASTING_FAILED;
	}
	Args = (client_params_t*)lpParam;

	ret_val = TestConnectionWithServer(Args);
	GO_TO_EXIT_ON_FAILURE(ret_val, "TestConnectionWithServer() failed!\n");
	DEBUG_PRINT(printf("user name: %s\n", Args->user_name));
	ret_val = ClientMainMenu(Args);
	GO_TO_EXIT_ON_FAILURE(ret_val, "ClientMainMenu() failed!\n");

EXIT:
	printf("Conversation ended.\n");
	closesocket(Args->socket);
	Args->socket = INVALID_SOCKET;
	return ret_val;
}



