#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "math_expression_parser.h"
#include "hard_coded_defiens.h"

void intToStr(int num, char* num_str) {
	sprintf(num_str, "%d", num);
}

int findFirstLeftOpeaningBarckets(const char *math_expression, int string_ind) {
	while (string_ind >= 0) {
		string_ind--;
		if (math_expression[string_ind]=='(') {
			return string_ind;
		}
	}
	return -1; //not found
}

int extractSimpleMathExpression(char simple_expression[], const char *math_expression, int closing_barckets_ind)
{
	int simple_exp_len = 0;
	int open_barckets_ind = findFirstLeftOpeaningBarckets(math_expression, closing_barckets_ind);
	if (open_barckets_ind == -1) {
		return -1;
	}
	else {
		simple_exp_len = closing_barckets_ind - open_barckets_ind-1;
		strncpy_s(simple_expression, sizeof(simple_expression), &math_expression[++open_barckets_ind], simple_exp_len);
	}
	return 0;
}

int updateMathExpWithResult(char * math_expression, const char * simpleMathExp, int result)
{
	char result_str[INT_STRING_MAX_SIZE];
	char *start_ptr;
	char math_expression_temp[MATH_STRING_MAX_LEN];
	int len_until_start = 0, len_until_end = 0, tail_len = 0;
	intToStr(result, &result_str);
	start_ptr = strstr(math_expression, simpleMathExp);
	len_until_start = start_ptr - math_expression-1;
	len_until_end = len_until_start + strlen(simpleMathExp) + 2;
	strncpy_s(math_expression_temp, sizeof(math_expression_temp), math_expression, len_until_start);
	strcat_s(math_expression_temp, sizeof(math_expression_temp), result_str);
	strcat_s(math_expression_temp, sizeof(math_expression_temp), &math_expression[len_until_end]);
	strcpy_s(math_expression, strlen(math_expression), math_expression_temp);
	return 0;
}

