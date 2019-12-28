#ifndef GUEST_THREAD_H
#define GUEST_THREAD_H
#include <windows.h>
#include "defines.h"

typedef struct {
	char name[MAX_NAME_LEN];
	int budget;	
	int initail_budget;
} Guest_t;

typedef struct {
	Guest_t *guest;
	Room_t *guests_room;
	Room_t *all_rooms;
	HANDLE start_day_sema;
	bool checked_in;
	bool checked_out;
	int num_of_rooms;
	int num_of_guests;

	FILE *fp;
}guest_params_t;

DWORD WINAPI GuestThread(LPVOID lpParam);
Room_t *RoomToGuest(Guest_t *guests_arr, Room_t *room_arr, int num_of_guests, int num_of_rooms);
int InitGuestThreadParams(guest_params_t * p_thread_params, Guest_t * guests_arr, int num_of_guests, int num_of_rooms, Room_t * room_arr, FILE * fp);
//Room_t *GuestToRoom(Guest_t *guest, Room_t *room_arr, int num_of_rooms);
//void GuestToRoom(guest_params_t *p_thread_params, Guest_t *guest, Room_t *room_arr, int num_of_rooms);
Room_t *GuestToRoom(guest_params_t *guest_t);
#endif