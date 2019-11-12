#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "hard_coded_defiens.h"
#include "process_handler.h"
#include "math_expression_parser.h"

int calculateResult(char *math_expression) {
	int result = 0, next_closing_brks_ind = 0;
	char* closing_brkts_ptr;
	char simpleMathExp[SIMPLE_MATH_STRING_MAX_LEN];

	closing_brkts_ptr = strchr(math_expression, ')');
	while (closing_brkts_ptr!=NULL) {
		next_closing_brks_ind = closing_brkts_ptr - math_expression;
		extractSimpleMathExpression(simpleMathExp, math_expression, next_closing_brks_ind);
		result = calcResultUsingSon(simpleMathExp);

		updateMathExpWithResult(math_expression, simpleMathExp, result);
		printf("%s\n", math_expression);

		closing_brkts_ptr = strchr(math_expression, ')');
	}

	return result;
}

int main(int argc, char *argv[])
{
	int result = 0;

	if (argc != 2)
	{
		printf("Illegal number of arguents! Try again\n");
		return ERROR_CODE;
	}

	printf("%s\n", argv[1]);
	result = calculateResult(argv[1]);
	return 0;
}