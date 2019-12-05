#ifndef _PROCESS_HANDLER_H
#define _PROCESS_HANDLER_H
#include "defines.h"

BOOL CreateProcessSimple(LPTSTR CommandLine, PROCESS_INFORMATION *ProcessInfoPtr);
int callTestGradesProcess(char *args_line, char *id);
#endif