/* module utils incluse all the shared definitions and gereral functions */

#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG false
#define PRINTS true
#define ALLOW_SAME_USER_NAME false


#define INT_MAX_LEN 20 // x64
#define RATIO_MAX_LEN 24 // .###
#define LINE_MAX_LEN 65 //
#define SERVER_ARGUMENTS_NUM 2
#define CLIENT_ARGUMENTS_NUM 4
#define USERNAME_MAX_LEN 20
#define MAX_STR_LEN 256
#define INPUT_ANSWER_STR_LEN 256
#define MOVE_STRING_MAX_LEN 9

/* protocols */
#define PROTOCOL_TYPE_STR_MAX_LEN 27
#define PARAM_STR_MAX_LEN 65
#define PROTOCOL_PARAM_LIST_SIZE 4

/* socket related */
#define MAX_CONN_LISTEN 10
#define NUMBER_OF_CLIENTS 2
#define SERVER_ADDRESS_STR "127.0.0.1"
#define WAIT_FOR_SECOND_PLAYER_TO_JOIN_GAME 30000 // 30 seconds
#define WAIT_FOR_SECOND_PLAYER_TO_CHOOSE_WHAT_TO_DO INFINITE 
#define CLIENT_RECIVE_TIMEOUT 15 //seconds 
#define SERVER_RECIVE_TIMEOUT 15 //seconds
#define CLIENT_VERSUS_TIMEOUT 30 //seconds
#define SERVER_WAIT_FOR_OTHER_PLAYER_MOVE 30 //seconds
#define ACCEPT_LIESNTER_INTERVALS_SEC 1 //seconds
#define WAIT_TIME_TO_MAIN_THREAD_TO_CLOSE_AFTER_EXIT 15000 //miliseconds

/* inline MACRO function defenition: */
#define STRINGS_ARE_EQUAL( Str1, Str2 ) ( strcmp( (Str1), (Str2) ) == 0 )
#define GO_TO_EXIT_ON_FAILURE(ret_val, msg) {if (SUCCESS != ret_val){printf("%s\n",msg);goto EXIT;}}

/* all posible return error code */
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
	SERVER_RECV_TIMEDOUT,
	SERVER_RECIVE_FAILED,
	THREAD_PARAMS_CASTING_FAILED,
	THREAD_CREATE_FAILED,
	THREAD_WAIT_FAILED,
	THREAD_GET_EXIT_CODE_FAILED,
	THRAD_WAIT_FAILED,
	THREAD_TERMINATE_FAILED,
	THREAD_CLOSE_ERROR,
	THREAD_TREMINATE,
	ACCEPT_THREAD_FAILED,
	CLIENT_THREAD_FAILED,
	CLIENT_THREAD_CLOSE_ERROR,
	CLOSE_SOCKET_CLIENT,
	CLIENT_EXIT_TRY_CONNECT,
	PROTOCOL_MSG_TYPE_ERROR,
	PROTOCOL_MESSAGE_INVALID,
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

/* moves enum */
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

/*convert move string to move enum*/
MOVES_ENUM StringToEnum(char *move);

/* 2d array allocation */
void AllocateFullParamList(char *** param_list);
void FreeFullParamList(char *** param_list);

/* string allocation */
ErrorCode_t AllocateString(char ** str_ptr, int len);

/*Funtction that rounds float #p points after the dot*/
double Round(double x, int p);

/* check if file exist in given path */
bool IsFileExists(char* filename);

/* debug */
#define DEBUG_PRINT(print) {if (DEBUG==true){printf("DEBUG: ");print;}}
#define SERVER_PRINT(print) {if (PRINTS==true){printf("");print;}}

#endif