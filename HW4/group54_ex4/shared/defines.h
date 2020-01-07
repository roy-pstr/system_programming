#ifndef _DEFINES_H
#define _DEFINES_H
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#define DEBUG true

#define USERNAME_MAX_LEN 20
#define INT_MAX_LEN 20 // x64
#define RATIO_MAX_LEN 24 // .###
#define LINE_MAX_LEN 65 //
#define SPACES_MAX_LEN 18 //


#define SERVER_ARGUMENTS_NUM 2
#define CLIENT_ARGUMENTS_NUM 4
#define MAX_STR_LEN 256
/* protocols */
#define PROTOCOL_TYPE_STR_MAX_LEN 27
#define PARAM_STR_MAX_LEN 22
#define PROTOCOL_PARAM_LIST_SIZE 4
#define PROTOCOL_MESSAGE_MAX_LEM (PROTOCOL_TYPE_STR_MAX_LEN+PROTOCOL_PARAM_LIST_SIZE*PARAM_STR_MAX_LEN+10)
/* socket related */
#define MAX_CONN_LISTEN 10
#define NUMBER_OF_CLIENTS 2
#define SERVER_ADDRESS_STR "127.0.0.1"
#define SERVER_PORT 8888
#define WAIT_FOR_SERVER_RESPONSE 15000 //15 seconds
#define WAIT_FOR_CLIENT_RESPONSE 15000 //15 seconds
#define WAIT_FOR_CLOSING_CLIENT_THREAD 5000

/* inline MACRO function defenition: */
#define STRINGS_ARE_EQUAL( Str1, Str2 ) ( strcmp( (Str1), (Str2) ) == 0 )
#define GO_TO_EXIT_ON_FAILURE(ret_val, msg) {if (SUCCESS != ret_val){printf("%s\n",msg);goto EXIT;}}


#define FILE_ERROR -1

typedef enum {
	SUCCESS,
	ILLEGAL_NUMBER_OF_ARGV,
	WSAStartup_FAILURE,
	SOCKET_ERROR_SEND_DATA,
	SOCKET_ERROR_RECV_DATA,
	THREAD_CREATE_FAILED,
	THREAD_WAIT_FAILED,
	THREAD_GET_EXIT_CODE_FAILED,
	CLIENT_THREAD_FAILED,
	CLIENT_THREAD_CLOSE_ERROR,
	CLOSE_SOCKET_CLIENT, 
	MALLOC_STRING_FAIL,
	PROTOCOL_MSG_TYPE_ERROR,
	PARAM_LIST_INDEX_OVERFLOW

}ErrorCode_t;

#define DEBUG_PRINT(print) {if (DEBUG==true){printf("DEBUG: ");print;}}

#endif