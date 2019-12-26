/* Description:
	day_thread module handles all the day thread methods involving start new day and end a day.
*/
#include <stdio.h>
#include <string.h>
#include "defines.h"
#include "day_thread.h"
#include "file_handler.h"

/* global semaphore */
extern int guests_per_day_count;
extern bool all_guests_checked_out;
extern HANDLE end_day_lock;

/* calling wait on the day lock mutex - this mutex is being released by the last
	guest that finish a day run. */
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

/* check wheater specific guest checked in today */
bool isCheckedInToday(guest_params_t *guest_params) {
	return (guest_params->checked_in == true &&
		(guest_params->guest->initail_budget - guest_params->guest->budget) == guest_params->guests_room->price);
}

/* Write the check in log for all guests */
int WriteCheckInLog(day_params_t *Args, int day_count) {
	int ret_val = SUCCESS;
	for (int i = 0; i < Args->num_of_guests; i++)
	{
		if (isCheckedInToday(&Args->guests_params[i])==true) {
			ret_val = WriteLog(Args->guests_params[i].guests_room->name, Args->guests_params[i].guest->name, "IN", day_count, Args->fp);
			GO_TO_EXIT_ON_FAILURE(ret_val, "WriteLog failed!");
		}
	}
EXIT:
	return ret_val;
}

/* At the end of every day this function check for every guest if his
	budget ran it, if so it handle the check out of this guest. */
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
			ret_val = WriteLog(Args->guests_params[i].guests_room->name, Args->guests_params[i].guest->name, "OUT", day_count, Args->fp);
			GO_TO_EXIT_ON_FAILURE(ret_val, "WriteLog failed!");
		}
	}
EXIT:
	return ret_val;
}

/* returns a number of guest that havent check out yet. */
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

/* this function release all the guests mutex so they all
	start new day. */
int ReleaseAllGuests(day_params_t *Args) {
	DWORD rel_code;
	LONG previous_count;
	for (int i = 0; i < Args->num_of_guests; i++)
	{
		if (Args->guests_params[i].checked_out) { continue; }
		/*release semaphore of start day for each guest */
		rel_code = ReleaseSemaphore(Args->guests_params[i].start_day_sema, 1, &previous_count);
		if (rel_code == FALSE) {
			printf("ReleaseSemaphore has failed!\n");
			return SEMAPHORE_RELEASE_FAILED;
		}
	}
	return SUCCESS;
}

/* check if all guests checked out and we can exit the thread */
bool areAllGuestsCheckedOut(day_params_t *Args) {
	for (int i = 0; i < Args->num_of_guests; i++)
	{
		if (Args->guests_params[i].checked_out==false) { return false; }
	}
	return true;
}

/* start new day:
	-check how many guests in the hotel today
	-start the day for all guests */
int StartDay(day_params_t *Args, int day_count) {
	//printf("day %d started.\n", day_count);
	int ret_val = SUCCESS;
	ret_val = HowManyGuestsInHotel(Args);
	GO_TO_EXIT_ON_FAILURE(ret_val, "HowManyGuestsInHotel failed!");
	ret_val = ReleaseAllGuests(Args);
	GO_TO_EXIT_ON_FAILURE(ret_val, "ReleaseAllGuests failed!");
EXIT:
	return ret_val;
}

/* end the current day:
	-write log for all guests that checked in today.
	-procced the day counter by one
	-do checkout for the guests all the guests that finished their budget */
int EndDay(day_params_t *Args, int *day_count) {
	//printf("day %d ended.\n", *day_count);
	int ret_val = SUCCESS;
	ret_val = WriteCheckInLog(Args, *day_count);
	GO_TO_EXIT_ON_FAILURE(ret_val, "WriteCheckInLog failed!");
	(*day_count)++;
	ret_val = tryCheckOutGuests(Args, *day_count);
	GO_TO_EXIT_ON_FAILURE(ret_val, "tryCheckOutGuests failed!");
	EXIT:
	return ret_val;
}

/* DayThread defenition:
	In charge of manage the day progress.
	As the thread starts a new day is starting, then the thread will wait for all the 
	guest threads to finish their first day, then it will procced to 'EndDay' which 
	do check out for guests whom finished their budget and print to LOG file
	check in and check out that happend in this day.
	Then all over again.
	The DayThread will finish its running when all guests have checked out.*/
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
		ret_val = StartDay(Args, day_counter);
		GO_TO_EXIT_ON_FAILURE(ret_val, "StartDay failed!");

		/* wait until day ended by guest*/
		ret_val = WaitEndDayLock();
		GO_TO_EXIT_ON_FAILURE(ret_val, "WaitEndDayLock failed!");

		/* end day */
		ret_val = EndDay(Args, &day_counter);
		GO_TO_EXIT_ON_FAILURE(ret_val, "EndDay failed!");

		/* end condition */
		all_guests_checked_out = areAllGuestsCheckedOut(Args);
	}
	printf("Total days: %d\n", day_counter);
EXIT:
	return ret_val;
}



