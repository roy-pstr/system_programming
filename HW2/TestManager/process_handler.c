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
/*	callTestGradesProcess:
		inputs: char *directory_arg, char *id_str
		output: exitcode/ERROR_CODE
		Description - Program to call a son process with all the handling of a process.
*/
int callTestGradesProcess(char *directory_arg, char *id_str)
{
	PROCESS_INFORMATION procinfo;
	DWORD				exitcode;
	DWORD				waitcode;
	BOOL				handlecheck;
	char *command_line;
	int curr_id = (int)atol(id_str); 
	int ret_val = SUCCESS_CODE;
	if (NULL == (command_line = (char *)malloc(strlen(SON_PROC_NAME) + strlen(directory_arg) + 3)))  // student Ids directory name memory allocation
	{
		printf("Memory Allocation failed! Try again...");
		return ERROR_CODE;
	}
	strcpy(command_line, SON_PROC_NAME); 
	strcat(command_line, "\"");
	strcat(command_line, directory_arg); 
	strcat(command_line, "\"");
	
	if (!CreateProcessSimple(command_line, &procinfo))
	{
		printf("Error! Fail to create process. (GetLastError: %d)\n", GetLastError());
		ret_val = ERROR_CODE;
		goto EXIT;
	}

	waitcode = WaitForSingleObject(procinfo.hProcess, PROCESS_TIMEOUT_IN_MILLISECONDS);
	if (WAIT_TIMEOUT == waitcode)
	{
		printf("Timeout error when waiting\n");
		ret_val = ERROR_CODE;
		goto EXIT;
	}
	else if (WAIT_FAILED == waitcode)
	{
		printf("WaitForSingleObject has failed\n");
		ret_val = ERROR_CODE;
		goto EXIT;
	}
	else if (WAIT_OBJECT_0 != waitcode)
	{
		printf("Error when waiting\n");
		ret_val = ERROR_CODE;
		goto EXIT;
	}

	if (!GetExitCodeProcess(procinfo.hProcess, &exitcode))
	{
		printf("Error when getting process exit code\n");
		ret_val = ERROR_CODE;
		goto EXIT;
	}
	if (exitcode != 0)
	{
		printf("Captain, we were unable to calculate %d\n", curr_id);
		ret_val = ERROR_CODE;
		goto EXIT;

	}

EXIT:
	if (NULL != command_line) {
		free(command_line);
	}
	if (NULL != procinfo.hProcess) {
		handlecheck = CloseHandle(procinfo.hProcess);
		if (!handlecheck)
		{
			printf("HANDLE failure! Aborting...\n");
			return ERROR_CODE;
		}
	}
	if (NULL != procinfo.hThread) {
		handlecheck = CloseHandle(procinfo.hThread);
		if (!handlecheck)
		{
			printf("HANDLE failure! Aborting...\n");
			return ERROR_CODE;
		}
	}
	return ret_val;
}
