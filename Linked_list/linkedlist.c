/*
  Copyright (c) 2023, Arka Mondal. All rights reserved.
  Use of this source code is governed by a BSD-style license that
  can be found in the LICENSE file.
*/

#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

static fnode_t * clibds_flist_reverse_bynode(fnode_t * current)
{
  fnode_t * prevnode, * nextnode;

  prevnode = NULL;

  while (current != NULL)
  {
    nextnode = current->next;
    current->next = prevnode;
    prevnode = current;
    current = nextnode;
  }

  return prevnode;
}

bool clibds_flist_init_bysize(flist_t * const restrict ptr, size_t size_given,
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

bool clibds_flist_pushback(flist_t * const restrict ptr, void * const data_given)
{
  fnode_t * current;
  void * dataptr;

  if (ptr == NULL)
    return false;

  current = (fnode_t *) ptr->mem_alloc((ptr->memsize) + sizeof(fnode_t));
  if (current == NULL)
    return false;
  else
  {
    dataptr = (void *) (current + 1);

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

size_t clibds_flist_pushback_from_array(flist_t * const restrict flist,
                                        void * const arr, size_t nelem)
{
  uint8_t * ptr, * end_ptr;
  size_t count;

  if ((flist == NULL) || (arr == NULL) || (nelem == 0))
    return 0;

  count = 0;
  end_ptr = ((uint8_t *) arr) + (flist->memsize * nelem);
  for (ptr = arr; ptr < end_ptr; ptr += flist->memsize, count++)
    if (!clibds_flist_pushback(flist, ptr))
      break;

  return count;
}

size_t clibds_flist_pushback_from_vector(flist_t * const restrict flist,
                                         vector_t * const vect)
{
  size_t count;

  if ((flist == NULL) || (vect == NULL)
      || (flist->memsize != vect->memsize))
    return 0;

  count = 0;

  clibds_vec_foreach(vect, it)
  {
    if (!clibds_flist_pushback(flist, it))
      break;
  }

  return count;
}

bool clibds_flist_pushfront(flist_t * const restrict ptr, void * const data_given)
{
  fnode_t * current, * temp;
  void * dataptr;

  if (ptr == NULL)
    return false;

  current = (fnode_t *) ptr->mem_alloc((ptr->memsize) + sizeof(fnode_t));
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

size_t clibds_flist_pushfront_from_array(flist_t * const restrict flist,
                                         void * const arr, size_t nelem)
{
  uint8_t * ptr, * end_ptr;
  size_t count;

  if ((flist == NULL) || (arr == NULL) || (nelem == 0))
    return 0;

  count = 0;
  end_ptr = ((uint8_t *) arr) + (flist->memsize * nelem);
  for (ptr = arr; ptr < end_ptr; ptr += flist->memsize, count++)
    if (!clibds_flist_pushfront(flist, ptr))
      break;

  return count;
}

size_t clibds_flist_pushfront_from_vector(flist_t * const restrict flist,
                                          vector_t * const vect)
{
  size_t count;

  if ((flist == NULL) || (vect == NULL)
      || (flist->memsize != vect->memsize))
    return 0;

  count = 0;

  clibds_vec_foreach(vect, it)
  {
    if (!clibds_flist_pushfront(flist, it))
      break;
  }

  return count;
}

bool clibds_flist_pushatindex(flist_t * const restrict ptr,
                              size_t index, void * const data_given)
{
  fnode_t * temp, * current;
  void * dataptr;
  size_t k;

  if (ptr == NULL)
    return false;

  temp = ptr->head;

  if ((index >= 1) && (index < ptr->size))
  {
    current = (fnode_t *) ptr->mem_alloc((ptr->memsize) + sizeof(fnode_t));
    if (current == NULL)
      return false;

    dataptr = (void *) (current + 1);

    memcpy(dataptr, data_given, ptr->memsize);

    current->data = dataptr;
    current->next = NULL;
    ptr->size += 1;

    for (k = 0; k < index - 1; k++)
      temp = temp->next;

    current->next = temp->next;
    temp->next = current;

    return true;
  }
  else if ((index == 0) && (index < ptr->size))
    return clibds_flist_pushfront(ptr, data_given);
  else
    return false;
}

bool clibds_flist_popfront(flist_t * const restrict ptr)
{
  fnode_t * temp;

  if (ptr == NULL)
    return false;

  temp = ptr->head;

  if (temp != NULL)
  {
    ptr->head = ptr->head->next;

    if (ptr->size == 1)
      ptr->tail = NULL;

    ptr->mem_free(temp);
    ptr->size -= 1;

    return true;
  }
  else
    return false;
}

bool clibds_flist_popback(flist_t * const restrict ptr)
{
  fnode_t * temp;

  if (ptr == NULL)
    return false;

  temp = ptr->head;

  if (temp != NULL)
  {
    if (ptr->size == 1)
    {
      ptr->head = NULL;
      ptr->tail = NULL;
      ptr->mem_free(temp);
    }
    else
    {
      while (temp->next->next != NULL)
      {
        temp = temp->next;
      }

      ptr->mem_free(temp->next);
      temp->next = NULL;
      ptr->tail = temp;
    }

    ptr->size -= 1;

    return true;
  }
  else
    return false;
}

bool clibds_flist_popatindex(flist_t * const restrict ptr, size_t index)
{
  fnode_t * temp, * delete;
  size_t k;

  if (ptr == NULL)
    return false;

  temp = ptr->head;

  if ((index >= 1) && (index < ptr->size))
  {
    for (k = 0; k < index - 1; k++)   // k < ptr->size - 1
      temp = temp->next;

    if (index == ptr->size - 1)       // end of the list (temp->next->next == NULL)
    {
      delete = temp->next;
      ptr->tail = temp;
      temp->next = NULL;
    }
    else
    {
      delete = temp->next;
      temp->next = delete->next;
    }

    ptr->size -= 1;
    ptr->mem_free(delete);

    return true;
  }
  else if ((index == 0) && (index < ptr->size))
    return clibds_flist_popfront(ptr);
  else
    return false;
}

size_t clibds_flist_clear(flist_t * const restrict ptr)
{
  fnode_t * temp;
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

size_t clibds_flist_delete(flist_t * const restrict ptr)
{
  size_t count;

  if (ptr == NULL)
    return 0;

  count = clibds_flist_clear(ptr);

  ptr->memsize = 0;

  return count;
}

void * clibds_flist_getatindex(flist_t * const restrict ptr, size_t index)
{
  fnode_t * temp;
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

size_t clibds_flist_reverse(flist_t * const restrict ptr)
{
  fnode_t * prevnode, * nextnode;
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
      ptr->head->next = prevnode;       // reverse the link
      prevnode = ptr->head;
      ptr->head = nextnode;
      count++;
    }

    ptr->head = prevnode;               // head becomes NULL so update the head pointer
  }

  return count;
}

bool clibds_flist_ispalindrome(flist_t * const restrict ptr, int (* compare)(const void *, const void *))
{
  fnode_t * fastptr, * slowptr;
  bool is_palindrome;

  if (ptr == NULL)
    return true;

  is_palindrome = true;
  fastptr = ptr->head;
  slowptr = ptr->head;

  if (ptr->size <= 1)
    return is_palindrome;

  while (fastptr != NULL)         // getting the address of the middle node of the List
  {
    fastptr = fastptr->next;
    if (fastptr == NULL)
      break;

    fastptr = fastptr->next;
    slowptr = slowptr->next;
  }

  // got the middle element of the linked list -> slowptr

  // resuing fastptr for the last node of the list and slowptr for the head of the node

  fastptr = clibds_flist_reverse_bynode(slowptr);
  slowptr = ptr->head;

  while ((fastptr != NULL) && (fastptr != slowptr))     // (fastptr != slowptr) can be neglected
  {
    if (compare(fastptr->data, slowptr->data) != 0)
    {
      is_palindrome = false;
      break;
    }
    fastptr = fastptr->next;
    slowptr = slowptr->next;
  }

  clibds_flist_reverse_bynode(ptr->tail);

  return is_palindrome;
}
