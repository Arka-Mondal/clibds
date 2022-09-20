/*
  Copyright (c) 2022, Arka Mondal. All rights reserved.
  Use of this source code is governed by a BSD-style license that
  can be found in the LICENSE file.
*/

#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

static node_t * reverse_list_bynode(node_t * current)
{
  node_t * prevnode, * nextnode;

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

bool initialize_bysize_list(list_t * const restrict ptr, size_t size_given)
{
  if (ptr == NULL)
    return false;

  ptr->head = NULL;
  ptr->tail = NULL;
  ptr->memsize = size_given;
  ptr->size = 0;

  return true;
}

bool pushback_list(list_t * const restrict ptr, void * data_given)
{
  node_t * current;
  void * dataptr;

  if (ptr == NULL)
    return false;

  current = (node_t *) malloc((ptr->memsize) + sizeof(node_t));
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

bool pushfront_list(list_t * const restrict ptr, void * data_given)
{
  node_t * current, * temp;
  void * dataptr;

  if (ptr == NULL)
    return false;

  current = (node_t *) malloc((ptr->memsize) + sizeof(node_t));
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

bool pushatindex_list(list_t * const restrict ptr, size_t index, void * data_given)
{
  node_t * temp, * current;
  void * dataptr;
  size_t k;

  if (ptr == NULL)
    return false;

  temp = ptr->head;

  if ((index >= 1) && (index < ptr->size))
  {
    current = (node_t *) malloc((ptr->memsize) + sizeof(node_t));
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
    return pushfront_list(ptr, data_given);
  else
    return false;
}

size_t clear_list(list_t * const restrict ptr)
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
    free(temp);
    count++;
  }

  ptr->tail = NULL;
  ptr->size = 0;

  return count;
}

size_t delete_list(list_t * const restrict ptr)
{
  size_t count;

  if (ptr == NULL)
    return 0;

  count = clear_list(ptr);

  ptr->memsize = 0;

  return count;
}

bool popfront_list(list_t * const restrict ptr)
{
  node_t * temp;

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

bool popback_list(list_t * const restrict ptr)
{
  node_t * temp;

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
      {
        temp = temp->next;
      }

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

bool popatindex_list(list_t * const restrict ptr, size_t index)
{
  node_t * temp, * delete;
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
    free(delete);

    return true;
  }
  else if ((index == 0) && (index < ptr->size))
    return popfront_list(ptr);
  else
    return false;
}

void * getnodedata_atindex_list(list_t * const restrict ptr, size_t index)
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

size_t reverse_list(list_t * const restrict ptr)
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
      ptr->head->next = prevnode;       // reverse the link
      prevnode = ptr->head;
      ptr->head = nextnode;
      count++;
    }

    ptr->head = prevnode;               // head becomes NULL so update the head pointer
  }

  return count;
}

bool is_palindrome_list(list_t * const restrict ptr, int (* compare)(const void *, const void *))
{
  node_t * fastptr, * slowptr;
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

  fastptr = reverse_list_bynode(slowptr);
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

  reverse_list_bynode(ptr->tail);

  return is_palindrome;
}
