#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "hard_coded_data.h"
#include "process_handler.h"
#include "math_expression_parser.h"

void runParsing(char *math_expression) {
	int result = 0, next_close_brks_ind = 0, output_ind = 0;
	char* next_close_brkt_ptr;
	char simple_math_exp[SIMPLE_MATH_STRING_MAX_LEN];
	FILE *fp;
	fp = fopen("Computation.txt", "w"); //ILAY FILE EXCEPTIONS
	next_close_brkt_ptr = strchr(math_expression, ')');
	fprintf(fp, "%s\n", math_expression);
	while (next_close_brkt_ptr != NULL) {
		output_ind++;
		next_close_brks_ind = (int)(next_close_brkt_ptr - math_expression);
		extractSimpleMathExpression(simple_math_exp, math_expression, next_close_brks_ind);
		result = calcResultUsingSon(simple_math_exp); //using other process...
		updateMathExpWithResult(math_expression, simple_math_exp, result);
		fprintf(fp, "%s\n", math_expression);

		next_close_brkt_ptr = strchr(math_expression, ')');
	}
	fclose(fp);

}

int main(int argc, char *argv[])
{
	int result = 0;
	if (argc != 2)
	{
		printf("Illegal number of arguents! Try again\n");
		return ERROR_CODE;
	}

	printf("%s\n", argv[1]); //debug

	runParsing(argv[1]);
	return SUCCESS_CODE;
}