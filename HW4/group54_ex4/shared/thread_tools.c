#include "thread_tools.h"

#include <stdio.h>
#include <string.h>

ErrorCode_t MyResetEvent(HANDLE event) {
	BOOL result = ResetEvent(event);
	return (result != 0) ? SUCCESS : EVENT_RESET_ERROR;
}
ErrorCode_t WaitForMutex(HANDLE mutex) {
	DWORD wait_code = WaitForSingleObject(mutex, INFINITE);
	if (wait_code != WAIT_OBJECT_0)
	{
		if (wait_code == WAIT_ABANDONED)
		{
			printf("Some thread has previously exited without releasing a mutex.\n");
			return (MUTEX_ABANDONED);
		}
		else {
			printf("Mutex wait failed\n");
			return(MUTEX_WAIT_FAILED);
		}

	}
	return SUCCESS;
}
ErrorCode_t ReleaseMutexSimp(HANDLE mutex) {
	DWORD rel_code;
	if (FALSE == (rel_code = ReleaseMutex(mutex))) {
		printf("released mutex failed with %d\n", GetLastError());
		return (MUTEX_RELEASE_FAILED);
	}
	return SUCCESS;
}
HANDLE CreateEventSimple()
{
	return CreateEvent(
		NULL,		/* default security attributes */
		TRUE,		/* manual-reset event */
		FALSE,      /* initial state is non-signaled */
		NULL);      /* name */
}
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