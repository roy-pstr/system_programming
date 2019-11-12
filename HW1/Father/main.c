#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "hard_coded_defiens.h"
#include "process_handler.h"

#define MATH_STRING_MAX_LEN 256
#define SIMPLE_MATH_STRING_MAX_LEN 32

int calculateResult(char *math_expression) {
	bool exit_loop = false;
	char* closingBarckets_ptr;
	char simpleMathExp[SIMPLE_MATH_STRING_MAX_LEN];
	int result, next_closing_brks_ind;

	closingBarckets_ptr = strchr(math_expression, ')');
	while (closingBarckets_ptr!=NULL) {
		next_closing_brks_ind = closingBarckets_ptr - math_expression;
		extractSimpleMathExpression(simpleMathExp, math_expression, next_closing_brks_ind);

		//result = calcResultUsingSon(simpleMathExp);
		//replaceMathExpWithResult(math_expression, simpleMathExp, result);
		print("%s", math_expression);

		closingBarckets_ptr = strchr(math_expression, ')');
	}

	return (int)(math_expression - '0');
}

int main(int argc, char *argv[])
{
	int result = 0;

	if (argc != 2)
	{
		printf("Illegal number of arguents! Try again\n");
		return ERROR_CODE;
	}

	printf("%s", argv[1]); //debug
	result = calculateResult(argv[1]);
	//int ecode = GetSonExitCode("Son.exe 40+2");
	//printf("Son Exit Code: %d\n", ecode);
	return 0;
}