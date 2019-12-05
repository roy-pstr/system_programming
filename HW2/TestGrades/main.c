//Authors – Roy Pasternak(204219273) & Ilay Amar(308520857)
//Project – TestGrades.exe
//Description – The program read gardes from all the files in the given folder (argv[1])
//				Then it calculates the final grade of the student and write it to 'final_ID.txt'
//				where ID is the students ID.


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <string.h>
#include "defines.h"
#include "thread_handler.h"
#include "grades_handler.h"

/* file names */
const char file_names_arr[NUM_OF_GRADES][MAX_FILE_NAME] = { "\\ex01.txt", "\\ex02.txt", "\\ex03.txt", "\\ex04.txt",
																	   "\\ex05.txt", "\\ex06.txt",
																	   "\\ex07.txt", "\\ex08.txt",
																	   "\\ex09.txt", "\\ex10.txt",
																	   "\\midterm.txt", "\\moedA.txt", "\\moedB.txt" };

int main(int argc, char *argv[]) 
{
	int grades_arr[NUM_OF_GRADES];

	/* check argv */
	if (argc != 2)
	{
		printf("Illegal number of arguents! Try again\n");
		return ERROR_CODE; 
	}

	/* Declare threads vairlibales */
	HANDLE thread_handles_arr[NUM_THREADS];
	DWORD thread_ids_arr[NUM_THREADS];
	GetGrade_params thread_params_arr[NUM_THREADS];

	/* Prepare parameters for thread */
	int i;
	for (i = 0; i < NUM_THREADS; i++) {
		InitGetGradeParams(&thread_params_arr[i], &grades_arr[i], argv[1], file_names_arr[i]);
	}
	
	/* parse id number out of argv[1] */
	char *id = argv[1] + strlen(argv[1]) - ID_LENGTH + 1;

	/* run mulitplay threads - each read one grade from file */
	if (ERROR_CODE == RunMultiplayThreads(NUM_THREADS, thread_handles_arr, thread_ids_arr, thread_params_arr)) {
		printf("Error when trying to run multiplay threads.\n");
		return ERROR_CODE;
	}

	/* calculate final grade */
	int ret_val = MainThreadFinalGradeHandle(argv[1], grades_arr, id);
	
	return ret_val;
}