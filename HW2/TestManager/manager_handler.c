#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "manager_handler.h"
#include "process_handler.h"
void allocateString(char **str_ptr, int len) {
	if (NULL == (*str_ptr = (char *)malloc(len)))
	{
		printf("Memory Allocation failed! Try again...\n");
		exit(ERROR_CODE);
	}
}

int MultipleProcessesCalling(char *directory)
{
	char id_number[ID_LENGTH];
	char *id_filepath, *final_filepath, *student_dir;

	/* allocate memory for strings */
	allocateString(&id_filepath, (strlen(directory) + strlen(ID_FILE_NAME) + 2)); //malloc, must free!
	allocateString(&final_filepath, (strlen(directory) + strlen(FINAL_GRADES_FILE_NAME) + 2)); //malloc, must free!
	allocateString(&student_dir, (strlen(directory) + ID_LENGTH + strlen(DIRECTORY_FOR_SON) + 2)); //malloc, must free!
	// DEBUG DO FUNCTION FOR ALL MALLOCS
	//if (NULL == (id_filepath = (char *)malloc(strlen(directory) + strlen(ID_FILE_NAME) + 2)))  // student Ids memory allocation
	//{
	//	printf("Memory Allocation failed! Try again...\n");
	//	exit(ERROR_CODE); 
	//}
	//if (NULL == (final_filepath = (char *)malloc(strlen(directory) + strlen(FINAL_GRADES_FILE_NAME) + 2)))  // student Ids memory allocation
	//{
	//	printf("Memory Allocation failed! Try again...\n");
	//	exit(ERROR_CODE);
	//}
	//if (NULL == (student_dir = (char *)malloc(strlen(directory) + ID_LENGTH + strlen(DIRECTORY_FOR_SON) + 2)))  // student Ids directory name memory allocation
	//{
	//	printf("Memory Allocation failed! Try again...");
	//	exit(ERROR_CODE);
	//}

	/* merge strings */
	MergeStrings(id_filepath, directory, ID_FILE_NAME); // concat dir name and studentIds.txt
	MergeStrings(final_filepath, directory, FINAL_GRADES_FILE_NAME);

	/* call process */
	ReadIdsAndCallProcess(id_number, directory, student_dir, id_filepath, final_filepath);

	/* free memory allocated in this scope */
	free(id_filepath);
	free(final_filepath);
	free(student_dir);
	return 0;

}

void ReadIdsAndCallProcess(char *id_ptr, char *directory_ptr, char *student_dir_ptr, char *id_file, char *final_file_dir)
{
	FILE *fp_ids;
	if (NULL == (fp_ids = fopen(id_file, "r")))
	{
		printf("File ERROR\n");
		exit(ERROR_CODE);
	}

	while (fgets(id_ptr, ID_LENGTH , fp_ids)) // get ids from file and call son process 
	{
		if (strlen(id_ptr) != ID_LENGTH - 1) // avoid bugs on id number
		{
			continue;
		}
		MergeStringsForProcessCmd(student_dir_ptr, directory_ptr, DIRECTORY_FOR_SON, id_ptr);

		callTestGradesProcess(student_dir_ptr, id_ptr); /// Calling Son
		WriteFinalGradeToFile(id_ptr, final_file_dir, student_dir_ptr);

	}
	fclose(fp_ids);
}

void MergeStrings(char *target, char *first, char *second)
{
	strcpy(target, first);  // DEBUG strcpy_s and strcat_s WHY DOESNT IT WORK?!
	strcat(target, second); 
	//strcpy_s(target, sizeof(char) * (strlen(target) + strlen(first) + strlen(second) + 1), first);
	//strcat_s(target, sizeof(char) * (strlen(target) + strlen(first) + strlen(second) + 1), second);
}

void MergeStringsForProcessCmd(char *target, char *first, char *second, char *third)
{
	strcpy(target, first);
	strcat(target, second);
	strcat(target, third);
}

void MergeStringsForStudentFinalGrade(char *target, char *first, char *second, char *third, char *forth)
{
	strcpy(target, first);
	strcat(target, second);
	strcat(target, third);
	strcat(target, forth);
}


void WriteFinalGradeToFile(char *id_num, char *filename, char *student_path)
{
	FILE *fp_students, *fp_final;
	char *student_file, grade[4];
	if (NULL == (student_file = (char *)malloc(strlen(student_path) + ID_LENGTH + strlen(FINAL_STUDENT_FILE_NAME) + strlen(TXT) + 3)))  // student Ids directory name memory allocation
	{
		printf("Memory Allocation failed! Try again...");
		exit(ERROR_CODE);
	}
	MergeStringsForStudentFinalGrade(student_file, student_path, FINAL_STUDENT_FILE_NAME, id_num, TXT);

	//files handling
	if (NULL == (fp_students = fopen(filename, "a"))) // final_grades.txt
	{
		printf("File ERROR\n");
		exit(ERROR_CODE);
	}
	if (NULL == (fp_final = fopen(student_file, "r"))) //final_#ID.txt
	{
		printf("File ERROR\n");
		exit(ERROR_CODE);
	}

	while (fgets(grade, ID_LENGTH, fp_final)) // get ids from file and call son process 
	{
		if (strlen(grade) < 2) // avoid bugs on id number DEBUG
		{
			continue;
		}
		fprintf(fp_students, "%s %s\n", id_num, grade);
	}
	fclose(fp_students);
	fclose(fp_final);
	free(student_file);
}