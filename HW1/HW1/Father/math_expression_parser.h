//Description – Handling strings represting math expression with only + and * operators and each operator and two numbers is 
//				Surronded by barckets.

#ifndef _MATH_EXPRESSION_PARSER_H
#define _MATH_EXPRESSION_PARSER_H
#define OUTPUT_TXT_FILE "Computation.txt"
#define INT_STRING_MAX_SIZE 11
void intToStr(int num, char* num_str);
int extractSimpleMathExpression(char simple_expression[], const char *math_expression, int closing_barckets_ind);
int updateMathExpWithResult(char *math_expression,const char *simpleMathExp, int result);
void calcMathExpStepByStep(char *math_expression, FILE *fp);
#endif