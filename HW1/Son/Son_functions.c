#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "Son_functions.h"

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

void readFirstTwoIntegersFromStr(const char *mathstring, unsigned long long *first_int_ptr, unsigned long long *second_int_ptr) {
	char *first_split_ptr = NULL;
	*first_int_ptr = (unsigned long long)strtol(mathstring, &first_split_ptr, 10);
	*second_int_ptr = (unsigned long long)strtol(++first_split_ptr, (char**)NULL, 10);
}

int parseMathString(const char *mathstring, unsigned long long *first_int_ptr, unsigned long long *second_int_ptr, char op_ptr[]) {
	op_ptr[0] = findOperatorBetweenTwoNumbers(mathstring);
	readFirstTwoIntegersFromStr(mathstring, first_int_ptr, second_int_ptr);
	return SUCCESS_CODE;
}


int parseAndCalculate(char *mathstring)
{
	/*input: string in the following formats:
		X+Y
		X*Y
		where X and Y are integers.
	return: the mathmetical result of the given equation.

	The function parses the input string to first int, second int and math operation
	and calculates the result.*/

	int str_ind = 0, X = 0, Y = 0;
	bool flag_Y = false, flag_PLUS = false;
	char curr_char;

	while (mathstring[str_ind] != '\0')
	{
		curr_char = mathstring[str_ind];
		str_ind++;
		if (curr_char == PLUS_SIGN || curr_char == MULTIPLE_SIGN)
		{
			flag_Y = true;
			if (curr_char == PLUS_SIGN)
			{
				flag_PLUS = true;
			}
			continue;
		}
		if (flag_Y==false)
		{
			X = X * 10 + (curr_char -'0');
		}
		else 
		{
			Y = Y * 10 + (curr_char - '0');
		}
	}

	if (flag_PLUS==true)
	{
		return X + Y;
	}
	return X * Y;
}
