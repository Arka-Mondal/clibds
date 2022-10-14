/*
  Copyright (c) 2022, Arka Mondal. All rights reserved.
  Use of this source code is governed by a BSD-style license that
  can be found in the LICENSE file.
*/

#include <stdlib.h>
#include <string.h>
#include "stack.h"

#define DEFAULT_RESIZE_FACTOR 1.5
#define DEFAULT_CAPACITY 16
#define MAX_CAPACITY ((size_t) -2)

static bool resize_buffer(stack_t * const restrict ptr)
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

bool initialize_bysize_stack(stack_t * const restrict ptr, size_t size_given)
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

  return true;
}

bool push_stack(stack_t * const restrict ptr, void * element)
{
  bool status;
  void * temp;

  if ((ptr == NULL) || (element == NULL))
    return false;

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
  ptr->buffer[ptr->size] = temp;
  ptr->size++;

  return true;
}

bool pop_stack(stack_t * const restrict ptr)
{
  if ((ptr == NULL) || (ptr->buffer == NULL) || (ptr->size == 0))
    return false;

  ptr->size--;
  free(ptr->buffer[ptr->size]);

  return true;
}

void * peek_stack(stack_t * const restrict ptr)
{
  if ((ptr != NULL) && (ptr->buffer != NULL) && (ptr->size > 0))
    return ptr->buffer[ptr->size - 1];
  else
    return NULL;
}

size_t clear_stack(stack_t * const restrict ptr)
{
  size_t count, i;

  if ((ptr == NULL) || (ptr->buffer == NULL))
    return 0;

  count = ptr->size;

  for (i = 0; i < count; i++)
    free(ptr->buffer[i]);

  ptr->size = 0;

  return count;
}

size_t delete_stack(stack_t * const restrict ptr)
{
  size_t count;

  if ((ptr == NULL) || (ptr->buffer == NULL))
    return 0;

  count = clear_stack(ptr);

  free(ptr->buffer);

  ptr->buffer = NULL;
  ptr->capacity = 0;
  ptr->memsize = 0;
  ptr->exp_factor = 0.0;

  return count;
}
