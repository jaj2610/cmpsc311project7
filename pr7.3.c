/* CMPSC 311, Spring 2013, Project 7
 * 
 * Author: Jacob Jones
 * Email: jaj5333@psu.edu
 * 
 * Author: Scott Cheloha
 * Email: ssc5145@psu.edu
 */

/*----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdbool.h>

#include "pr7.h"
#include "wrapper.h"
#include "builtin.h"
#include "linked.h"
#include "exec.h"

#define MAXARGS 128

char *s_filename = "pr7.init";  // default startup file name
const char *prog = "pr7";
char *infile_name = "[no name]";

int i_flag = 0;
int e_flag = 0;
int v_flag = 0;
int d_flag = 0;
int s_flag = 0;
int exec_flag = 0;

int command_count = 0;	// number of commands submitted since startup

pid_t shell_pid = 0;
pid_t fg_pid = 0;
pid_t fg_pgid = 0;
pid_t bg_pgid = 0;


struct process_list *bg_processes;

/*----------------------------------------------------------------------------*/

int main(int argc, char *argv[])
{
	shell_pid = getpid();

	eval_options(argc, argv);

	/* install non-default signal handlers for shell */
	Signal(SIGCHLD, handler_SIGCHLD, __func__, __LINE__);
	Signal(SIGINT, handler_SIGINT, __func__, __LINE__);

	int status = EXIT_SUCCESS;

	/* Say hello */
	if (v_flag)
	{
		verbose_greeting();
	}

	bg_processes = process_list_allocate();

	read_startup_file((bool) !s_flag);

	/* Can't continue unless interactivity or a command file are specified */
	if (argc == 1)
	{
		usage(EXIT_FAILURE);
	}

	status = read_input(argc, argv);

	return status;
}

/*----------------------------------------------------------------------------*/

void usage(int status)
{
  if (status == EXIT_SUCCESS)
  {
    printf("usage: %s [-h] [-e] [-v] [-d] [-V] [-s sfile] [command file]\n", prog);
    puts("  -h           print help");
    puts("  -i           interactive mode");
    puts("  -e           echo commands before execution");
    puts("  -v           verbose mode; enable extra printing; can be repeated");
    puts("  -d           debug information for memory allocation");
    puts("  -V           enables verbose and debug modes");
    puts("  -s sfile     use sfile as startup file, default is pr7.init");
	  puts("\nNOTE: Use -i OR specify a command file for input");
  }
  else
  {
    fprintf(stderr, "%s: Try '%s -h' for usage information.\n", prog, prog);
  }

  exit(status);
}

/*-----------------------------------------------------------------------------*/

void verbose_greeting(void)
{
	printf("-%s: Hello, %s!\n", prog, getenv("USER"));
	Options();
}

/*----------------------------------------------------------------------------*/

void read_startup_file(bool quiet)
{
	FILE *fp;

	/* If the user specifies '-', we take our reads from stdin,
	 * otherwise we attempt to open s_filename for reading
	 */
	if (!strcmp(s_filename, "-"))
	{
		fp = stdin;
	}
	else if ( (fp = fopen(s_filename, "r")) == NULL )
	{
			if (!quiet)
			{
				fprintf(stderr, "-%s: failed to open startup file %s\n",
						prog, s_filename);
			}

			return;
	}

	char cmdline[MAX_LINE];                /* command line */

	while (fgets(cmdline, MAX_LINE, fp) != NULL)
	{
		eval_line(cmdline);
	}

	return;
}

/*----------------------------------------------------------------------------*/

void print_prompt(int newline)
{
	if (newline)
	{
		puts("");
	}

	printf("%s %d%% ", prog, command_count);
	command_count++;
	return;
}

/*----------------------------------------------------------------------------*/

int prompt(int status)
{
  char cmdline[MAX_LINE];                /* command line */

  while (1)
  {
    /* issue prompt and read command line */
    print_prompt(0);
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

int read_input(int argc, char *argv[])
{
  FILE *infile;
  char *infile_name; 
  int status;

  if (i_flag) 
  { 
    infile = stdin; infile_name = "[stdin]";
  }
  else
  {
    infile_name = argv[argc-1];  /* also use strdup()?...no */
    if ((infile = Fopen(infile_name, "r", __func__, __LINE__)) == NULL)
    {
	 	 printf("-%s: shell failed to read command file: ensure file '%s' exists\n",
			 prog, argv[argc-1]);

		exit(EXIT_FAILURE);  // brutally fail
    }
  }

  char cmdline[MAX_LINE];

  if (i_flag)
  {
    print_prompt(0);
  }

  while (fgets(cmdline, MAX_LINE, infile) != NULL)
  {

    /* issue prompt and read command line */
    /*
    if (feof(infile))
    { 
      break;
    }
    */

    status = eval_line(cmdline);

    if (i_flag)
    {
      print_prompt(0);
    }
  }

  if (ferror(infile))
  {
    printf("-%s: error reading input: %s\n", prog, strerror(errno));
  }

  return status;
}

/*----------------------------------------------------------------------------*/

// Runs a getopt() loop to harvest command-line options for pr7
void eval_options(int argc, char *argv[])
{
  extern char *optarg;
  extern int optopt;
  int ch;

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
        v_flag = 1; d_flag = 1;
        break;
      case 's':
        s_flag++;

        if (s_flag > 1)
        {
          fprintf(stderr, "-%s: error: cannot specify more than one startup file\n", 
					 prog);
          exit(1);
        }
        else
        {
          s_filename = optarg;
        }
        break;
      case '?':
        fprintf(stderr, "-%s: error: invalid option '%c'\n", prog, optopt);
        usage(EXIT_FAILURE);
        break;
      case ':':
        fprintf(stderr, "-%s: error: option '%c' needs filename argument\n",
				  prog, optopt);
		  usage(EXIT_FAILURE);
		  break;
      default:
        usage(EXIT_FAILURE);
        break;
    }
  }
}

/*----------------------------------------------------------------------------*/


int eval_line(char *cmdline)
{
  char *Argv[MAXARGS];  /* Argv for execve() */
  char buf[MAX_LINE];    /* holds modified command line */

  strcpy(buf, cmdline); /* buf[] will be modified by parse() */

  int background = parse(buf, Argv);
  int status = EXIT_SUCCESS;

  /* ignore empty lines */
  if (Argv[0] == NULL)          
  {
    return status;
  }

  if (e_flag)
  {
    for (int i = 0; Argv[i] != NULL; i++)
    {
      printf("%s ", Argv[i]);
    }
    puts("");
  }
  
  if (Builtin(Argv))
  {
    return status;
  }

  status = new_child(Argv, background, status);

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

/* Prints an error message
 * format is:
 * 	-[program name]: [command name]: [msg1]
 */
void shell_msg(const char* function_name, const char* msg)
{
	fprintf(stderr, "-%s: %s: %s\n", prog, function_name, msg);
}

/*----------------------------------------------------------------------------*/

void handler_SIGCHLD(int signum)
{
	(void) signum;
  int status;

  /* We received a SIGCHLD signal from a background process,
  *  so we loop through
	*  the background process list and reap all zombies.
	*/

  for (pid_t pid = waitpid(-1, &status, WNOHANG);
       pid != 0 && pid != -1;
       pid = waitpid(-1, &status, WNOHANG))
	{
      if (pid == fg_pid)
      {
        fg_pid = fg_pgid = 0;
      }
      else
      {
        process_list_pop(bg_processes, pid);
		   
		  // reprint the prompt
			print_prompt(0);
			fflush(stdout);
      }
	}

  return;
}

/*----------------------------------------------------------------------------*/

void handler_SIGINT(int signum)
{
	// if the fg process group exists, forward sigint to 
	// every process in the group
	if (fg_pgid != 0)
	{
		Kill(-1 * fg_pgid, signum, __func__, __LINE__);
		puts("");
	}
	// if the fg process group does not exist,
	// simply re-print the prompt
	else if (getpid() == shell_pid)
	{
		printf("\b\b  ");
		print_prompt(1);
		fflush(stdout);
	}

	return;
}

/*----------------------------------------------------------------------------*/
