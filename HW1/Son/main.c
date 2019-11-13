#include <stdio.h>
#include "Son_functions.h"
#include "simple_math_str_parser.h"
#include "hard_coded_data.h"

#define CHECK_MAIN_STATUS(status) if(status!=SUCCESS_CODE){ return ERROR_CODE;}

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

int main(int argc, char *argv[])
{
	int first_int=0, second_int=0, result=0;
	char operator_ch[1];
	int main_status = SUCCESS_CODE;
	
	if (argc != 2)
	{
		printf("Illegal number of arguents! Try again\n");
		return ERROR_CODE;
	}

	main_status = parseMathString(argv[1], &first_int, &second_int, operator_ch);
	CHECK_MAIN_STATUS(main_status)

	main_status = calcResult(&result, first_int, second_int, operator_ch);
	CHECK_MAIN_STATUS(main_status)

	return result;
}
