//Authors – Roy Pasternak(204219273) & Ilay Amar(308520857)
//Project – EX1
//Description – Son project's main.c file. it's goal is to calculate the given math expression (X+Y OR X*Y) and to return the 
//				result as it's exit code.

#include <stdio.h>
#include <stdbool.h>
#include "simple_math_str_parser.h"
#include "hard_coded_data.h"

#define CHECK_MAIN_STATUS(status) if(status!=true){ return ERROR_CODE;}

bool calcResult(int *result, int first_int, int second_int, char operator_ch[]) {
	switch (operator_ch[0]) {
	case PLUS_SIGN:
		*result = first_int + second_int;
		break;
	case MULTIPLE_SIGN:
		*result = first_int * second_int;
		break;
	default:
		return false;
	}
	return true;
}

int main(int argc, char *argv[])
{
	int first_int=0, second_int=0, result=0;
	char operator_ch[1];
	int main_status = true;
	
	if (argc != 2)
	{
		printf("Illegal number of arguents! Try again\n");
		return ERROR_CODE;
	}

	//split the input string X+Y or X*Y to X,Y, and operator char:
	main_status = parseMathString(argv[1], &first_int, &second_int, operator_ch);
	CHECK_MAIN_STATUS(main_status)

	//calculate the result of the given math eqaution:
	main_status = calcResult(&result, first_int, second_int, operator_ch);
	CHECK_MAIN_STATUS(main_status)

	return result;
}
