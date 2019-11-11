#include <stdio.h>
#include "Son_functions.h"
/*The following function gets as an input the argv[1] string, parse it and returns the solution of the math phrase*/
int ParseAndCalculate(char *mathstring)
{
	int i = 0, X = 0, Y = 0, flag_Y=0, flag_PLUS=0;
	char curr_num;
	while (mathstring[i] != '\0')
	{
		curr_num = mathstring[i];
		i++;
		if (curr_num == PLUS_SIGN || curr_num == MULTIPLE_SIGN)
		{
			flag_Y = 1;
			if (curr_num == PLUS_SIGN)
			{
				flag_PLUS = 1;
			}
			continue;
		}
		if (!flag_Y)
		{
			X = X * 10 + (curr_num -'0');
		}
		else 
		{
			Y = Y * 10 + (curr_num - '0');
		}
	}
	if (flag_PLUS)
	{
		return X + Y;
	}
	return X * Y;
}
/*The following fuction returns an integer represented by a string*/
int StringToInteger(char strnumber[])
{
	return (int)strtol(strnumber, (char **)NULL, 10); /* Built-in function for converting string to int with the assumption the string is legal*/
}