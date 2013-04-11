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

/* external global variables */
extern const char *prog;
extern int i_flag;
extern int e_flag;
extern int v_flag;
extern int d_flag;
extern int s_flag;
extern int exec_flag;	// 0 for vp, 1 for ve, 2 for lp
extern pid_t fg_pid;
extern pid_t fg_pgid;
extern pid_t bg_pgid;
extern char *s_filename;
extern struct process_list *bg_processes;

extern char **environ;



/* Prototypes for functions in pr7.3.c */
void list_options(void);
int prompt(int status);
void eval_options(int argc, char *argv[]);
int eval_line(char *cmdline);                   /* evaluate a command line */
int parse(char *buf, char *Argv[]);             /* build the Argv array */
void shell_msg(const char* function_name, const char* msg);

// Still need work
void sig_chld(int signum);
void sig_int(int signum);
void sig_stp(int signum);



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
