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
int parse(char *buf, char *Argv[]);             /* build the Argv array */
int builtin(char *Argv[]);                      /* if builtin command, run it */
void Quit(void);
void Echo(char *Argv[]);
void Dir(void);
void Cdir(char *Argv[]);
void Penv(char *Argv[]);
void Senv(char *Argv[]);
void Unsenv(char *Argv[]);
void Help(void);

char *prog = "[no name]";
int h_flag = 0;
int i_flag = 0;
int e_flag = 0;
int v_flag = 0;
int d_flag = 0;
int s_flag = 0;
char *s_filename = "[no name]";



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
  int ch;

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
  char *Argv[MAXARGS];  /* Argv for execve() */
  char buf[MAX_LINE];    /* holds modified command line */
  pid_t pid;            /* process id */

  strcpy(buf, cmdline); /* buf[] will be modified by parse() */

  int background = parse(buf, Argv);
  int status = EXIT_SUCCESS;

  /* ignore empty lines */
  if (Argv[0] == NULL)          
  { 
    return status;
  }
  if (builtin(Argv))
  {
    return status;
  }

  return status;
}

/*----------------------------------------------------------------------------*/

/* parse the command line and build the Argv array
 *
 * Compare to parseline() in CS:APP Fig. 8.24.
 */

int parse(char *buf, char *Argv[])
{
  char *delim;          /* points to first whitespace delimiter */
  int argc = 0;         /* number of args */
  int bg;               /* background job? */

  buf[strlen(buf)-1] = ' '; /* Replace trailing ’\n’ with space */

  while (*buf && (*buf == ' ')) /* Ignore leading spaces */
  {
    buf++;
  }

  /* build the Argv list */
  while ((delim = strchr(buf, ' ')))
  {
    Argv[argc++] = buf;               /* start Argv[i] */
    *delim = '\0';                    /* terminate Argv[i] */
    buf = delim + 1;                  /* start Argv[i+1]? */

    while (*buf && (*buf == ' '))
    {
      buf++;
    }
  }
  Argv[argc] = NULL;

  if (argc == 0)                      /* blank line */
  { 
    return 0;
  }

  /* Should the job run in the background? */
  if ((bg = (strcmp(Argv[argc-1], "&") == 0)))
  {
    Argv[--argc] = NULL;
  }

  return bg;
}

/*----------------------------------------------------------------------------*/

/* if first arg is a builtin command, run it and return true
 *
 * Compare to builtin_command() in CS:APP Fig. 8.23.
 */

int builtin(char *Argv[])
{
  if (!strcmp(Argv[0], "quit"))     /* quit command */
  { 
    Quit();
		return 1;
  }

  if (!strcmp(Argv[0], "echo"))		/* echo command */
  {
  	Echo(Argv);
		return 1;
  }

  if (!strcmp(Argv[0], "dir"))		/* dir command */
  {
		Dir();
		return 1;
  }

  if (!strcmp(Argv[0], "cdir"))		/* cdir command */
  {
		Cdir(Argv);
		return 1;
  }

  if (!strcmp(Argv[0], "penv"))		/* penv command */
  {
		Penv(Argv);
		return 1;
  }

  if (!strcmp(Argv[0], "senv"))		/* penv command */
  {
		Senv(Argv);
		return 1;
  }

  if (!strcmp(Argv[0], "unsenv"))		/* penv command */
  {
		Unsenv(Argv);
		return 1;
  }

  if (!strcmp(Argv[0], "help"))		/* penv command */
  {
		Help();
		return 1;
  }

  if (!strcmp(Argv[0], "&"))        /* ignore singleton & */
  {
    return 1;
  }

  return 0;                             /* not a builtin command */
}

/*----------------------------------------------------------------------------*/

void Quit(void)
{
  exit(0);
}

void Echo(char *Argv[])
{
  int i = 1;
  while(Argv[i] != NULL)
  {
    printf("%s ",Argv[i]);
    i++;
  }
  printf("\n");
}

void Dir(void)
{
  ;
}

void Cdir(char *Argv[])
{
  ;
}

void Penv(char *Argv[])
{
  ;
}

void Senv(char *Argv[])
{
  ;
}

void Unsenv(char *Argv[])
{
  ;
}

void Help(void)
{
  ;
}
