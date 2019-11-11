#include <stdio.h>
#include "Father_functions.h"
int main()
{
	int ecode = GetSonExitCode("Son.exe 40+2");
	printf("Son Exit Code: %d\n", ecode);
	return 0;
}