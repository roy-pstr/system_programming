#include "file_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int WritefileToGradesArray(char *filepath, int *grade_ptr, char *gradetype_ptr)
{
	FILE *fp;
	char filename[MAX_CMD_LENGTH + MAX_FILENMAE_LENGTH];
	printf("filepath_BEFORE = %s\n", filepath);
	strcpy_s(filename, sizeof(filename), filepath);
	strcat_s(filename, sizeof(filename), gradetype_ptr);
	printf("filepath_AFTER = %s\n", filename);
	if (NULL == (fp = fopen(filename, "r")))
	{
		printf("File ERROR\n");
		return -1; // DEBUG ERROR_CODE
	}
	int grade = 0; // DEBUG NUM_OF_GRADES
	char gradeFromFile[MAX_LINE_LENGTH]; // DEBUG MAX_LINE_LENGTH
	fgets(gradeFromFile, MAX_LINE_LENGTH, fp); fclose(fp);
	printf("STR GRADE = %s \n", gradeFromFile);
	grade = (int)atol(gradeFromFile);
	printf("INT GRADE = %d \n", grade);//
	*grade_ptr = grade;
	printf("ARRAY GRADE = %d \n", *grade_ptr); //WORKS
	return 0;
}