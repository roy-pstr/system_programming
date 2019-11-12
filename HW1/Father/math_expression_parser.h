#pragma once

int extractSimpleMathExpression(char simple_expression[], const char *math_expression, int closing_barckets_ind);

int updateMathExpWithResult(char *math_expression,const char *simpleMathExp, int result);

void padWithBarckets(char * simpleMathExp);
