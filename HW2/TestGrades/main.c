#include "get_grade_thread.h"
#include <stdbool.h>
#define ERROR_CODE ((int)(-1))
#define NUM_THREADS 2

#include "file_handler.h"
#include "grades_handler.h"
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[]) 
{
	//check inputs
	if (argc != 2)
	{
		printf("Illegal number of arguents! Try again\n");
		return -1;  // DEBUG ERROR_CODE
	}
	int GradesArray[NUM_OF_GRADES];
	const char GradesTypesArray[NUM_OF_GRADES][MAX_FILENMAE_LENGTH] = {"ex01.txt", "ex02.txt", 
																	   "ex03.txt", "ex04.txt", 
																	   "ex05.txt", "ex06.txt", 
																	   "ex07.txt", "ex08.txt", 
																	   "ex09.txt", "ex10.txt", 
																	   "midterm.txt", "moedA.txt", "moedB.txt"}; // DEBUG GLOBAL
	const char file_paths_arr[NUM_OF_GRADES][MAX_FILENMAE_LENGTH] = { "C:\Users\roypa\OneDrive\Documents\GitHub\system_programming\HW2\students_gardes\grades_204219273\ex01.txt",
																		"C:\Users\roypa\OneDrive\Documents\GitHub\system_programming\HW2\students_gardes\grades_204219273\ex02.txt"};
	GetAllGrades(&GradesArray[0], NUM_THREADS, &file_paths_arr);
	return 0;
	WritefileToGradesArray(argv[1], &GradesArray[0], GradesTypesArray[0]);
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