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
extern HANDLE guests_per_day_count_lock;
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
	return (SUCCESS);
}

/* lock -> update global counter -> release lock */
DWORD LockUpdateGlobalCounterUnlock()
{
	/* wait for mutex to be available */
	DWORD wait_code = WaitForSingleObject(guests_per_day_count_lock, SEMAPHORE_TIMEOUT_IN_MILLISECONDS);
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

	/* ........Critical Section Start................ */
	guests_per_day_count--;
	/* ........Critical Section End.................. */

	LONG previous_count;
	DWORD rel_code;
	if (FALSE == (rel_code = ReleaseSemaphore(guests_per_day_count_lock, 1, &previous_count))) {
		printf("ReleaseSemaphore has failed!\n");
		return SEMAPHORE_RELEASE_FAILED;
	}
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

	/* find the room for the guest */
	Args->guests_room = GuestToRoom(Args); 

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
		LockUpdateGlobalCounterUnlock();
		//guests_per_day_count--;

		if (guests_per_day_count == 0) {
			ReleaseEndDayLock();
		}
	}
EXIT:
	return ret_val;
}

Room_t *GuestToRoom(guest_params_t *guest_t) {
	int i;
	for (i = 0; i < guest_t->num_of_rooms; i++)
	{
		if (((!(guest_t->guest->budget % guest_t->all_rooms[i].price)) && (guest_t->guest->budget >= guest_t->all_rooms[i].price))) {
			guest_t->all_rooms[i].vacancy_counter = guest_t->all_rooms[i].capacity;
			return &guest_t->all_rooms[i];
		}
	}
	return NULL;
}
