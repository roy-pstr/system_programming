#ifndef _USER_INTERACE_H
#define _USER_INTERACE_H

#include "utils.h"
#include "msg_protocol.h"

#define ON_CONNECT_TO_SERVER "Conencted to server on %s:%d\n"
#define FAILURE_ON_CONNECT_TO_SERVER "Failed connecting to server on %s:%d.\n"
#define TIMEOUT_FAILURE_FROM_SERVER "Connection to server on %s:%d has been lost.\n"
#define SERVER_DENEID_CLIENT  "Server on %s:%d denied the connection request.\n"
#define SERVER_MAIN_MENU_MSG "Choose what to do next:\n1. Play against another client\n2. Play against the server\n3. View the leaderboard\n4. Quit\n"
#define CHOOSE_WHAT_NEXT_LEADERBOARD "Choose what to do next:\n1. Refresh leaderboard\n2. Return to the main menu\n"
#define CHOOSE_MOVE "Choose a move from the list: Rock, Paper, Scissors, Lizard or Spock:\n"
#define CHOOSE_MOVE_UNVALID "Unvalid move! try again...\n"
#define GAME_RESULT_MSG "You played: %s\n%s played : %s\n%s won!\n"
#define TEKO_MSG "You played: %s\n%s played : %s\n"
#define END_GAME_MENU "Choose what to do next:\n1. Play again\n2. Return to the main menu\n"
#define CLIENT_LEFT_GAME "%s has left the game!\n"
#define CHOOSE_WHAT_NEXT_ON_CLIENT_CONNECTION_FAILURE "Choose what to do next:\n1. Try to reconnect\n2. Exit\n"
#define GAME_RESULTS "You played: %s\n%s played: %s\n%s won!\n"
typedef enum {
	CLIENT_TRY_RECONNECT,
	CLIENT_EXIT_RECONNECT,
	INVALID_MENU_CODE,
}MenuCode_t;

MOVES_ENUM ChooseMove();
PROTOCOL_ENUM EndGameMenu();
MenuCode_t ReconnectMenu();
PROTOCOL_ENUM LeaderboardMenu();
PROTOCOL_ENUM MainMenu();
void PrintGameResult(protocol_t * p_prot);
#endif


