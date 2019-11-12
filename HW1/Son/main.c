#include <stdio.h>
#include "Son_functions.h"

int main(int argc, char *argv[])
{
	unsigned long long first_int=0, second_int=0;
	char operator_ch[1];
	unsigned long long result;
	if (argc != 2)
	{
		printf("Illegal number of arguents! Try again\n");
		return ERROR_CODE;
	}

	parseMathString(argv[1], &first_int, &second_int, operator_ch);

	switch (operator_ch[0]) {
	case PLUS_SIGN:
		result = first_int + second_int;
		break;
	case MULTIPLE_SIGN:
		result = first_int * second_int;
		break;
	default:
		return ERROR_CODE;
	}
	//result = parseAndCalculate(argv[1]);
	printf("%llu\n", result); //debug
	return result;
}
