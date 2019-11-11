#include <stdio.h>
#include "Son_functions.h"

int main(int argc, char *argv[])
{
	int result;
	if (argc != 2)
	{
		printf("Illegal number of arguents! Try again\n");
		return ERROR_CODE;
	}
	result = ParseAndCalculate(argv[1]);
	//printf("%d", result);
	return result;
}
