#include <stdio.h>
#include "thread_handler.h"
#include "threads_runner.h"
#include "day_thread.h"

extern bool all_guests_checked_out;
extern HANDLE end_day_lock;

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
int RunGuestsThreads(int number_of_threads, HANDLE *p_thread_handles, DWORD *p_thread_ids, guest_params_t *p_thread_params)
{
	int i;
	int ret_val = SUCCESS;

	end_day_lock = CreateSemaphoreSimple(0,1);
	HANDLE day_handle;
	DWORD day_thread_id;
	day_params_t day_params;
	day_params.guests_params = p_thread_params;
	day_params.num_of_guests = number_of_threads;
	/*create day thread:*/
	if (NULL == (day_handle = CreateThreadSimple(DayThread, &day_params, &day_thread_id)))
	{
		printf("Error when creating day thread: %d\n", GetLastError());
		ret_val = THREAD_CREATE_FAILED;
		goto EXIT;
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

	/*handle guests wait code*/
	DWORD wait_code;
	wait_code = WaitForMultipleObjects(number_of_threads, p_thread_handles, TRUE, GUEST_THREAD_TIMEOUT_IN_MILLISECONDS);
	if (WAIT_OBJECT_0 != wait_code || WAIT_FAILED == wait_code || WAIT_TIMEOUT == wait_code)
	{
		printf("WaitForMultipleObjects has failed\n");
		ret_val = THREAD_WAIT_FAILED;
		goto EXIT;
	}

	/* signel to DayThread that all guests checked out */
	all_guests_checked_out = true;

	/* wait for DayThread to return value */
	wait_code = WaitForSingleObject(day_handle, DAY_THREAD_TIMEOUT_IN_MILLISECONDS);
	if (WAIT_OBJECT_0 != wait_code || WAIT_FAILED == wait_code || WAIT_TIMEOUT == wait_code)
	{
		printf("WaitForSingleObject has failed\n");
		ret_val = THREAD_WAIT_FAILED;
		goto EXIT;
	}
	
	/*handle guests exit code*/
	for (i = 0; i < number_of_threads; i++) {
		if (SUCCESS != (ret_val = HandlerExitCode(p_thread_handles[i]))) {
			printf("HandleDayThreadExitCode failed.");
			goto EXIT;
		}
	}

	/*handle day exit code*/
	if (SUCCESS != (ret_val = HandlerExitCode(day_handle))) {
		printf("HandleDayThreadExitCode failed.");
		goto EXIT;
	}

EXIT:
	/*close guest threads handels*/
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
	/* close day thread */
	if (NULL != day_handle) {
		if (FALSE == CloseHandle(day_handle))
		{
			printf("Error when closing thread: %d\n", GetLastError());
			return THREAD_CLOSE_ERROR;
		}
	}
	return ret_val;
}
