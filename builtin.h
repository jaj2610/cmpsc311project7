
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

#endif /* BUILTIN_H */
