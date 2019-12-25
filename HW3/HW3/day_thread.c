#include <stdio.h>
#include <string.h>
#include "defines.h"
#include "day_thread.h"
#include "file_handler.h"

/* global semaphore */
extern int guests_per_day_count;
extern bool all_guests_checked_out;
extern HANDLE end_day_lock;

int WaitEndDayLock() {
	/* wait for room mutex to be available */
	DWORD wait_code = WaitForSingleObject(end_day_lock, SEMAPHORE_TIMEOUT_IN_MILLISECONDS);
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

int isCheckedInToday(guest_params_t *guest_params) {
	return (guest_params->checked_in == true &&
		(guest_params->guest->initail_budget - guest_params->guest->budget) == guest_params->guests_room->price);
}

int WriteCheckInLog(day_params_t *Args, int day_count) {
	int ret_val = SUCCESS;
	for (int i = 0; i < Args->num_of_guests; i++)
	{
		if (isCheckedInToday(&Args->guests_params[i])) {
			WriteLog(Args->guests_params[i].guests_room, Args->guests_params[i].guest->name, "IN", day_count, Args->fp);
		}
	}
	return ret_val;
}

int tryCheckOutGuests(day_params_t *Args, int day_count){
	int ret_val = SUCCESS;
	for (int i = 0; i < Args->num_of_guests; i++)
	{
		if (Args->guests_params[i].guest->budget == 0 && Args->guests_params[i].checked_out == false) {
			/* update number of available places in room after guest checked out */
			Args->guests_params[i].guests_room->vacancy_counter++;
			/* update that the guest has checked out */
			Args->guests_params[i].checked_out = true;
			//printf("guest %s checked out.\n", Args->guests_params[i].guest->name);
			WriteLog(Args->guests_params[i].guests_room, Args->guests_params[i].guest->name, "OUT", day_count, Args->fp);
		}
	}
	return ret_val;
}

int HowManyGuestsInHotel(day_params_t *Args) {
	int ret_val = SUCCESS;
	for (int i = 0; i < Args->num_of_guests; i++)
	{
		if (!Args->guests_params[i].checked_out) {
			guests_per_day_count++;
		}
	}
	return ret_val;
}

int ReleaseAllGuests(day_params_t *Args) {
	DWORD rel_code;
	LONG previous_count;
	for (int i = 0; i < Args->num_of_guests; i++)
	{
		if (Args->guests_params[i].checked_out) { continue; }
		/*release semaphore of start day for each guest */
		rel_code = ReleaseSemaphore(Args->guests_params[i].start_day_sema, 1, &previous_count);
		if (rel_code == FALSE) {
			return SEMAPHORE_RELEASE_FAILED;
		}
	}
	return SUCCESS;
}

bool areAllGuestsCheckedOut(day_params_t *Args) {
	for (int i = 0; i < Args->num_of_guests; i++)
	{
		if (Args->guests_params[i].checked_out==false) { return false; }
	}
	return true;
}

int StartDay(day_params_t *Args, int day_count) {
	//printf("day %d started.\n", day_count);
	HowManyGuestsInHotel(Args);
	ReleaseAllGuests(Args);
	return SUCCESS;
}

int EndDay(day_params_t *Args, int *day_count) {
	//printf("day %d ended.\n", *day_count);
	WriteCheckInLog(Args, *day_count);
	(*day_count)++;
	tryCheckOutGuests(Args, *day_count);
	return SUCCESS;
}

DWORD WINAPI DayThread(LPVOID lpParam)
{
	/* Handle arguments passed */
	day_params_t *Args;
	if (NULL == lpParam) {
		printf("error with arguments passed to thread\n");
		return GUEST_PARAMS_CASTING_FAILED;
	}
	Args = (day_params_t*)lpParam;

	int ret_val = SUCCESS;
	int day_counter = 1;
	while (all_guests_checked_out == false) {

		Sleep(SLEEP_TIME);

		/* start day */
		StartDay(Args, day_counter);

		/* wait until day ended by guest*/
		WaitEndDayLock(); 

		/* end day */
		EndDay(Args, &day_counter);
		
		/* end condition */
		all_guests_checked_out = areAllGuestsCheckedOut(Args);
	}

	printf("Total days: %d\n", day_counter);
	return SUCCESS;
}



