#ifndef _GARDES_HANDLER_H
#define _GARDES_HANDLER_H
#include "defines.h"

int ReadGradeFromFile(const char *filepath, int *grade_ptr);
float AverageMaxEightEx(int *grades_arr);
float sumOfTwoMin(int *grades_arr, int arr_size);
int handleMoedAB(int A, int B);
int finalStudentGrade(float exes, int midterm, int moed);
int MainThreadFinalGradeHandle(char *path, int grades_arr[NUM_OF_GRADES], char *id_ptr);
void MergeStringsForStudentFinalGrade(char *target, char *first, char *second, char *third, char *forth);
#endif