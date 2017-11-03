#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lists_and_arrays.h"




/*************************************creatList Function*************************************
The creatList function dynamically allocates space for a linked list with the struct
type LinkedList. It takes in nothing, and returns a pointer to the empty linked list. the head
and tail pointers are initialized pointing to NULL. It is used to create lists for the dictionary 
file specified in the spellrc file, and the user text file specified as a command line argument.
It is used in the populate list function.
*********************************************************************************************/
LinkedList* createList(void)
{
	LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList)); 	/*allocate space for list*/
	(*list).head = NULL;								/*set head and tail to null to make empty list*/
	(*list).tail = NULL;
	return list;
}





/*************************************insertLast Function*************************************
The insertLast function inserts a new node of type struct Node into a linked list. the function 
takes a pointer to the list which is to be used, and the data which is to be stored. the node 
is dynamically allocated, then the new data copied into it. the node next pointer is set to null,
previous pointer set to the list tail. if the list is empty then the list head and tail pointers
are set to the new node, if the list is not empty then the last node's next pointer and the list
tail pointer are set to the new node. the function is used in the populate list function.
*********************************************************************************************/
void insertLast(LinkedList* list, char newData[52])
{
	Node* newNode = (Node*)malloc(sizeof(Node));		/*allocate space for new node*/
	strcpy(newNode->data, newData);							/*copy data into new node*/
	newNode->previous = NULL;							/*sets previous pointer to null to get rid of any extra data*/
	newNode->next = NULL; 								/*new node next pointer set to null as it is the last node*/
	newNode->previous = list->tail;						/*new node previous pointer set to the old last node*/
	if(list->tail != NULL)								/*test if the list has any data*/
		{
			list->tail->next = newNode;		/*If there is already data in the list then the old last nodes next pointer points to the new node*/
		}
	else
		{
			list->head = newNode;						/*if the list was originally empty then the lists head points to the new node*/
		}
	list->tail = newNode;								/*list tail points to new node*/
}





/*************************************listCount Function*************************************
the listCount function counts the number of nodes in a list. it takes in a pointer to the list
to be counted, and returns a double (in case of extreemly large lists), which is the length
of the list. It is used in the makeArray function.
*********************************************************************************************/
int listCount(LinkedList* list)
{
	int count = 0;							/*counter variable*/
	Node* tempNode;
	if((list->head != NULL) && (list->tail != NULL)) 	/*test if the list has any nodes*/
	{
		tempNode = list->head; 				/*set temp node to the first node*/
		count = 1;
		while (tempNode->next != NULL) 			/* repeatedly set temp node to the next node and incriment i untill the last node is reached*/
		{
			tempNode = tempNode->next;
			count += 1;
		}	
	}
return count;
}





/*************************************freeList Function*************************************
the freeList function frees all the space that was dynamically allocated to make the linked list.
the function creates a temporary node pointer and sets it to the end of the linked list. 
the list tail is set to the next to last node, and the space pointed to by the temp node is freed.
after the last node is freed, the list is freed. the function is used at the end of main to free
the space allocated for the dictionary and user text linked lists.
*********************************************************************************************/

void freeList(LinkedList* list)
{
	Node* tempNode;
	while(list->head != NULL) 				/*continually free the last node untill the list is empty*/
	{
		tempNode = list->tail;				/*set temp node to last node*/
		if((list->head) == (list->tail))
		{
			free(list->tail);
			list->tail = NULL;
			list->head = NULL;
		}
		else
		{		
			list->tail = list->tail->previous; 	/*list tail points to second to last node*/
			list->tail->next = NULL; 			/*set second to last node 'next' to null as it will become the last node*/
			free(tempNode);		
		}
	}
	free(list);	
}





/*************************************populateList Function***********************************
the populateList function creates a linked list from a text file. It takes in the file name of 
the text document and returns a pointer to the populated linked list. The function opens the 
source file to read, scans the first word and saves it to a local variable, creates a linked 
list with the createList function, and inserts the first node with the first word. The function
then repeatedly scans a single word, and inserts a new node with the insertLast function and the 
scanned word until the end of file is reached. the source file is closed and the created list is 
returned. if the source file cannot be opened an error message is printed.
*********************************************************************************************/
  LinkedList* populateList(char* fileName)
  {
  	char currentWord[52];
  	FILE* file = fopen(fileName,"r"); 
  	Node* tempNode;
  	LinkedList* list;

  	if (file != NULL)
  	{	
  		fscanf(file,"%s", currentWord);		/*reads file once and saves string to variable*/
  		list = createList();				/*creates list*/
  		insertLast(list, currentWord);		/*creates first node with variable*/
  		tempNode = list->head;				/*moves curson*/
  	
		while(fscanf(file,"%s",currentWord)==1)
			{
				insertLast(list,currentWord);	/*creates new node with variable*/
				tempNode = tempNode->next;		/*moves curson*/
				currentWord[0]='\0';			/*make local variable empty to avoid double printing when empty lines are read*/
			}
  	}
	else 
	{
		printf("\n\nError: Could not open file to populate list.\n");
	}

	fclose(file);	
	return list;
  }






/*************************************makeArray Function*************************************
the makeArray function makes a dynamically allocated 2d array of strings from a linked list of
strings. the function takes in the list to be used to make the array, and a pointer to a size 
variable to record the size of the 2d array that is created after it is known. the function 
returns a pointer to the 2d array. the function uses the listcount function to determine the number
of entries for the array of strings and then dynamically allocates memory for that number of 
string pointers. for each node in the linked list, the function determines the length of each
string, dynamically allocates space for the strin in the 2d array, and copies the data from the 
linked list node to the 2d array entry. the array size is updated and a pointer to the array is
returned. if the file cannot be read an error message is reported. 
*********************************************************************************************/
char** makeArray(LinkedList* list, int* size)
{
	
	int length = 0;
	Node* tempNode;
	int i = 0;
	char** arrayP;
	
	if((list->head != NULL) && (list->tail != NULL)) 	/*test if the list has any nodes*/
	{

		length = listCount(list);						/*get info for array size*/
		arrayP = (char**)malloc(length*sizeof(char*)); 			/*make space for array*/
		tempNode = list->head;							/*set temp node to the first node*/
					
		for(i = 0; i < length; i++)
		{
			arrayP[i] = ((char*)malloc((strlen(tempNode->data) +1)*sizeof(char)));
			strcpy(arrayP[i],tempNode->data);			/*copy node data into array*/
			tempNode = tempNode->next;
		}
	}
	else
	{
		printf("\n\nError: Cannot make array, list is empty.\n");
	}
*size = length;


return arrayP;
}






/*************************************freeArray Function*************************************
The freeArray function frees the dynamically allocated memory of a 2d array. The function takes
in a pointer to a 2d array and the length of the array and returns nothing. the function frees
the elements of the array (the strings) and then frees the array (pointers to the strings).
*********************************************************************************************/
void freeArray(char** array, int length)
{
	int i = 0;

	for(i=0;i<length;i++)
		{
			free(array[i]);
		}
		free(array);
}




/******************************* printCorectedArray Function *************************************
The printCorectedArray prints the corrected list to the previously specified user text file, overwriting
the old text and replacing it with the text after the check function finishes correcting any spelling.
the function takes in the filename of the user file, the 2d array of words after the correction process, 
and the length of the array. the function returns nothing. the function prints each string in the 2d
array for the length of the array.
***********************************************************************************************/
void printCorrectedArray(FILE* filename, char** array, int length)
{
	int i;
	for(i = 0; i < length; i++)
	{
		fprintf(filename,"%s ", array[i]);
	}
}