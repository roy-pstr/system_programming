/* Description:
	grades_handler module handles reading gardes from files and calulcation of final grade
*/
#include "grades_handler.h"
#include "defines.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*ReadGradeFromFile
	Input:
		const char *filepath	- file path to .txt file
		int *grade_ptr			- grade pointer

	The function open the .txt file specific in the Args given,
	Then it reads the grade written inside the file and write it to the grade pointer in args
*/
int ReadGradeFromFile(const char *filepath, int *grade_ptr)
{
	/* open file: */
	FILE *fp;
	if (NULL == (fp = fopen(filepath, "r")))
	{
		printf("File ERROR\n");
		exit(ERROR_CODE); 
	}
	/* read grade from file: */
	char gradeFromFile[MAX_GRADE_LENGTH];
	fgets(gradeFromFile, MAX_GRADE_LENGTH, fp);
	fclose(fp);

	/* convert to int and save in pointer value */
	*grade_ptr = (int)atol(gradeFromFile);
	if (*grade_ptr < 60)
	{
		*grade_ptr = 0;
	}
	return SUCCESS_CODE;
}

/*return the sum of the two min values in grades_arr*/
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

/*return the average value of the eight higher grades in grades_arr*/
float AverageMaxEightEx(int *grades_arr)
{
	int i;
	float avg_grades = 0.0;
	int two_mins_sum = sumOfTwoMin(grades_arr, NUM_OF_EXES);
	for (i = 0; i < NUM_OF_EXES; i++)
	{
		if (grades_arr[i] < 60)
		{
			continue;
		}
		else {
			avg_grades = avg_grades + (float)grades_arr[i];
		}
		
	}
	avg_grades = (avg_grades - two_mins_sum)/8.0;
	return avg_grades;
}
/*return the correct Moead grade*/
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
}
/* calc the fianl student grade according to the given instructions */
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
/* write the final grade calculted inro 'final_id.txt' file */
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
/*merge the given strings and write to target */
void MergeStringsForStudentFinalGrade(char *target, char *first, char *second, char *third, char *forth)
{
	strcpy(target, first);
	strcat(target, second);
	strcat(target, third);
	strcat(target, forth);
}