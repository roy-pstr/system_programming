#include <stdbool.h>
#include "defines.h"

typedef struct Room_st {
	char *name;
	int price;
	int capacity;
	bool occupied;
} Room;

typedef struct Guest_st {
	char *name;
	int budget;
	bool checked;
} Guest;


bool parseRooms(Room *rooms_arr, const char *file_path);

bool nextDay(int *counter);

int main(int argc, char *argv[]) {
	
	//check argv:
	if (argc != 2)
	{
		printf("Illegal number of arguents! Try again\n");
		return ERROR_CODE;
	}

	int day_counter = 1;
	Room rooms_arr[MAX_ROOMS];
	Guest guests_arr[MAX_GUESTS];


	printf("day_counter: %d", day_counter);
	return 0;
}
/*
day:
- checkout
- checkin
- each guest is a thread loo

*/