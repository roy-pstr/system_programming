#include "socket_client.h"
#include "user_interface.h" 
#include "msg_protocol.h"
#include "socket_tools.h"
#include "utils.h"
#include "csv_handler.h"

extern SOCKET m_socket;

/* connection functions */
ErrorCode_t ConnectClient(char *server_ip, int server_port) {
	ErrorCode_t ret_val = SUCCESS;

	//Create a sockaddr_in object clientService and set values.
	SOCKADDR_IN clientService;
	unsigned long Address;
	Address = inet_addr(server_ip);
	if (Address == INADDR_NONE)
	{
		printf("The string \"%s\" cannot be converted into an ip address. ending program.\n", server_ip);
		ret_val = SOCKET_INIT_IP_ADDR_ERROR;
		return ret_val;
	}
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = Address;
	clientService.sin_port = htons(server_port); //Setting the port to connect to.

	// Call the connect function, passing the created socket and the sockaddr_in structure as parameters. 
	if (connect(m_socket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
		DEBUG_PRINT(printf("Error at connect( ): %ld\n", WSAGetLastError()));
		ret_val = SOCKET_CONNECT_FAILED;
	}
	return ret_val;
}
ErrorCode_t TryToConnectClient(char *server_ip, int server_port, char username[]) {
	ErrorCode_t ret_val = SUCCESS;
	protocol_t recv_protocol;
	InitProtocol(&recv_protocol);
	MenuCode_t menu_code;
	bool connection_success = false;
	while (!connection_success) {
		ret_val = ConnectClient(server_ip, server_port);
		if (ret_val != SUCCESS) {
			printf(FAILURE_ON_CONNECT_TO_SERVER, server_ip, server_port);
			menu_code = ReconnectMenu();
			if (CLIENT_TRY_RECONNECT == menu_code) {
				continue; /* try reconnect */
			}
			else if (CLIENT_EXIT_RECONNECT == menu_code) {
				ret_val = CLIENT_EXIT_TRY_CONNECT;
				goto EXIT;
			}
		}

		/* connect succeeded */
		ret_val = SendProtcolMsgWithParams(&m_socket, CLIENT_REQUEST, &username, 1);
		GO_TO_EXIT_ON_FAILURE(ret_val, "SendProtcolMsgWithParams() failed!");

		/* wait for sever response*/
		
		ret_val = RecvData_WithTimeout(&m_socket, &recv_protocol, CLIENT_RECIVE_TIMEOUT);
		GO_TO_EXIT_ON_FAILURE(ret_val, "RecvData_WithTimeout() failed.\n");

		switch (GetType(&recv_protocol)) {
		case SERVER_APPROVED:
			connection_success = true;
			continue; /* exit while loop with SUCCESS */
		case SERVER_DENIED:
			/* go back to try to connect client */
			printf(SERVER_DENEID_CLIENT, server_ip, server_port);
			menu_code = ReconnectMenu();
			if (CLIENT_TRY_RECONNECT == menu_code) {
				ret_val = CreateSocket(&m_socket);
				GO_TO_EXIT_ON_FAILURE(ret_val, "CreateSocket() failed.\n");
				continue; /* try reconnect */
			}
			else if (CLIENT_EXIT_RECONNECT == menu_code) {
				ret_val = CLIENT_EXIT_TRY_CONNECT;
				goto EXIT;
			}
		default:
			ret_val = PROTOCOL_MSG_TYPE_ERROR;
			GO_TO_EXIT_ON_FAILURE(ret_val, "Server sent invalid protocol type!");
			break;
		}
	}
		
EXIT:
	FreeProtocol(&recv_protocol);
	return ret_val;
}

/* ClientVsClient functions */
ErrorCode_t StartGameClientVsClient() {
	ErrorCode_t ret_val = SUCCESS;
	protocol_t recv_protocol;
	InitProtocol(&recv_protocol);
	bool still_playing = true;
	MOVES_ENUM player_move;
	PROTOCOL_ENUM end_game_menu_res;
	while (still_playing) {
		/* CLIENT_VERSUS already sent, wait for response from server */
		ret_val = RecvData_WithTimeout(&m_socket, &recv_protocol, CLIENT_VERSUS_TIMEOUT);
		GO_TO_EXIT_ON_FAILURE(ret_val, "RecvData_WithTimeout() failed.\n");
		if (SERVER_PLAYER_MOVE_REQUEST == GetType(&recv_protocol)) {
			player_move = ChooseMove();
			ret_val = SendProtcolMsgWithParams(&m_socket, CLIENT_PLAYER_MOVE, &MOVES_STRINGS[player_move], 1);
			GO_TO_EXIT_ON_FAILURE(ret_val, "SendProtcolMsgNoParams() failed!\n");

			/* wait for SERVER_GAME_RESULTS response */
			ret_val = RecvData_WithTimeout(&m_socket, &recv_protocol, INFINITE);
			GO_TO_EXIT_ON_FAILURE(ret_val, "RecvData_WithTimeout() failed.\n");
			if (SERVER_GAME_RESULTS == GetType(&recv_protocol)) {
				PrintGameResult(&recv_protocol);

				/* wait to recive from server */
				ret_val = RecvData_WithTimeout(&m_socket, &recv_protocol, CLIENT_RECIVE_TIMEOUT);
				GO_TO_EXIT_ON_FAILURE(ret_val, "RecvData_WithTimeout() failed.\n");

				/* next must come SERVER_GAME_OVER_MENU from server */
				if (SERVER_GAME_OVER_MENU == GetType(&recv_protocol)) {
					/* let the user choose what to do */
					end_game_menu_res = EndGameMenu();

					/* send to server */
					ret_val = SendProtcolMsgNoParams(&m_socket, end_game_menu_res);
					GO_TO_EXIT_ON_FAILURE(ret_val, "SendProtcolMsgNoParams() failed!\n");

					switch (end_game_menu_res) {
					case CLIENT_REPLAY:
						continue; /* goto start of loop and wait to get the server move */
					case CLIENT_MAIN_MENU:
						still_playing = false; /* exit loop! */
						continue;
					default:
						ret_val = PROTOCOL_MSG_TYPE_ERROR;
						GO_TO_EXIT_ON_FAILURE(ret_val, "Invalid protocol type!\n");
						break;
					}
				}
				else {
					ret_val = PROTOCOL_MSG_TYPE_ERROR;
					GO_TO_EXIT_ON_FAILURE(ret_val, "Server sent invalid protocol type!\n");
				}
			}
		}
		else if (SERVER_OPPONENT_QUIT == GetType(&recv_protocol)) {
			printf(OPONENT_LEFT_GAME, GetParam(recv_protocol.param_list_head, 0));
			still_playing = false; /* exit loop! */
			continue;
		}
		else {
			ret_val = PROTOCOL_MSG_TYPE_ERROR;
			GO_TO_EXIT_ON_FAILURE(ret_val, "Server sent invalid protocol type!\n");
		}
	}
EXIT:
	FreeProtocol(&recv_protocol);
	return ret_val;
}
ErrorCode_t WaitForOpponent() {
	ErrorCode_t ret_val = SUCCESS;
	protocol_t recv_protocol;
	InitProtocol(&recv_protocol);
	ret_val = RecvData_WithTimeout(&m_socket, &recv_protocol, INFINITE);
	GO_TO_EXIT_ON_FAILURE(ret_val, "RecvData_WithTimeout() failed.\n");
	if (SERVER_INVITE == GetType(&recv_protocol)) {
		DEBUG_PRINT(printf("Playing against: %s\n", GetParam(recv_protocol.param_list_head, 0)));
		ret_val = StartGameClientVsClient();
		GO_TO_EXIT_ON_FAILURE(ret_val, "StartGameClientVsClient() failed!\n");
	}
	else if (SERVER_NO_OPPONENTS == GetType(&recv_protocol)) {
		DEBUG_PRINT(printf("No opponent, back to main menu\n"));
	}
	else {
		ret_val = PROTOCOL_MSG_TYPE_ERROR;
		GO_TO_EXIT_ON_FAILURE(ret_val, "Server sent invalid protocol type!\n");
	}
EXIT:
	FreeProtocol(&recv_protocol);
	return ret_val;
}

/* ClientVsCpu functions */
ErrorCode_t StartGameClientVsCpu() {
	ErrorCode_t ret_val = SUCCESS;
	protocol_t recv_protocol;
	InitProtocol(&recv_protocol);
	bool still_playing = true;
	MOVES_ENUM player_move;
	PROTOCOL_ENUM end_game_menu_res;
	while (still_playing) {
		/* CLIENT_CPU already sent, wait for response from server */
		ret_val = RecvData_WithTimeout(&m_socket, &recv_protocol, CLIENT_RECIVE_TIMEOUT);
		GO_TO_EXIT_ON_FAILURE(ret_val, "RecvData_WithTimeout() failed.\n");
		if (SERVER_PLAYER_MOVE_REQUEST == GetType(&recv_protocol)) {
			player_move = ChooseMove();
			ret_val = SendProtcolMsgWithParams(&m_socket, CLIENT_PLAYER_MOVE, &MOVES_STRINGS[player_move] , 1);
			GO_TO_EXIT_ON_FAILURE(ret_val, "SendProtcolMsgNoParams() failed!\n");
			
			ret_val = RecvData_WithTimeout(&m_socket, &recv_protocol, CLIENT_RECIVE_TIMEOUT);
			GO_TO_EXIT_ON_FAILURE(ret_val, "RecvData_WithTimeout() failed.\n");
			if (SERVER_GAME_RESULTS == GetType(&recv_protocol))
			{
				PrintGameResult(&recv_protocol);

				/* wait to recive from server */
				ret_val = RecvData_WithTimeout(&m_socket, &recv_protocol, CLIENT_RECIVE_TIMEOUT);
				GO_TO_EXIT_ON_FAILURE(ret_val, "RecvData_WithTimeout() failed.\n");

				/* next must come SERVER_GAME_OVER_MENU from server */
				if (SERVER_GAME_OVER_MENU == GetType(&recv_protocol)) {
					/* let the user choose what to do */
					end_game_menu_res = EndGameMenu();

					/* send to server */
					ret_val = SendProtcolMsgNoParams(&m_socket, end_game_menu_res);
					GO_TO_EXIT_ON_FAILURE(ret_val, "SendProtcolMsgNoParams() failed!\n");

					switch (end_game_menu_res) {
					case CLIENT_REPLAY:
						continue; /* goto start of loop and wait to get the server move */
					case CLIENT_MAIN_MENU:
						still_playing = false; /* exit loop! */
						continue;
					default:
						ret_val = PROTOCOL_MSG_TYPE_ERROR;
						GO_TO_EXIT_ON_FAILURE(ret_val, "Invalid protocol type!\n");
						break;
					}
				}
				else {
					ret_val = PROTOCOL_MSG_TYPE_ERROR;
					GO_TO_EXIT_ON_FAILURE(ret_val, "Server sent invalid protocol type!\n");
				}
			}
		}
		else {
			ret_val = PROTOCOL_MSG_TYPE_ERROR;
			GO_TO_EXIT_ON_FAILURE(ret_val, "Server sent invalid protocol type!\n");
		}
	}
EXIT:
	FreeProtocol(&recv_protocol);
	return ret_val;
}

/* Leaderboard */
ErrorCode_t GoToClientLeaderboard() {
	ErrorCode_t ret_val = SUCCESS;
	protocol_t recv_protocol;
	InitProtocol(&recv_protocol);
	PROTOCOL_ENUM menu_ret_val;
	bool wait_for_leaderboard_from_server = true;
	while (wait_for_leaderboard_from_server) {
		ret_val = RecvData_WithTimeout(&m_socket, &recv_protocol, CLIENT_RECIVE_TIMEOUT);
		GO_TO_EXIT_ON_FAILURE(ret_val, "RecvData_WithTimeout() failed.\n");
		if (SERVER_LEADERBOARD == GetType(&recv_protocol)) {
			/* printing leaderboard */
			//printf("%s\n", recv_protocol.leaderboard_param);
			printParamsList(recv_protocol.param_list_head);
			ret_val = RecvData_WithTimeout(&m_socket, &recv_protocol, CLIENT_RECIVE_TIMEOUT);
			GO_TO_EXIT_ON_FAILURE(ret_val, "RecvData_WithTimeout() failed.\n");
			if (SERVER_LEADERBORAD_MENU == GetType(&recv_protocol)) {
				menu_ret_val = LeaderboardMenu();
				ret_val = SendProtcolMsgNoParams(&m_socket, menu_ret_val);
				GO_TO_EXIT_ON_FAILURE(ret_val, "SendProtcolMsgNoParams() failed!\n");
				switch (menu_ret_val) {
				case CLIENT_REFRESH:
					continue;
				case CLIENT_MAIN_MENU:
					wait_for_leaderboard_from_server = false; /* exit loop! */
					continue;
				default:
					ret_val = PROTOCOL_MSG_TYPE_ERROR;
					GO_TO_EXIT_ON_FAILURE(ret_val, "Invalid protocol type!\n");
					break;
				}
			}
			else {
				ret_val = PROTOCOL_MSG_TYPE_ERROR;
				GO_TO_EXIT_ON_FAILURE(ret_val, "Server sent invalid protocol type!\n");
			}
		}
		else {
			ret_val = PROTOCOL_MSG_TYPE_ERROR;
			GO_TO_EXIT_ON_FAILURE(ret_val, "Server sent invalid protocol type!\n");
		}
	}
	/* return to main_menu */

EXIT:
	FreeProtocol(&recv_protocol);
	return ret_val;
}

/* Start Game */
ErrorCode_t StartGameClient(char *server_ip, int server_port, char username[]) {
	ErrorCode_t ret_val = SUCCESS;
	PROTOCOL_ENUM main_menu_protocol;
	protocol_t recv_protocol;
	InitProtocol(&recv_protocol);
	/* wait for SERVER_MAIN_MENU */
	bool still_alive = true;
	while (still_alive) {
		ret_val = RecvData_WithTimeout(&m_socket, &recv_protocol, INFINITE);
		GO_TO_EXIT_ON_FAILURE(ret_val, "RecvData_WithTimeout() failed.\n");
		if (SERVER_MAIN_MENU == GetType(&recv_protocol)) {
			/* show menu */
			main_menu_protocol = MainMenu();

			/* send client decision to server */
			ret_val = SendProtcolMsgNoParams(&m_socket, main_menu_protocol);
			GO_TO_EXIT_ON_FAILURE(ret_val, "SendProtcolMsgNoParams() failed!");
			
			switch (main_menu_protocol) {
			case CLIENT_VERSUS:
				ret_val = WaitForOpponent();
				GO_TO_EXIT_ON_FAILURE(ret_val, "WaitForOpponent() failed!\n");
				break;
			case CLIENT_CPU:
				/* client vs cpu */
				ret_val = StartGameClientVsCpu();
				GO_TO_EXIT_ON_FAILURE(ret_val, "StartGameClientVsCpu() failed!\n");
				break;
			case CLIENT_LEADERBOARD:
				/* client leaderboard */
				ret_val = GoToClientLeaderboard();
				GO_TO_EXIT_ON_FAILURE(ret_val, "GoToClientLeaderboard() failed!\n");
				break;
			case CLIENT_DISCONNECT:
				/* send nothing back to client, close current client thread */
				still_alive = false; /* exit loop */
				continue;
			default:
				ret_val = PROTOCOL_MSG_TYPE_ERROR;
				GO_TO_EXIT_ON_FAILURE(ret_val, "Invalid protocol type!");
				break;
			}
		}
		else {
			ret_val = PROTOCOL_MSG_TYPE_ERROR;
			GO_TO_EXIT_ON_FAILURE(ret_val, "Server sent invalid protocol type!");
		}
	}
EXIT:
	FreeProtocol(&recv_protocol);
	/* close socket...*/
	if (m_socket != INVALID_SOCKET) {
		if (closesocket(m_socket) == SOCKET_ERROR)
			printf("Failed to close Client socket, error %ld. Ending program\n", WSAGetLastError());
		m_socket = INVALID_SOCKET;
	}
	return ret_val;
}