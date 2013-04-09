/* CMPSC 311 Project 7 version 3
 *
 * Author:   Jacob Jones
 * Email:    jaj5333@psu.edu
 *
 * originally by: Don Heller
 */

/*----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

#include "wrapper.h"

#define MAXLINE 128
#define MAXARGS 128

int eval_line(char *cmdline);                   /* evaluate a command line */
int parse(char *buf, char *argv[]);             /* build the argv array */
int builtin(char *argv[]);                      /* if builtin command, run it */

char *prog = "[no name]";
int i_flag = 0;
int e_flag = 0;
int v_flag = 0;
int d_flag = 0;

/*-----------------------------------------------------------------------------*/

static void usage(int status)
{
  if (status == EXIT_SUCCESS)
    {
      printf("usage: %s [-h] [-e] [-v] [-d] [-f file]\n", prog);
      printf("  -h           print help\n");
      printf("  -v           verbose mode; enable extra printing; can be repeated\n");
      printf("  -f file      input filename; default is hakefile or Hakefile\n");
      printf("  -f file      input filename; default is hakefile or Hakefile\n");
    }
  else
    {
      fprintf(stderr, "%s: Try '%s -h' for usage information.\n", prog, prog);
    }

  exit(status);
}

/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/

int main(int argc, char *argv[])
{
  int ret = EXIT_SUCCESS;
  char cmdline[MAXLINE];                /* command line */

  while (1)
    {
      /* issue prompt and read command line */
      printf("%s%% ", argv[0]);
      fgets(cmdline, MAXLINE, stdin);   /* cmdline includes trailing newline */
      if (feof(stdin))                  /* end of file */
        { break; }

      ret = eval_line(cmdline);
    }

  return ret;
}

/*----------------------------------------------------------------------------*/
