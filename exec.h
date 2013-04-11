/* CMPSC 311, Spring 2013, Project 7
 * 
 * Author: Jacob Jones
 * Email: jaj5333@psu.edu
 * 
 * Author: Scott Cheloha
 * Email: ssc5145@psu.edu
 */

#ifndef EXEC_H
#define EXEC_H

//------------------------------------------------------------------------------

#include <sys/types.h>

//------------------------------------------------------------------------------

/* new_child() deals with a new command not built-in to pr7
 * command is forked into new process and put in fg or bg group
 * process is then passed along to exec(ve,vp,lp)
 */
int new_child(char *Argv[], int background, int status);

/* exec() passes a process to exec(ve,vp,lp) depending on what flags are toggled
 * 0 for vp, 1 for ve, 2 for lp
 */
void exec(char *Argv[], int status);

//------------------------------------------------------------------------------

#endif
