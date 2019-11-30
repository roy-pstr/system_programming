#include "grades_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	result = (int)(ceil(0.2*exes + 0.2*tmp_midterm + 0.6*moed));

	return result;


}