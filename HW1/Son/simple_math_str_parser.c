//Description – Handling strings from the following format: X+Y OR X*Y. Implemntaion file.

#include <stdio.h>
#include "simple_math_str_parser.h"
#include "hard_coded_data.h"

/*findOperatorBetweenTwoNumbers:
	input:
		mathstring - a string conatin two integers and a charcter between them in the following format: X+Y or X*Y
	return:
		functions return '+', '*' or NULL if non of them were found.
*/
char findOperatorBetweenTwoNumbers(const char *mathstring) {
	char *operator_ptr = NULL;
	operator_ptr = strchr(mathstring, PLUS_SIGN);
	if (operator_ptr == NULL) {
		operator_ptr = strchr(mathstring, MULTIPLE_SIGN);
		return MULTIPLE_SIGN;
		if (operator_ptr == NULL) {
			return NOT_FOUND;
		}
	}
	else {
		return PLUS_SIGN;
	}
}

/*readFirstTwoIntegersFromStr:
	input:
		mathstring - a string conatin two integers and one charcter seprates between them in the following format: X+Y or X*Y
	output:
		first_int = X, second_int = Y
*/
void readFirstTwoIntegersFromStr(const char *mathstring, int *first_int_ptr, int *second_int_ptr) {
	char *first_split_ptr = NULL;
	*first_int_ptr = (int)strtol(mathstring, &first_split_ptr, 10);
	*second_int_ptr = (int)strtol(++first_split_ptr, (char**)NULL, 10);
}

/*parseMathString:
	input: 
		mathstring - a string conatin two integers and one charcter seprates between them in the following format: X+Y or X*Y
	output:	
		first_int = X, second_int = Y, op_ptr[0] = '+'/'*'
*/
int parseMathString(const char *mathstring, int *first_int_ptr, int *second_int_ptr, char op_ptr[]) {
	op_ptr[0] = findOperatorBetweenTwoNumbers(mathstring);
	if (op_ptr[0] == NOT_FOUND)
	{
		return ERROR_CODE;
	}
	readFirstTwoIntegersFromStr(mathstring, first_int_ptr, second_int_ptr);
	return SUCCESS_CODE;
}

int calcResult(int *result, int first_int, int second_int, char operator_ch[]) {
	switch (operator_ch[0]) {
	case PLUS_SIGN:
		*result = first_int + second_int;
		break;
	case MULTIPLE_SIGN:
		*result = first_int * second_int;
		break;
	default:
		return ERROR_CODE;
	}
	return SUCCESS_CODE;
}
