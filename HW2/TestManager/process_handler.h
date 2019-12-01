#ifndef _PROCESS_HANDLER_H
#define _PROCESS_HANDLER_H

#define ERROR_CODE -1
#define TIMEOUT_IN_MILLISECONDS 5000
#define SON_PROC_NAME "TestGrades.exe "
BOOL CreateProcessSimple(LPTSTR CommandLine, PROCESS_INFORMATION *ProcessInfoPtr);
int callTestGradesProcess(char *args_line, char *id);
#endif