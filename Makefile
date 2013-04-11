# Makefile for pr7 shell

INC = pr7.3.c wrapper.c builtin.c
LIB = pr7.h wrapper.h builtin.h

pr7 : $(INC) $(LIB)
	gcc -std=c99 -Wall -Wextra -pedantic -o pr7 $(INC) $(LIB)

clean :
	rm -f pr7 a.out *.o
	
