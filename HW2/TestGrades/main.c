#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "defines.h"
#include "file_handler.h"
#include "thread_handler.h"
#include "grades_handler.h"

int main(int argc, char *argv[]) 
{
	//check argv:
	if (argc != 2)
	{
		printf("Illegal number of arguents! Try again\n");
		return -1;  // DEBUG ERROR_CODE
	}

	//define arrays:
	int GradesArray[NUM_OF_GRADES];
	const char GradesTypesArray[NUM_OF_GRADES][MAX_FILENMAE_LENGTH] = {"ex01.txt", "ex02.txt", 
																	   "ex03.txt", "ex04.txt", 
																	   "ex05.txt", "ex06.txt", 
																	   "ex07.txt", "ex08.txt", 
																	   "ex09.txt", "ex10.txt", 
																	   "midterm.txt", "moedA.txt", "moedB.txt"}; // DEBUG GLOBAL
	const char file_paths_arr[NUM_OF_GRADES][MAX_FILENMAE_LENGTH] = { "C:\\Users\\roypa\\OneDrive\\Documents\\GitHub\\system_programming\\HW2\\students_gardes\\grades_204219273\\ex01.txt",
		"C:\\Users\\roypa\\OneDrive\\Documents\\GitHub\\system_programming\\HW2\\students_gardes\\grades_204219273\\ex02.txt" };
	//int status = RunMultiplayThreads(&GradesArray[0], file_paths_arr);
	//return 0; 
	//int status = GetAllGrades(&GradesArray[0], NUM_THREADS, file_paths_arr);
	
	ReadGradeFromFile(argv[1], &GradesArray[0]);
	printf("Print from main: %d\n", GradesArray[0]);
	int check[13] = { 10, 90, 30, 40, 50, 60, 70, 80, 90, 100, 70, 60 };
	float exes = AverageMaxEightEx(check);
	printf("Final Ex Grade = %f \n", exes);
	int moeds = handleMoedAB(check[11],check[12]);
	printf("Final MOED Grade = %d \n", moeds);
	float final_grade = finalStudentGrade(exes, check[10], moeds);
	printf("Final Grade = %f \n", final_grade);

	

	return 0;
	
}