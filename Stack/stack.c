#include <stdlib.h>
#include <string.h>
#include "stack.h"

void internal_initialize_stack(stack_t * const ptr, size_t size_given)
{
  ptr->head = NULL;
  ptr->memsize = size_given;
  ptr->size = 0;
}

bool stack_push(stack_t * const ptr, void * data_given)
{
  void * dataptr;
  snode_t * current, * temp;

  current = (snode_t *) malloc((ptr->memsize) + sizeof(snode_t));
  if (current == NULL)
    return false;
  else
  {
    dataptr = (void *) (current + 1);

    memcpy(dataptr, data_given, ptr->memsize);

    current->data = dataptr;
    temp = ptr->head;
    ptr->head = current;
    current->next = temp;
    ptr->size += 1;

    return true;
  }
}

bool stack_pop(stack_t * const ptr)
{
  snode_t * temp;

  temp = ptr->head;

  if (temp != NULL)
  {
    ptr->head = ptr->head->next;

    free(temp);
    ptr->size -= 1;

    return true;
  }
  else
    return false;
}

void * stack_peek(stack_t * const ptr)
{
  if (ptr->head != NULL)
    return ptr->head->data;
  else
    return NULL;
}

size_t delete_stack(stack_t * ptr)
{
  snode_t * temp;
  size_t count;

  count = 0;

  while (ptr->head != NULL)
  {
    temp = ptr->head;
    ptr->head = ptr->head->next;
    free(temp);
    count++;
  }

  ptr->memsize = 0;
  ptr->size = 0;

  return count;
}
