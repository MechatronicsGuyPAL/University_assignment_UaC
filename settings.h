#include "check.h"
#ifndef SETTING_H
#define SETTING_H
/****************************************** settings Struct **************************************************
the settings struct is used to hold data from the spellrc file.The struct is initialized in the main and the 
fields of the settings struct are populated with the function getSettings(). the function check() uses the 
maxcorrection value, the function populateList() uses the dictionary value, and getAutoSetting() function
uses the correction value to select one of the two functions, autoYes or userSelect, are to be used with the 
check() function through the use of a function pointer.
**************************************************************************************************************/
typedef struct{
	char autocorrect[4];
	char dictionary[52];
	int correction;
	}settings;

	


int getSettings(settings* setting);
int autoYes(char* word, char* suggestion);
int userSelect(char* word, char* suggestion);
ActionFunc getAutoSetting(settings* setting);

#endif
