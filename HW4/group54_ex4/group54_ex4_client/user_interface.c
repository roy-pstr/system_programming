#include "user_interface.h"

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
			return CLIENT_REFRESH;
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