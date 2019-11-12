#include <stdio.h>
#include "hard_coded_defiens.h"
#include "process_funcs.h"

int main(int argc, char *argv[])
{

	if (argc != 2)
	{
		printf("Illegal number of arguents! Try again\n");
		return ERROR_CODE;
	}

	printf("%s", argv[1]); //debug


	int ecode = GetSonExitCode("Son.exe 40+2");
	printf("Son Exit Code: %d\n", ecode);
	return 0;
}