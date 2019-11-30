#include <stdio.h>
#include "manager_handler.h"
int main(int argc, char *argv[]) 
{
	// NEED TO CREATE A FUNCTION THAT CREATES THE CMD LINE AND SENDS IT TO A PROCESS (TestGrades.exe)

	//need to do argv[1] as grade directory and argv[1] + grades_id to send to process.

		//check argv:
	if (argc != 2)
	{
		printf("Illegal number of arguents! Try again\n");
		return -1;  // DEBUG ERROR_CODE
	}
	MultipleProcessesCalling(argv[1]);
	return 0;
}