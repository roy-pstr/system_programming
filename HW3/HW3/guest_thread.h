#ifndef GUEST_THREAD_H
#define GUEST_THREAD_H
#include <windows.h>
#include "defines.h"

typedef struct {
	int a;
	int b;
	int sleep;
}guest_params_t;

DWORD WINAPI GuestThread(LPVOID lpParam);

#endif