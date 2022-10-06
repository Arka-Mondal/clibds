/*
  Copyright (c) 2022, Arka Mondal. All rights reserved.
  Use of this source code is governed by a BSD-style license that
  can be found in the LICENSE file.
*/

#include <stdlib.h>
#include <string.h>
#include "pqueue.h"

#define parent_node(X) (((X) > 0) ? (((X) - 1) >> 1) : 0)
#define left_child(X) (((X) << 1) + 1)
#define right_child(X) (((X) << 1) + 2)

#define DEFAULT_RESIZE_FACTOR 1.5
#define DEFAULT_CAPACITY 16
#define MAX_CAPACITY ((size_t) -2)


/*
 * Maintains the heap property of the priority queue.
*/

static void max_heapify(pqueue_t * const restrict ptr, size_t index)
{
  size_t left, right, largest;
  void * temp;

  while (true)
  {
    left = left_child(index);
    right = right_child(index);

    if ((left < ptr->size) && (ptr->comparator(ptr->buffer[left], ptr->buffer[index]) > 0))
      largest = left;
    else
      largest = index;

    if ((right < ptr->size) && (ptr->comparator(ptr->buffer[right], ptr->buffer[largest]) > 0))
      largest = right;

    if (largest != index)
    {
      temp = ptr->buffer[index];
      ptr->buffer[index] = ptr->buffer[largest];
      ptr->buffer[largest] = temp;

      index = largest;
    }
    else
      break;
  }
}

/*
 * Exapnds the buffer capacity of the priority_queue. It returns
 * false if the capacity already reached the maximum level or if
 * the buffer allocation failed.
 *
 * In case the exapnsion would overflow the capacity range, a maximum
 * capacity is allocated instead.
*/

static bool resize_buffer(pqueue_t * const restrict ptr)
{
  size_t new_capacity, temp_capacity;
  void ** new_buffer;

  if (ptr->capacity == MAX_CAPACITY)
    return false;

  new_capacity = ptr->capacity * ptr->exp_factor;

  /*
   * If the new_capacity is smaller or equal to the capacity
   * then the overflow has ocurred. It check the validity of
   * the new_capacity
  */

  if (new_capacity <= ptr->capacity)
    temp_capacity = MAX_CAPACITY;
  else
    temp_capacity = new_capacity;

  new_buffer = (void **) malloc(temp_capacity * sizeof(void *));
  if (new_buffer == NULL)
    return false;

  memcpy(new_buffer, ptr->buffer, ptr->size * sizeof(void *));

  free(ptr->buffer);

  ptr->capacity = temp_capacity;
  ptr->buffer = new_buffer;

  return true;
}

/*
 * It initializes the priority queue variable. And allocates the buffer for
 * the priority queue.
*/

bool initialize_bysize_pq(pqueue_t * const restrict ptr, size_t size_given,
                   int (* compare)(const void *, const void *))
{
  if ((ptr == NULL) || (size_given == 0))
    return false;

  ptr->buffer = (void **) malloc(DEFAULT_CAPACITY * sizeof(void *));
  if (ptr->buffer == NULL)
    return false;

  ptr->size = 0;
  ptr->capacity = DEFAULT_CAPACITY;
  ptr->memsize = size_given;
  ptr->exp_factor = DEFAULT_RESIZE_FACTOR;
  ptr->comparator = compare;

  return true;
}

/*
 * Pushes a new element into the priority queue.
 *
 * It returns false if resizing of the buffer or the
 * allocation of spcae for the new element failed.
*/

bool push_pq(pqueue_t * const restrict ptr, void * element)
{
  bool status;
  size_t parent_index, current_index;
  void * temp;


  if (ptr->capacity <= ptr->size)
  {
    status = resize_buffer(ptr);

    if (status == false)
      return false;
  }

  temp = (void *) malloc(ptr->memsize);
  if (temp == NULL)
    return false;

  memcpy(temp, element, ptr->memsize);

  current_index = ptr->size;
  ptr->buffer[current_index] = temp;

  parent_index = parent_node(current_index);
  ptr->size++;

  while (current_index != 0)
  {
    if (ptr->comparator(ptr->buffer[current_index], ptr->buffer[parent_index]) > 0)
    {
      temp = ptr->buffer[current_index];
      ptr->buffer[current_index] = ptr->buffer[parent_index];
      ptr->buffer[parent_index] = temp;

      current_index = parent_index;
      parent_index = parent_node(current_index);
    }
    else
      break;
  }

  return true;
}

/*
 * Returns the most prioritized element from the priority queue
 * without popping it from the priority queue.
 *
 * It returns NULL if if the buffer is empty.
*/

void * peek_pq(pqueue_t * const restrict ptr)
{
  if ((ptr != NULL) && (ptr->buffer != NULL))
    return ptr->buffer[0];
  else
    return NULL;
}

/*
 * Removes the most prioritized element from the priority queue.
*/

bool pop_pq(pqueue_t * const restrict ptr)
{
  void * temp;

  if (ptr == NULL)
    return false;

  temp = ptr->buffer[0];
  ptr->buffer[0] = ptr->buffer[ptr->size - 1];
  // ptr->buffer[ptr->size - 1] = NULL;

  free(temp);

  ptr->size--;

  max_heapify(ptr, 0);

  return true;
}

/*
 * It removes all the elements from the priority queue.
 * Making the priority queue empty.
*/

size_t clear_pq(pqueue_t * const restrict ptr)
{
  size_t length, i;

  if (ptr == NULL)
    return 0;

  length = ptr->size;

  for (i = 0; i < length; i++)
    free(ptr->buffer[i]);

  ptr->size = 0;

  return length;
}

/*
 * It destroys the priority queue and removes all the elements
 * from it.
 *
 * It returns the size of the priority queue.
*/

size_t delete_pq(pqueue_t * const restrict ptr)
{
  size_t rv;

  if (ptr == NULL)
    return 0;

  rv = clear_pq(ptr);

  free(ptr->buffer);

  ptr->buffer = NULL;
  ptr->capacity = 0;
  ptr->memsize = 0;
  ptr->exp_factor = 0.0;
  ptr->comparator = NULL;

  return rv;
}
