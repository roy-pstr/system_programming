#include "accept_thread.h"
#include "thread_communication.h"
DWORD AcceptThread(LPVOID lpParam)
{
	ErrorCode_t ret_val = SUCCESS;

	accept_params_t *Args;
	if (NULL == lpParam) {
		printf("error with arguments passed to thread\n");
		return THREAD_PARAMS_CASTING_FAILED;
	}
	Args = (accept_params_t*)lpParam;
	Args->accepted = accept(Args->main, NULL, NULL);
	if (Args->accepted == INVALID_SOCKET)
	{
		printf("Accepting connection with client failed, error %ld\n", WSAGetLastError());
		ret_val = ACCEPT_THREAD_FAILED;
		goto EXIT;
	}
	DEBUG_PRINT(printf("Client Connected.\n"));
	

EXIT:
	return ret_val;
}