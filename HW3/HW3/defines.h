#ifndef _DEFINES_H
#define _DEFINES_H
#include <stdbool.h>
#include <windows.h>

#define ERROR_CODE ((int)(-1))
#define SUCCESS_CODE ((int)(0))
#define MAX_LINE_LEN 100
#define MAX_NAME_LEN 20
#define MAX_ROOMS 5
#define MAX_GUESTS 15

#define GUEST_THREAD_TIMEOUT_IN_MILLISECONDS 10000
#define DAY_THREAD_TIMEOUT_IN_MILLISECONDS 2000
#define MUTEX_TIMEOUT_IN_MILLISECONDS 10000
#define SEMAPHORE_TIMEOUT_IN_MILLISECONDS 10000
#define SLEEP_TIME 50
#define ROOMS_TXT_FILE "\\rooms.txt"
#define NAMES_TXT_FILE "\\names.txt"
#define LOG_FILE "\\roomLog.txt"


typedef enum {
	SUCCESS,
	FILE_OPEN_FAILED,
	FILE_READING_FAILED,
	MUTEX_OPEN_FAILED,
	MUTEX_CREATE_FAILED,
	MUTEX_WAIT_FAILED,
	MUTEX_ABANDONED,
	MUTEX_RELEASE_FAILED,
	MUTEX_CLOSE_FAILED,
	SEMAPHORE_RELEASE_FAILED,
	SEMAPHORE_WAIT_FAILED,
	SEMAPHORE_CREATE_FAILED,
	SEMAPHORE_CLOSE_FAILED,
	GUEST_PARAMS_CASTING_FAILED,
	GUEST_THREAD_FAILED,
	THREAD_CREATE_FAILED,
	THREAD_GET_EXIT_CODE_FAILED,
	THREAD_WAIT_FAILED,
	THREAD_FAILED,
	THREAD_CLOSE_ERROR,
	FILE_ERROR
	
} ErrorCode_t;

#define GO_TO_EXIT_ON_FAILURE(ret_val, msg) {if (SUCCESS != ret_val){printf("%s\n",msg);goto EXIT;}}

typedef struct {
	char name[MAX_NAME_LEN];
	int price;
	int capacity;
	int vacancy_counter; 
	HANDLE room_mutex;
} Room_t;


#endif