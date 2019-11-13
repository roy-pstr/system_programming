#include <stdio.h>
#include "hard_coded_data.h"

char findOperatorBetweenTwoNumbers(char *mathstring) {
	char *operator_ptr = NULL;
	operator_ptr = strchr(mathstring, PLUS_SIGN);
	if (operator_ptr == NULL) {
		operator_ptr = strchr(mathstring, MULTIPLE_SIGN);
		return MULTIPLE_SIGN;
		if (operator_ptr == NULL) {
			return NULL;
		}
	}
	else {
		return PLUS_SIGN;
	}
}

void readFirstTwoIntegersFromStr(const char *mathstring, int *first_int_ptr, int *second_int_ptr) {
	char *first_split_ptr = NULL;
	*first_int_ptr = (int)strtol(mathstring, &first_split_ptr, 10);
	*second_int_ptr = (int)strtol(++first_split_ptr, (char**)NULL, 10);
}

int parseMathString(const char *mathstring, int *first_int_ptr, int *second_int_ptr, char op_ptr[]) {
	op_ptr[0] = findOperatorBetweenTwoNumbers(mathstring);
	if (op_ptr[0] == NULL) 
	{
		return ERROR_CODE;
	}
	readFirstTwoIntegersFromStr(mathstring, first_int_ptr, second_int_ptr);
	return SUCCESS_CODE;
}
