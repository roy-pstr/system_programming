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

#define THREAD_TIMEOUT_IN_MILLISECONDS 10000
#define MUTEX_TIMEOUT_IN_MILLISECONDS 10000
#define SEMAPHORE_TIMEOUT_IN_MILLISECONDS 10000
#define SLEEP_TIME 100
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
	ILLEGAL_LETTER_WAS_READ,
	SEMAPHORE_RELEASE_FAILED,
	SEMAPHORE_WAIT_FAILED,
	GUEST_PARAMS_CASTING_FAILED,
	THREAD_CREATE_FAILED,
	THREAD_GET_EXIT_CODE_FAILED,
	THREAD_WAIT_FAILED,
	THREAD_FAILED,
	THREAD_CLOSE_ERROR
} ErrorCode_t;


typedef struct {
	char name[MAX_NAME_LEN];
	int price;
	int capacity;
	HANDLE capacity_sem;
	HANDLE room_mutex;
} Room_t;

typedef struct {
	char name[MAX_NAME_LEN];
	int budget;
	bool checked_in;
	bool checked_out;
} Guest_t;
#endif