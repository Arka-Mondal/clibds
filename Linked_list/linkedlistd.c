/*
  Copyright (c) 2023, Arka Mondal. All rights reserved.
  Use of this source code is governed by a BSD-style license that
  can be found in the LICENSE file.
*/

#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

bool clibds_list_init_conf(list_conf_t * const restrict conf_ptr,
                           void * (* given_conf_mem_alloc)(size_t),
                           void (* given_conf_mem_free)(void *))
{
  if (conf_ptr == NULL)
    return false;

  conf_ptr->conf_mem_alloc = (given_conf_mem_alloc != NULL) ? given_conf_mem_alloc : malloc;
  conf_ptr->conf_mem_free = (given_conf_mem_free != NULL) ? given_conf_mem_free : free;

  return true;
}

bool clibds_list_init_bysize(list_t * const restrict ptr, size_t size_given,
                             list_conf_t * const conf_ptr)
{
  if ((ptr == NULL) || ((conf_ptr != NULL)
     && ((conf_ptr->conf_mem_alloc == NULL)
     || (conf_ptr->conf_mem_free == NULL))))
    return false;

  if (conf_ptr == NULL)
  {
    ptr->mem_alloc = malloc;
    ptr->mem_free = free;
  }
  else
  {
    ptr->mem_alloc = conf_ptr->conf_mem_alloc;
    ptr->mem_free = conf_ptr->conf_mem_free;
  }

  ptr->head = NULL;
  ptr->tail = NULL;
  ptr->memsize = size_given;
  ptr->size = 0;

  return true;
}

bool clibds_list_pushback(list_t * const restrict ptr, void * const data_given)
{
  void * dataptr;
  node_t * current;

  if (ptr == NULL)
    return false;

  current = (node_t *) ptr->mem_alloc((ptr->memsize) + sizeof(node_t));
  if (current == NULL)
    return false;
  else
  {
    dataptr = (void *) (current + 1);

    memcpy(dataptr, data_given, ptr->memsize);

    current->data = dataptr;
    current->previous = ptr->tail;
    current->next = NULL;
    ptr->size++;

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

size_t clibds_list_pushback_from_array(list_t * const restrict list,
                                       void * const arr, size_t nelem)
{
  uint8_t * ptr, * end_ptr;
  size_t count;

  if ((list == NULL) || (arr == NULL) || (nelem == 0))
    return 0;

  count = 0;
  end_ptr = ((uint8_t *) arr) + (list->memsize * nelem);
  for (ptr = arr; ptr < end_ptr; ptr += list->memsize, count++)
    if (!clibds_list_pushback(list, ptr))
      break;

  return count;
}

bool clibds_list_pushfront(list_t * const restrict ptr, void * const data_given)
{
  void * dataptr;
  node_t * current;

  if (ptr == NULL)
    return false;

  current = (node_t *) ptr->mem_alloc((ptr->memsize) + sizeof(node_t));
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
    ptr->head->previous = current;
    ptr->head = current;
    ptr->size++;

    return true;
  }
}

size_t clibds_list_pushfront_from_array(list_t * const restrict list,
                                       void * const arr, size_t nelem)
{
  uint8_t * ptr, * end_ptr;
  size_t count;

  if ((list == NULL) || (arr == NULL) || (nelem == 0))
    return 0;

  count = 0;
  end_ptr = ((uint8_t *) arr) + (list->memsize * nelem);
  for (ptr = arr; ptr < end_ptr; ptr += list->memsize, count++)
    if (!clibds_list_pushfront(list, ptr))
      break;

  return count;
}

bool clibds_list_pushatindex(list_t * const restrict ptr, size_t index,
                             void * const data_given)
{
  node_t * current, * temp;
  void * dataptr;
  size_t k;

  if (ptr == NULL)
    return false;

  temp = ptr->head;

  if ((index >= 1) && (index < ptr->size))
  {
    current = (node_t *) ptr->mem_alloc(ptr->memsize + sizeof(node_t));
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

    ptr->size++;

    return true;
  }
  else if ((index == 0) && (index < ptr->size))
    return clibds_list_pushfront(ptr, data_given);
  else
    return false;
}

bool clibds_list_popfront(list_t * const restrict ptr)
{
  node_t * temp;

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

    ptr->mem_free(temp);
    ptr->size--;

    return true;
  }
  else
    return false;
}

bool clibds_list_popback(list_t * const restrict ptr)
{
  node_t * temp;

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

    ptr->mem_free(temp);
    ptr->size--;

    return true;
  }
  else
    return false;
}

bool clibds_list_popatindex(list_t * const restrict ptr, size_t index)
{
  node_t * temp, * delete;
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

    ptr->mem_free(delete);
    ptr->size--;

    return true;
  }
  else if ((index == 0) && (index < ptr->size))
    return clibds_list_popfront(ptr);
  else
    return false;
}

size_t clibds_list_clear(list_t * const restrict ptr)
{
  node_t * temp;
  size_t count;

  if (ptr == NULL)
    return 0;

  count = 0;

  while (ptr->head != NULL)
  {
    temp = ptr->head;
    ptr->head = ptr->head->next;

    ptr->mem_free(temp);
    count++;
  }

  ptr->tail = NULL;
  ptr->size = 0;

  return count;
}

size_t clibds_list_delete(list_t * const restrict ptr)
{
  size_t count;

  if (ptr == NULL)
    return 0;

  count = clibds_list_clear(ptr);

  ptr->memsize = 0;

  return count;
}

void * clibds_list_getatindex(list_t * const restrict ptr, size_t index)
{
  node_t * temp;
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

size_t clibds_list_reverse(list_t * const restrict ptr)
{
  node_t * prevnode, * nextnode;
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
/*
bool clibds_list_ispalindrome(list_t * const restrict ptr,
                         int (* compare)(const void *, const void *))
{
  node_t * begin, * end;
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
*/
