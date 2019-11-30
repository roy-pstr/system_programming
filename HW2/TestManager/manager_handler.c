#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "manager_handler.h"
#include "process_handler.h"

int MultipleProcessesCalling(char *directory)
{
	FILE *fp;
	char id_number[ID_LENGTH];
	char *id_filepath;
	if (NULL == (id_filepath = (char *)malloc(strlen(directory) + strlen(ID_FILE_NAME) + 2)))  // student Ids memory allocation
	{
		printf("Memory Allocation failed! Try again...\n");
		exit(ERROR_CODE); 
	}
	MergeStrings(id_filepath, directory, ID_FILE_NAME); // concat dir name and studentIds.txt

	if (NULL == (fp = fopen(id_filepath, "r")))
	{
		printf("File ERROR\n");
		exit(ERROR_CODE);
	}


	while (fgets(id_number, sizeof(id_number), fp)) // get ids from file and call son process 
	{
		if (strlen(id_number) != ID_LENGTH - 1) // avoid bugs on id number
		{
			continue;
		}
		printf("ID = %s \n", id_number);
		callTestGragesProcess(directory, id_number); /// Calling Son
	}
	WriteFinalGradeToFile();
	free(id_filepath);
	fclose(fp);
	return 0;

}

void MergeStrings(char *target, char *first, char *second)
{
	strcpy(target, first);  // DEBUG strcpy_s and strcat_s WHY DOESNT IT WORK?!
	strcat(target, second); 
	//strcpy_s(target, sizeof(char) * (strlen(target) + strlen(first) + strlen(second) + 1), first);
	//strcat_s(target, sizeof(char) * (strlen(target) + strlen(first) + strlen(second) + 1), second);
}
void WriteFinalGradeToFile()
{
	printf("WriteFinalGradeToFile needs to be done!!!\n");
}