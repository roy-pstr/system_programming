#include "file_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int concatFileNameToPath(char *filepath, const char *folderpath, const char *filename) {
	int size_of_filepath = strlen(folderpath) + strlen(filename) + 1;
	//if (NULL == (filepath = (char *)realloc(filepath, size_of_filepath)))
	//{
	//	printf("Memory Allocation failed! Try again...");
	//	return -1; // DEBUG ERROR_CODE
	//}
	strcpy_s(filepath, sizeof(char) * size_of_filepath, folderpath);
	strcat_s(filepath, sizeof(char) * size_of_filepath, filename);
}

int WritefileToGradesArray(const char *filepath, int *grade_ptr, const char *gradetype_ptr)
{
	FILE *fp;
	char *filename = NULL;
	char gradeFromFile[MAX_LINE_LENGTH];
	int grade = 0;
	int size_of_filepath = strlen(filepath) + strlen(gradetype_ptr) + 1;
	if (NULL == (filename = (char *)malloc(size_of_filepath)))
	{
		printf("Memory Allocation failed! Try again...");
		return -1; // DEBUG ERROR_CODE
	}
	concatFileNameToPath(filename, filepath, gradetype_ptr);
	//int size_of_filename = strlen(filepath) + strlen(gradetype_ptr) + 1 ;
	

	//if ( NULL == (filename = (char *) malloc(size_of_filename)))
	//{
	//	printf("Memory Allocation failed! Try again...");
	//	return -1; // DEBUG ERROR_CODE
	//}
	//strcpy_s(filename, sizeof(char) * size_of_filename, filepath);
	//strcat_s(filename, sizeof(char) * size_of_filename, gradetype_ptr);
	if (NULL == (fp = fopen(filename, "r")))
	{
		printf("File ERROR\n");
		return -1; // DEBUG ERROR_CODE
	}
	fgets(gradeFromFile, MAX_LINE_LENGTH, fp); fclose(fp);
	grade = (int)atol(gradeFromFile);
	if (grade < 60)
	{
		grade = 0;
	}
	*grade_ptr = grade;
	free(filename);
	return 0;
}
