//Description: This module create opens a son process and return its exit code 
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "process_handler.h"

//Program for process creation simple fuicntion from recitation//
BOOL CreateProcessSimple(LPTSTR CommandLine, PROCESS_INFORMATION *ProcessInfoPtr)
{
	STARTUPINFO	startinfo = { sizeof(STARTUPINFO), NULL, 0 }; /* <ISP> here we */
															  /* initialize a "Neutral" STARTUPINFO variable. Supplying this to */
															  /* CreateProcess() means we have no special interest in this parameter. */
															  /* This is equivalent to what we are doing by supplying NULL to most other */
															  /* parameters of CreateProcess(). */

	return CreateProcess(NULL, /*  No module name (use command line). */
		CommandLine,			/*  Command line. */
		NULL,					/*  Process handle not inheritable. */
		NULL,					/*  Thread handle not inheritable. */
		FALSE,					/*  Set handle inheritance to FALSE. */
		NORMAL_PRIORITY_CLASS,	/*  creation/priority flags. */
		NULL,					/*  Use parent's environment block. */
		NULL,					/*  Use parent's starting directory. */
		&startinfo,				/*  Pointer to STARTUPINFO structure. */
		ProcessInfoPtr			/*  Pointer to PROCESS_INFORMATION structure. */
	);
}
//Program to get son exit code//
int callTestGragesProcess(char *directory_arg, char *id_str)
{
	PROCESS_INFORMATION procinfo;
	DWORD				exitcode;
	DWORD				waitcode;
	BOOL				handlecheck;
	char *command_line;
	if (NULL == (command_line = (char *)malloc(strlen(SON_PROC_NAME) + strlen(directory_arg) + strlen(id_str) + strlen(DIRECTORY_FOR_SON)+ 4)))  // student Ids directory name memory allocation
	{
		printf("Memory Allocation failed! Try again...");
		exit(ERROR_CODE);
	}
	strcpy(command_line, SON_PROC_NAME); // DEBUG strcpy_s and strcat_s WHY DOESNT IT WORK?!
	strcat(command_line, directory_arg); 
	strcat(command_line, DIRECTORY_FOR_SON);
	strcat(command_line, id_str);
	strcat(command_line, "\\");
	printf("command_line = %s\n", command_line);
	
	BOOL retVal = CreateProcessSimple(&command_line, &procinfo); // DEBUG - THIS IS 0 maybe because of problems in TestGrades.exe
	if (retVal == 0)
	{
		printf("FATAL Error! process is not created, Aborting...\n");
		return ERROR_CODE;
	}
	waitcode = WaitForSingleObject(procinfo.hProcess, TIMEOUT_IN_MILLISECONDS);
	if (waitcode != 0)
	{
		printf("TIMEOUT failure! Aborting...\n"); /// DEBUG CHECK ALL THESE STUFF!!!
		return ERROR_CODE;
	}
	GetExitCodeProcess(procinfo.hProcess, &exitcode);
	handlecheck = CloseHandle(procinfo.hProcess);
	if (!handlecheck)
	{
		printf("HANDLE failure! Aborting...\n");
		return ERROR_CODE;
	}
	free(command_line);  // DEBUG - Till here works great
	return exitcode;
}
