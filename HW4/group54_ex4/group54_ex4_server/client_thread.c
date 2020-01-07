#include "client_thread.h"
#include "socket_tools.h"
#include "msg_protocol.h"
#include "csv_handler.h"

ErrorCode_t TestConnectionWithServer(SOCKET *t_socket) {
	ErrorCode_t ret_val = SUCCESS;
	bool client_connected = false;
	protocol_t protocol_msg;
	while (!client_connected) {
		ret_val = RecvData(t_socket, &protocol_msg);
		GO_TO_EXIT_ON_FAILURE(ret_val, "RecvData() failed.\n");
		switch (GetType(&protocol_msg)) {
		case CLIENT_REQUEST:
			ret_val = SendProtcolMsgNoParams(t_socket, SERVER_APPROVED);
			GO_TO_EXIT_ON_FAILURE(ret_val, "SendProtcolMsg() failed!\n");
			ret_val = SendProtcolMsgNoParams(t_socket, SERVER_MAIN_MENU);
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

extern Node *Leaderboard_head;
ErrorCode_t ClientLeaderboard(SOCKET *t_socket) {
	DEBUG_PRINT("ClientLeaderboard.\n");
	ErrorCode_t ret_val = SUCCESS;
	protocol_t recv_protocol;
	char *leaderboard_str = NULL;
	int linkedlist_depth = 0, string_size = 0;
	bool exit = false;
	while (!exit) {
		linkedlist_depth = LengthOfLinkedList(Leaderboard_head);
		string_size = linkedlist_depth * LINE_MAX_LEN + SPACES_MAX_LEN;
		ret_val = AllocateString(&leaderboard_str, linkedlist_depth);
		GO_TO_EXIT_ON_FAILURE(ret_val, "SendProtcolMsgWithParams() failed!\n");
		LinkedListToStr(Leaderboard_head, &leaderboard_str, string_size);

		/* send leaderboard to client */
		ret_val = SendProtcolMsgWithParams(t_socket, SERVER_LEADERBOARD, &leaderboard_str, 1);
		GO_TO_EXIT_ON_FAILURE(ret_val, "SendProtcolMsgWithParams() failed!\n");

		/* send show leaderboard menu to client */
		ret_val = SendProtcolMsgNoParams(t_socket, SERVER_LEADERBORAD_MENU);
		GO_TO_EXIT_ON_FAILURE(ret_val, "SendProtcolMsgWithParams() failed!\n");

		/* wait for response frrom client */
		ret_val = RecvData(t_socket, &recv_protocol);
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
	}
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
