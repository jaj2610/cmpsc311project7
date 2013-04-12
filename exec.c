/* CMPSC 311, Spring 2013, Project 7
 * 
 * Author: Jacob Jones
 * Email: jaj5333@psu.edu
 * 
 * Author: Scott Cheloha
 * Email: ssc5145@psu.edu
 */

/* Here, we implement:
*  new_child -- forks a new child and puts it in fg or bg pgid
*  exec -- needs to be completed with flags, should check which exec to use (ve,vp,lp)
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "exec.h"
#include "pr7.h"
#include "wrapper.h"
#include "linked.h"

//------------------------------------------------------------------------------

int new_child(char *Argv[], int background, int status)
{
  pid_t pid;

  if ((pid = Fork(__func__, __LINE__)) == (pid_t) -1)
  {
    return 1;	// fork() failed, return to prompt
  }

  /* If we're in the child process,
	* we need to place it in the appropriate process group
	*/
  if (pid == 0)
  {
	 /* foreground process */
    if (!background)
    {
      fg_pid = getpid();		// set the global foreground PID to child's PID
      setpgid(0, 0);				// create new process group for child
      fg_pgid = getpgrp();		// set global foreground GPID to child's PID
    }
	 /* background process */
	 else
    {
      // if bg_pgid has not been set, set it to the child's pid
      if (bg_pgid == 0)
      {
        setpgid(0,0);
        bg_pgid = getpgrp();
      }
      else
      {
        setpgid(0, bg_pgid);	// add the child to the background PID group
      }

      // modify background process list
      process_list_append(bg_processes, Argv[0], getpid(), bg_pgid);

      if (v_flag)
      {
        fprintf(stderr, "-%s: %s added to background process list.\n",
        		prog, Argv[0]);
      }
    }

    exec(Argv, status);
  }
  /* If we're in the parent process, we either:
	* 	wait (foreground process)
	* 	return to shell (background process)
	*/

	/* Parent forked a foreground process */
	if (!background)
   {
     waitpid(-1, &status, 0);
     return 0;
   }
	else
	{
		return 0;
	}

  return status;
}

void exec(char *Argv[], int status)
{
  // use execvp
  if (exec_flag == 0)
  {
    if ((status = execvp(Argv[0], Argv)) == -1)
    {
      fprintf(stderr, "-%s: execvp() failed: %s\n", prog, strerror(errno));
      _exit(EXIT_FAILURE);
    }
  }

  // use execve
  if (exec_flag == 1)
  {
    ;
  }

  // use execlp
  if (exec_flag == 2)
  {
    ;
  }
}
