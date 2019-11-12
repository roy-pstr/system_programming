#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#include "Son_functions.h"


int parseAndCalculate(char *mathstring)
{
	/*input: string in the following formats:
		X+Y
		X*Y
		where X and Y are integers.
	return: the mathmetical result of the given equation.

	The function parses the input string to first int, second int and math operation
	and calculates the result.*/

	int str_ind = 0, X = 0, Y = 0;
	bool flag_Y = false, flag_PLUS = false;
	char curr_char;

	while (mathstring[str_ind] != '\0')
	{
		curr_char = mathstring[str_ind];
		str_ind++;
		if (curr_char == PLUS_SIGN || curr_char == MULTIPLE_SIGN)
		{
			flag_Y = true;
			if (curr_char == PLUS_SIGN)
			{
				flag_PLUS = true;
			}
			continue;
		}
		if (flag_Y==false)
		{
			X = X * 10 + (curr_char -'0');
		}
		else 
		{
			Y = Y * 10 + (curr_char - '0');
		}
	}

	if (flag_PLUS==true)
	{
		return X + Y;
	}
	return X * Y;
}
