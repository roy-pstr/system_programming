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

int main(int argc, char *argv[]) {

	//check argv:
	if (argc != 2)
	{
		printf("Illegal number of arguents! Try again\n");
		return ERROR_CODE;
	}

	HANDLE p_thread_handles[MAX_GUESTS];
	DWORD p_thread_ids[MAX_GUESTS];
	guest_params_t p_thread_params[MAX_GUESTS];
	Room_t rooms_arr[MAX_ROOMS];
	Guest_t guests_arr[MAX_GUESTS];
	HANDLE day_handle;
	DWORD day_thread_id;
	day_params_t day_params;
	int num_of_guests = 0;
	int num_of_rooms = 0;
	int ret_val = SUCCESS;

	/* load rooms and guests from .txt files: */
	LoadRoomList(argv[1], &num_of_rooms, rooms_arr);
	LoadGuestList(argv[1], &num_of_guests, guests_arr);


	/* initailize parameters for theards */
	InitGuestThreadParams(p_thread_params, guests_arr, num_of_guests, num_of_rooms, rooms_arr);
	
	/* initailize counter */
	guests_per_day_count = num_of_guests;

	if (SUCCESS != (ret_val = RunGuestsThreads(1, p_thread_handles, p_thread_ids, p_thread_params))) {
		printf("RunGuestsThreads failed.");
		return ret_val;
	}
	return ret_val;
}

