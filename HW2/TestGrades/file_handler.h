#pragma once
#define MAX_LINE_LENGTH 4
#define NUM_OF_GRADES 13 // DEBUG NEEDS TO BE IN COMMON
#define MAX_FILENMAE_LENGTH 12 //DEBUG
#define MAX_CMD_LENGTH 256 //DEBUG
int WritefileToGradesArray(char *filepath, int *grade_ptr, char *gradetype_ptr);