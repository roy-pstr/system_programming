#ifndef _MANAGER_HANDLER_H
#define _MANAGER_HANDLER_H

#define ID_LENGTH 10
#define GRADE_LENGTH 4

#define ID_FILE_NAME "\\studentIds.txt"
#define ERROR_CODE -1 //DEBUG
#define DIRECTORY_FOR_SON "\\grades_"
#define FINAL_GRADES_FILE_NAME "\\final_grades.txt"
#define FINAL_STUDENT_FILE_NAME "\\final_" // SHOULD BE IN COMMON
#define TXT ".txt"


void MergeStrings(char *target, char *first, char *second);
void MergeStringsForProcessCmd(char *target, char *first, char *second, char *third);
int MultipleProcessesCalling(char *directory);
void WriteFinalGradeToFile(char *id_num, char *filename, char *student_path);
void ReadIdsAndCallProcess(char *id_ptr, char *directory_ptr, char *student_dir_ptr, char *id_file, char *final_file_dir);
void WriteFinalGradeToFile(char *id_num, char *filename, char *student_path);
void MergeStringsForStudentFinalGrade(char *target, char *first, char *second, char *third, char *forth);

#endif