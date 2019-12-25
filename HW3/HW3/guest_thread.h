#ifndef GUEST_THREAD_H
#define GUEST_THREAD_H
#include <windows.h>
#include "defines.h"

typedef struct {
	char name[MAX_NAME_LEN];
	int budget;	
} Guest_t;

typedef struct {
	Guest_t *guest;
	Room_t *guests_room;
	HANDLE start_day_sema;
	bool checked_in;
	bool checked_out;
}guest_params_t;

DWORD WINAPI GuestThread(LPVOID lpParam);

int InitGuestThreadParams(guest_params_t *p_thread_params, Guest_t *guests_arr, int num_of_guests, Room_t *room_arr);

#endif