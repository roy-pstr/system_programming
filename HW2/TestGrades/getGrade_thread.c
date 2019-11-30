#include <stdio.h>
#include <string.h>
#include "getGrade_thread.h"
#include "file_handler.h"

void InitGetGradeParams(GetGrade_params *params, int *grade_ptr, char* folder_path, const char *file_name) {
	strcpy_s(params->file_name, sizeof(char)*MAX_FILE_NAME, file_name);
	params->folder_path = folder_path;
	params->grade = grade_ptr;
}

DWORD WINAPI GetGradeThread(LPVOID lpParam)
{
	Sleep(10);
	/* Handle arguments passed */
	GetGrade_params *Args;
	if (NULL == lpParam) {
		printf("error with arguments passed to thread GetGrade\n");
		return ERROR_CODE;
	}
	Args = (GetGrade_params*)lpParam;

	/* concat file_name to folder_path */
	char *file_path;
	int size_of_filepath = strlen(Args->folder_path) + strlen(Args->file_name) + 1 ;
	if ( NULL == (file_path = (char *) malloc(size_of_filepath)))
	{
		printf("Memory Allocation failed (inside thread)! Try again...");
		return ERROR_CODE; 
	}
	strcpy_s(file_path, sizeof(char) * size_of_filepath, Args->folder_path);
	strcat_s(file_path, sizeof(char) * size_of_filepath, Args->file_name);
	
	/* open file and save grade to *Args->grade */
	ReadGradeFromFile(file_path, Args->grade);

	/* free memory allcoated in this scope */
	free(file_path);
	return SUCCESS_CODE;
}


