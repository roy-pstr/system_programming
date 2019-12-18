#include <stdbool.h>
#include "defines.h"
#include "file_handler.h"

int main(int argc, char *argv[]) {
	Room_t rooms_arr[MAX_ROOMS];
	Guest_t guests_arr[MAX_GUESTS];
	//check argv:
	if (argc != 2)
	{
		printf("Illegal number of arguents! Try again\n");
		return ERROR_CODE;
	}
	HandleRoomsFile(argv[1],&rooms_arr);
	HandleNamesFile(argv[1],&guests_arr);
	int day_counter = 1;
	


	printf("day_counter: %d", day_counter);
	return 0;
}

