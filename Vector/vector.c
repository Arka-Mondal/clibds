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
  uint8_t * new_buffer;

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

  new_buffer = (uint8_t *) vec_ptr->mem_alloc(temp_capacity * vec_ptr->memsize);
  if (new_buffer == NULL)
    return false;

  memcpy(new_buffer, vec_ptr->buffer, vec_ptr->size * vec_ptr->memsize);

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

bool clibds_vec_init_bysize(vector_t * const vec_ptr, size_t element_size,
                            vector_conf_t * const conf_ptr)
{
  if ((vec_ptr == NULL) || (element_size == 0)
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

  vec_ptr->buffer = (uint8_t *) vec_ptr->mem_alloc(vec_ptr->capacity * element_size);
  if (vec_ptr->buffer == NULL)
    return false;

  vec_ptr->memsize = element_size;
  vec_ptr->size = 0;


  return true;
}

/*
 * Inserts a new element to the end the vector.
 *
 * It returns false if resizing of the buffer or the
 * allocation of spcae for the new element failed.
*/

bool clibds_vec_push(vector_t * const vec_ptr, void * const data_given)
{
  bool status;
  void * addr;

  if ((vec_ptr == NULL) || (vec_ptr->buffer == NULL)
      || (data_given == NULL))
    return false;

  if (vec_ptr->size >= vec_ptr->capacity)
  {
    // resize the buffer

    status = clibds_vec_resizebuffer(vec_ptr);
    if (!status)
      return false;
  }

  addr = vec_ptr->buffer + (vec_ptr->size * vec_ptr->memsize);
  memcpy(addr, data_given, vec_ptr->memsize);
  vec_ptr->size++;

  return true;
}

/*
 * Inserts an array of elements to the end the vector.
 *
 * It returns the number of inserted elements.
*/

size_t clibds_vec_push_from_array(vector_t * const vect, void * const arr, size_t nelem)
{
  uint8_t * ptr, * end_ptr;
  size_t count;

  if ((vect == NULL) || (arr == NULL) || (nelem == 0))
    return 0;

  count = 0;

  end_ptr = ((uint8_t *) arr) + (vect->memsize * nelem);
  for (ptr = arr; ptr < end_ptr; ptr += vect->memsize, count++)
    if (!clibds_vec_push(vect, ptr))
      break;

  return count;
}

/*
 * Inserts elements to the end the vector from a list.
 *
 * It returns the number of inserted elements.
*/

size_t clibds_vec_push_from_list(vector_t * const vect, list_t * const list)
{
  size_t count;

  if ((vect == NULL) || (list == NULL) || (vect->memsize != list->memsize))
    return 0;

  count = 0;

  clibds_list_foreach(list, it)
  {
    if (!clibds_vec_push(vect, it->data))
      break;
    count++;
  }

  return count;
}

/*
 * Inserts elements to the end the vector from a forward list.
 *
 * It returns the number of inserted elements.
*/

size_t clibds_vec_push_from_flist(vector_t * const vect, flist_t * const flist)
{
  size_t count;

  if ((vect == NULL) || (flist == NULL) || (vect->memsize != flist->memsize))
    return 0;

  count = 0;

  clibds_flist_foreach(flist, it)
  {
    if (!clibds_vec_push(vect, it->data))
      break;
    count++;
  }

  return count;
}

/*
 * Inserts a new element to the end of the vector assuming
 * sufficient capacity of the buffer.
 *
 * It returns false if there is not enough capacity of
 * the buffer or the allocation of spcae for the new element failed.
*/

bool clibds_vec_push_assumecapacity(vector_t * const vec_ptr, void * const data_given)
{
  void * addr;

  if ((vec_ptr == NULL) || (vec_ptr->buffer == NULL)
      || (data_given == NULL))
    return false;

  if (vec_ptr->size >= vec_ptr->capacity)
    return false;

  addr = vec_ptr->buffer + (vec_ptr->size * vec_ptr->memsize);
  memcpy(addr, data_given, vec_ptr->memsize);
  vec_ptr->size++;

  return true;
}

/*
 * Inserts a new element to an index the vector.
 *
 * It returns false if resizing of the buffer or the
 * allocation of spcae for the new element failed.
*/

bool clibds_vec_insert(vector_t * const vec_ptr, size_t index,
                       void * const data_given)
{
  bool status;
  void * addr_from, * addr_to;

  if ((vec_ptr == NULL) || (vec_ptr->buffer == NULL)
      || (data_given == NULL) || (index > vec_ptr->size))
    return false;

  if (index == vec_ptr->size)
    return clibds_vec_push(vec_ptr, data_given);

  if (vec_ptr->size >= vec_ptr->capacity)
  {
    // resize the buffer

    status = clibds_vec_resizebuffer(vec_ptr);
    if (!status)
      return false;
  }

  // shifts the elemets [index...size) to the higher index
  addr_from = vec_ptr->buffer + (index * vec_ptr->memsize);
  addr_to = vec_ptr->buffer + ((index + 1) * vec_ptr->memsize);
  memmove(addr_to, addr_from, (vec_ptr->size - index) * vec_ptr->memsize);
  memcpy(addr_from, data_given, vec_ptr->memsize);
  vec_ptr->size++;

  return true;
}

/*
 * Inserts a new element to an index of the vector assuming
 * sufficient capacity of the buffer.
 *
 * It returns false if there is not enough capacity of
 * the buffer or the allocation of spcae for the new element failed.
*/

bool clibds_vec_insert_assumecapacity(vector_t * const vec_ptr,
                                      size_t index, void * const data_given)
{
  void * addr_from, * addr_to;

  if ((vec_ptr == NULL) || (vec_ptr->buffer == NULL)
      || (data_given == NULL) || (index > vec_ptr->size))
    return false;

  if (index == vec_ptr->size)
    return clibds_vec_push_assumecapacity(vec_ptr, data_given);

  if (vec_ptr->size >= vec_ptr->capacity)
    return false;

  // shifts the elemets [index...size) to the higher index
  addr_from = vec_ptr->buffer + (index * vec_ptr->memsize);
  addr_to = vec_ptr->buffer + ((index + 1) * vec_ptr->memsize);
  memmove(addr_to, addr_from, (vec_ptr->size - index) * vec_ptr->memsize);
  memcpy(addr_from, data_given, vec_ptr->memsize);
  vec_ptr->size++;

  return true;
}

/*
 * Removes an element from the back of the vector
 *
 * It returns false if the buffer is NULL or the vec_ptr is NULL
 * otherwise true.
*/

bool clibds_vec_pop(vector_t * const vec_ptr)
{
  if ((vec_ptr == NULL) || (vec_ptr->buffer == NULL)
      || (vec_ptr->size == 0))
    return false;

  vec_ptr->size--;

  return true;
}

/*
 * Removes an element from an index of the vector
 *
 * It returns false if the buffer is NULL or the vec_ptr is NULL
 * or the index is out of bound otherwise true.
*/

bool clibds_vec_remove(vector_t * const vec_ptr, size_t index)
{
  void * addr_from, * addr_to;

  if ((vec_ptr == NULL) || (vec_ptr->buffer == NULL)
      || (index >= vec_ptr->size))
    return false;

  if (index == vec_ptr->size - 1)
    return clibds_vec_pop(vec_ptr);

  addr_from = vec_ptr->buffer + ((index + 1) * vec_ptr->memsize);
  addr_to = vec_ptr->buffer + (index * vec_ptr->memsize);
  memmove(addr_to, addr_from, (vec_ptr->size - index - 1) * vec_ptr->memsize);
  vec_ptr->size--;

  return true;
}

/*
 * Returns the last element in the vector.
 *
 * Returns NULL if the vec_ptr is NULL or the buffer is NULL
 * or the size of the vector is 0.
*/

void * clibds_vec_getlast(vector_t * const vec_ptr)
{
  if ((vec_ptr == NULL) || (vec_ptr->buffer == NULL)
      || (vec_ptr->size == 0))
    return NULL;

  return vec_ptr->buffer + (vec_ptr->size - 1) * vec_ptr->memsize;
}

/*
 * Returns the element at an index in the vector.
 *
 * Returns NULL if the vec_ptr is NULL or the buffer is NULL
 * or the size of the vector is 0 or index out of bound.
*/

void * clibds_vec_getatindex(vector_t * const vec_ptr, size_t index)
{
  if ((vec_ptr == NULL) || (vec_ptr->buffer == NULL)
      || (index >= vec_ptr->size))
    return NULL;

  return vec_ptr->buffer + index * vec_ptr->memsize;
}

/*
 * Clones a vector
 *
 * Returns false if the new_vec is NULL or the old_vec is NULL
 * or the memory allocation is failed.
*/

bool clibds_vec_clone(vector_t * const new_vec, vector_t * const old_vec)
{
  uint8_t * temp;

  if ((new_vec == NULL) || (old_vec == NULL))
    return false;

  new_vec->size = old_vec->size;
  new_vec->capacity = old_vec->capacity;
  new_vec->memsize = old_vec->memsize;
  new_vec->exp_factor = old_vec->exp_factor;
  new_vec->mem_alloc = old_vec->mem_alloc;
  new_vec->mem_free = old_vec->mem_free;

  temp = (uint8_t *) new_vec->mem_alloc(old_vec->capacity * old_vec->memsize);
  if (temp == NULL)
    return false;

  memcpy(new_vec->buffer, old_vec->buffer, old_vec->size * old_vec->memsize);

  return true;
}

/*
 * Creates a slice of a vector[index_start:index_end]
 *
 * Returns false if the new_vec is NULL or the old_vec is NULL
 * or the memory allocation is failed.
*/

bool clibds_vec_slice(vector_t * const new_vec, vector_t * const old_vec,
                      size_t index_start, size_t index_end)
{
  uint8_t * temp;
  void * addr;
  size_t capacity_needed, i, capacity;

  if ((new_vec == NULL) || (old_vec == NULL) || (index_end < index_start)
      || (index_start >= old_vec->size) || (index_end >= old_vec->size))
    return false;

  capacity_needed = (index_end - index_start + 1);
  capacity = capacity_needed;

  // round up to next power of 2
  capacity--;
  for (i = 1; i <= sizeof(size_t) * 4; i = i << 1)
    capacity |= capacity >> i;
  capacity++;

  // check for overflow
  if (capacity < capacity_needed)
    capacity = VEC_MAX_CAPACITY;


  new_vec->size = capacity_needed;
  new_vec->capacity = capacity;
  new_vec->memsize = old_vec->memsize;
  new_vec->exp_factor = old_vec->exp_factor;
  new_vec->mem_alloc = old_vec->mem_alloc;
  new_vec->mem_free = old_vec->mem_free;

  temp = (uint8_t *) new_vec->mem_alloc(capacity * old_vec->memsize);
  if (temp == NULL)
    return false;

  new_vec->buffer = temp;
  addr = old_vec->buffer + index_start * old_vec->memsize;
  memcpy(new_vec->buffer, addr, capacity_needed * old_vec->memsize);

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
  size_t length;

  if ((vec_ptr == NULL) || (vec_ptr->buffer == NULL))
    return 0;

  length = vec_ptr->size;
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
