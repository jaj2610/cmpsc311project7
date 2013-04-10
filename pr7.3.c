/* CMPSC 311 Project 7 version 3
 *
 * Author:   Jacob Jones
 * Email:    jaj5333@psu.edu
 *
 */

/*----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

#include "wrapper.h"
#include "pr7.h"

#define MAXARGS 128

int eval_line(char *cmdline);                   /* evaluate a command line */
int parse(char *buf, char *argv[]);             /* build the argv array */
int builtin(char *argv[]);                      /* if builtin command, run it */

char *prog = "[no name]";
int h_flag = 0;
int i_flag = 0;
int e_flag = 0;
int v_flag = 0;
int d_flag = 0;



/*-----------------------------------------------------------------------------*/

static void usage(int status)
{
  if (status == EXIT_SUCCESS)
  {
    printf("usage: %s [-h] [-e] [-v] [-d] [-V] [-s f]\n", prog);
    printf("  -h           print help\n");
    printf("  -i           iteractive mode");
    printf("  -e           echo commands before execution\n");
    printf("  -v           verbose mode; enable extra printing; can be repeated\n");
    printf("  -d           debug information for memory allocation\n");
    printf("  -V           enables verbose and debug modes");
    printf("  -s f         use startup file f, default is pr7.init\n");
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
  extern char *optarg;
  extern int optind;
  extern int optopt;
  extern int opterr;

  prog = argv[0];

  /* set flags */
  while ((ch = getopt(argc, argv, ":hievdVs:")) != -1)
  {
    switch (ch) 
    {
      case 'h':
        usage(EXIT_SUCCESS);
        break;
      case 'i':
        i_flag = 1;
        break;
      case 'e':
        e_flag = 1;
        break;
      case 'v':
        v_flag = 1;
        break;
      case 'd':
        d_flag = 1;
        break;
      case 'V':
        v_flag = 1;
        d_flag = 1;
        break;
      case 's':
        s_flag++;
        if (s_flag > 1)
        {
          fprintf(stderr, "%s: invalid option, only one file as input\n", prog);
        }
        else
        {
          s_filename = optarg;
        }
        break;
      case '?':
        fprintf(stderr, "%s: invalid option '%c'\n", prog, optopt);
        usage(EXIT_FAILURE);
        break;
      case ':':
        fprintf(stderr, "%s: invalid option '%c' (missing argument)\n", prog, optopt);
      default:
        usage(EXIT_FAILURE);
        break;
    }
  }

  int status = EXIT_SUCCESS;
  char cmdline[MAX_LINE];                /* command line */

  while (1)
  {
    /* issue prompt and read command line */
    printf("%s$ ", argv[0]);
    fgets(cmdline, MAX_LINE, stdin);   /* cmdline includes trailing newline */
    if (feof(stdin))                   /* end of file */
    { 
      break; 
    }

    status = eval_line(cmdline);
  }

  return status;
}

/*----------------------------------------------------------------------------*/

int eval_line(char *cmdline)
{
  char *argv[MAXARGS];  /* argv for execve() */
  char buf[MAXLINE];    /* holds modified command line */
  pid_t pid;            /* process id */

  strcpy(buf, cmdline); /* buf[] will be modified by parse() */

  int background = parse(buf, argv);
  int status = EXIT_SUCCESS;

  /* ignore empty lines */
  if (argv[0] == NULL)          
  { 
    return status;
  }
}

/*----------------------------------------------------------------------------*/

/* parse the command line and build the argv array
 *
 * Compare to parseline() in CS:APP Fig. 8.24.
 */

int parse(char *buf, char *argv[])
{
  char *delim;          /* points to first whitespace delimiter */
  int argc = 0;         /* number of args */
  int bg;               /* background job? */

  buf[strlen(buf)-1] = ’ ’; /* Replace trailing ’\n’ with space */

  while (*buf && (*buf == ’ ’)) /* Ignore leading spaces */
  {
    buf++;
  }

  /* build the argv list */
  while ((delim = strchr(buf, ' ')))
  {
    argv[argc++] = buf;               /* start argv[i] */
    *delim = '\0';                    /* terminate argv[i] */
    buf = delim + 1;                  /* start argv[i+1]? */

    while (*buf && (*buf == ' '))
    {
      buf++;
    }
  }
  argv[argc] = NULL;

  if (argc == 0)                      /* blank line */
  { 
    return 0;
  }

  /* Should the job run in the background? */
  if ((bg = (strcmp(argv[argc-1], "&") == 0)))
  {
    argv[--argc] = NULL;
  }

  return bg;
}

/*----------------------------------------------------------------------------*/

/* if first arg is a builtin command, run it and return true
 *
 * Compare to builtin_command() in CS:APP Fig. 8.23.
 */

int builtin(char *argv[], char *buf)
{
  if (!strcmp(argv[0], "quit"))     /* quit command */
  { 
    	Quit();
		return 1;
  }

  if (!strcmp(argv[0], "echo"))		/* echo command */
  {
  		Echo(argv);
		return 1;
  }

  if (!strcmp(argv[0], "dir"))		/* dir command */
  {
		Dir();
		return 1;
  }

  if (!strcmp(argv[0], "cdir"))		/* cdir command */
  {
		Cdir(argv);
		return 1;
  }

  if (!strcmp(argv[0], "penv"))		/* penv command */
  {
		Penv(argv);
		return 1;
  }

  if (!strcmp(argv[0], "senv"))		/* penv command */
  {
		Senv(argv);
		return 1;
  }

  if (!strcmp(argv[0], "unsenv"))		/* penv command */
  {
		Unsenv(argv);
		return 1;
  }

  if (!strcmp(argv[0], "help"))		/* penv command */
  {
		help();
		return 1;
  }

  if (!strcmp(argv[0], "&"))        /* ignore singleton & */
  {
    return 1;
  }

  return 0;                             /* not a builtin command */
}

/*----------------------------------------------------------------------------*/
