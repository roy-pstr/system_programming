#ifndef _USER_INTERACE_H
#define _USER_INTERACE_H

#include "defines.h"

#define ON_CONNECT_TO_SERVER "Conencted to server on %s:%d\n"
#define FAILURE_ON_CONNECT_TO_SERVER "Failed connecting to server on %s:%d.\nChoose what to do next:\n1. Try to reconnect\n2. Exit\n"
#define TIMEOUT_FAILURE_FROM_SERVER "Connection to server on %s:%d has been lost.\nChoose what to do next :\n	1. Try to reconnect\n	2. Exit\n"
#define SERVER_DENEID_CLIENT  "Server on %s:%d denied the connection request.\nChoose what to do next:\n1. Try to reconnect\n2. Exit\n"
#define SERVER_MAIN_MENU_MSG "Choose what to do next:\n1. Play against another client\n2. Play against the server\n3. View the leaderboard\n4. Quit\n"
#define CHOOSE_WHAT_NEXT "Choose what to do next:\n1. Refresh leaderboard\n2. Return to the main menu\n"
#define CHOOSE_MOVE "Choose a move from the list: Rock, Paper, Scissors, Lizard or Spock:\n"
#define GAME_RESULT_MSG "You played: %s\n%s played : %s\n%s won!\n"
#define TEKO_MSG "You played: %s\n%s played : %s\n"
#define END_GAME_MENU "Choose what to do next:\n1. Play again\n2. Return to the main menu\n"
#define CLIENT_LEFT_GAME "%s has left the game!\n"
#endif