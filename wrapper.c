/* CMPSC 311, Spring 2013, Project 7
 *
 * Author:   Scott Cheloha
 * Email:    ssc5145@psu.edu
 *
 * originally by: Don Heller
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

#include "wrapper.h"

//------------------------------------------------------------------------------

// utility functions
//   safe_string(() -- guard against null pointer for character string output

const char *safe_string(const char *str)
{
  if (str == NULL) return "(null)";
  else return str;
}

//------------------------------------------------------------------------------

// check function return values
//   function	standards
//   malloc	C and Posix
//   strdup	      Posix
//   fopen	C and Posix
// We follow the standard protoypes of the original functions.
// Compare these to the error-checking wrappers in CS:APP, csapp.h and csapp.c.

// #define Malloc(size) cmpsc311_malloc(size, __func__, __LINE__)

void *Malloc(size_t size, const char *func, const int line)
{
  void *p = malloc(size);
  if (p == NULL)
    {
      fprintf(stderr, "%s: %s() at line %d failed: malloc(): %s\n",
	prog, func, line, strerror(errno));
    }

  if (d_flag)
    { // which address?
      fprintf(stderr, "%s: malloc(%zd) at %p from %s line %d\n",
	prog, size, p, func, line);
    }

  return p;
}

char *Strdup(const char *s, const char *func, const int line)
{
  char *p = strdup(s);
  if (p == NULL)
    {
      fprintf(stderr, "%s: %s() at line %d failed: strdup(): %s\n",
	prog, func, line, strerror(errno));
    }

  if (d_flag)
  { // which address?
      fprintf(stderr, "%s: strdup(%zd) at %p from %s line %d\n",
	prog, strlen(s)+1, (void *) p, func, line);
  }

  return p;
}

FILE *Fopen(const char * restrict filename, const char * restrict mode,
	const char *func, const int line)
{
  FILE *f = fopen(filename, mode);
  if (f == NULL)
  {
      fprintf(stderr, "%s: %s() at line %d failed: fopen(%s): %s\n",
	prog, func, line, filename, strerror(errno));
  }

  return f;
}

pid_t Fork(void)
{
	pid_t pid;

	if ((pid = fork()) < 0) {
		fprintf(stderr, "fork() failed: %s", strerror(errno));
	}

	return pid;
}

//------------------------------------------------------------------------------

