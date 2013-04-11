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
	// If Argv[1] is NULL, no env. variable or value was specified, so we do nothing
	if (!Argv[1])
	{
		return;
	}

	
	// If no '=' is found, the command does not conform to formatting specs,
	// so issue a message, free buf, and return to prompt
	char *index_of_equal;
	if ((index_of_equal = strpbrk(Argv[1], "=")) == NULL)
	{
		shell_msg("senv", "format is 'senv name=value'");
		return;
	}

	// Allocate space for name
	// If we fail, free buf and return to prompt
	
	char *name;
	int namelength = index_of_equal - Argv[1];
	if ((name = Malloc(namelength + 1, __func__, __LINE__)) == NULL)
	{
		shell_msg("senv", strerror(errno));
		return;
	}

	// Copy from buf to name
	strncpy(name, Argv[1], namelength);
	name[namelength+1] = '\0';


	// Allocate space for value
	// If we fail, free buf, free name, and return to prompt

	char *value;
	int valuelength = strlen(Argv[1]) - namelength;
	if ((value = Malloc(valuelength, __func__, __LINE__)) == NULL)
	{
		shell_msg("senv", strerror(errno));
		free(name);
		return;
	}

	// Copy from buf to value
	strncpy(value, Argv[1]+namelength+1, valuelength-1);
	value[valuelength] = '\0';


	// Having successfully parsed name and value,
	// We can tell our verbose users exactly what we're doing
	if (v_flag)
	{
		fprintf(stderr, "-%s: %s: setting environment variable %s to %s\n",
				prog, "senv", name, value);
	}

	// Attempt to set the environment variable
	if ((setenv(name, value, 1) == -1))
	{
		shell_msg("senv", strerror(errno));
		free(name);
		free(value);
	}

		if (v_flag)
	{
		fprintf(stderr, "-%s: %s: successfully set environment variable %s to %s\n",
				prog, "senv", name, value);
	}


	// Free allocations and return to shell
	free(name);
	free(value);
	return;
}

/*----------------------------------------------------------------------------*/

void Unsenv(char *Argv[])
{
	// If Argv[1] is NULL, we do nothing
	if (Argv[1] == NULL)
	{
		return;
	}

	// Ensure variable exists before attempting to unset
	if (getenv(Argv[1]) == NULL)
	{
		fprintf(stderr, "-%s: %s: cannot unset %s; variable does not exist\n",
				prog, "unsenv", Argv[1]);
		return;
	}

	// Let the verbose user know what we're doing
	if (v_flag)
	{
		fprintf(stderr, "-%s: unsenv: removing environment variable %s\n",
				prog, Argv[1]);
	}

	// Attempt to unset the specified environment variable
	if (unsetenv(Argv[1]) == -1)
	{
		shell_msg("unsenv", strerror(errno));
		return;
	}

	if (v_flag)
	{
		fprintf(stderr, "-%s: unsenv: successfully removed environment variable %s\n",
				prog, Argv[1]);
	}

	return;
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

