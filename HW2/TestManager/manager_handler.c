#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "manager_handler.h"
#include "process_handler.h"

/*allocateString:
	inputs:
		char **str_ptr 
		int len
	outputs: 
		None
	Description:
		This is a malloc function with all the needed debugging.
*/
int allocateString(char **str_ptr, int len) {
	if (NULL == (*str_ptr = (char *)malloc(len)))
	{
		printf("Memory Allocation failed! Try again...\n");
		return ERROR_CODE;
	}
	return SUCCESS_CODE;
}

/*MultipleProcessesCalling:
	inputs:
		char *directory
	outputs:
		None
	Description:
		The function that runs on main and call the son process TestGrades.
*/
int MultipleProcessesCalling(char *directory)
{
	char id_number[ID_LENGTH];
	char *id_filepath=NULL, *final_filepath = NULL, *student_dir = NULL;

	/* allocate memory for strings */
	int ret_val = 0;
	if (ERROR_CODE == allocateString(&id_filepath, (strlen(directory) + strlen(ID_FILE_NAME) + 2)) ||
		ERROR_CODE == allocateString(&final_filepath, (strlen(directory) + strlen(FINAL_GRADES_FILE_NAME) + 2)) ||
		ERROR_CODE == allocateString(&student_dir, (strlen(directory) + ID_LENGTH + strlen(DIRECTORY_FOR_SON) + 2)) ){
		ret_val = ERROR_CODE;
		goto EXIT;
	}

	/* merge strings */
	MergeStrings(id_filepath, directory, ID_FILE_NAME); // concat dir name and studentIds.txt
	MergeStrings(final_filepath, directory, FINAL_GRADES_FILE_NAME);

	/* call process */
	ret_val = ReadIdsAndCallProcess(id_number, directory, student_dir, id_filepath, final_filepath);

	/* free memory allocated in this scope */
EXIT:
	if (NULL != student_dir) {
		free(student_dir);
	}
	if (NULL != final_filepath) {
		free(final_filepath);
	}
	if (NULL != id_filepath) {
		free(id_filepath);
	}
	return ret_val;
}
/*ReadIdsAndCallProcess:
	inputs:
		char *id_ptr, char *directory_ptr, char *student_dir_ptr, char *id_file, char *final_file_dir
	outputs:
		None
	Description:
		This function creates a son process for each id from studentsIds.txt,
		after the processes are finished, it writes the final results to final_grades.txt.
*/
int ReadIdsAndCallProcess(char *id_ptr, char *directory_ptr, char *student_dir_ptr, char *id_file, char *final_file_dir)
{
	FILE *fp_ids, *fp_students;
	int ret_val = SUCCESS_CODE;
	/* studentsIds.txt */
	if (NULL == (fp_ids = fopen(id_file, "r"))) 
	{
		printf("File ERROR\n");
		return ERROR_CODE;
	}

	/* final_grades.txt */
	if (NULL == (fp_students = fopen(final_file_dir, "w"))) 
	{
		printf("File ERROR\n");
		ret_val = ERROR_CODE;
		goto EXIT;
	}

	while (fgets(id_ptr, ID_LENGTH , fp_ids)) // get ids from studentsIds.txt and call son process 
	{
		if (strlen(id_ptr) != ID_LENGTH - 1) // avoid bugs on id number
		{
			continue;
		}
		MergeStringsForProcessCmd(student_dir_ptr, directory_ptr, DIRECTORY_FOR_SON, id_ptr); // /grades_#ID
		/*Calling son*/
		if (SUCCESS_CODE != callTestGradesProcess(student_dir_ptr, id_ptr)) {
			ret_val = ERROR_CODE;
			goto EXIT;
		}

		if (SUCCESS_CODE != WriteFinalGradeToFile(&fp_students, id_ptr, final_file_dir, student_dir_ptr)) {
			ret_val = ERROR_CODE;
			goto EXIT;
		}
	}

EXIT:
	if(NULL!= fp_ids)
		fclose(fp_ids);
	if (NULL != fp_students)
		fclose(fp_students);
	return ret_val;
}

/*MergeStrings, MergeStringsForProcessCmd, MergeStringsForStudentFinalGrade :
	inputs:
		char *target, char *first, char *second (etc...)
	outputs:
		None
	Description:
		The following 3 functions are concating strings in order to creates full paths for directories and files
		that are sent to process, or need to be read/write. (for all the project).
*/
void MergeStrings(char *target, char *first, char *second)
{
	strcpy(target, first); 
	strcat(target, second); 
	
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

/*WriteFinalGradeToFile:
	inputs:
		char FILE **fp_students, char *id_num, char *filename, char *student_path
	outputs:
		None
	Description:
		This is a helping function in order to write the final grades to final_grades.txt
*/
int WriteFinalGradeToFile(FILE **fp_students, char *id_num, char *filename, char *student_path)
{
	FILE *fp_final;
	char *student_file, grade[4];
	int ret_val=0;

	if (NULL == (student_file = (char *)malloc(strlen(student_path) + ID_LENGTH + strlen(FINAL_STUDENT_FILE_NAME) + strlen(TXT) + 3)))  // student Ids directory name memory allocation
	{
		printf("Memory Allocation failed! Try again...");
		return ERROR_CODE;
	}
	MergeStringsForStudentFinalGrade(student_file, student_path, FINAL_STUDENT_FILE_NAME, id_num, TXT);

	if (NULL == (fp_final = fopen(student_file, "r"))) //final_#ID.txt
	{
		printf("File ERROR\n");
		ret_val=ERROR_CODE;
		goto EXIT;
	}

	while (fgets(grade, ID_LENGTH, fp_final)) // get ids from file and call son process 
	{
		if (strlen(grade) < 2) // avoid bugs on id number DEBUG
		{
			continue;
		}
		fprintf(*fp_students, "%s %s\n", id_num, grade);
	}
EXIT:
	if (NULL != fp_final) {
		fclose(fp_final);
	}
		
	if (NULL != student_file) {
		free(student_file);
	}
	
	return SUCCESS_CODE;
}