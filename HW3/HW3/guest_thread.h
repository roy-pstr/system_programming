#ifndef GUEST_THREAD_H
#define GUEST_THREAD_H
#include <windows.h>
#include "defines.h"

typedef struct {
	Guest_t *guest;
	Room_t *room;
}guest_params_t;

DWORD WINAPI GuestThread(LPVOID lpParam);

#endif