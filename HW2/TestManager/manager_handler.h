#ifndef _MANAGER_HANDLER_H
#define _MANAGER_HANDLER_H

#include "defines.h"

#define ID_FILE_NAME "\\studentIds.txt"
#define DIRECTORY_FOR_SON "\\grades_"
#define FINAL_GRADES_FILE_NAME "\\final_grades.txt"


void MergeStrings(char *target, char *first, char *second);
void MergeStringsForProcessCmd(char *target, char *first, char *second, char *third);
void MultipleProcessesCalling(char *directory);
void ReadIdsAndCallProcess(char *id_ptr, char *directory_ptr, char *student_dir_ptr, char *id_file, char *final_file_dir);
void MergeStringsForStudentFinalGrade(char *target, char *first, char *second, char *third, char *forth);
void WriteFinalGradeToFile(FILE **fp_students, char *id_num, char *filename, char *student_path);

#endif