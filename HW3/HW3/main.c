#include <stdbool.h>
#include <stdio.h>
#include "defines.h"
#include "file_handler.h"
#include "guest_thread.h"
#include "thread_handler.h"
#include "day_thread.h"

/* global semaphore */
HANDLE guest_per_day_count;
int guests_per_day_count = 0;
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

	int guests_number = 0;
	LoadRoomList(argv[1],rooms_arr);
	LoadGuestList(argv[1],&guests_number, guests_arr);
	//RoomToGuest(&rooms_arr[0], &p_thread_params[0]);
	guest_per_day_count = CreateSemaphoreSimple(MAX_GUESTS, MAX_GUESTS);

	InitGuestThreadParams(p_thread_params, guests_arr, guests_number);
	guests_per_day_count = guests_number;
	RunThreadsForAllGuests(guests_number, p_thread_handles, p_thread_ids, p_thread_params);


	//printf("day_counter: %d", day_counter);
	return 0;
}

