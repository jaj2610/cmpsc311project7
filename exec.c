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
  pid_t pid;            /* process id */

  // begin forking child process
  if ((pid = Fork(__func__, __LINE__)) == (pid_t) -1)
  {
    return 1;
  }

  // place child process foreground or background
  if (pid == 0)
  {
    // set fg pgid and fg pid
    if (!background)
    {
      fg_pid = getpid();
      setpgid(0, 0);
      fg_pgid = fg_pid;
    }
    // place child in bg pgid
    else
    {
      // if bg pgid has not been set, set to child's pid
      if (bg_pgid == -3)
      {
        setpgid(0,0);
        bg_pgid = getpid();
      }
      else
      {
        setpgid(0, bg_pgid);
      }
    }

    exec(Argv, status);
  }
  else
  {
    // deal with fg process
    if (!background)
    {
      waitpid(-1, &status, 0);
      return 0;
    }
    else
    {
      // modify background process list
      process_list_append(bg_processes, Argv[0], pid, bg_pgid);

      if (v_flag)
      {
        fprintf(stderr, "%s successfully added to background process list.\n",
        Argv[0]);
      }

      return 0;
    }
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
      exit(1);
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
