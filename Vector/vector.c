/*
  Copyright (c) 2023, Arka Mondal. All rights reserved.
  Use of this source code is governed by a BSD-style license that
  can be found in the LICENSE file.
*/

#include <stdlib.h>
#include <string.h>
#include "vector.h"

/*
 * Exapnds the buffer capacity. It returns false if the
 * capacity already reached the maximum level or if the
 * buffer allocation failed.
 *
 * In case the exapnsion would overflow the capacity range, a maximum
 * capacity is allocated instead.
*/

static bool clibds_vec_resizebuffer(vector_t * const restrict vec_ptr)
{
  size_t new_capacity, temp_capacity;
  void ** new_buffer;

  if (vec_ptr->capacity == VEC_MAX_CAPACITY)
    return false;

  new_capacity = vec_ptr->capacity * vec_ptr->exp_factor;

  /*
   * If the new_capacity is smaller or equal to the capacity
   * then the overflow has ocurred. It check the validity of
   * the new_capacity
  */

  if (new_capacity <= vec_ptr->capacity)
    temp_capacity = VEC_MAX_CAPACITY;
  else
    temp_capacity = new_capacity;

  new_buffer = (void **) vec_ptr->mem_alloc(temp_capacity * sizeof(void *));
  if (new_buffer == NULL)
    return false;

  memcpy(new_buffer, vec_ptr->buffer, vec_ptr->size * sizeof(void *));

  vec_ptr->mem_free(vec_ptr->buffer);

  vec_ptr->capacity = temp_capacity;
  vec_ptr->buffer = new_buffer;

  return true;
}

/*
 * It initializes the conf variable.
 *
 * If the given_init_capacity is 0, then is uses the default settings.
 * It is also true for the expansion factor (<= 1) and the two memory management
 * functions.
*/

bool clibds_vec_init_conf(vector_conf_t * const conf_ptr,
                          size_t given_init_capacity, size_t given_exp_factor,
                          void * (* given_conf_mem_alloc)(size_t),
                          void (* given_conf_mem_free)(void *))
{
  if (conf_ptr == NULL)
    return false;

  conf_ptr->init_capacity = (given_init_capacity != 0) ? given_init_capacity : DEFAULT_INIT_CAP;
  conf_ptr->init_exp_factor = (given_exp_factor > 1) ? given_exp_factor : DEFAULT_EXP_FAC;
  conf_ptr->conf_mem_alloc = (given_conf_mem_alloc != NULL) ? given_conf_mem_alloc : malloc;
  conf_ptr->conf_mem_free = (given_conf_mem_free != NULL) ? given_conf_mem_free : free;

  return true;
}

/*
 * It initializes the vector. And allocates the buffer for
 * the vector.
*/

bool clibds_vec_init_bysize(vector_t * const vec_ptr, size_t size_given,
                            vector_conf_t * const conf_ptr)
{
  if ((vec_ptr == NULL) || (size_given == 0)
      || ((conf_ptr != NULL) && ((conf_ptr->init_exp_factor <= 1)
      || (conf_ptr->init_capacity == 0) || (conf_ptr->conf_mem_alloc == NULL)
      || (conf_ptr->conf_mem_free == NULL))))
    return false;

  if (conf_ptr == NULL)
  {
    vec_ptr->capacity = DEFAULT_INIT_CAP;
    vec_ptr->exp_factor = DEFAULT_EXP_FAC;
    vec_ptr->mem_alloc = malloc;
    vec_ptr->mem_free = free;
  }
  else
  {
    vec_ptr->capacity = conf_ptr->init_capacity;
    vec_ptr->exp_factor = conf_ptr->init_exp_factor;
    vec_ptr->mem_alloc = conf_ptr->conf_mem_alloc;
    vec_ptr->mem_free = conf_ptr->conf_mem_free;
  }

  vec_ptr->buffer = (void **) vec_ptr->mem_alloc(vec_ptr->capacity * sizeof(void *));
  if (vec_ptr->buffer == NULL)
    return false;

  vec_ptr->memsize = size_given;
  vec_ptr->size = 0;


  return true;
}

/*
 * Inserts a new element to the the vector.
 *
 * It returns false if resizing of the buffer or the
 * allocation of spcae for the new element failed.
*/

bool clibds_vec_pushback(vector_t * const vec_ptr, void * const data_given)
{
  bool status;
  void * temp_data;

  if ((vec_ptr == NULL) || (vec_ptr->buffer == NULL))
    return false;

  if (vec_ptr->size >= vec_ptr->capacity)
  {
    // resize the buffer

    status = clibds_vec_resizebuffer(vec_ptr);
    if (!status)
      return false;
  }

  temp_data = (void *) vec_ptr->mem_alloc(vec_ptr->memsize);
  if (temp_data == NULL)
    return false;

  memcpy(temp_data, data_given, vec_ptr->memsize);
  vec_ptr->buffer[vec_ptr->size] = temp_data;
  vec_ptr->size++;

  return true;
}

/*
 * Removes an element from the back of the vector
 *
 * It returns false if the buffer is NULL or the vec_ptr is NULL
 * otherwise true.
*/

bool clibds_vec_popback(vector_t * const vec_ptr)
{
  void * temp;

  if ((vec_ptr == NULL) || (vec_ptr->buffer == NULL))
    return false;

  temp = vec_ptr->buffer[vec_ptr->size - 1];

  vec_ptr->mem_free(temp);

  vec_ptr->size--;

  return true;
}

/*
 * It removes all the elements from the vector.
 * Making the vector empty.
 *
 * It returns the size of the vector.
*/

size_t clibds_vec_clear(vector_t * const vec_ptr)
{
  size_t length, i;

  if ((vec_ptr == NULL) || (vec_ptr->buffer == NULL))
    return 0;

  length = vec_ptr->size;

  for (i = 0; i < length; i++)
    vec_ptr->mem_free(vec_ptr->buffer[i]);

  vec_ptr->size = 0;

  return length;
}

/*
 * It destroys the vector and removes all the elements
 * from it.
 *
 * It returns the size of the vector.
*/

size_t clibds_vec_delete(vector_t * const vec_ptr)
{
  size_t len;

  if ((vec_ptr == NULL) || (vec_ptr->buffer == NULL))
    return 0;

  len = clibds_vec_clear(vec_ptr);
  vec_ptr->mem_free(vec_ptr->buffer);

  vec_ptr->buffer = NULL;
  vec_ptr->capacity = 0;
  vec_ptr->memsize = 0;
  vec_ptr->exp_factor = 0;

  return len;
}
