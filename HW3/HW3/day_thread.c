#include <stdio.h>
#include <string.h>
#include "defines.h"
#include "day_thread.h"

/* global semaphore */
extern int guests_per_day_count;
extern bool all_guests_checked_out;

int startNewDay(day_params_t *Args) {
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
		guests_per_day_count++;
	}
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
	printf("day started.\n");
	while (all_guests_checked_out == false)
	{
		if (guests_per_day_count == 0) {
			printf("day ended.\n");
			printf("day started.\n");
			if (SUCCESS != (ret_val = startNewDay(Args))) {
				printf("startNewDay in DayThread faield.\n");
				return ret_val;
			}
		}
	}
	printf("day thread exit\n");
	return SUCCESS;
}



