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
	Room_t *all_rooms;
	HANDLE start_day_sema;
	bool checked_in;
	int days_guest_in_room;
	bool checked_out;
	int num_of_rooms;
	int num_of_guests;

	FILE *fp;
} guest_params_t;

DWORD WINAPI GuestThread(LPVOID lpParam);
Room_t *GuestToRoom(guest_params_t *guest_t);



#endif