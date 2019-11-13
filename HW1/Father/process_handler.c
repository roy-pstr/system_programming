#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "process_handler.h"

// This module create opens a son process and return its exit code //

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
int calcResultUsingSon(char *args_line)
{
	PROCESS_INFORMATION procinfo;
	DWORD				exitcode;
	char command_line[CMD_LINE_MAX_LEN];
	strcpy_s(command_line,sizeof(command_line) , SON_EXE_NAME);
	strcat_s(command_line, sizeof(command_line), args_line);
	//printf("%s\n", command_line); //debug
	BOOL retVal = CreateProcessSimple(&command_line, &procinfo);
	if (retVal == 0)
	{
		printf("FATAL! process not created, Aborting...");
		return 1;
	}
	WaitForSingleObject(procinfo.hProcess, TIMEOUT_IN_MILLISECONDS);
	GetExitCodeProcess(procinfo.hProcess, &exitcode);
	//printf("ILAY EXIT CODE %d\n", exitcode);
	CloseHandle(procinfo.hProcess);
		//WAITCODE ILAY
	return exitcode;
}
