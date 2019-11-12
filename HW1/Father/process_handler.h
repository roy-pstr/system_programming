#pragma once
#include <stdio.h>
#include <windows.h>
#define TIMEOUT_IN_MILLISECONDS 5000
BOOL CreateProcessSimple(LPTSTR CommandLine, PROCESS_INFORMATION *ProcessInfoPtr);
int calcResultUsingSon(char *command);
