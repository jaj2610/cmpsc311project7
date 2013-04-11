/* CMPSC 311, Spring 2013, Project 7
 * 
 * Author: Jacob Jones
 * Email: jaj5333@psu.edu
 * 
 * Author: Scott Cheloha
 * Email: ssc5145@psu.edu
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#include "wrapper.h"
#include "linked.h"
#include "pr7.h"

//------------------------------------------------------------------------------

struct linked_list *linked_list_allocate(void)
{
  struct linked_list *list = Malloc(sizeof(struct linked_list), __func__, __LINE__);

  list->head = list->tail = NULL;
  list->reference_count = 0;

  return list;
}

//------------------------------------------------------------------------------

struct linked_list *linked_list_reference(struct linked_list * const list)
{
  list->reference_count++;

  return list;
}

//------------------------------------------------------------------------------

void linked_list_deallocate(struct linked_list * const list)
{
  if (--list->reference_count > 0)
    { return; }

  struct node *prev = NULL;
  for (struct node *p = list->head; p != NULL; p = p->next)
  {
      free(prev);	// free(NULL) is harmless
      free(p->command);
      prev = p;
  }
  free(prev);		// now, prev == list->tail

  free(list);
}

//------------------------------------------------------------------------------

void linked_list_print(const struct linked_list * const list)
{
  puts("Processes:\n");

  if (list->head == NULL)
    { puts("  none"); }
  else
    {
      for (struct node *p = list->head; p != NULL; p = p->next)
	{ printf("  [%d]   [%d]   [%s]   %s\n", p->pid, p->pgid, p->status, p->command);; }
    }
}

//------------------------------------------------------------------------------

void linked_list_append(struct linked_list * const list, const char *command, pid_t pid, pid_t pgid)
{
  struct node *p = Malloc(sizeof(struct node), __func__, __LINE__);

  p->next = NULL;
  p->status = "Running";
  p->pid = pid;
  p->pgid = pgid;
  p->command = Strdup(command,  __func__, __LINE__);

  if (list->head == NULL)	// empty list, list->tail is also NULL
    {
      list->head = list->tail = p;
    }
  else
    {
      list->tail->next = p;
      list->tail = p;
    }
}
