//Authors – Roy Pasternak(204219273) & Ilay Amar(308520857)
//Project – EX1
//Description – Father project's main.c file. it's goal is to calculate a math expression step by step by splitting the math expression to
//				simpler expression in the X+Y or X*Y  format and sending those expression to 'SON' program.

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "hard_coded_data.h"
#include "process_handler.h"
#include "math_expression_parser.h"

#define OUTPUT_TXT_FILE "Computation.txt"

/*
	input:
		- math_expression : string represnting math expression which obey to the rule of using barckets for each mathemtical sign calculation.
		- fp : output file pointer

		description:
			The function loop over the math expression and calculate it's final result output
		
		output:
			all the outputs of the function are written to '*fp'.
*/
void calcMathExpStepByStep(char *math_expression, FILE *fp) {
	int result = 0, next_close_brks_ind = 0, output_ind = 0;
	char* next_close_brkt_ptr;
	char simple_math_exp[SIMPLE_MATH_STRING_MAX_LEN];
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
	//check inputs
	if (argc != 2)
	{
		printf("Illegal number of arguents! Try again\n");
		return ERROR_CODE;
	}

	//open file
	FILE *fp;
	if (NULL == (fp = fopen(OUTPUT_TXT_FILE, "w"))) {
		printf("File ERROR\n");
		return ERROR_CODE;
	}
	
	//run
	calcMathExpStepByStep(argv[1], fp);
	return SUCCESS_CODE;
}