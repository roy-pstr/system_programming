#include "get_grade_thread.h"
#include <stdbool.h>
#define ERROR_CODE ((int)(-1))
#define NUM_THREADS 2
int main()
{
	int grades[NUM_THREADS];
	GetAllGrades(&grades[0], NUM_THREADS);
	return 0;
}