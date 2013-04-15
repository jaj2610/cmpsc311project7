/* CMPSC 311, Spring 2013, Project 7
 * 
 * Author: Jacob Jones
 * Email: jaj5333@psu.edu
 * 
 * Author: Scott Cheloha
 * Email: ssc5145@psu.edu
 */

#ifndef BUILTIN_H
#define BUILTIN_H

/* Builtin() compares Argv[0] to the builtin commands.
 * If a match is found, the command is run, and builtin returns 1.
 * If a match is not found, builtin returns 0.
 */
int Builtin(char *Argv[]);

/* Quit() exits the shell.
 * If background processes are still running, Quit() refuses to exit,
 * and instead displays the currently running background processes.
 */
void Quit(void);

/* Echo() prints the remaining command-line arguments from Argv[].
 * The arguments are separated by a space each, ala bash's 'echo'
 */
void Echo(char *Argv[]);

/* Dir() prints the current working directory to the console.
 * Compare to sh's 'pwd'.
 */
void Dir(void);

/* Cdir() changes the current working directory to Argv[1].
 * If Argv[1] is NULL, the environment variable HOME is used instead.
 * Compare to sh's 'cd' or 'chdir'.
 */
void Cdir(char *Argv[]);

/* Penv() prints the specified environment variable.
 * If not variable is specified, Penv() prints all known variables.
 * If the variable does not exist, Penv() prints nothing, and does not complain.
 * Compare to sh's 'printenv'
 */
void Penv(char *Argv[]);

/* Senv() sets the specified variable to a given value.
 * Format is
 * 	senv name=value
 *
 * Senv() complains if no '=' is present in the argument string.
 * Compare to csh's 'setenv' or 'set'
 */
void Senv(char *Argv[]);

/* Unsenv() removes the specified environment variable
 * Format is
 * 	unsenv name
 *
 * Unsenv() complains if the given variable does not exist
 * Compare to sh's 'unset'
 */
void Unsenv(char *Argv[]);

/* Pjobs() Prints the current background processes, held by list */
void Pjobs(void);

/* Help() prints the various builtin commands implemented for Project 7.
 * For a complete list, see requirement (1) of Project 7 spec.
 * We also implement:
 * 	limit -- prints various limit macros (mostly defined in pr7.h)
 * 	toggle -- user can toggle verbosity, debug, and echo from interactive mode
 */
void Help(void);

/* Toggle allows the user to toggle on/off echo, verbosity, and debug output
 * Format is
 * 	toggle [e|v|d]
 */
void Toggle(char *Argv[]);

/* User-friendly wrapper for Print_Options() */
void Options(void);

/* Prints the current command line options to the console
 * Note that this is not a builtin command, and is only run when
 * 'options' is called from the shell.
 */
void Print_Options(void);


/* Set() allows toggling echo, exec types, verbose, and debug */
void Set(char *Argv[]);

/* Secho() toggles exec type (vp,ve,lp) */
void Sexec(char *Argv[]);

/* Secho() toggles echo on or off */
void Secho(char *Argv[]);

/* Secho() toggles verbose on or off */
void Sverbose(char *Argv[]);

/* Secho() toggles debug on or off */
void Sdebug(char *Argv[]);

/* Prints formatted list of command shell limitations.
 * First optional extra credit in project 7 specs.
 */
void Limits(void);

/* forcibly kill a background process
 * optional extra credit in project 7 specs.
 */
void pr7_Kill(void);

#endif /* BUILTIN_H */
