#include <stdio.h>
#include "Son_functions.h"

#define MULTIPLE_SIGN	'*'
#define PLUS_SIGN		'+'
#define ERROR_CODE		-1
#define SUCCESS_CODE	0
#define CHECK_MAIN_STATUS(status) if(status!=SUCCESS_CODE){ return ERROR_CODE;}

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

int calcResult(unsigned long long *result, unsigned long long first_int, unsigned long long second_int, char operator_ch[]) {
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
	unsigned long long first_int=0, second_int=0, result=0;
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

	//result = parseAndCalculate(argv[1]);
	printf("%llu\n", result); //debug

	return result;
}
