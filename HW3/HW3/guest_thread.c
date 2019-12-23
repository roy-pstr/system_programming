#include <stdio.h>
#include <string.h>
#include "guest_thread.h"

DWORD LockUpdateAndUnlock(HANDLE MutexHandle)
{
	BOOL ReleaseRes;

	/* wait for mutex*/
	DWORD wait_code = WaitForSingleObject(MutexHandle, MUTEX_TIMEOUT_IN_MILLISECONDS);
	if (wait_code != WAIT_OBJECT_0)
	{
		if (wait_code == WAIT_ABANDONED)
		{
			printf("Some thread has previously exited without releasing a mutex."
				" This is not good programming. Please fix the code.\n");
			return (MUTEX_ABANDONED);
		}
		else {
			printf("Mutex wait failed\n");
			return(MUTEX_WAIT_FAILED);
		}

	}
	printf("(from thread %d): inside mutex\n", GetCurrentThreadId());

	/* ........Critical Section Start................ */

	/* ........Critical Section End.................. */

	/*release mutex*/
	DWORD rel_code = ReleaseMutex(MutexHandle);
	if (rel_code == FALSE) {
		return (MUTEX_RELEASE_FAILED);
	}
	printf("(from thread %d): released mutex\n", GetCurrentThreadId());
	return (SUCCESS);
}

void checkOut(guest_params_t *Args) {
	DWORD rel_code;
	LONG previous_count;

	/*release semaphore*/
	rel_code = ReleaseSemaphore(
		Args->room->capacity_sem,
		1, 		/* Signal that exactly one cell was filled */
		&previous_count);
	if (rel_code == FALSE)
		return (SEMAPHORE_RELEASE_FAILED);
	printf("(from thread %d): Previous count is: %ld\n", GetCurrentThreadId(), previous_count);
	Args->guest->checked_out = true;
}

void spendTheDay(guest_params_t *Args) {
	Args->guest->budget -= Args->room->price;
}

void tryToCheckIn(guest_params_t *Args) {
	DWORD wait_code, rel_code;
	BOOL ret_val;
	

	/* wait for semaphore*/
	wait_code = WaitForSingleObject(Args->room->capacity_sem, SEMAPHORE_TIMEOUT_IN_MILLISECONDS);
	if (wait_code != WAIT_OBJECT_0) {
		printf("Error when waiting for simaphore: %d\n", wait_code);
		return SEMAPHORE_WAIT_FAILED;
	}

	//printf("(from thread %d): inside empty_semaphore\n", Args->a);

	LockUpdateAndUnlock(NULL);

	
}

void waitForNextDay(HANDLE MutexHandle) {

}
DWORD WINAPI GuestThread(LPVOID lpParam)
{
	/* Handle arguments passed */
	guest_params_t *Args;
	if (NULL == lpParam) {
		printf("error with arguments passed to thread\n");
		return GUEST_PARAMS_CASTING_FAILED;
	}
	Args = (guest_params_t*)lpParam;

	while (!Args->guest->checked_out) 
	{
		/* meaning he is still in the hotel, looking for a room or spending the day */
		Sleep(SLEEP_TIME);
		if (Args->guest->checked_in) {
			spendTheDay(Args);
		}
		else{
			tryToCheckIn(Args);
		}
		/* wait for all guests to enter 'waitForNextDay' state and then release the mutex */
		waitForNextDay(Args->guest->next_day_mutex); 
	}

	checkOut(Args);

	return (SUCCESS);
}