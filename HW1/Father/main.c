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