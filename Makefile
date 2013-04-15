# Makefile for pr7 shell

# CMPSC 311, Spring 2013, Project 7
#
# Author:		Jacob Jones
# Email:		jaj5333@psu.edu
#
# Author:		Scott Cheloha
# Email:		ssc5145@psu.edu
#
 
INC = pr7.3.c wrapper.c builtin.c linked.c exec.c
LIB = pr7.h wrapper.h builtin.h linked.h exec.h

pr7-gcc : $(INC) $(LIB)
	gcc -std=c99 -Wall -Wextra -pedantic -o pr7 $(INC) $(LIB)

# NOTE: Does not fully function on solaris and linux systems
pr7-sun-c99 : $(INC) $(LIB)
	c99 -v -D_POSIX_C_SOURCE=200112L -D_XOPEN_SOURCE=600 -o pr7 $(INC) $(LIB)

pr7-sun-gcc : $(INC) $(LIB)
	gcc -std=c99 -Wall -Wextra -pedantic -D_POSIX_C_SOURCE=200112L -D_XOPEN_SOURCE=600 -o pr7 $(INC) $(LIB)

pr7-linux : $(INC) $(LIB)
	gcc -std=c99 -Wall -Wextra -pedantic -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=700 -o pr7 $(INC) $(LIB)

clean :
	rm -f pr7 a.out *.o
