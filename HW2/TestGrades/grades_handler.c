#include "grades_handler.h"
#include "defines.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

float sumOfTwoMin(int *grades_arr, int arr_size)
{
	int i;
	float first, second;
	first = second = GRADE_MAX;
	for (i = 0; i < arr_size; i++)
	{
		/* If current element is smaller than first
		   then update both first and second */
		if (grades_arr[i] <= first)
		{
			second = first;
			first = grades_arr[i];
		}

		/* If arr[i] is in between first and second
		   then update second  */
		else if (grades_arr[i] <= second)
			second = grades_arr[i];
	}
	return first + second;
}
float AverageMaxEightEx(int *grades_arr)
{
	int i;
	float avg_grades = 0.0;
	int two_mins_sum = sumOfTwoMin(grades_arr, NUM_OF_EXES);
	for (i = 0; i < NUM_OF_EXES; i++)
	{
		avg_grades = avg_grades + (grades_arr[i]+0.0);
	}
	avg_grades = (avg_grades - two_mins_sum)/8.0;
	return avg_grades;
}
int handleMoedAB(int A, int B)
{
	int result;
	if (B > 0)
	{
		result = B;
	}
	else
	{
		result = A;
	}
	if (result < 60)
	{
		result = 0;
	}
	return result;
}// DEBUG THIS TO GET FINAL GRADE
int finalStudentGrade(float exes, int midterm, int moed)
{
	float tmp_midterm = 0.0;
	float tmp_moed = moed + 0.0;
	int result = 0;
	if (midterm >= 60)
	{
		tmp_midterm = midterm + 0.0;
	}
	result = ceil(0.2*exes + 0.2*tmp_midterm + 0.6*moed);
	return result;
}
void MainThreadFinalGradeHandle(char *path, int grades_arr[NUM_OF_GRADES], char *id_ptr)
{
	char *final_student_grade_file;
	FILE *fp_final;
	if (NULL == (final_student_grade_file = (char *)malloc(strlen(path) + ID_LENGTH + strlen(FINAL_STUDENT_FILE_NAME) +strlen(TXT) + 3)))  // student Ids directory name memory allocation
	{
	printf("Memory Allocation failed! Try again...");
	exit(ERROR_CODE);
	}
	MergeStringsForStudentFinalGrade(final_student_grade_file, path, FINAL_STUDENT_FILE_NAME, id_ptr, TXT);

	float exes = AverageMaxEightEx(grades_arr);
	int moeds = handleMoedAB(grades_arr[11], grades_arr[12]);
	int final_grade = finalStudentGrade(exes, grades_arr[10], moeds);

	if (NULL == (fp_final = fopen(final_student_grade_file, "w")))
	{
		printf("File ERROR\n");
		exit(ERROR_CODE);
	}
	fprintf(fp_final,"%d",final_grade);

	fclose(fp_final);
	free(final_student_grade_file);
}
void MergeStringsForStudentFinalGrade(char *target, char *first, char *second, char *third, char *forth)
{
	strcpy(target, first);
	strcat(target, second);
	strcat(target, third);
	strcat(target, forth);
}