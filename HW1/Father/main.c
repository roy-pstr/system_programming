#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "hard_coded_data.h"
#include "process_handler.h"
#include "math_expression_parser.h"

void runParsing(char *math_expression, char **parser_outputs) {
	int result = 0, next_close_brks_ind = 0, output_ind = 0;
	char* next_close_brkt_ptr;
	char simple_math_exp[SIMPLE_MATH_STRING_MAX_LEN];

	next_close_brkt_ptr = strchr(math_expression, ')');
	writeStringToOutputsArray(parser_outputs, output_ind, math_expression);
	while (next_close_brkt_ptr!=NULL) {
		output_ind++;
		next_close_brks_ind = (int)(next_close_brkt_ptr - math_expression);
		extractSimpleMathExpression(simple_math_exp, math_expression, next_close_brks_ind);
		result = calcResultUsingSon(simple_math_exp);
		updateMathExpWithResult(math_expression, simple_math_exp, result);
		writeStringToOutputsArray(parser_outputs, math_expression);
		printf("%s\n", math_expression);

		next_close_brkt_ptr = strchr(math_expression, ')');
	}
}
 
void releaseOutputsMemory(char **parser_outputs) {

}

void writeParsingsToFile(char **parser_outputs) {

}

int main(int argc, char *argv[])
{
	int result = 0;
	char **parser_outputs;

	if (argc != 2)
	{
		printf("Illegal number of arguents! Try again\n");
		return ERROR_CODE;
	}

	parser_outputs = malloc(1*sizeof(char*));
	if (parser_outputs) {
		parser_outputs[0] = malloc(MATH_STRING_MAX_LEN);
	}
	else {
		printf("memory allocation have been failed.\n");
		return ERROR_CODE;
	}

	printf("%s\n", argv[1]); //debug
	runParsing(argv[1], parser_outputs);
	writeParsingsToFile(parser_outputs);
	releaseOutputsMemory(parser_outputs);
	return SUCCESS_CODE;
}