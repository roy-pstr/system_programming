//Authors – Roy Pasternak(204219273) & Ilay Amar(308520857)
//Project – group54_ex4_server.exe
//Description – 

#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "thread_tools.h"
#include "main_thread.h"

enum THREAD_INDEXING {
	EXIT_THREAD,
	MAIN_THREAD,
	TOTAL_THREADS_AMOUNT
};
bool main_thread_still_alive = true;
bool exit_server = false;
int main(int argc, char *argv[]) {
	ErrorCode_t ret_val = SUCCESS;
	
	//check argv:
	if (argc != SERVER_ARGUMENTS_NUM)
	{
		printf("Illegal number of arguents! Try again\n");
		return ILLEGAL_NUMBER_OF_ARGV;
	}

	DEBUG_PRINT(printf("============SERVER============\n"));

	/* get port */
	main_params_t main_thread_args;
	main_thread_args.port = (int)strtol(argv[1],NULL,10); /* string to int.... */
	
	HANDLE thread_handles[TOTAL_THREADS_AMOUNT];
	InitHandels(thread_handles, TOTAL_THREADS_AMOUNT);

	if (NULL == (thread_handles[EXIT_THREAD] = CreateThreadSimple((LPTHREAD_START_ROUTINE)MyExitThread, NULL)))
	{
		printf("Error when creating exit thread: %d\n", GetLastError());
		ret_val = THREAD_CREATE_FAILED;
		goto EXIT;
	}

	if (NULL == (thread_handles[MAIN_THREAD] = CreateThreadSimple((LPTHREAD_START_ROUTINE)MainThread, &main_thread_args)))
	{
		printf("Error when creating main thread: %d\n", GetLastError());
		ret_val = THREAD_CREATE_FAILED;
		goto EXIT;
	}

	/* this part works like a listener. if EXIT THREAD will return a value so we will close the MAIN THREAD, and if the MAIN THREAD
		returns a value so we close the exit thread.*/
	DWORD wait_code = WaitForMultipleObjects(TOTAL_THREADS_AMOUNT, thread_handles, FALSE, INFINITE);
	if (wait_code == WAIT_OBJECT_0+EXIT_THREAD) {/* exit thread returned a value */
		DEBUG_PRINT(printf("exit thread signled.\n"));
		exit_server = true; /* main thread start closing itself. */
		/* wait for main thread to close */
		wait_code = WaitForSingleObject(thread_handles[MAIN_THREAD], WAIT_TIME_TO_MAIN_THREAD_TO_CLOSE_AFTER_EXIT);
		if (wait_code != WAIT_OBJECT_0) {
			printf("Error when waiting to main thread.\n");
		}
	}
	else if (wait_code == WAIT_OBJECT_0 + MAIN_THREAD) {/* main thread returned a value */
		DEBUG_PRINT(printf("main thread signled.\n"));
		goto EXIT;
	}
	else {
		printf("WaitForMultipleObjects() failed. Ending program\n");
		ret_val = THRAD_WAIT_FAILED;
		goto EXIT;
	}
	
EXIT:
	/* close "main" and "exit" threads */
	DEBUG_PRINT(printf("Trying to close \"exit\" and \"main\" threads .\n"));
	ret_val = CloseThreads(thread_handles, 2);
	return ret_val;
}