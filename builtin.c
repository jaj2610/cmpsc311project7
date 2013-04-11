/*
 * Author:		Scott Cheloha
 * Email:		ssc5145@psu.edu
 *
 * Author:		Jake Jones
 * Email:		jaj5333@psu.edu
 */

/* Implementation of the "built-in" shell commands as specified
 * in requirement (1) of the Project 7 specs.
 *
 * Here, we implement:
 * 	quit -- exit the shell (originally called "exit"
 * 		--NOTE: we need to fully implement this
 * 	echo
 * 	dir
 * 	cdir
 * 	penv -- needs writing
 * 	senv -- needs writing
 * 	unsenv -- needs writing
 * 	help
 * 	limits -- print the limit macros from pr7.h
 * 	toggle -- toggle verbosity, echo, debug modes from within the shell
 */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/param.h>

#include "builtin.h"
#include "pr7.h" // included for shell_msg()
#include "wrapper.h"

int Builtin(char *Argv[])
{
	errno = 0; // reset errno 

	/* quit command */
	if (!strcmp(Argv[0], "quit")) { 
		Quit(); return 1;
	}

	/* echo command */
	if (!strcmp(Argv[0], "echo")) {
		Echo(Argv);
		return 1;
	}
	/* dir command */
	if (!strcmp(Argv[0], "dir")) {
	Dir(); return 1;
	}

	/* cdir command */
	if (!strcmp(Argv[0], "cdir")) {
		Cdir(Argv); return 1;
	}

	/* penv command */
	if (!strcmp(Argv[0], "penv")) {
		Penv(Argv); return 1;
	}

	/* senv command */
	if (!strcmp(Argv[0], "senv")) {
		Senv(Argv); return 1;
	}

	/* unsenv command */
	if (!strcmp(Argv[0], "unsenv")) {
		Unsenv(Argv); return 1;
	}

	/* penv command */
	if (!strcmp(Argv[0], "help")) {
		Help(); return 1;
	}

	/* ignore singleton & */
	if (!strcmp(Argv[0], "&")) {
		return 1;
	}

  return 0;                       /* not a builtin command */
}

/*----------------------------------------------------------------------------*/

void Quit(void)
{
  exit(0);
}

/*----------------------------------------------------------------------------*/

void Echo(char *Argv[])
{
	for (int i = 1; Argv[i] != NULL; i++)
	{
		printf("%s ", Argv[i]);
	}

	puts("");
}

/*----------------------------------------------------------------------------*/

void Dir(void)
{
	char *buf;

	if (v_flag)
	{
		shell_msg("dir", "printing working directory");
	}
		

	// Allocate MAXPATHLEN bytes for buf
	if ((buf = Malloc(MAXPATHLEN, __func__, __LINE__)) == NULL) {
		shell_msg("dir", strerror(errno));
		return;
	}

	// Call getwd() 
	getwd(buf);

	// Check if getwd() failed,
	// print a message on failure, print the directory name on success
	if (errno != 0) {
		shell_msg("dir", strerror(errno));	
	}
	else {
		printf("%s\n", getwd(buf));
	}

	// free() buf and return
	free(buf);
	return;
}

/*----------------------------------------------------------------------------*/

void Cdir(char *Argv[])
{
	const char *target_dir;

	// If the directory is unspecified, use env. var. HOME as target directory
	if (Argv[1] == NULL) {
		target_dir = getenv("HOME");
	}
	// Otherwise, use the specified directory
	else {
		target_dir = Argv[1];
	}

	if (v_flag)
	{
		fprintf(stderr, "-%s: %s: changing working directory to %s\n",
				prog, "cdir", target_dir);
	}	

	// Check that the directory is not NULL
	if (target_dir)
	{
		// attempt to change the working directory to Argv[1]
		if (chdir(target_dir) == -1) {
			shell_msg("cdir", strerror(errno));
			return;
		}
		
		// attempt to set env. var. PWD to Argv[1]
		if (setenv("PWD", target_dir, 1) == -1) {
			shell_msg("cdir", strerror(errno));
			return;
		}

		if (v_flag)
		{
			fprintf(stderr, "-%s: %s: working directory changed to %s\n",
					prog, "cdir", target_dir);
		}	
	}
	// Strange circumstance where Argv[1] NULL but not caught,
	// or HOME is unspecified by the environment
	else {
		shell_msg("cdir", "strange error; HOME is not set");
	}
}

/*----------------------------------------------------------------------------*/

void Penv(char *Argv[])
{
	char *env_var;

	// If Argv[1] is not NULL, print the env. var. for it
	if (Argv[1]) 
	{
		// If getenv() succeeds, print the variable
		if ((env_var = getenv(Argv[1])) != NULL) 
		{
			printf("%s\n", env_var);
		}

		return;
	}

	// If Argv[1] is NULL, print all available environment variables
	env_var = *environ;

	for (int i = 0; env_var != NULL; i++)
	{
		printf("%s\n", env_var);
		env_var = *(environ + i);
	}
}

/*----------------------------------------------------------------------------*/

void Senv(char *Argv[])
{
  ;
}

/*----------------------------------------------------------------------------*/

void Unsenv(char *Argv[])
{
  ;
}

/*----------------------------------------------------------------------------*/

void Help(void)
{
  puts("built-in commands:");
  puts("  help       print this list");
  puts("  quit       terminate pr7 if no background processes are running");
  puts("  echo       print remaining command-line arguments");
  puts("  dir        print current working directory");
  puts("  cdir       change working directory");
  puts("  penv       print one or all environment variables");
  puts("  senv       set an environment variable");
  puts("  unsenv     unset an environment variables");
}

