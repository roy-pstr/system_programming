#ifndef _DEFINES_H
#define _DEFINES_H
#include <stdio.h>
#include <stdbool.h>

#define SERVER_ARGUMENTS_NUM 2
#define CLIENT_ARGUMENTS_NUM 2
#define USERNAME_MAX_LEN 20
#define WAIT_FOR_SERVER_RESPONSE 15000 //15 seconds
#define WAIT_FOR_CLIENT_RESPONSE 15000 //15 seconds

typedef enum {
	SUCCESS,
	ILLEGAL_NUMBER_OF_ARGV,
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