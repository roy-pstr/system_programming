#include <windows.h>
#include "guest_thread.h"
#include "day_thread.h"
int RunGuestsAndDayThreads(int number_of_threads, HANDLE *p_thread_handles, DWORD *p_thread_ids, guest_params_t *p_thread_params, day_params_t *day_params);

int InitGuestAndDayThreadParams(day_params_t * day_params, guest_params_t * p_thread_params, Guest_t * guests_arr, int num_of_rooms, Room_t * room_arr);
