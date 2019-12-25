#include <stdbool.h>
#include <stdio.h>
#include "defines.h"
#include "file_handler.h"
#include "guest_thread.h"
#include "thread_handler.h"
#include "day_thread.h"

/* global semaphore */
int guests_per_day_count = 0;
bool all_guests_checked_out = false;
HANDLE end_day_lock;

int main(int argc, char *argv[]) {

	//check argv:
	if (argc != 2)
	{
		printf("Illegal number of arguents! Try again\n");
		return ERROR_CODE;
	}

	int ret_val = SUCCESS;
	
	/* load rooms and guests from .txt files: */
	day_params_t day_params;
	Room_t rooms_arr[MAX_ROOMS];
	Guest_t guests_arr[MAX_GUESTS];
	int num_of_rooms = 0;
	LoadRoomList(argv[1], &num_of_rooms, rooms_arr);
	LoadGuestList(argv[1], &day_params.num_of_guests, guests_arr);

	/* open log file for writing */
	OpenLogFile(&day_params.fp, argv[1]);

	/* initailize parameters for theards */
	guest_params_t p_thread_params[MAX_GUESTS];
	InitGuestThreadParams(p_thread_params, guests_arr, day_params.num_of_guests, num_of_rooms, rooms_arr);
	day_params.guests_params = p_thread_params;

	/* guests threads vars */
	HANDLE p_thread_handles[MAX_GUESTS];
	DWORD p_thread_ids[MAX_GUESTS];
	if (SUCCESS != (ret_val = RunGuestsThreads(day_params.num_of_guests, p_thread_handles, p_thread_ids, p_thread_params, day_params))) {
		printf("RunGuestsThreads failed.");
		goto EXIT;
	}

EXIT:
	if (NULL != day_params.fp) {
		fclose(day_params.fp);
	}
	return ret_val;
}

