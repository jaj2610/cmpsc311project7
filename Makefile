# Makefile for pr7 shell
#CMPSC 311, Spring 2013, Project 7
#
#Author:		Jake Jones
#Email:		jaj5333@psu.edu
#
#Author:		Scott Cheloha
#Email:		ssc5145@psu.edu
 
INC = pr7.3.c wrapper.c builtin.c linked.c
LIB = pr7.h wrapper.h builtin.h linked.h

pr7 : $(INC) $(LIB)
	gcc -std=c99 -Wall -Wextra -pedantic -o pr7 $(INC) $(LIB)

clean :
	rm -f pr7 a.out *.o