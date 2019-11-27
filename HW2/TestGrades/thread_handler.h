#ifndef _THREAD_HANDLER_H
#define _THREAD_HANDLER_H
#include <windows.h>
#include "get_grade_thread.h"
#include "file_handler.h"
#define NUM_THREADS 2
int GetAllGrades(int *grades, int grades_size, const char file_paths_arr[NUM_THREADS][MAX_FILENMAE_LENGTH]);

#endif