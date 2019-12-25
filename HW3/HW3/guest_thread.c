#include <stdio.h>
#include <string.h>
#include "guest_thread.h"

/* global semaphore */
extern int guests_per_day_count;

DWORD LockUpdateAndUnlock(HANDLE MutexHandle)
{
	/* wait for mutex*/
	DWORD wait_code = WaitForSingleObject(MutexHandle, MUTEX_TIMEOUT_IN_MILLISECONDS);
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

	/* ........Critical Section End.................. */

	/*release mutex*/
	DWORD rel_code = ReleaseMutex(MutexHandle);
	if (rel_code == FALSE) {
		return (MUTEX_RELEASE_FAILED);
	}
	printf("(from thread %d): released mutex\n", GetCurrentThreadId());
	return (SUCCESS);
}

int checkOut(guest_params_t *Args) {
	DWORD rel_code;
	LONG previous_count;
	/*release semaphore of room capacity so other guests may check in */
	rel_code = ReleaseSemaphore(Args->guests_room->capacity_sem,1,&previous_count);
	if (rel_code == FALSE) {
		return SEMAPHORE_RELEASE_FAILED;
	}
	printf("(from thread %d): Previous count is: %ld\n", GetCurrentThreadId(), previous_count);

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
	DWORD wait_code;
	BOOL ret_val;


	/* wait for semaphore*/
	wait_code = WaitForSingleObject(Args->guests_room->capacity_sem, SEMAPHORE_TIMEOUT_IN_MILLISECONDS);
	if (wait_code != WAIT_OBJECT_0) {
		printf("Error when waiting for simaphore: %d\n", wait_code);
		return SEMAPHORE_WAIT_FAILED;
	}

	//printf("(from thread %d): inside empty_semaphore\n", Args->a);

	LockUpdateAndUnlock(NULL);

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
		//if (Args->guest->checked_in) {
		//	spendTheDay(Args);
		//}
		//else {
		//	tryToCheckIn(Args);
		//}


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

