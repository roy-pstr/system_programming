//Description – Handling strings represting math expression with only + and * operators and each operator and two numbers is 
//				Surronded by barckets.

#ifndef _MATH_EXPRESSION_PARSER_H
#define _MATH_EXPRESSION_PARSER_H

int extractSimpleMathExpression(char simple_expression[], const char *math_expression, int closing_barckets_ind);

int updateMathExpWithResult(char *math_expression,const char *simpleMathExp, int result);

#endif