/* CMPSC 311, Spring 2013, Project 7
 * 
 * Author: Jacob Jones
 * Email: jaj5333@psu.edu
 * 
 * Author: Scott Cheloha
 * Email: ssc5145@psu.edu
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

#include "wrapper.h"
#include "pr7.h"


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
      fprintf(stderr, "-%s: %s() at line %d failed: fopen(%s): %s\n",
	prog, func, line, filename, strerror(errno));
  }

  if (d_flag)
  {
	  fprintf(stderr, "-%s: %s() at line %d succeeded: Fopen() opened file %s\n",
			  prog, func, line, filename);
  }

  return f;
}

int Fclose(FILE *stream, const char *func, const int line)
{
	if (stream == NULL)
	{
		if (d_flag)
		{
			fprintf(stderr, 
				"-%s: %s() at line %d failed: fclose() cannot close null stream\n",
				prog, func, line);
		}
		else
		{
			fprintf(stderr, "-%s: fclose() cannot close null stream\n",
					prog);
		}

		return EOF;
	}

	if (fclose(stream) == EOF)
	{
		if (d_flag)
		{
			fprintf(stderr, "-%s: %s() at line %d: fclose() failed: %s\n",
				prog, func, line, strerror(errno));
		}
		else
		{
			fprintf(stderr, "-%s: fclose() failed: %s\n",
					prog, strerror(errno));
		}

		return EOF;
	}

	return 0;
}

pid_t Fork(const char *func, const int line)
{
	pid_t pid;

	if ((pid = fork()) == (pid_t) -1) 
	{
		if (d_flag)
		{
			fprintf(stderr, "-%s: %s() at line %d: fork() from process %d failed: %s\n",
				prog, func, line, getpid(), strerror(errno));
		}
		else
		{
			fprintf(stderr, "-%s: fork() from process %d failed: %s\n",
				prog, getpid(), strerror(errno));
		}
	}

	return pid;
}

int Kill(pid_t pid, int sig, const char* func, const int line)
{
	int response;
	response = kill(pid, sig);
	if (response == -1)
	{
		if (d_flag) 
		{
			fprintf(stderr, "-%s: %s() at line %d: kill(%d, %d) failed: %s\n",
					prog, func, line, pid, sig, strerror(errno));
  		}
		else 
		{
			fprintf(stderr, "-%s: kill(%d, %d) failed: %s\n",
					prog, pid, sig, strerror(errno));
		}
	}
			
	return response;
}

sighandler_t Signal(int signum, sighandler_t handler)
{
  sighandler_t response;

  response = signal(signum, handler);

  if (response == SIG_ERR)
  {
    fprintf(stderr, "-%s: signal(%d, handler) failed: %s\n",
			 prog, signum, strerror(errno));
  }

  return response;
}

//------------------------------------------------------------------------------
