#include "user_interface.h"
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
	return UNDEFINED_MOVE;
}

MOVES_ENUM ChooseMove() {
	bool valid_input = false;
	char user_answer[MOVE_STRING_MAX_LEN];
	MOVES_ENUM move = UNDEFINED_MOVE;
	while (!valid_input) {
		printf(CHOOSE_MOVE);
		gets_s(user_answer, sizeof(user_answer)); //Reading a string from the keyboard
		move = StringToEnum(user_answer);
		if (move == UNDEFINED_MOVE) {
			printf(CHOOSE_MOVE_UNVALID);
		}
		else {
			valid_input = true; /*exit loop*/
		}
	}
	return move;
}

PROTOCOL_ENUM EndGameMenu() {
	bool valid_input = false;
	char user_answer[INPUT_ANSWER_STR_LEN];
	while (!valid_input) {
		printf(END_GAME_MENU);
		gets_s(user_answer, sizeof(user_answer)); //Reading a string from the keyboard
		if (STRINGS_ARE_EQUAL(user_answer, "1")) {
			return CLIENT_REPLAY;
		}
		else if (STRINGS_ARE_EQUAL(user_answer, "2")) {
			return CLIENT_MAIN_MENU;
		}
	}
	return ERROR_MSG_TYPE;
}

MenuCode_t ReconnectMenu() {
	bool valid_input = false;
	char user_answer[INPUT_ANSWER_STR_LEN];
	while (!valid_input) {
		printf(CHOOSE_WHAT_NEXT_ON_CLIENT_CONNECTION_FAILURE);
		gets_s(user_answer, sizeof(user_answer)); //Reading a string from the keyboard
		if (STRINGS_ARE_EQUAL(user_answer, "1")) { /* Try to reconnect */
			return CLIENT_TRY_RECONNECT;
		}
		else if (STRINGS_ARE_EQUAL(user_answer, "2")) { /* Exit */
			return CLIENT_EXIT_RECONNECT;
		}
	}
	return ERROR_MSG_TYPE;
}

PROTOCOL_ENUM LeaderboardMenu() {
	bool valid_input = false;
	char user_answer[INPUT_ANSWER_STR_LEN];
	while (!valid_input) {
		printf(CHOOSE_WHAT_NEXT_LEADERBOARD);
		gets_s(user_answer, sizeof(user_answer)); //Reading a string from the keyboard
		if (STRINGS_ARE_EQUAL(user_answer, "1")) { 
			return CLIENT_REPLAY;
		}
		else if (STRINGS_ARE_EQUAL(user_answer, "2")) { 
			return CLIENT_MAIN_MENU;
		}
	}
	return ERROR_MSG_TYPE;
}

PROTOCOL_ENUM MainMenu() {
	bool valid_input = false;
	char user_answer[INPUT_ANSWER_STR_LEN];
	while (!valid_input) {
		printf(SERVER_MAIN_MENU_MSG);
		gets_s(user_answer, sizeof(user_answer)); //Reading a string from the keyboard
		if (STRINGS_ARE_EQUAL(user_answer, "1")) { /* Try to reconnect */
			return CLIENT_VERSUS;
		}
		else if (STRINGS_ARE_EQUAL(user_answer, "2")) { 
			return CLIENT_CPU;
		}
		else if (STRINGS_ARE_EQUAL(user_answer, "3")) { 
			return CLIENT_LEADERBOARD;
		}
		else if (STRINGS_ARE_EQUAL(user_answer, "4")) {
			return CLIENT_DISCONNECT;
		}
	}
	return ERROR_MSG_TYPE;
}