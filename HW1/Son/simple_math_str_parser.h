#pragma once
#include <stdbool.h>

bool parseMathString(const char *mathstring, int *first_int_ptr, int *second_int_ptr, char op_ptr[]);