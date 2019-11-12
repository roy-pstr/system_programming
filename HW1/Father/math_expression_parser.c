#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "math_expression_parser.h"

int findFirstLeftOpeaningBarckets(const char *math_expression, int string_ind) {
	while (string_ind >= 0) {
		string_ind--;
		if (strncmp(math_expression[string_ind], '(', 1) == 0) {
			return string_ind;
		}
	}
	return -1; //not found
}

void extractSimpleMathExpression(char simple_expression[], const char *math_expression, int closing_barckets_ind)
{
	int simple_exp_len = 0;
	int open_barckets_ind = findLeftOpeaningBarckets(math_expression, closing_barckets_ind);
	if (open_barckets_ind == -1) {
		return NULL;
	}
	else {
		simple_exp_len = closing_barckets_ind - open_barckets_ind;
		strncpy_s(simple_expression, sizeof(simple_expression), &math_expression[open_barckets_ind], simple_exp_len);
	}
}

int replaceMathExpWithResult(char * math_expression, const char * simpleMathExp, int result)
{
	return 0;
}
