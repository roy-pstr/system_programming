#include <stdbool.h>
#include <stdio.h>
#include "defines.h"
#include "thread_handler.h"
#include "day_thread.h"

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
static HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine,
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

/*
	Input:
		int number_of_threads				- total number of threads
		HANDLE *p_thread_handles			- thread handles array (size of number_of_threads)
		DWORD *p_thread_ids					- thread ids array (size of number_of_threads)
		GetGrade_params *p_thread_params	- thread params array (size of number_of_threads)

	Returns:
		SUCCESS_CODE (0) -	on success of all threads.
		ERROR_CODE (-1) -	on failure of one of the threads.
*/
int RunThreadsForAllGuests(int number_of_threads, HANDLE *p_thread_handles, DWORD *p_thread_ids, guest_params_t *p_thread_params)
{
	int i;
	int ret_val = SUCCESS;
	/* create day thread */
	HANDLE day_handle;
	DWORD day_thread_id;
	day_params_t day_params;
	day_params.guests_params = p_thread_params;
	day_params.num_of_guests = number_of_threads;
	if (NULL == (day_handle = CreateThreadSimple(DayThread, &day_params, &day_thread_id)))
	{
		printf("Error when creating day thread: %d\n", GetLastError());
		ret_val = THREAD_CREATE_FAILED;
		goto EXIT; /* must updated!!!! */
	}

	/*create guests threads:*/
	for (i = 0; i < number_of_threads; i++) {
		if (NULL == (p_thread_handles[i] = CreateThreadSimple(GuestThread, &p_thread_params[i], &p_thread_ids[i])))
		{
			printf("Error when creating thread (thread number %d): %d\n", i, GetLastError());
			ret_val = THREAD_CREATE_FAILED;
			goto EXIT;
		}
	}

	/*handle wait code*/
	DWORD wait_code;
	wait_code = WaitForMultipleObjects(number_of_threads, p_thread_handles, TRUE, THREAD_TIMEOUT_IN_MILLISECONDS);
	if (WAIT_TIMEOUT == wait_code)
	{
		printf("Timeout error when waiting\n");
		ret_val = THREAD_WAIT_FAILED;
		goto EXIT;
	}
	else if (WAIT_FAILED == wait_code)
	{
		printf("WaitForMultipleObjects has failed\n");
		ret_val = THREAD_WAIT_FAILED;
		goto EXIT;
	}
	else if (WAIT_OBJECT_0 != wait_code)
	{
		printf("Error when waiting\n");
		ret_val = THREAD_WAIT_FAILED;
		goto EXIT;
	}

	/*handle exit code*/
	DWORD lpExitCode;
	for (i = 0; i < number_of_threads; i++) {
		if (FALSE == GetExitCodeThread(p_thread_handles[i], &lpExitCode))
		{
			printf("Error when getting thread exit code\n");
			ret_val = THREAD_GET_EXIT_CODE_FAILED;
			goto EXIT;
		}
		if (lpExitCode != SUCCESS) {
			printf("Error with thread (thread number: %d) exit code (%d)\n", i, lpExitCode);
			ret_val = THREAD_FAILED;
			goto EXIT;
		}
	}

	/*close threads handels*/
EXIT:
	for (i = 0; i < number_of_threads; i++)
	{
		if (NULL != p_thread_handles[i]) {
			if (FALSE == CloseHandle(p_thread_handles[i]))
			{
				printf("Error when closing thread: %d\n", GetLastError());
				return THREAD_CLOSE_ERROR;
			}
		}
	}
	return ret_val;
}