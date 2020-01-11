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
	if (strncmp(move, "ROCK",4) == 0)
	{
		return ROCK;
	}
	if (strncmp(move, "PAPER",5) == 0)
	{
		return PAPER;
	}
	if (strncmp(move, "SCISSORS",7) == 0)
	{
		return SCISSORS;
	}
	if (strncmp(move, "LIZARD",6) == 0)
	{
		return LIZARD;
	}
	if (strncmp(move, "SPOCK",5) == 0)
	{
		return SPOCK;
	}
	return UNDEFINED_MOVE;
}
void AllocateFullParamList(char ***param_list) {
	// dynamically create array of pointers 
	*param_list = (char **)malloc(PROTOCOL_PARAM_LIST_SIZE * sizeof(char *));

	// dynamically allocate memory for each row
	for (int r = 0; r < PROTOCOL_PARAM_LIST_SIZE; r++)
		(*param_list)[r] = (char *)malloc(PARAM_STR_MAX_LEN * sizeof(char));
}
void FreeFullParamList(char ***param_list) {
	for (int i = 0; i < PROTOCOL_PARAM_LIST_SIZE; i++)
		free((*param_list)[i]);
	free(*param_list);
}