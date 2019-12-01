#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <string.h>
#include "defines.h"
#include "file_handler.h"
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
	printf("TestGrades.exe\n");

	//check argv:
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
	int i, status=0;
	char *id = argv[1] + strlen(argv[1]) - ID_LENGTH + 1 ; // Parsing id from argument
	for (i = 0; i < NUM_THREADS; i++) {
		InitGetGradeParams(&thread_params_arr[i], &grades_arr[i], argv[1], file_names_arr[i]);
	}

	/* run mulitplay threads */
	if (ERROR_CODE == RunMultiplayThreads(NUM_THREADS, thread_handles_arr, thread_ids_arr, thread_params_arr)) {
		printf("Error when trying to run multiplay threads.\n");
		return ERROR_CODE;
	}
	MainThreadFinalGradeHandle(argv[1], grades_arr, id);
	
		
 /*float exes = AverageMaxEightEx(grades_arr);
	printf("Final Ex Grade = %f \n", exes);
	int moeds = handleMoedAB(grades_arr[11], grades_arr[12]);
	printf("Final MOED Grade = %d \n", moeds);
	int final_grade = finalStudentGrade(exes, grades_arr[10], moeds);
	printf("Final Grade = %d \n", final_grade);
	return 0;*/
}