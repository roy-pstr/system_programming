//Description – Handling strings represting math expression with only + and * operators and each operator and two numbers is 
//				Surronded by barckets.

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "math_expression_parser.h"
#include "hard_coded_data.h"



void intToStr(int num, char* num_str) {
	sprintf(num_str, "%d", num);
}

/*findFirstLeftOpeaningBarckets:
	input:
		math_expression - a string
		string_ing - start pos to begin the search from

	description:
		The function start sreaching from math_expression[string_ing] towards the start of the string
		for '('.
	returns:
		if found, returns the '(' index position in string. if not found return -1
*/
int searchForLeftOpeaningBarckets(const char *math_expression, int string_ind) {
	while (string_ind >= 0) {
		string_ind--;
		if (math_expression[string_ind]=='(') {
			return string_ind;
		}
	}
	return -1; //not found
}

/*extractSimpleMathExpression:
	input:
		math_expression - a string
		closing_barckets_ind - a index in the string

	description - the function copy a substring from 'math_expression' and write it to the output.
				the substring is all the charcters left to the given index until the first '(' found.

	output:
		simple_expression - a substr of 'math_expression'
*/
int extractSimpleMathExpression(char simple_expression[], const char *math_expression, int closing_barckets_ind)
{
	int simple_exp_len = 0;
	int open_barckets_ind = searchForLeftOpeaningBarckets(math_expression, closing_barckets_ind);
	if (open_barckets_ind == -1) {
		return -1;
	}
	else {
		simple_exp_len = closing_barckets_ind - open_barckets_ind-1;
		strncpy_s(simple_expression, SIMPLE_MATH_STRING_MAX_LEN, &math_expression[++open_barckets_ind], simple_exp_len);
	}
	return 0;
}

/*updateMathExpWithResult:
	input:
		math_expression - a string
		simple_math_exp - a substring
		result

	output:
		math_expression- a string

	description:
	The function search for substr 'simple_math_exp' inside the string 'math_expression' and replace it
	and one char to the left of it and one char to the right of it (those are the barckets) with the string 
	representetive of 'result'
*/
int updateMathExpWithResult(char *math_expression, const char *simple_math_exp, int result)
{
	char result_str[INT_STRING_MAX_SIZE], math_expression_temp[MATH_STRING_MAX_LEN];
	char *substr_start_ptr;
	int distance_to_begining = 0, distance_to_end = 0, tail_len = 0;

	intToStr(result, &result_str);
	if (NULL == (substr_start_ptr = strstr(math_expression, simple_math_exp)) )
	{
		printf("can not find simple_math_string in math_expression");
		return ERROR_CODE;
	}
	distance_to_begining = substr_start_ptr - math_expression-1; 
	distance_to_end = distance_to_begining + strlen(simple_math_exp) + 2; // the +2 is for skipping also the ( .. )
	strncpy_s(math_expression_temp, sizeof(math_expression_temp), math_expression, distance_to_begining); //copy math_exp until the substr
	strcat_s(math_expression_temp, sizeof(math_expression_temp), result_str); // add the substr the end of the string
	strcat_s(math_expression_temp, sizeof(math_expression_temp), &math_expression[distance_to_end]); //copy the rest of the math exp to the end of the modified string
	strcpy_s(math_expression, strlen(math_expression), math_expression_temp); //copy temp back to the original varaible.
	return SUCCESS_CODE;
}

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
