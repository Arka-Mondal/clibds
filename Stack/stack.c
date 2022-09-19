/*
  Copyright (c) 2022, Arka Mondal. All rights reserved.
  Use of this source code is governed by a BSD-style license that
  can be found in the LICENSE file.
*/

#include <stdlib.h>
#include <string.h>
#include "stack.h"

bool initialize_bysize_stack(stack_t * const restrict ptr, size_t size_given)
{
  if (ptr == NULL)
    return false;

  ptr->head = NULL;
  ptr->memsize = size_given;
  ptr->size = 0;

  return true;
}

bool push_stack(stack_t * const restrict ptr, void * data_given)
{
  void * dataptr;
  snode_t * current, * temp;

  if (ptr == NULL)
    return false;

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

bool pop_stack(stack_t * const restrict ptr)
{
  snode_t * temp;

  if (ptr == NULL)
    return false;

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

void * peek_stack(stack_t * const restrict ptr)
{
  if ((ptr != NULL) && (ptr->head != NULL))
    return ptr->head->data;
  else
    return NULL;
}

size_t clear_stack(stack_t * const restrict ptr)
{
  snode_t * temp;
  size_t count;

  if (ptr == NULL)
    return 0;

  count = 0;

  while (ptr->head != NULL)
  {
    temp = ptr->head;
    ptr->head = ptr->head->next;
    free(temp);
    count++;
  }

  ptr->size = 0;

  return count;
}

size_t delete_stack(stack_t * const restrict ptr)
{
  size_t count;

  if (ptr == NULL)
    return 0;

  count = clear_stack(ptr);

  ptr->memsize = 0;

  return count;
}
