#include "thread_handler.h"
#include <stdbool.h>
#define ERROR_CODE ((int)(-1))
#define SUCCESS_CODE ((int)(0))
#define MAX_STRING 50
#define NUM_THREADS 2

static HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine,
	LPVOID p_thread_parameters,
	LPDWORD p_thread_id)
{
	HANDLE thread_handle;

	if (NULL == p_start_routine)
	{
		printf("Error when creating a thread");
		printf("Received null pointer");
		exit(ERROR_CODE);
	}

	if (NULL == p_thread_id)
	{
		printf("Error when creating a thread");
		printf("Received null pointer");
		exit(ERROR_CODE);
	}

	thread_handle = CreateThread(
		NULL,            /*  default security attributes */
		0,               /*  use default stack size */
		p_start_routine, /*  thread function */
		p_thread_parameters,     /*  argument to thread function */
		0,               /*  use default creation flags */
		p_thread_id);    /*  returns the thread identifier */

	if (NULL == thread_handle)
	{
		printf("Couldn't create thread\n");
		exit(ERROR_CODE);
	}

	return thread_handle;
}

int GetAllGrades(int *grades, int grades_size) {
	HANDLE p_thread_handles[NUM_THREADS];
	DWORD p_thread_ids[NUM_THREADS];

	DWORD wait_code;
	BOOL ret_val;
	/* Prepare parameters for thread */

	int g1 = 20;
	int g2 = 50;
	get_grade_params p_thread_args[NUM_THREADS] = {	{ "C:\Users\roypa\OneDrive\Documents\GitHub\system_programming\HW2\students_gardes\grades_204219273\ex01.txt", &g1 },
										{ "C:\Users\roypa\OneDrive\Documents\GitHub\system_programming\HW2\students_gardes\grades_204219273\ex02.txt", &g2 }, };
	//InitParams(&p_thread_args, "C:\Users\roypa\OneDrive\Documents\GitHub\system_programming\HW2\students_gardes\grades_204219273\ex01.txt", &g);
	/* Create thread */
	p_thread_handles[0] = CreateThreadSimple(GetGrade, &p_thread_args[0], &p_thread_ids[0]);
	p_thread_handles[1] = CreateThreadSimple(GetGrade, &p_thread_args[1], &p_thread_ids[1]);
	//p_thread_handles[1] = CreateThreadSimple(GetGrade, &p_thread_ids[1]);
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