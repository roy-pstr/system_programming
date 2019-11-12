#pragma once
//char* findNextClosingBrackets(const char *math_expression);

void extractSimpleMathExpression(char simple_expression[], const char *math_expression, int closing_barckets_ind);

int replaceMathExpWithResult(char *math_expression,const char *simpleMathExp, int result);