CC = gcc
CFLAGS = -c -Wall -pedantic -ansi -g

OBJ1 = main.o settings.o lists_and_arrays.o check.o yo_dawg.o
EXEC1 = SpellChecker

$(EXEC1) : $(OBJ1)
	$(CC) $(OBJ1) -o $(EXEC1)

main.o : main.c check.h eval.h lists_and_arrays.h settings.h
	$(CC) main.c $(CFLAGS)

settings.o : settings.c settings.h check.h
	$(CC) settings.c $(CFLAGS)

lists_and_arrays.o : lists_and_arrays.c lists_and_arrays.h 
	$(CC) lists_and_arrays.c $(CFLAGS)

yo_dawg.o : yo_dawg.c yo_dawg.h check.h eval.h lists_and_arrays.h settings.h
	$(CC) yo_dawg.c $(CFLAGS)


clean :
	rm -f *.o *.*~ *~
