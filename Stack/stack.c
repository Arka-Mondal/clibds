/*
  Copyright (c) 2023, Arka Mondal. All rights reserved.
  Use of this source code is governed by a BSD-style license that
  can be found in the LICENSE file.
*/

#include <stdlib.h>
#include <string.h>
#include "stack.h"

bool clibds_stack_init_conf(stack_conf_t * const restrict conf_ptr, size_t given_init_capacity,
                            void * (* given_conf_mem_alloc)(size_t),
                            void (* given_conf_mem_free)(void *))
{
  if (conf_ptr == NULL)
    return false;

  return clibds_vec_init_conf(conf_ptr, given_init_capacity,
                              given_conf_mem_alloc, given_conf_mem_free);
}

bool clibds_stack_destroy_conf(stack_conf_t * const restrict conf_ptr)
{
  if (conf_ptr == NULL)
    return false;

  return clibds_vec_destroy_conf(conf_ptr);
}

bool clibds_stack_init_bysize(stack_t * const stack_ptr, size_t size_given,
                              stack_conf_t * const conf_ptr)
{
  if (stack_ptr == NULL)
    return false;

  return clibds_vec_init_bysize(stack_ptr, size_given,
                                (conf_ptr != NULL) ? conf_ptr : NULL);
}

bool clibds_stack_push(stack_t * const stack_ptr, void * data_given)
{
  if (stack_ptr == NULL)
    return false;

  return clibds_vec_push(stack_ptr, data_given);
}

size_t clibds_stack_push_from_array(stack_t * const stack, void * const arr, size_t nelem)
{
  if (stack == NULL)
    return false;

  return clibds_vec_push_from_array(stack, arr, nelem);
}

bool clibds_stack_pop(stack_t * const stack_ptr)
{
  if (stack_ptr == NULL)
    return false;

  return clibds_vec_pop(stack_ptr);
}

void * clibds_stack_peek(stack_t * const stack_ptr)
{
  if (stack_ptr == NULL)
    return false;

  return clibds_vec_getlast(stack_ptr);
}

size_t clibds_stack_clear(stack_t * const stack_ptr)
{
  if (stack_ptr == NULL)
    return 0;

  return clibds_vec_clear(stack_ptr);
}

size_t clibds_stack_delete(stack_t * const stack_ptr)
{
  if (stack_ptr == NULL)
    return 0;

  return clibds_vec_delete(stack_ptr);
}

bool clibds_stack_clone(stack_t * const nstack_ptr, stack_t * const stack_ptr)
{
  if (nstack_ptr == NULL || stack_ptr == NULL)
    return false;

  return clibds_vec_clone(nstack_ptr, stack_ptr);
}
