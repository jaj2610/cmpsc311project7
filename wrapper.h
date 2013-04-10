/* CMPSC 311, Spring 2013, Project 7
 *
 * Author:   Scott Cheloha
 * Email:    ssc5145@psu.edu
 *
 * originally by: Don Heller
 */

#ifndef WRAPPER_H
#define WRAPPER_H

#include <stdio.h>
#include <sys/types.h>

//------------------------------------------------------------------------------

// utility functions
//   safe_string(() -- guard against null pointer for character string output

const char *safe_string(const char *str);

//------------------------------------------------------------------------------

// check function arguments
//   verify() -- tf is expected to be true; if not, print msg and quit

#define verify(tf, msg) cmpsc311_verify(tf, msg, __func__, __LINE__)

void cmpsc311_verify(const bool tf,
	const char *msg, const char *func, const int line);

//------------------------------------------------------------------------------

// Error-checking wrappers for some common functions
//

void *Malloc(size_t size,
	const char *func, const int line);

char *Strdup(const char *s,
	const char *func, const int line);

FILE *Fopen(const char * restrict filename, const char * restrict mode,
	const char *func, const int line);

pid_t Fork(void);

//------------------------------------------------------------------------------

#endif

