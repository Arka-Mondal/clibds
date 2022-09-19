/*
  Copyright (c) 2022, Arka Mondal. All rights reserved.
  Use of this source code is governed by a BSD-style license that
  can be found in the LICENSE file.
*/

#include <stdlib.h>
#include <string.h>
#include "queue.h"

bool initialize_bysize_deque(deque_t * const restrict ptr, size_t size_given)
{
  if (ptr == NULL)
    return false;

  ptr->head = NULL;
  ptr->tail = NULL;
  ptr->memsize = size_given;
  ptr->size = 0;

  return false;
}

bool pushfront_deque(deque_t * const restrict ptr, void * data_given)
{
  qnode_t * current, * temp;
  void * dataptr;

  if (ptr == NULL)
    return false;

  current = (qnode_t *) malloc((ptr->memsize) + sizeof(qnode_t));
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

    if (ptr->tail == NULL)
      ptr->tail = ptr->head;

    return true;
  }
}

bool pushback_deque(deque_t * const restrict ptr, void * data_given)
{
  qnode_t * current;
  void * dataptr;

  if (ptr == NULL)
    return false;

  current = (qnode_t *) malloc((ptr->memsize) + sizeof(qnode_t));
  if (current == NULL)
    return false;
  else
  {
    dataptr = (void *)(current + 1);

    memcpy(dataptr, data_given, ptr->memsize);

    current->data = dataptr;
    current->next = NULL;
    ptr->size += 1;

    if (ptr->head == NULL)
    {
      ptr->head = current;
      ptr->tail = current;
    }
    else
    {
      ptr->tail->next = current;
      ptr->tail = current;
    }

    return true;
  }
}

bool popfront_deque(deque_t * const restrict ptr)
{
  qnode_t * temp;

  if (ptr == NULL)
    return false;

  temp = ptr->head;

  if (temp != NULL)
  {
    ptr->head = ptr->head->next;

    if (ptr->size == 1)
      ptr->tail = NULL;

    free(temp);
    ptr->size -= 1;

    return true;
  }
  else
    return false;
}


bool popback_deque(deque_t * const restrict ptr)
{
  qnode_t * temp;

  if (ptr == NULL)
    return false;

  temp = ptr->head;

  if (temp != NULL)
  {
    if (ptr->size == 1)
    {
      ptr->head = NULL;
      ptr->tail = NULL;
      free(temp);
    }
    else
    {
      while (temp->next->next != NULL)
        temp = temp->next;

      free(temp->next);
      temp->next = NULL;
      ptr->tail = temp;
    }

    ptr->size -= 1;

    return true;
  }
  else
    return false;
}

void * peekfront_deque(deque_t * const restrict ptr)
{
  if ((ptr != NULL) && (ptr->head != NULL))
    return ptr->head->data;
  else
    return NULL;
}

void * peekback_deque(deque_t * const restrict ptr)
{
  if ((ptr != NULL) && (ptr->head != NULL))
    return ptr->tail->data;
  else
    return NULL;
}

size_t clear_deque(deque_t * const restrict ptr)
{
  qnode_t * temp;
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

  ptr->tail = NULL;
  ptr->size = 0;

  return count;
}

size_t delete_deque(deque_t * const restrict ptr)
{
  size_t count;

  if (ptr == NULL)
    return 0;

  count = clear_deque(ptr);
  ptr->memsize = 0;

  return count;
}
