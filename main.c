#include <stdio.h>
#include "yo_dawg.h"

/**************************************************************************************************
program checks the spelling of a file and updates the file with corrections. It takes a single command
line argument which is the name of the file to be checked. the file is only checked if the correct 
number of command line arguments are entered and the user file can be opened.
***************************************************************************************************/
int main(int argc, char** argv)
{
	
	FILE* userFileR = fopen(argv[1],"r");  	
		
	if(argc == 2) 
	{
		
		if (userFileR != NULL)
		{
			/*this function description/code is in yo_dawg.c*/
			yo_dawg_can_you_check_this_for_me(argv[1], userFileR);
		}
		else
		{
			printf("\n\nError: File can not be opened\n");
		}
	}
	else
	{
		printf("\n\nError: Wrong number of arguments\n");
	}
	
	return 0;
}