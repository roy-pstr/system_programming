#include <stdio.h>
#include <string.h>
#include "defines.h"
#include "day_thread.h"

/* global semaphore */
extern int guests_per_day_count;
extern bool all_guests_checked_out;

int tryCheckOutGuests(guest_params_t *Args){
	int ret_val = SUCCESS;
	//printf("from tryCheckOutGuests: %s budget: %d\n", Args->guest->name, Args->guest->budget);
	if (Args->guest->budget == 0) {
		/* update number of available places in room after guest checked out */
		Args->guests_room->vacancy_counter++;
		/* update that the guest has checked out */
		Args->checked_out = true;
	}
	return ret_val;
}

int startNewDay(day_params_t *Args) {
	DWORD rel_code;
	LONG previous_count;
	for (int i = 0; i < Args->num_of_guests; i++)
	{
		tryCheckOutGuests(&Args->guests_params[i]);
		
		/*release semaphore of start day for each guest */
		rel_code = ReleaseSemaphore(Args->guests_params[i].start_day_sema, 1, &previous_count);
		if (rel_code == FALSE) {
			return SEMAPHORE_RELEASE_FAILED;
		}

		if (Args->guests_params[i].checked_out) { continue; }
		guests_per_day_count++;
	}
	return SUCCESS;
}

bool GuestsInHotel(day_params_t *Args) {
	for (int i = 0; i < Args->num_of_guests; i++)
	{
		if (Args->guests_params[i].checked_out==false) { return true; }
	}
	return false;
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
	printf("day %d started.\n", day_counter);
	while (true)
	{
		Sleep(SLEEP_TIME);
		if (all_guests_checked_out == false && guests_per_day_count == 0) {
			printf("day %d ended.\n", day_counter);
			printf("day %d started.\n", day_counter);
			if (SUCCESS != (ret_val = startNewDay(Args))) {
				printf("startNewDay in DayThread faield.\n");
				return ret_val;
			}
		}
		if (all_guests_checked_out == true) {
			break;
		}
		day_counter++;
	}
	printf("day thread exit\n");
	return SUCCESS;
}



