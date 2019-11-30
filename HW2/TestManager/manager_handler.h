#ifndef _MANAGER_HANDLER_H
#define _MANAGER_HANDLER_H

#define ID_LENGTH 10
#define ID_FILE_NAME "\\studentIds.txt"
#define ERROR_CODE -1 //DEBUG
#define DIRECTORY_FOR_SON "\\grades_"

void MergeStrings(char *target, char *first, char *second);
int MultipleProcessesCalling(char *directory);
void WriteFinalGradeToFile();



#endif