//Authors – Roy Pasternak(204219273) & Ilay Amar(308520857)
//Project – group54_ex4_server.exe
//Description – 

#include "defines.h"
int main(int argc, char *argv[]) {
	int ret_val = SUCCESS;

	//check argv:
	if (argc != SERVER_ARGUMENTS_NUM)
	{
		printf("Illegal number of arguents! Try again\n");
		return ILLEGAL_NUMBER_OF_ARGV;
	}

	/* get port */
	int port = argv[1]; /* string to int.... */

	return ret_val;
}