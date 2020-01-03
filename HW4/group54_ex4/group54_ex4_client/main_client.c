//Authors – Roy Pasternak(204219273) & Ilay Amar(308520857)
//Project – group54_ex4_client.exe
//Description –

#include <stdio.h>
#include <string.h>
#include "defines.h"
#include "socket_client.h"
SOCKET m_socket;

ErrorCode_t ParseArgumnets(int argc, char *argv[], char *server_ip, long *server_port, char username[USERNAME_MAX_LEN]) {
	server_ip = argv[1];
	char *ptr;
	*server_port = strtol(argv[2],&ptr,10);
	strcpy_s(username, USERNAME_MAX_LEN, argv[3]);
	return SUCCESS;
}

int main(int argc, char *argv[]) {
	ErrorCode_t ret_val = SUCCESS;

	//check argv:
	if (argc != CLIENT_ARGUMENTS_NUM)
	{
		printf("Illegal number of arguents! Try again\n");
		return ILLEGAL_NUMBER_OF_ARGV;
	}
	printf("============CLIENT============\n");
	/* parse arguments */
	char *server_ip = NULL;
	long server_port = 0;
	char username[USERNAME_MAX_LEN]; 
	ret_val = ParseArgumnets(argc, argv, server_ip, &server_port, username);
	GO_TO_EXIT_ON_FAILURE(ret_val, "ParseArgumnets() failed.\n");
	SetClient();

EXIT:
	return ret_val;
}