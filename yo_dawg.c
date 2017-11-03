#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "check.h"
#include "eval.h"
#include "lists_and_arrays.h"
#include "settings.h"
#include "yo_dawg.h"


/************************************yo_dawg Function******************************************
the yo_dawg function is used to implement the order of operations for spell checking a text document.
The function populates the setting struct with the getSettings() function which gets the parameters 
from the spellrc file. It then populates a linked list with the contents of the dictionary with the 
function populate list. it then makes a dynamically allocated 2d array to hold the contents of the 
dictionary linked list with the function makeArray(). the populateList() and makeArray() function
procedure is then repeated for the text file supplied by the user. The function then sets the autosetting
function pointer to autoYes() or userSelect() based on the autocorrect setting field. it the uses the 
provided check() function to correct the spelling of the user file with the dictionary file. 
The function then prints the corrected array to the user file, replacing the original information.
It finally frees all the dynamically allocated memory that was reserved. the function returns nothing.
**********************************************************************************************/
void yo_dawg_can_you_check_this_for_me(char* filename, FILE* userFileR)
{
	settings* setting = (settings*)malloc(sizeof(settings));
	LinkedList* dictList = NULL;
	LinkedList* userList = NULL;
	char** dictArray = NULL;
	char** userArray = NULL;
	int dictLength = 0;
	int userLength = 0;
	int flag = 0;
	ActionFunc autosetting;
	FILE* userFileW;
	
	
	/*populate setting struct fields from the spellrc file*/
	/*getSettings() function code/description is in settings.c*/
	flag = getSettings(setting);

	if(flag)
	{
	
		/*loads contents of dictionary into linked list and then into an array*/
		/*populateList() and makeArray() function code/descriptions are in lists_and_arrays.c*/
		dictList = populateList(setting->dictionary);
		dictArray = makeArray(dictList, &dictLength);
	
		/*loads contents of dictionary into linked list and then into an array*/
		/*populateList() and makeArray() function code/descriptions are in lists_and_arrays.c*/
		userList = populateList(filename);
		userArray = makeArray(userList, &userLength);
		
		/*set autosetting pointer based on autocorrect field in setting struct*/
		/*getAutoSetting() function code/description is in settings.c*/

 		autosetting = getAutoSetting(setting);
 		/*perform spell check with provided check() function*/	
 		/*check() function code/description is in check.c*/		
 		check(userArray, userLength, dictArray, dictLength, setting->correction, *autosetting);
 	
 		
 		/*close user file to read, then open user file to write*/
 		fclose(userFileR);
 		userFileW = fopen(filename,"w");
 		/*update user file with corrected information and close user file to write*/
 		/*printCorrectedArray() function code/description is in lists_and_arrays.c*/
 		printCorrectedArray(userFileW, userArray, userLength);
		fclose(userFileW);
		/*free all dynamically allocated memory*/
		freeArray(dictArray, dictLength);
		freeArray(userArray,userLength);
		freeList(dictList);
		freeList(userList);
	}
	free(setting);
}
