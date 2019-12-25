#include <stdio.h>
#include <string.h>
#include "guest_thread.h"
#include "file_handler.h"

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
	while (Args->guest->budget != 0)
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
		//printf("guest %s -> started a new day\n", Args->guest->name);

		/* let guest spend the day or if not in a room - 
			try to check in a room if available */
		if (Args->checked_in) {
			spendTheDay(Args);
		}
		else {
			tryToCheckIn(Args);
			if (Args->checked_in) {
				printf("guest %s is checked in room: %s\n", Args->guest->name, Args->guests_room->name);
				//WriteLog(Args->guests_room, Args->guest->name, "IN", day_count, Args->fp);
				spendTheDay(Args);
			}
		}

		/* global counter of guests that started a new day.*/
		guests_per_day_count--;

		if (guests_per_day_count == 0) {
			ReleaseEndDayLock();
		}
	}
	return SUCCESS;
}

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

int InitGuestThreadParams(guest_params_t *p_thread_params, Guest_t *guests_arr, int num_of_guests, int num_of_rooms, Room_t *room_arr) {
	for (int i = 0; i < num_of_guests; i++)
	{
		p_thread_params->guest = guests_arr;
		p_thread_params->guest->initail_budget = p_thread_params->guest->budget;
		p_thread_params->guests_room = RoomToGuest(guests_arr, room_arr, num_of_guests, num_of_rooms);
		p_thread_params->guests_room->vacancy_counter = p_thread_params->guests_room->capacity;
		p_thread_params->guests_room->room_mutex = CreateMutexSimple();
		p_thread_params->checked_in = false;
		p_thread_params->checked_out = false;
		p_thread_params->start_day_sema = CreateSemaphoreSimple(0,1);
		p_thread_params++;
		guests_arr++;
	}
}

