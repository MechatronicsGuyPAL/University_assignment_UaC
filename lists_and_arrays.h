#ifndef LLIST_H
#define LLIST_H

/******************************** Node Struct ********************************
The node struct is used to hold data in the form of a linked list. each node is 
dynamically allocated and holds an array with space for a string of 50 characters 
or less, and a pointer to the next and previous nodes in the list.
*******************************************************************************/
typedef struct Node
{
	char data[52];
	struct Node* next;
	struct Node* previous;
}Node;

/******************************LinkedList Struct*******************************
The LinkedList struct is used to keep track of a linked list of data. It is 
dynamically allocated and holds pointers to the head and tail of the list.
******************************************************************************/
typedef struct LinkedList
{
	Node* head;
	Node* tail;
}LinkedList;



LinkedList* createList(void);
void insertLast(LinkedList* list, char*);
int listCount(LinkedList* list);
void freeList(LinkedList* list);
LinkedList* populateList(char* fileName);
char** makeArray(LinkedList* list, int* size);
void freeArray(char** array, int length);
void printCorrectedArray(FILE* filename, char** array, int length);

#endif