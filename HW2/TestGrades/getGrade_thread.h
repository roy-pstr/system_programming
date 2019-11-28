#ifndef _getGrade_thread_H
#define _getGrade_thread_H
#include <windows.h>
#include <string.h>
typedef struct parmas_st {
	char *file_path;
	int *grade;
}get_grade_params;

DWORD WINAPI GetGrade(LPVOID lpParam);

#endif