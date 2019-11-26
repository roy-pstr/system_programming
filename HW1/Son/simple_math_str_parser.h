//Description – Handling strings from the following format: X+Y OR X*Y.
#ifndef _SIMPLE_MATH_STR_PARSER_H
#define _SIMPLE_MATH_STR_PARSER_H
#include <stdbool.h>
char findOperatorBetweenTwoNumbers(const char *mathstring);
void readFirstTwoIntegersFromStr(const char *mathstring, int *first_int_ptr, int *second_int_ptr);
int parseMathString(const char *mathstring, int *first_int_ptr, int *second_int_ptr, char op_ptr[]);
int calcResult(int *result, int first_int, int second_int, char operator_ch[]);
#endif