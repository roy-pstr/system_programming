#include <stdbool.h>
#include "defines.h"


int main(int argc, char *argv[]) {
	
	//check argv:
	if (argc != 2)
	{
		printf("Illegal number of arguents! Try again\n");
		return ERROR_CODE;
	}

	int day_counter = 1;
	Room_t rooms_arr[MAX_ROOMS];
	Guest_t guests_arr[MAX_GUESTS];


	printf("day_counter: %d", day_counter);
	return 0;
}

