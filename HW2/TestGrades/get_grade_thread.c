#include "get_grade_thread.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define ERROR_CODE ((int)(-1))
#define SUCCESS_CODE ((int)(0))
#define MAX_STRING 50



void InitParams(get_grade_params *params,const char * str, int *ptr) {
	strcpy(params->file_path, str);
	params->grade = ptr;
}

DWORD WINAPI GetGrade(LPVOID lpParam)
{
	get_grade_params *Args;
	if (NULL == lpParam) {
		printf("error with arguments passed to thread GetGrade");
		exit(ERROR_CODE);
	}
	Args = (get_grade_params*)lpParam;

	printf("Args are: %s ; %d \n", Args->file_path, *Args->grade);

	return 0;
}

