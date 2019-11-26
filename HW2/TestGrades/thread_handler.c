#include "thread_handler.h"
#include <stdbool.h>
#define ERROR_CODE ((int)(-1))

void GetAllGrades(int grades[], int grades_size) {
	HANDLE p_thread_handles[NUM_THREADS];
	DWORD p_thread_ids[NUM_THREADS];
	DWORD wait_code;
	BOOL ret_val;
	// Create two threads, each thread performs on task.
	p_thread_handles[0] = CreateThreadSimple(GetGrade, &p_thread_ids[0]);
	p_thread_handles[1] = CreateThreadSimple(GetGrade, &p_thread_ids[1]);
	// Wait for IO thread to receive exit command and terminate
	wait_code = WaitForSingleObject(p_thread_handles[0], INFINITE);
	if (WAIT_OBJECT_0 != wait_code)
	{
		printf("Error when waiting");
		return ERROR_CODE;
	}
	// Close thread handles
	int i;
	for (i = 0; i < NUM_THREADS; i++)
	{
		ret_val = CloseHandle(p_thread_handles[i]);
		if (false == ret_val)
		{
			printf("Error when closing\n");
			return ERROR_CODE;
		}
	}
}

static DWORD WINAPI GetGrade(LPVOID lpParam)
{
	while (1)
	{
		SendDiagnosticsToMicrosoft();
	}
}

static HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine,
	LPDWORD p_thread_id)
{
	/* Should check for NULL pointers. Skipped for the sake of simplicity. */

	return CreateThread(
		NULL,            /*  default security attributes */
		0,               /*  use default stack size */
		p_start_routine, /*  thread function */
		NULL,            /*  argument to thread function */
		0,               /*  use default creation flags */
		p_thread_id);    /*  returns the thread identifier */
}



