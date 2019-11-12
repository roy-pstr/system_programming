#pragma once
#define MULTIPLE_SIGN	'*'
#define PLUS_SIGN		'+'
#define ERROR_CODE		-1
#define SUCCESS_CODE	0

int parseAndCalculate(char *mathstring);

int parseMathString(const char *mathstring, unsigned long long *first_int_ptr, unsigned long long *second_int_ptr, char op_ptr[]);
