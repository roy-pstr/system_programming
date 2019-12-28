/* Description:
	guest_thread module handles all the guest thread methods involving check in and out from a room.
*/
#include <stdio.h>
#include <string.h>
#include "guest_thread.h"
#include "file_handler.h"
#include "thread_handler.h"

/* global semaphore */
extern int guests_per_day_count;
extern HANDLE end_day_lock;

int ReleaseEndDayLock() {
	DWORD rel_code;
	LONG previous_count;
	/*release semaphore */
	rel_code = ReleaseSemaphore(end_day_lock, 1, &previous_count);
	if (rel_code == FALSE) {
		printf("released semaphore failed with %d\n", GetLastError());
		return SEMAPHORE_RELEASE_FAILED;
	}
	return SUCCESS;
}

/* lock mutex of room so only one guest will try to update room vacancy at a time
	update the vacancy if there is place
	release the mutex*/
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
	//printf("(from thread %d): inside mutex\n", GetCurrentThreadId());

	/* ........Critical Section Start................ */
	if (room->vacancy_counter > 0)
	{
		room->vacancy_counter--;
		*checked_in = true;
		//printf("room %s (%p): %d : %d\n", room->name, room, room->vacancy_counter, room->capacity);
	}
	/* ........Critical Section End.................. */

	/*release mutex*/
	DWORD rel_code;
	if (FALSE == (rel_code = ReleaseMutex(room->room_mutex))) {
		printf("released mutex failed with %d\n", GetLastError());
		return (MUTEX_RELEASE_FAILED);
	}
	//printf("(from thread %d): released mutex\n", GetCurrentThreadId());
	return (SUCCESS);
}

/* update guest budget */
void spendTheDay(guest_params_t *Args) {
	Args->guest->budget -= Args->guests_room->price;
	Args->days_guest_in_room++;
}

/* if there is space in room guest will check in*/
int tryToCheckIn(guest_params_t *Args) {
	BOOL ret_val;
	ret_val = LockUpdateRoomUnlock(Args->guests_room, &Args->checked_in);
	GO_TO_EXIT_ON_FAILURE(ret_val, "LockUpdateRoomUnlock failed!");
EXIT:
	return ret_val;
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

/* Guest Thread:
	handles guest - each day a guest will try to check in if his not already
	then a guest will spend the day which is updating the budget
	when the guests budget is finished the thread will exit
	it updates the guest_pre_day counter so the day thread will know when all guests 
	went through a day. untill it happens the thread is in wait for the mutex to be released.
	*/
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
	while (Args->guest->budget != 0)
	{
		/*	guest is still in the hotel,
			looking for a room
			or for spending the day */

		Sleep(SLEEP_TIME);

		/* Args->guest->start_day_sema is a semaphore works as mutex 
			it's role is to make sure each guest procceds only one day.*/
		ret_val = waitForDayStart(Args->start_day_sema);
		GO_TO_EXIT_ON_FAILURE(ret_val, "waitForDayStart failed!");

		/* let guest spend the day or if not in a room - 
			try to check in a room if available */
		if (Args->checked_in) {
			spendTheDay(Args);
		}
		else {
			ret_val = tryToCheckIn(Args);
			GO_TO_EXIT_ON_FAILURE(ret_val, "tryToCheckIn failed!");
			if (Args->checked_in) {
				spendTheDay(Args);
			}
		}

		/* global counter of guests that started a new day.*/
		guests_per_day_count--;

		if (guests_per_day_count == 0) {
			ReleaseEndDayLock();
		}
	}
EXIT:
	return ret_val;
}

/* need to update this function */
Room_t * RoomToGuest(Guest_t *guests_arr, Room_t *room_arr, int num_of_guests, int num_of_rooms) {
	int i,j;
	for (i = 0 ; i < num_of_guests ; i++)
	{
		for (j = 0; j < num_of_rooms; j++)
		{
			if (((!(guests_arr->budget % room_arr->price)) && (guests_arr->budget >= room_arr->price))) {
				return room_arr;
			}
			room_arr++;
		}
		guests_arr++;
	}
}

int InitGuestThreadParams(guest_params_t *p_thread_params, Guest_t *guests_arr, int num_of_guests, int num_of_rooms, Room_t *room_arr, FILE *fp) {
	int ret_val = SUCCESS;
	for (int i = 0; i < num_of_guests; i++)
	{
		p_thread_params->guest = guests_arr;
		p_thread_params->guest->initail_budget = p_thread_params->guest->budget;
		p_thread_params->guests_room = RoomToGuest(guests_arr, room_arr, num_of_guests, num_of_rooms);
		p_thread_params->guests_room->vacancy_counter = p_thread_params->guests_room->capacity;
		p_thread_params->checked_in = false;
		p_thread_params->days_guest_in_room = 0;
		p_thread_params->checked_out = false;
		p_thread_params->fp = fp;
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
