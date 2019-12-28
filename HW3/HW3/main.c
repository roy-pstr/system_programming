//Authors – Roy Pasternak(204219273) & Ilay Amar(308520857)
//Project – HW3.exe
//Description – The program manage an hotel. it reads list of rooms and guests and hanle
//				check in and check out for every room and guest.
//				it prints the number of days until all guests checked out.

#include <stdbool.h>
#include <stdio.h>
#include "defines.h"
#include "file_handler.h"
#include "guest_thread.h"
#include "thread_handler.h"
#include "threads_runner.h"
#include "day_thread.h"

/* global semaphore */
int guests_per_day_count = 0;
bool all_guests_checked_out = false;
HANDLE end_day_lock;
HANDLE guests_per_day_count_lock;
HANDLE day_handle;

int main(int argc, char *argv[]) {

	//check argv:
	if (argc != 2)
	{
		printf("Illegal number of arguents! Try again\n");
		return ERROR_CODE;
	}

	int ret_val = SUCCESS;
	
	day_params_t day_params;
	Room_t rooms_arr[MAX_ROOMS];
	Guest_t guests_arr[MAX_GUESTS];
	int num_of_rooms = 0;

	/* open log file for writing */
	ret_val = OpenLogFile(&day_params.fp, argv[1]);
	GO_TO_EXIT_ON_FAILURE(ret_val, "OpenLogFile failed!");
	
	/* load rooms and guests from .txt files: */
	ret_val = LoadRoomList(argv[1], &num_of_rooms, rooms_arr);
	GO_TO_EXIT_ON_FAILURE(ret_val,"LoadRoomList failed!");

	ret_val = LoadGuestList(argv[1], &day_params.num_of_guests, guests_arr);
	GO_TO_EXIT_ON_FAILURE(ret_val, "LoadGuestList failed!");

	/* initailize parameters for theards */
	guest_params_t p_thread_params[MAX_GUESTS];
	ret_val = InitGuestAndDayThreadParams(&day_params, p_thread_params, guests_arr,  num_of_rooms, rooms_arr);
	GO_TO_EXIT_ON_FAILURE(ret_val, "InitGuestThreadParams failed!");

	/* guests threads vars */
	HANDLE p_thread_handles[MAX_GUESTS];
	DWORD p_thread_ids[MAX_GUESTS];
	ret_val = RunGuestsAndDayThreads(day_params.num_of_guests, p_thread_handles, p_thread_ids, p_thread_params, &day_params);
	GO_TO_EXIT_ON_FAILURE(ret_val, "RunGuestsThreads failed!");

EXIT:
	/* close the room log file */
	if (NULL != day_params.fp) {
		fclose(day_params.fp);
	}
	return ret_val;

}

