#ifndef _UTILS_H
#define _UTILS_H
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG true

#define INT_MAX_LEN 20 // x64
#define RATIO_MAX_LEN 24 // .###
#define LINE_MAX_LEN 65 //
#define SPACES_MAX_LEN 50 //
#define SERVER_ARGUMENTS_NUM 2
#define CLIENT_ARGUMENTS_NUM 4
#define USERNAME_MAX_LEN 20
#define MAX_STR_LEN 256
#define INPUT_ANSWER_STR_LEN 10
#define MOVE_STRING_MAX_LEN 9
/* protocols */
#define PROTOCOL_TYPE_STR_MAX_LEN 27
#define PARAM_STR_MAX_LEN 50
#define PROTOCOL_PARAM_LIST_SIZE 4
#define PROTOCOL_MESSAGE_MAX_LEN (PROTOCOL_TYPE_STR_MAX_LEN+PROTOCOL_PARAM_LIST_SIZE*PARAM_STR_MAX_LEN+10)+500
#define LEADERBOARD_STR_MAX_LEN 500
/* socket related */
#define MAX_CONN_LISTEN 10
#define NUMBER_OF_CLIENTS 2
#define SERVER_ADDRESS_STR "127.0.0.1"
#define SERVER_PORT 8888
#define WAIT_FOR_SERVER_RESPONSE 15000 //15 seconds
#define WAIT_FOR_CLIENT_RESPONSE 15000 //15 seconds
#define WAIT_FOR_CLOSING_CLIENT_THREAD 5000
#define WAIT_FOR_SECOND_PLAYER_TO_JOIN_GAME 30000 // 30 seconds
#define WAIT_FOR_SECOND_PLAYER_TO_CHOOSE_WHAT_TO_DO INFINITE // 30 seconds

/* inline MACRO function defenition: */
#define STRINGS_ARE_EQUAL( Str1, Str2 ) ( strcmp( (Str1), (Str2) ) == 0 )
#define GO_TO_EXIT_ON_FAILURE(ret_val, msg) {if (SUCCESS != ret_val){printf("%s\n",msg);goto EXIT;}}


typedef enum {
	SUCCESS,
	ILLEGAL_NUMBER_OF_ARGV,
	WSASTARTUP_FAILURE,
	FILE_ERROR,
	MALLOC_STRING_FAIL,
	SOCKET_CREATE_ERROR,
	SOCKET_INIT_IP_ADDR_ERROR,
	SOCKET_CONNECT_FAILED,
	SOCKET_ERROR_SEND_DATA,
	SOCKET_ERROR_RECV_DATA,
	SOCKET_ACCEPT_FAILED,
	SERVER_DENIED_CONNECT,
	THREAD_PARAMS_CASTING_FAILED,
	THREAD_CREATE_FAILED,
	THREAD_WAIT_FAILED,
	THREAD_GET_EXIT_CODE_FAILED,
	THRAD_WAIT_FAILED,
	THRAD_TERMINATE_FAILED,
	CLIENT_THREAD_FAILED,
	CLIENT_THREAD_CLOSE_ERROR,
	CLOSE_SOCKET_CLIENT,
	CLIENT_EXIT_TRY_CONNECT,
	PROTOCOL_MSG_TYPE_ERROR,
	INVALID_MESSAGE_PROTOCOL,
	PARAM_LIST_INDEX_OVERFLOW,
	EVENT_CREATE_FAILED,
	EVENT_WAIT_ERROR,
	EVENT_WAIT_TIMEOUT,
	EVENT_RESET_ERROR,
	SET_EVENT_ERROR,
	MUTEX_CREATE_FAILED,
	MUTEX_ABANDONED,
	MUTEX_WAIT_FAILED,
	MUTEX_RELEASE_FAILED,
	GET_OPPENONT_NAME_FAILED,
}ErrorCode_t;

typedef enum {
	ROCK,
	PAPER,
	SCISSORS,
	LIZARD,
	SPOCK,
	UNDEFINED_MOVE
} MOVES_ENUM;

static const char *MOVES_STRINGS[] = {
	"ROCK",
	"PAPER",
	"SCISSORS",
	"LIZARD",
	"SPOCK"
};

/*return move as enum*/
MOVES_ENUM StringToEnum(char *move);

void AllocateFullParamList(char *** param_list);

void FreeFullParamList(char *** param_list);

#define DEBUG_PRINT(print) {if (DEBUG==true){printf("DEBUG: ");print;}}

#endif