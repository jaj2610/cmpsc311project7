/* CMPSC 311, Spring 2013, Project 7
 * 
 * Author: Jacob Jones
 * Email: jaj5333@psu.edu
 * 
 * Author: Scott Cheloha
 * Email: ssc5145@psu.edu
 */

#ifndef LINKED_H
#define LINKED_H

//------------------------------------------------------------------------------

#include <sys/types.h>


// singly-linked list of names

struct node
{
  struct node *next;	// NULL indicates end-of-list
  pid_t pid;			// process id
  pid_t pgid;			// process group id
  char *status;			// Running / Stopped
  char *command;		// command issued to start process
};

struct process_list
{
  struct node *head;	// NULL indicates empty list
  struct node *tail;
  int reference_count;	// for delayed deallocation of the list
};

//------------------------------------------------------------------------------

/* process_list_allocate() allocates a new process_list */
struct process_list *process_list_allocate(void);

/* process_list_reference() keeps track of times process_list is referenced */
struct process_list *process_list_reference(struct process_list * const list);

/* process_list_deallocate() deallocates a process_list completely */
void process_list_deallocate(struct process_list * const list);

/* process_list_print() prints entire process_list */
void process_list_print(const struct process_list * const list);

/* process_list_append() adds a node to a process_list */
void process_list_append(struct process_list * const list, const char *command, pid_t pid, pid_t pgid);

/* process_list_allocate() removes a node from a process_list */
void process_list_pop(struct process_list * const list, pid_t pid);

//------------------------------------------------------------------------------

#endif
