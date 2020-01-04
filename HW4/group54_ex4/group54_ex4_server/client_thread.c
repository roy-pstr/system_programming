#include "client_thread.h"
#include "socket_tools.h"
#include "msg_protocol.h"
//Service thread is the thread that opens for each successful client connection and "talks" to the client.
DWORD ClientThread(SOCKET *t_socket)
{
	char SendStr[MAX_STR_LEN];

	BOOL Done = FALSE;
	TransferResult_t SendRes;
	TransferResult_t RecvRes;

	strcpy(SendStr, "Welcome to this server!");

	SendRes = SendString(SendStr, *t_socket);

	if (SendRes == TRNS_FAILED)
	{
		printf("Service socket error while writing, closing thread.\n");
		closesocket(*t_socket);
		return 1;
	}

	while (!Done)
	{
		char *AcceptedStr = NULL;

		RecvRes = ReceiveString(&AcceptedStr, *t_socket); /* no '\0' at end of string for AcceptedStr!! */

		if (RecvRes == TRNS_FAILED)
		{
			printf("Service socket error while reading, closing thread.\n");
			closesocket(*t_socket);
			return 1;
		}
		else if (RecvRes == TRNS_DISCONNECTED)
		{
			printf("Connection closed while reading, closing thread.\n");
			closesocket(*t_socket);
			return 1;
		}
		else
		{
			printf("Got string : %s\n", AcceptedStr); 
			msg_t protocol_msg;
			InitMsg_t(&protocol_msg);
			ParseMessage(AcceptedStr, (int)strlen(AcceptedStr)+1, &protocol_msg);
			DEBUG_PRINT(PrintProtocol(&protocol_msg));
		}

		//After reading a single line, checking to see what to do with it
		//If got "hello" send back "what's up?"
		//If got "how are you?" send back "great"
		//If got "bye" send back "see ya!" and then end the thread
		//Otherwise, send "I don't understand"

		if (STRINGS_ARE_EQUAL(AcceptedStr, "hello"))
		{
			strcpy(SendStr, "what's up?");
		}
		else if (STRINGS_ARE_EQUAL(AcceptedStr, "how are you?"))
		{
			strcpy(SendStr, "great");
		}
		else if (STRINGS_ARE_EQUAL(AcceptedStr, "bye"))
		{
			strcpy(SendStr, "see ya!");
			Done = TRUE;
		}
		else
		{
			strcpy(SendStr, "I don't understand");
		}

		SendRes = SendString(SendStr, *t_socket);

		if (SendRes == TRNS_FAILED)
		{
			printf("Service socket error while writing, closing thread.\n");
			closesocket(*t_socket);
			return 1;
		}

		free(AcceptedStr);
	}

	printf("Conversation ended.\n");
	closesocket(*t_socket);
	*t_socket = INVALID_SOCKET;
	return 0;
}
