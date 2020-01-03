//Authors – Roy Pasternak(204219273) & Ilay Amar(308520857)
//Project – group54_ex4_server.exe
//Description – 

#include <stdio.h>
#include <stdlib.h>
#include "defines.h"
#include "game_ui.h"

int main(int argc, char *argv[]) {
	ErrorCode_t ret_val = SUCCESS;

	//check argv:
	if (argc != SERVER_ARGUMENTS_NUM)
	{
		printf("Illegal number of arguents! Try again\n");
		return ILLEGAL_NUMBER_OF_ARGV;
	}

	DEBUG_PRINT("============SERVER============\n");

	/* get port */
	int port = (int)strtol(argv[1],NULL,10); /* string to int.... */
	
	ret_val = StartGameServer(port);
	GO_TO_EXIT_ON_FAILURE(ret_val, "StartGameServer failed.\n");

EXIT:
	return ret_val;
}