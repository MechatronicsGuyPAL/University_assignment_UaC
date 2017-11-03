#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "settings.h"
#include "check.h"


/********************************************getSettings Function**********************************
The get settings function is used to populate an existing struct of type 'settings'. the settings 
struct has three fields; autocorrect, settings, and maxcorrecting. The function opens the spellrc 
file, scans a single line and copies it into a string local variable. The string is scanned for 
three key words that match each of the three struct fields. if a keyword is detected then the 
string is read to populate the corresponding struct field. The function returns true if each line
of the spellrc file contains a proper setting and the file contains each of the three types of settings.
 the function returns false if both of those conditions are not met.
****************************************************************************************************/
int getSettings(settings* setting)
{
	int state = 0;
	int cor = 0;
	int dict = 0;
	int autoC = 0;
	int flag = 1;
	char currentLine[500];
	char checkCorrection[] = "maxcorrection";
	int maxCorrection;
	char checkDict[] = "dictionary";
	char dictionary[52];
	char checkAuto[] = "autocorrect";
	char autocorrect[4];

	FILE* file = fopen("spellrc","r"); 

	/*scans the file line by line untill end of file is reached. check each line for the three keywords*/
	do
	{
		fgets(currentLine, 500, file);
		state = 0;					/*variable to check if any keywords are in current line*/
		if(strstr(currentLine,checkCorrection)!=NULL) /* checks line for keyword*/				
		{
			state = 1;
			cor = 1;		/*variable to check of maxcorrection is read at least once*/
			sscanf(currentLine, "maxcorrection = %d", &maxCorrection);
			setting->correction = maxCorrection;
				
		}

		if(strstr(currentLine,checkDict)!=NULL) /* checks line for keyword*/
		{
			state = 1;
			dict = 1; 		/*variable to check of dictionary is read at least once*/
			sscanf(currentLine, "dictionary = %s", dictionary); 
			strcpy(setting->dictionary, dictionary);
				
		}

		if(strstr(currentLine,checkAuto)!=NULL) /* checks line for keyword*/
		{
			state = 1;
			autoC = 1; 		/*variable to check of autocorrect is read at least once*/
			sscanf(currentLine, "autocorrect = %s", autocorrect);
			strcpy(setting->autocorrect, autocorrect);
		}

		if(state != 1)
		{
			printf("\nProgram stopped.\nSpellrc file format error.\n\n");
			flag = 0;
		}

	}while((feof(file)==0)&&(flag!=0));

	fclose(file);
	

	if(autoC != 1)
	{
		printf("Error: Missing 'autocorrect' setting in spellrc file.\n\n");
		flag = 0;
	}
	if(dict != 1)
	{
		printf("Error: Missing 'dictionary' setting in spellrc file.\n\n");
		flag = 0;
	}
	if(cor != 1)
	{
		printf("Error: Missing 'maxcorrection' setting in spellrc file.\n\n");
		flag = 0;
	}

	return flag;
}




/********************************** autoYes Function ****************************************
the autoYes function takes in pointers to a word that has been identified and the suggested
replacement for it. The the function returns a 1 signifying that the correction is to be used 
unless the suggestion pointer is set to null, in which case the function returns 0 signifying 
that the original word is to be kept. autoYes is the counter part to the userSelect function. 
Either the userSelect or autoYes function is used by the check function. If the spellrc 
autocorrect setting is "yes" then autoYes is used by the check function, if the spellrc 
autocorrect setting is "no" then userSelect is used by the check function. the selection of the
two functions is made with the getAutoSetting function.
**********************************************************************************************/

int autoYes(char* word, char* suggestion)
{
	int returnval = 1;
	if (suggestion == NULL)
	{
		returnval = 0;
	}
return returnval;
}

/********************************** userSelect Function ****************************************
the userSelect function takes in pointers to a word that has been identified and the suggested
replacement for it. The user is given the option to keep the original word or to accept the 
recommended change. The user is propted to enter "Y" or "N", and reprompted if any other letter 
is entered. the function returns true if the change is to be accepted and false if the change is not 
accepted or if the suggestion pointer equals null. The userSelect function is the counter part to the 
autoYes function. Either the userSelect or autoYes function is used by the check function. If 
the spellrc autocorrect setting is "yes" then autoYes is used by the check function, if the spellrc 
autocorrect setting is "no" then userSelect is used by the check function. the selection of the
two functions is made with the getAutoSetting function.
**********************************************************************************************/

int userSelect(char* word, char* suggestion)
{
	int returnval = 2;
	char decision;
	if (suggestion == NULL)
	{
		returnval = 0;
	}
	else
	{
		printf("The word '%s' is misspelled. Replace with '%s'? (Y/N)\n", word, suggestion);
		scanf(" %c", &decision);
		do
		{
			if(returnval >= 3)
			{
				printf("Unexpected user information. Enter 'Y' for Yes or 'N' for No.\n");
				scanf(" %c",&decision);
			}
		
			if((strncmp(&decision,"y",1)==0) || (strncmp(&decision,"Y",1)==0))
			{
				returnval = 1;
			}

			else if((strncmp(&decision,"n",1)==0)||(strncmp(&decision,"N",1)==0))
			{
				returnval = 0;
			}

			else if((returnval != 0) || (returnval != 1))
			{
				returnval = 3;
			}
		}while(returnval>=2);
		
	}
	return returnval;
}






/********************************** getAutoSetting Function ****************************************
The getAutoSetting function determines whether the check function should use the autoYes or userSelect
function. the function takes in the populated setting struct and returns a function pointer of type
ActionFunc. The function checks the autocorrect field of the setting file. if the field is set to a 
reasonable varient of yes then the autoYes function is selected, otherwise the userSelect function 
is selected. a pointer to the chosen function is then returned.
**********************************************************************************************/
ActionFunc getAutoSetting(settings* setting)
{
	ActionFunc autosetting;

	if((strcmp((setting->autocorrect),"yes")==0)|(strcmp((setting->autocorrect),"Yes")==0)|(strcmp((setting->autocorrect),"YES")==0))
 	{
 		autosetting = &autoYes;
 	}
 	else
 	{
 		autosetting = &userSelect;
 	}

 	return autosetting;
}
