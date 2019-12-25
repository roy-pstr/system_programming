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


