
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

/* Needs writing: */
void Penv(char *Argv[]);
void Senv(char *Argv[]);
void Unsenv(char *Argv[]);

/* Pjobs() Prints the current background processes, held by list */
void Pjobs(const struct process_list * const list);

/* Help() prints the various builtin commands implemented in pr7
 */
void Help(void);

#endif /* BUILTIN_H */
