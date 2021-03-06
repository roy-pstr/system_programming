/* Description:
	thread_handler module handles the running of mutiplay threads
*/

#include <stdio.h>
#include "thread_handler.h"
#include "threads_runner.h"

/* global vars */
extern bool all_guests_checked_out;
extern HANDLE end_day_lock;
extern HANDLE guests_per_day_count_lock;
extern HANDLE day_handle;

int InitGlobalSemaphores() {
	int ret_val = SUCCESS;
	/* create the global semaphore which controls the day progress */
	if (NULL == (end_day_lock = CreateSemaphoreSimple(0, 1))) {
		printf("Error when creating end_day_lock Semaphore: %d\n", GetLastError());
		ret_val = SEMAPHORE_CREATE_FAILED;
		goto EXIT;
	}
	/* create the global semaphore which controls the access to guests_per_day_count counter*/
	if (NULL == (guests_per_day_count_lock = CreateSemaphoreSimple(1, 1))) {
		printf("Error when creating guests_per_day_count_lock Semaphore: %d\n", GetLastError());
		ret_val = SEMAPHORE_CREATE_FAILED;
		goto EXIT;
	}
	EXIT:
	return ret_val;
}

/* close rooms, guests open handles and global handels 'end_day_lock', 'guests_per_day_count_lock'*/
int CloseAllOpenHandels(guest_params_t *p_thread_params, int num_of_guests, Room_t *rooms, int num_of_rooms) {
	int ret_val = SUCCESS;
	for (int i = 0; i < num_of_guests; i++)
	{

		if (NULL != p_thread_params->start_day_sema) {
			if (FALSE == CloseHandle(p_thread_params->start_day_sema))
			{
				printf("Error when closing semaphore: %d\n", GetLastError());
				return SEMAPHORE_CLOSE_FAILED;
			}
		}
		p_thread_params++;
	}
	for (int j = 0; j < num_of_rooms; j++)
	{
		if (NULL != rooms->room_mutex) {
			if (FALSE == CloseHandle(rooms->room_mutex))
			{
				printf("Error when closing mutex: %d\n", GetLastError());
				return MUTEX_CLOSE_FAILED;
			}
		}
		rooms++;
	}

	if (NULL != guests_per_day_count_lock) {
		if (FALSE == CloseHandle(guests_per_day_count_lock))
		{
			printf("Error when closing semaphore: %d\n", GetLastError());
			return SEMAPHORE_CLOSE_FAILED;
		}
	}

	if (NULL != end_day_lock) {
		if (FALSE == CloseHandle(end_day_lock))
		{
			printf("Error when closing semaphore: %d\n", GetLastError());
			return SEMAPHORE_CLOSE_FAILED;
		}
	}

	if (NULL != day_handle) {
		if (FALSE == CloseHandle(day_handle))
		{
			printf("Error when closing thread: %d\n", GetLastError());
			return THREAD_CLOSE_ERROR;
		}
	}
	return ret_val;
}

/*
	Input:
		int number_of_threads				- total number of threads
		HANDLE *p_thread_handles			- thread handles array (size of number_of_threads)
		DWORD *p_thread_ids					- thread ids array (size of number_of_threads)
		guest_params_t *p_thread_params		- thread params array (size of number_of_threads)
		day_params_t *day_params			- day thread params
	Returns:
		SUCCESS (0) -	on success of all threads.
		else		-	failure.
	The functions first run the DayThread then run all of GuestThreads and wait for all of the guests thread.
	Then it will procced and wait for DayThread to finish running (which should be immidently)
	Then handle exit codes and close all handels.
*/
int RunGuestsAndDayThreads(int number_of_threads, HANDLE *p_thread_handles, DWORD *p_thread_ids, guest_params_t *p_thread_params, day_params_t *day_params)
{
	int i;
	int ret_val = SUCCESS;
	
	DWORD day_thread_id;

	/* create the global semaphores */
	ret_val = InitGlobalSemaphores();
	GO_TO_EXIT_ON_FAILURE(ret_val, "InitGlobalSemaphores failed!");

	/*create day thread:*/
	if (NULL == (day_handle = CreateThreadSimple(DayThread, day_params, &day_thread_id)))
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

	/* close all open handels: rooms, guests open handles and global handels 'end_day_lock', 'guests_per_day_count_lock', 'day_end' */
	ret_val = CloseAllOpenHandels(p_thread_params, day_params->num_of_guests, p_thread_params->all_rooms, p_thread_params->num_of_rooms);
	GO_TO_EXIT_ON_FAILURE(ret_val, "CloseAllOpenHandels failed!");

	return ret_val;
}

int InitGuestAndDayThreadParams(day_params_t *day_params, guest_params_t *p_thread_params, Guest_t *guests_arr, int num_of_rooms, Room_t *room_arr) {
	int ret_val = SUCCESS;
	day_params->guests_params = p_thread_params;
	for (int i = 0; i < day_params->num_of_guests; i++)
	{
		p_thread_params->num_of_guests = day_params->num_of_guests;
		p_thread_params->guest = guests_arr;
		p_thread_params->num_of_rooms = num_of_rooms;
		p_thread_params->all_rooms = room_arr;
		p_thread_params->checked_in = false;
		p_thread_params->days_guest_in_room = 0;
		p_thread_params->checked_out = false;
		p_thread_params->fp = day_params->fp;
		if (NULL == (p_thread_params->start_day_sema = CreateSemaphoreSimple(0, 1))) {
			printf("Error when creating Semaphore: %d\n", GetLastError());
			ret_val = SEMAPHORE_CREATE_FAILED;
			goto EXIT;
		}
		p_thread_params++;
		guests_arr++;
	}
EXIT:
	return ret_val;
}