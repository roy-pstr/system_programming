#pragma once
#include <stdio.h>
#include <windows.h>
#define TIMEOUT_IN_MILLISECONDS 5000
#define CMD_LINE_MAX_LEN 264
#define SON_EXE_NAME "son.exe "

BOOL CreateProcessSimple(LPTSTR CommandLine, PROCESS_INFORMATION *ProcessInfoPtr);
int calcResultUsingSon(char *args_line);
