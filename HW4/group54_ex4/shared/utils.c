#include "utils.h"
#include <ctype.h>

void UpperString(char* str) {
	char *s = str;
	while (*s) {
		*s = toupper((unsigned char)*s);
		s++;
	}
}
MOVES_ENUM StringToEnum(char *move)
{
	UpperString(move);
	if (strcmp(move, "ROCK") == 0)
	{
		return ROCK;
	}
	if (strcmp(move, "PAPER") == 0)
	{
		return PAPER;
	}
	if (strcmp(move, "SCISSORS") == 0)
	{
		return SCISSORS;
	}
	if (strcmp(move, "LIZARD") == 0)
	{
		return LIZARD;
	}
	if (strcmp(move, "SPOCK") == 0)
	{
		return SPOCK;
	}
	return UNDEFINED_MOVE;
}

/*Function for malloc*/
ErrorCode_t AllocateString(char **str_ptr, int len) {
	if (NULL == (*str_ptr = (char *)malloc(len)))
	{
		printf("Memory Allocation failed! Try again...\n");
		return(MALLOC_STRING_FAIL);
	}
	return SUCCESS;
}