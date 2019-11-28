#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "getGrade_thread.h"
#include "file_handler.h"
#include "defines.h"

void InitParams(get_grade_params *params,const char * str, int *ptr) {
	strcpy(params->file_path, str);
	params->grade = ptr;
}

DWORD WINAPI GetGrade(LPVOID lpParam)
{
	Sleep(10);
	get_grade_params *Args;
	if (NULL == lpParam) {
		printf("error with arguments passed to thread GetGrade");
		exit(ERROR_CODE);
	}
	Args = (get_grade_params*)lpParam;
	ReadGradeFromFile(Args->file_path, Args->grade);
	return 0;
}

