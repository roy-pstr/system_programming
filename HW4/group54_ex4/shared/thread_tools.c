#include "thread_tools.h"

#include <stdio.h>
#include <string.h>

void InitHandels(HANDLE *thread_handels, int size) {
	int Ind;
	for (Ind = 0; Ind < size; Ind++)
	{
		*thread_handels = NULL;
		thread_handels++;
	}
}

HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine, LPVOID p_thread_parameters)
{
	HANDLE thread_handle;

	if (NULL == p_start_routine)
	{
		printf("Error when creating a thread");
		printf("Received null pointer");
		return NULL;
	}

	thread_handle = CreateThread(
		NULL,            /*  default security attributes */
		0,               /*  use default stack size */
		p_start_routine, /*  thread function */
		p_thread_parameters,     /*  argument to thread function */
		0,               /*  use default creation flags */
		NULL);			/*  returns the thread identifier */

	if (NULL == thread_handle)
	{
		printf("Couldn't create thread\n");
		return NULL;
	}

	return thread_handle;
}

/* Handle exit code of an handler */
ErrorCode_t HandlerExitCode(HANDLE p_thread_handle) {
	int ret_val = SUCCESS;
	/*handle exit code*/
	DWORD lpExitCode;
	if (FALSE == GetExitCodeThread(p_thread_handle, &lpExitCode))
	{
		printf("Error when getting thread exit code: %d\n", GetLastError());
		ret_val = THREAD_GET_EXIT_CODE_FAILED;
	}
	else if (lpExitCode != SUCCESS) {
		printf("Error with thread exit code (%d)\n", lpExitCode);
		ret_val = CLIENT_THREAD_FAILED;
	}
	return ret_val;
}