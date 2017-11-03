FILES INCLUDED:
	-main.c
	-Makefile
	-spellrc - contains the three settings used as the parameters for the program
	-linux.words - supplied dictionary file
	-yo_dawg.c - contains the function yo_dawg_can_you_check_this_for_me()
		which controlls the flow of the program and the function calls used
		in that flow.
	-yo_dawg.h
	-lists_and_arrays.c - contains all functions pertaining to linked lists and arrays
	-lists_and_arrays.h
	-settings.c - contains all functions pertaining to the setting parameters for the program
	-settings.h
	-check.c - supplied check function
	-check.h
	-eval.c - supplied eval function
	-eval.h

ASSIGNMENT COMPLETION:
	The program sucessfully completes the following:
	-Accept a sinlge command-line parameter.
	-Read the settings file "spellrc" and stores it's contents in a struct.
	-Read the dictionary file and places the contents into a linked list
		-copies the linked list to a dynamically allocated array.
	-Read the user file and places the contents into a linked list
		-copies the linked list to a dynamically allocated array.
	-Invoke the pre-existing check() function and the callback function
		of specified type ActionFunc.
	-Write the corrected user file array back to the original user text file.

VALGRIND ISSUES:
	No memory leaks are possible, no errors reported(suppressed:12 from 8).

TEST INFORMATION:
	The program was sucessfully tested with:
	Room #: 314.219
	Service Tag: BDWZF2S
