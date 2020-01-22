#include "utils.h"
#include <ctype.h>
#include <sys/stat.h>
#include <math.h>

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
/*Function for malloc*/
ErrorCode_t AllocateString(char **str_ptr, int len) {
	if (NULL == (*str_ptr = (char *)malloc(len)))
	{
		printf("Memory Allocation failed! Try again...\n");
		return(MALLOC_STRING_FAIL);
	}
	return SUCCESS;
}
/*Funtction that rounds float #p points after the dot*/
double Round(double x, int p)
{
	if (x != 0.0) {
		return ((floor((fabs(x)*pow((double)(10.0), p)) + 0.5)) / pow((double)(10.0), p))*(x / fabs(x));
	}
	else {
		return 0.0;
	}
}
/*Check if a file exist*/
bool IsFileExists(char* filename)
{
	struct stat buffer;
	int exist = stat(filename, &buffer);
	if (exist == 0)
	{
		/*file exists*/
		return true;
	}
	else
	{
		/*file not found*/
		return false;
	}
}