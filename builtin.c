/* CMPSC 311, Spring 2013, Project 7
 * 
 * Author: Jacob Jones
 * Email: jaj5333@psu.edu
 * 
 * Author: Scott Cheloha
 * Email: ssc5145@psu.edu
 */

/* Implementation of the "built-in" shell commands as specified
 * in requirement (1) of the Project 7 specs.
 *
 * Per requirements of project 7, we implement:
=======
 * 	quit -- exit the shell, unless bg processes are active (originally called "exit")
 *    echo -- print remaining command-line arguments
 *    dir  -- print name of current working directory
 *    cdir -- change working directory
 *    penv -- print one or all environment variables
 *    senv -- set an environment variable
 *    unsenv -- unset an environment variable
 *    pjobs  -- list currently active bg processes
 * 	help   -- list builtin shell commands (commands implemented in builtin.c)
 *
 * In addition, we implement:
 * 	limits -- needs to be written, print the limit macros from pr7.h
 *    (set echo -- toggle echo mode on or off) -- replaced by toggle
 * 	(set debug -- toggle debug mode on or off) -- replaced by toggle
 *    set exec -- toggle exec types (lp,vp,ve)
 *    (set verbose -- toggle verbose mode on or off) -- replaced by toggle
 * 	toggle -- toggle verbosity, echo, debug modes from within the shell
 */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/param.h>

#include "builtin.h"
#include "pr7.h"              // included for shell_msg()
#include "wrapper.h"
#include "linked.h"

int Builtin(char *Argv[])
{
	errno = 0; // reset errno 

	if (!strcmp(Argv[0], "pr7")) {
		return 1;
	}
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

	/* pjobs command */
	if (!strcmp(Argv[0], "pjobs")) {
		Pjobs(); return 1;
	}

	/* toggle command */
	if (!strcmp(Argv[0], "toggle")) {
		Toggle(Argv); return 1;
	}

	/* options command */
	if (!strcmp(Argv[0], "options")) {
		Options(); return 1;
	}

	/* set command -- remove? */
	if (!strcmp(Argv[0], "set")) {
		Set(Argv); return 1;
	}

	/* ignore singleton & -- why is this here, Don? */
	if (!strcmp(Argv[0], "&")) {
		return 1;
	}

	/* not a builtin command */
	return 0;
}

/*----------------------------------------------------------------------------*/

void Quit(void)
{
	if (bg_processes->head != NULL)
	{
		puts("Background processes are still running.");
		Pjobs();
	}
	else
	{
		if (v_flag)
		{
			printf("-%s: See you later, %s!\n", prog, getenv("USER"));
		}

		exit(0);
	}
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

	// Free() buf and return
	Free(buf, __func__, __LINE__);
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
		fprintf(stderr, "-%s: changing working directory to %s\n",
				"cdir", target_dir);
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
	// so issue a message, Free buf, and return to prompt
	char *index_of_equal;
	if ((index_of_equal = strpbrk(Argv[1], "=")) == NULL)
	{
		shell_msg("senv", "format is 'senv name=value'");
		return;
	}

	// Allocate space for name
	// If we fail, Free buf and return to prompt
	
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
	// If we fail, Free buf, Free name, and return to prompt

	char *value;
	int valuelength = strlen(Argv[1]) - namelength;
	if ((value = Malloc(valuelength, __func__, __LINE__)) == NULL)
	{
		shell_msg("senv", strerror(errno));
		Free(name, __func__, __LINE__);
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
		Free(name, __func__, __LINE__);
		Free(value, __func__, __LINE__);
	}

		if (v_flag)
	{
		fprintf(stderr, "-%s: %s: successfully set environment variable %s to %s\n",
				prog, "senv", name, value);
	}


	// Free allocations and return to shell
	Free(name, __func__, __LINE__);
	Free(value, __func__, __LINE__);
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
  puts("  toggle     toggle a command line argument (e, v, or d)");
  puts("  options    prints all active command line arguments (e, i, v, d, and s)");
}

void Pjobs(void)
{
	process_list_print(bg_processes);
}

/*----------------------------------------------------------------------------*/

void Toggle(char *Argv[])
{
	// If Argv[1] is null, nothing was specified,
	// so we issue a message and return to prompt
	if (Argv[1] == NULL)
	{
		fprintf(stderr, "-%s: error: nothing to toggle\n", prog);
		return;
	}

	if (!strcmp(Argv[1], "e"))
	{
		if (e_flag)
		{
			e_flag = 0;
		}
		else
		{
			e_flag = 1;
		}

		printf("-%s: echo mode is now ", prog);
		if (e_flag)
		{
			puts("ON");
		}
		else
		{
			puts("OFF");
		}

		return;
	}

	if (!strcmp(Argv[1], "v"))
	{
		if (v_flag)
		{
			v_flag = 0;
		}
		else
		{
			v_flag = 1;
		}

		printf("-%s: verbose output is now ", prog);
		if (v_flag)
		{
			puts("ON");
		}
		else
		{
			puts("OFF");
		}

		return;
	}

	if (!strcmp(Argv[1], "d"))
	{
		if (d_flag)
		{
			d_flag = 0;
		}
		else
		{
			d_flag = 1;
		}

		printf("-%s: debug output is now ", prog);
		if (d_flag)
		{
			puts("ON");
		}
		else
		{
			puts("OFF");
		}

		return;
	}

	fprintf(stderr, "-%s: error: don't know how to toggle %s\n" \
						 "-%s: try 'toggle [d|e|v]'\n",
						 prog, Argv[1], prog);
}

void Options(void)
{
	printf("-%s: Running with:\n", prog);
	Print_Options();
}

void Print_Options()
{
  if (i_flag)
  {
    puts("   [-i] interactive prompt");
  }

  if (e_flag)
  {
    puts("   [-e] echo commands before execution");
  }

  if (v_flag)
  {
    puts("   [-v] verbose output");
  }

  if (d_flag)
  {
    puts("   [-d] debug output");
  }

  if (s_flag)
  {
    printf("   [-s] alternate startup file %s\n", s_filename);
  }

}

/*----------------------------------------------------------------------------*/

void Set(char *Argv[])
{
	if (Argv[1] == NULL)
	{
		fprintf(stderr, "-%s: set format:\n", prog);
		fprintf(stderr, "-%s:    set [echo|verbose|debug] [on|off]\n", prog);
		fprintf(stderr, "-%s:    set [exec] [vp|ve|lp]\n", prog);
	}
	else if (!strcmp(Argv[1], "exec")) {
		Sexec(Argv);
	}
	else if (!strcmp(Argv[1], "echo")) {
		Secho(Argv);
	}
	else if (!strcmp(Argv[1], "verbose")) {
		Sverbose(Argv);
	}
	else if (!strcmp(Argv[1], "debug")) {
		Sdebug(Argv);
	}
	else
	{
		fprintf(stderr, "-%s: error: don't know how to set %s\n", prog, Argv[1]);
	}
}

/*----------------------------------------------------------------------------*/

void Sexec(char *Argv[])
{
	if (!strcmp(Argv[2], "vp")) {
		exec_flag = 0;
	}

	if (!strcmp(Argv[2], "ve")) {
		exec_flag = 1;
	}

	if (!strcmp(Argv[2], "lp")) {
		exec_flag = 2;
	}

	if (v_flag)
	{
		fprintf(stderr, "-%s: set: set exec mode %s\n",
				prog, Argv[2]);
	}
}


/*----------------------------------------------------------------------------*/


void Secho(char *Argv[])
{
	if (!strcmp(Argv[2], "on")) {
		e_flag = 1;
	}
	else if (!strcmp(Argv[2], "off")) {
		e_flag = 0;
	}
	else {
		fprintf(stderr, "-%s: error: don't know how to set echo \"%s\"\n", 
				prog, Argv[2]);
		return;
	}

	if (v_flag)
	{
		fprintf(stderr, "-%s: set: successfully set echo %s\n",
				prog, Argv[2]);
	}
}

/*----------------------------------------------------------------------------*/

void Sverbose(char *Argv[])
{
	if (!strcmp(Argv[2], "on")) {
		v_flag = 1;
	}
	else if (!strcmp(Argv[2], "off")) {
		v_flag = 0;
	}
	else {
		fprintf(stderr, "-%s: error: don't know how to set verbose \"%s\"\n", 
				prog, Argv[2]);
		return;
	}

	if (v_flag)
	{
		fprintf(stderr, "-%s: set: successfully set verbose %s\n",
				prog, Argv[2]);
	}
}

/*----------------------------------------------------------------------------*/

void Sdebug(char *Argv[])
{
	if (!strcmp(Argv[2], "on")) {
		d_flag = 1;
	}
	else if (!strcmp(Argv[2], "off")) {
		d_flag = 0;
	}
	else {
		fprintf(stderr, "-%s: error: don't know how to set debug \"%s\"\n", 
				prog, Argv[2]);
		return;
	}

	if (v_flag)
	{
		fprintf(stderr, "-%s: set: successfully set debug %s\n",
				prog, Argv[2]);
	}
}
