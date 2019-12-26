/* Description:
	thread_handler module handles simple threading functions implemnation.
*/
#include <stdbool.h>
#include <stdio.h>
#include "defines.h"
#include "thread_handler.h"

HANDLE CreateMutexSimple()
{
	return CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);
}
HANDLE CreateSemaphoreSimple(int initial_count, int max_count)
{
	return CreateSemaphore(
		NULL,	/* Default security attributes */
		initial_count,		/* Initial Count */
		max_count,		/* Maximum Count */
		NULL); /* name */
}
HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine,
	LPVOID p_thread_parameters,
	LPDWORD p_thread_id)
{
	HANDLE thread_handle;

	if (NULL == p_start_routine)
	{
		printf("Error when creating a thread");
		printf("Received null pointer");
		return NULL;
	}

	if (NULL == p_thread_id)
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
		p_thread_id);    /*  returns the thread identifier */

	if (NULL == thread_handle)
	{
		printf("Couldn't create thread\n");
		return NULL;
	}

	return thread_handle;
}

/* Handle exit code of an handler */
int HandlerExitCode(HANDLE p_thread_handle) {
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
		ret_val = THREAD_FAILED;
	}
	return ret_val;
}
