#ifndef _DAY_THREAD_H
#define _DAY_THREAD_H
#include <windows.h>
#include "guest_thread.h"

typedef struct {
	guest_params_t *guests_params;
	int num_of_guests;
	FILE *fp;
} day_params_t;

int WaitEndDayLock();

DWORD WINAPI DayThread(LPVOID lpParam);

#endif