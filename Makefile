# Makefile for pr7 shell

INC = pr7.3.c wrapper.c
LIB = pr7.h wrapper.h

pr7 : $(INC) $(LIB)
	gcc -std=c99 -Wall -Wextra -pedantic -o pr7 $(INC) $(LIB)

clean : 
	rm pr7*.o
	rm pr7
