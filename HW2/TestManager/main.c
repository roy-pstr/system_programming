/*Description - The program creates multiple processes for each ID of a student, each process is TestGrades.exe
				After the processes end, final_grades.txt is written with all students grades.
*/
#include <stdio.h>
#include "manager_handler.h"
int main(int argc, char *argv[]) 
{
	//check argv:
	if (argc != 2)
	{
		printf("Illegal number of arguents! Try again\n");
		return ERROR_CODE;
	}
	// main process function for TestManager:
	MultipleProcessesCalling(argv[1]);
	return 0;
}