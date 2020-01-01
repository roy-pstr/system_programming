//Authors – Roy Pasternak(204219273) & Ilay Amar(308520857)
//Project – group54_ex4_client.exe
//Description –
#include "defines.h"
int main(int argc, char *argv[]) {
	int ret_val = SUCCESS;

	//check argv:
	if (argc != CLIENT_ARGUMENTS_NUM)
	{
		printf("Illegal number of arguents! Try again\n");
		return ILLEGAL_NUMBER_OF_ARGV;
	}

	/* parse arguments */
	char *server_ip; /* ip address of the server */
	int server_port; /* ip address of the server */ 
	char username[USERNAME_MAX_LEN]; /* username - is not CASE sensetive*/
	//ParseArgumnets(argc, argv, &server_ip, &server_port, &username);

	return ret_val;
}