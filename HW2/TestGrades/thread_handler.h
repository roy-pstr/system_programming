#ifndef _THREAD_HANDLER_H
#define _THREAD_HANDLER_H
#include <windows.h>
#include "getGrade_thread.h"
#include "file_handler.h"
#include "defines.h"
int GetAllGrades(int *grades, int grades_size, const char file_paths_arr[NUM_THREADS][MAX_FILENMAE_LENGTH]);
int RunMultiplayThreads(int *grades, const char file_paths_arr[NUM_THREADS][MAX_FILENMAE_LENGTH]);
#endif