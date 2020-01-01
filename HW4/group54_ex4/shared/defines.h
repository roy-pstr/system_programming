#ifndef _DEFINES_H
#define _DEFINES_H
#include <stdio.h>
#include <stdbool.h>

#define USERNAME_MAX_LEN 20
#define SERVER_ARGUMENTS_NUM 2
#define CLIENT_ARGUMENTS_NUM 4
#define MAX_STR_LEN 256
/* socket related */
#define MAX_CONN_LISTEN 10
#define SERVER_MAX_LOOPS 1
#define SERVER_ADDRESS_STR "127.0.0.1"
#define SERVER_PORT 8888
#define WAIT_FOR_SERVER_RESPONSE 15000 //15 seconds
#define WAIT_FOR_CLIENT_RESPONSE 15000 //15 seconds

/* inline MACRO function defenition: */
#define STRINGS_ARE_EQUAL( Str1, Str2 ) ( strcmp( (Str1), (Str2) ) == 0 )

typedef enum {
	SUCCESS,
	ILLEGAL_NUMBER_OF_ARGV,
	SOCKET_ERROR_SEND_DATA,
	SOCKET_ERROR_RECV_DATA
}ErrorCode_t;

void PrintError(char *msg) {
	printf("%s\n", msg);
}

void TestReturnValue(bool ret_val, char *msg) {
	if (SUCCESS != ret_val) {
		PrintError(msg);
	}
}

#endif