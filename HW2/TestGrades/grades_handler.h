#pragma once
#define GRADE_MAX 101
#define NUM_OF_GRADES 13
#define NUM_OF_EXES 10
float AverageMaxEightEx(int *grades_arr);
float sumOfTwoMin(int *grades_arr, int arr_size);
int handleMoedAB(int A, int B);
int finalStudentGrade(float exes, int midterm, int moed);
void MainThreadFinalGradeHandle(char *path, int grades_arr[NUM_OF_GRADES], char *id_ptr);
void MergeStringsForStudentFinalGrade(char *target, char *first, char *second, char *third, char *forth);
