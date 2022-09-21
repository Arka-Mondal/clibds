/*
  Copyright (c) 2022, Arka Mondal. All rights reserved.
  Use of this source code is governed by a BSD-style license that
  can be found in the LICENSE file.
*/

#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

bool initialize_bysize_listd(listd_t * const restrict ptr, size_t size_given)
{
  if (ptr == NULL)
    return false;

  ptr->head = NULL;
  ptr->tail = NULL;
  ptr->memsize = size_given;
  ptr->size = 0;

  return true;
}

bool pushback_listd(listd_t * const restrict ptr, void * data_given)
{
  void * dataptr;
  dnode_t * current;

  if (ptr == NULL)
    return false;

  current = (dnode_t *) malloc((ptr->memsize) + sizeof(dnode_t));
  if (current == NULL)
    return false;
  else
  {
    dataptr = (void *) (current + 1);

    memcpy(dataptr, data_given, ptr->memsize);

    current->data = dataptr;
    current->previous = ptr->tail;
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

bool pushfront_listd(listd_t * const restrict ptr, void * data_given)
{
  void * dataptr;
  dnode_t * current;

  if (ptr == NULL)
    return false;

  current = (dnode_t *) malloc((ptr->memsize) + sizeof(dnode_t));
  if (current == NULL)
    return false;
  else
  {
    dataptr = (void *) (current + 1);

    memcpy(dataptr, data_given, ptr->memsize);

    if (ptr->size == 0)
      ptr->tail = current;

    current->data = dataptr;
    current->previous = NULL;
    current->next = ptr->head;
    ptr->head = current;
    ptr->size += 1;

    return true;
  }
}

bool pushatindex_listd(listd_t * const restrict ptr, size_t index, void * data_given)
{
  dnode_t * current, * temp;
  void * dataptr;
  size_t k;

  if (ptr == NULL)
    return false;

  temp = ptr->head;

  if ((index >= 1) && (index < ptr->size))
  {
    current = (dnode_t *) malloc(ptr->memsize + sizeof(dnode_t));
    if (current == NULL)
      return false;

    dataptr = (void *) (current + 1);

    memcpy(dataptr, data_given, ptr->memsize);

    current->data = dataptr;
    // current->next = NULL;
    // current->previous = NULL;

    for (k = 0; k < index - 1; k++)
      temp = temp->next;

    current->next = temp->next;
    current->previous = temp;
    temp->next->previous = current;
    temp->next = current;

    ptr->size += 1;

    return true;
  }
  else if ((index == 0) && (index < ptr->size))
    return pushfront_listd(ptr, data_given);
  else
    return false;
}

bool popfront_listd(listd_t * const restrict ptr)
{
  dnode_t * temp;

  if (ptr == NULL)
    return false;

  if (ptr->head != NULL)
  {
    if (ptr->size == 1)
      ptr->tail = NULL;

    temp = ptr->head;
    ptr->head = ptr->head->next;

    if (ptr->head != NULL)
      ptr->head->previous = NULL;

    free(temp);
    ptr->size -= 1;

    return true;
  }
  else
    return false;
}

bool popback_listd(listd_t * const restrict ptr)
{
  dnode_t * temp;

  if (ptr == NULL)
    return false;

  if (ptr->head != NULL)
  {
    if (ptr->size == 1)
      ptr->head = NULL;

    temp = ptr->tail;
    ptr->tail = ptr->tail->previous;

    if (ptr->tail != NULL)
      ptr->tail->next = NULL;

    free(temp);
    ptr->size -= 1;

    return true;
  }
  else
    return false;
}

bool popatindex_listd(listd_t * const restrict ptr, size_t index)
{
  dnode_t * temp, * delete;
  size_t k;

  if (ptr == NULL)
    return false;

  temp = ptr->head;

  if ((index >= 1) && (index < ptr->size))
  {
    for (k = 0; k < index - 1; k++)
      temp = temp->next;

    if (index == ptr->size - 1)     // the last node should be deleted
    {
      delete = temp->next;
      ptr->tail = temp;
      temp->next = NULL;
    }
    else
    {
      delete = temp->next;
      temp->next = delete->next;
      delete->next->previous = temp;
    }

    free(delete);
    ptr->size -= 1;

    return true;
  }
  else if ((index == 0) && (index < ptr->size))
    return popfront_listd(ptr);
  else
    return false;
}

size_t clear_listd(listd_t * const restrict ptr)
{
  dnode_t * temp;
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

size_t delete_listd(listd_t * const restrict ptr)
{
  size_t count;

  if (ptr == NULL)
    return 0;

  count = clear_listd(ptr);

  ptr->memsize = 0;

  return count;
}

void * getnode_atindex_listd(listd_t * const restrict ptr, size_t index)
{
  dnode_t * temp;
  size_t k;

  if (ptr == NULL)
    return NULL;

  temp = ptr->head;

  if (temp == NULL)
    return NULL;
  else
  {
    for (k = 0; k < index; k++)
      temp = temp->next;

    return temp->data;
  }
}

size_t reverselist_listd(listd_t * const restrict ptr)
{
  dnode_t * prevnode, * nextnode;
  size_t count;

  if (ptr == NULL)
    return 0;

  count = 0;

  if (ptr->size >= 2)
  {
    prevnode = NULL;
    ptr->tail = ptr->head;

    while (ptr->head != NULL)
    {
      nextnode = ptr->head->next;

      // reverse the link
      ptr->head->next = prevnode;
      ptr->head->previous = nextnode;

      // increment the pointer
      prevnode = ptr->head;
      ptr->head = nextnode;
      count++;
    }

    ptr->head = prevnode;
  }

  return count;
}

bool is_palindrome_listd(listd_t * const restrict ptr, int (* compare)(const void *, const void *))
{
  dnode_t * begin, * end;
  bool is_palindrome;

  if (ptr == NULL)
    return true;

  begin = ptr->head;
  end = ptr->tail;
  is_palindrome = true;

  while ((begin->next != end) && (begin != end))
  {
    if (compare(begin->data, end->data) != 0)
    {
      is_palindrome = false;
      break;
    }

    begin = begin->next;
    end = end->previous;
  }

  return is_palindrome;
}
