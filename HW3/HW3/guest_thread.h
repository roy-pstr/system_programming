#ifndef GUEST_THREAD_H
#define GUEST_THREAD_H
#include <windows.h>
#include "defines.h"

typedef struct {
	char name[MAX_NAME_LEN];
	int budget;
	Room_t *guests_room;
	HANDLE next_day_mutex;
	bool checked_in;
	bool checked_out;
} Guest_t;

typedef struct {
	Guest_t *guest;
	Room_t *room;
}guest_params_t;

DWORD WINAPI GuestThread(LPVOID lpParam);

#endif