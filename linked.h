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

// singly-linked list of names

struct node
{
  struct node *next;	// NULL indicates end-of-list
  pid_t pid;			// process id
  pid_t pgid;			// process group id
  char *status;			// Running / Stopped
  char *command;		// command issued to start process
};

struct linked_list
{
  struct node *head;	// NULL indicates empty list
  struct node *tail;
  int reference_count;	// for delayed deallocation of the list
};

//------------------------------------------------------------------------------

struct linked_list *linked_list_allocate(void);

struct linked_list *linked_list_reference(struct linked_list * const list);

void linked_list_deallocate(struct linked_list * const list);

void linked_list_print(const struct linked_list * const list);

void linked_list_append(struct linked_list * const list, const char *command, pid_t pid, pid_t pgid);

//------------------------------------------------------------------------------

#endif