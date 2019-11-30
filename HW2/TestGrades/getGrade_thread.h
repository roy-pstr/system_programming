#ifndef _getGrade_thread_H
#define _getGrade_thread_H

#include <windows.h>
#include "defines.h"

typedef struct get_grade_parmas_st {
	char file_name[MAX_FILE_NAME];
	char *folder_path;
	int *grade;
}GetGrade_params;

void InitGetGradeParams(GetGrade_params *params, int *grade_ptr, char* folder_path, const char *file_name);

DWORD WINAPI GetGradeThread(LPVOID lpParam);
#endif