/* CMPSC 311, Spring 2013, Project 7
 * 
 * Author: Jacob Jones
 * Email: jaj5333@psu.edu
 * 
 * Author: Scott Cheloha
 * Email: ssc5145@psu.edu
 */

#ifndef WRAPPER_H
#define WRAPPER_H

#include <stdio.h>
#include <sys/types.h>
#include <signal.h>

//------------------------------------------------------------------------------

// utility functions
//   safe_string(() -- guard against null pointer for character string output

const char *safe_string(const char *str);
typedef void (*sighandler_t)(int);

// Error-checking wrappers for some common functions
//

void *Malloc(size_t size,
	const char *func, const int line);

char *Strdup(const char *s,
	const char *func, const int line);

FILE *Fopen(const char * restrict filename, const char * restrict mode,
	const char *func, const int line);

int Fclose(FILE *stream, const char *func, const int line);

pid_t Fork(void);

int Kill(pid_t pid, int sig);

sighandler_t Signal(int signum, sighandler_t handler);

//------------------------------------------------------------------------------

#endif /* WRAPPER_H */