#ifndef _PROCESS_HANDLER_H
#define _PROCESS_HANDLER_H
#include "defines.h"
//#define ERROR_CODE -1
#define TIMEOUT_IN_MILLISECONDS 10000
#define SON_PROC_NAME "TestGrades.exe "
BOOL CreateProcessSimple(LPTSTR CommandLine, PROCESS_INFORMATION *ProcessInfoPtr);
int callTestGradesProcess(char *args_line, char *id);
#endif