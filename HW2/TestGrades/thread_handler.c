/* Description: 
	thread_handler module handles the running of mutiplay threads
*/

#include <stdbool.h>
#include <stdio.h>
#include "thread_handler.h"

#define MAX_THREAD_WAIT_TIME 5000

/*
	Simplify version of CreateThread
*/
static HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine,
	LPVOID p_thread_parameters,
	LPDWORD p_thread_id)
{
	HANDLE thread_handle;

	if (NULL == p_start_routine)
	{
		printf("Error when creating a thread");
		printf("Received null pointer");
		exit(ERROR_CODE);
	}

	if (NULL == p_thread_id)
	{
		printf("Error when creating a thread");
		printf("Received null pointer");
		exit(ERROR_CODE);
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
		exit(ERROR_CODE);
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
int RunMultiplayThreads(int number_of_threads, HANDLE *p_thread_handles, DWORD *p_thread_ids, GetGrade_params *p_thread_params)
{
	int i;

	/*create threads:*/
	for (i = 0; i < number_of_threads; i++) {
		p_thread_handles[i] = CreateThreadSimple(GetGradeThread, &p_thread_params[i], &p_thread_ids[i]);
		if (NULL == p_thread_handles[i])
		{
			printf("Error when creating thread (thread number %d): %d\n",i, GetLastError());
			return ERROR_CODE;
		}
	}

	/*handle wait code*/
	DWORD wait_code;
	wait_code = WaitForMultipleObjects(number_of_threads, p_thread_handles, TRUE, MAX_THREAD_WAIT_TIME);
	if (WAIT_TIMEOUT == wait_code)
	{
		printf("Timeout error when waiting\n");
		return ERROR_CODE;
	}
	else if (WAIT_FAILED == wait_code)
	{
		printf("WaitForMultipleObjects has failed\n");
		return ERROR_CODE;
	}
	else if (WAIT_OBJECT_0 != wait_code)
	{
		printf("Error when waiting\n");
		return ERROR_CODE;
	}

	/*handle exit code*/
	DWORD lpExitCode;
	BOOL ret_val;
	for (i = 0; i < number_of_threads; i++) {
		ret_val = GetExitCodeThread(p_thread_handles[i], &lpExitCode);
		if (FALSE == ret_val)
		{
			printf("Error when getting thread exit code\n");
			return ERROR_CODE;
		}
		if (lpExitCode == ERROR_CODE) {
			printf("Error with thread (thread number: %d) exit code (%d)\n", i, ERROR_CODE);
			return ERROR_CODE;
		}
	}

	/*close threads handels*/
	for (i = 0; i < number_of_threads; i++)
	{
		ret_val = CloseHandle(p_thread_handles[i]);
		if (FALSE == ret_val)
		{
			printf("Error when closing thread: %d\n", GetLastError());
			return ERROR_CODE;
		}
	}

	return SUCCESS_CODE;
}


