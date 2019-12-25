#include <stdio.h>
#include <string.h>
#include "guest_thread.h"

/* global semaphore */
extern int guests_per_day_count;

DWORD LockUpdateRoomUnlock(Room_t *room, bool *checked_in)
{
	/* wait for room mutex to be available */
	DWORD wait_code = WaitForSingleObject(room->room_mutex, MUTEX_TIMEOUT_IN_MILLISECONDS);
	if (wait_code != WAIT_OBJECT_0)
	{
		if (wait_code == WAIT_ABANDONED)
		{
			printf("Some thread has previously exited without releasing a mutex."
				" This is not good programming. Please fix the code.\n");
			return (MUTEX_ABANDONED);
		}
		else {
			printf("Mutex wait failed\n");
			return(MUTEX_WAIT_FAILED);
		}

	}
	printf("(from thread %d): inside mutex\n", GetCurrentThreadId());

	/* ........Critical Section Start................ */
	if (room->vacancy_counter > 0)
	{
		room->vacancy_counter--;
		*checked_in = true;
	}
	/* ........Critical Section End.................. */

	/*release mutex*/
	DWORD rel_code;
	if (FALSE == (rel_code = ReleaseMutex(room->room_mutex))) {
		printf("released mutex failed with %d\n", GetLastError());
		return (MUTEX_RELEASE_FAILED);
	}
	printf("(from thread %d): released mutex\n", GetCurrentThreadId());
	return (SUCCESS);
}

int checkOut(guest_params_t *Args) {
	DWORD rel_code;
	LONG previous_count;
	/* update number of available places in room after guest checked out */
	Args->guests_room->vacancy_counter++;

	/*release semaphore of start new day before closing thread... */
	rel_code = ReleaseSemaphore(Args->start_day_sema, 1, &previous_count);
	if (rel_code == FALSE) {
		return MUTEX_RELEASE_FAILED;
	}
	printf("(from thread %d): released mutex\n", GetCurrentThreadId());
	Args->checked_out = true;
	return SUCCESS;
}

void spendTheDay(guest_params_t *Args) {
	Args->guest->budget -= Args->guests_room->price;
}

int tryToCheckIn(guest_params_t *Args) {
	BOOL ret_val;

	if (SUCCESS != (ret_val = LockUpdateRoomUnlock(Args->guests_room, &Args->checked_in ))) {
		printf("LockUpdateRoomUnlock failed!\n");
		return ret_val;
	}
	return SUCCESS;
}

int waitForDayStart(HANDLE SemaHandle) {
	/* wait for mutex*/
	DWORD wait_code = WaitForSingleObject(SemaHandle, MUTEX_TIMEOUT_IN_MILLISECONDS);
	if (wait_code != WAIT_OBJECT_0)
	{
		if (wait_code == WAIT_ABANDONED)
		{
			printf("Some thread has previously exited without releasing a mutex."
				" This is not good programming. Please fix the code.\n");
			return (MUTEX_ABANDONED);
		}
		else {
			printf("Mutex wait failed\n");
			return(MUTEX_WAIT_FAILED);
		}

	}
	return SUCCESS;
}

DWORD WINAPI GuestThread(LPVOID lpParam)
{
	/* Handle arguments passed */
	guest_params_t *Args;
	if (NULL == lpParam) {
		printf("error with arguments passed to thread\n");
		return GUEST_PARAMS_CASTING_FAILED;
	}
	Args = (guest_params_t*)lpParam;

	int ret_val = SUCCESS;
	while (!Args->checked_out)
	{
		/*	guest is still in the hotel,
			looking for a room
			or for spending the day */

		Sleep(SLEEP_TIME);

		/* Args->guest->start_day_sema is a semaphore works as mutex 
			it's role is to make sure each guest procceds only one day.*/
		if (SUCCESS != (ret_val = waitForDayStart(Args->start_day_sema))) {
			printf("waitForDayStart failed with error code: %d\n", ret_val);
			return ret_val;
		}
		printf("guest %s -> started a new day\n", Args->guest->name);

		/* global counter of guests that started a new day.*/
		guests_per_day_count--;

		/* let guest spend the day or if not in a room - 
			try to check in a room if available */
		if (Args->checked_in) {
			spendTheDay(Args);
		}
		else {
			tryToCheckIn(Args);
			if (Args->checked_in) {
				spendTheDay(Args);
			}
		}


		/* guest ran out of budget -> 
			checkout - release mutex and semaphore*/
		if (Args->guest->budget == 0) {
			if (SUCCESS != (ret_val = checkOut(Args))) {
				printf("checkOut failed with error code: %d\n", ret_val);
				return ret_val;
			}
			printf("guest %s -> checked out", Args->guest->name);
		}

		Args->checked_out = true;
	}
	return SUCCESS;
}

Room_t * RoomToGuest(Guest_t *guests_arr, Room_t *room_arr, int num_of_guests) {
}

int InitGuestThreadParams(guest_params_t *p_thread_params, Guest_t *guests_arr, int num_of_guests, Room_t *room_arr) {
	for (int i = 0; i < num_of_guests; i++)
	{
		p_thread_params->guest = guests_arr;
		p_thread_params->guests_room = RoomToGuest(guests_arr, room_arr, num_of_guests);
		p_thread_params->checked_in = false;
		p_thread_params->checked_out = false;
		p_thread_params->start_day_sema = CreateSemaphoreSimple(1,1);
		p_thread_params++;
		guests_arr++;
	}
}

