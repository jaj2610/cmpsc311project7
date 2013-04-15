/* CMPSC 311, Spring 2013, Project 7
 * 
 * Author: Jacob Jones
 * Email: jaj5333@psu.edu
 * 
 * Author: Scott Cheloha
 * Email: ssc5145@psu.edu
 */

#ifndef PR7_H
#define PR7_H

/* CMPSC 311 Project 7 starter kit
 *
 * This file is provided as part of the project description and starter kit.
 * If you modify it, then put your name, email address, and the date, in this
 * comment, and include this file in the materials you turn in.
 */

/*----------------------------------------------------------------------------*/

#include <limits.h>
#include <sys/param.h>
#include <stdbool.h>


/* Global Variables */
extern char *s_filename;
extern const char *prog;
extern int i_flag;
extern int e_flag;
extern int v_flag;
extern int d_flag;
extern int s_flag;
extern int exec_flag;	// 0 for vp, 1 for ve, 2 for lp

extern int command_count;

extern pid_t shell_pid;
extern pid_t fg_pid;
extern pid_t fg_pgid;
extern pid_t bg_pgid;

extern struct process_list *bg_processes;

extern char **environ;

/*----------------------------------------------------------------------------*/


/* Prototypes for functions in pr7.3.c */


/* Starts the prompt with a greeting and lists
 * the command line options set.
 * Only runs if -v is set at the command line.
 */
void verbose_greeting(void);

/* Reads the startup file specified by the user
 * pr7.init is run by default, unless -s is set from the command line.
 * a "-" argument to -s sets stdin as the stream to read,
 * otherwise the argument is opened as a normal file.
 *
 * If pr7.init is not present, nothing is said.
 * If the file argument to -s is not present the shell complains and exits.
 */
void read_startup_file(bool quiet);

/* prints the prompt to standard output.
 * if newline != 0, a newline is printed first.
 */
void print_prompt(int newline);


/* Loop that prints the prompt and processes
 * commands entered by the user, line by line.
 * Only runs if -i is set at the command line.
 */
int prompt(int status);

/* Function containing a getopt() loop for
 * processing command line arguments
 * and assigning the appropriate global variables.
 *
 * Runs once at the start of pr7's main()
 */
void eval_options(int argc, char *argv[]);

/* Evaluates a given command and takes appropriate action. */
int eval_line(char *cmdline);                   

/* Builds the Argv[] array from a command line.
 * Run both in prompt() and when a startup file is specified.
 */
int parse(char *buf, char *Argv[]);

void shell_msg(const char* function_name, const char* msg);

/* custom signal handlers for SIGINT and SIGCHLD */
void handler_SIGCHLD(int signum);
void handler_SIGINT(int signum);

/*----------------------------------------------------------------------------*/


/* Some limits for the shell. */

/*
 *  MAX_LINE            input line length
 *  MAX_PATH            directory name length
 *  MAX_CHILDREN        number of child processes
 */

/* use the minimal value for maximal portability */
#ifdef _POSIX_C_SOURCE



#define MAX_LINE        _POSIX_MAX_INPUT
#define MAX_PATH        _POSIX_PATH_MAX
#define MAX_CHILDREN    _POSIX_CHILD_MAX


#else /* use the default value for this system */

#define MAX_LINE        MAX_INPUT
#define MAX_PATH        PATH_MAX
#define MAX_CHILDREN    CHILD_MAX

/* Alternative versions can be obtained from
 *   #include <unistd.h>
 *   sysconf(_SC_LINE_MAX)
 *   sysconf(_SC_CHILD_MAX)
 */

#endif /* _POSIX_C_SOURCE */

/*----------------------------------------------------------------------------*/

#endif /* PR7_H */
