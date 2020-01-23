#include "main_thread.h"
#include "socket_server.h"
#include "thread_communication.h"
/* Thread definition */
DWORD MainThread(LPVOID lpParam)
{
	ErrorCode_t ret_val = SUCCESS;

	/* Handle arguments passed */
	main_params_t *Args;
	if (NULL == lpParam) {
		printf("error with arguments passed to thread\n");
		return THREAD_PARAMS_CASTING_FAILED;
	}
	Args = (main_params_t*)lpParam;
	/* make sure GameSession is dead */
	if (IsFileExists(GS_NAME)){
		DeleteGameSessionFile();
	}
	ret_val = StartGameServer(Args->port);
	GO_TO_EXIT_ON_FAILURE(ret_val, "StartGameServer failed.\n");

EXIT:
	DEBUG_PRINT(printf("Server(main thread): \"byebye.\"\n"));
	return ret_val;
}
extern bool main_thread_still_alive;

DWORD MyExitThread()
{
	ErrorCode_t ret_val = SUCCESS;
	char user_input[MAX_STR_LEN];
	while (main_thread_still_alive) {
		gets_s(user_input, sizeof(user_input)); //Reading a string from the keyboard
		if (STRINGS_ARE_EQUAL(user_input, "exit")) {
			break; /*exit thread*/
		}
	}
	return ret_val;
}

