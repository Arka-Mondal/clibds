/*
  Copyright (c) 2023, Arka Mondal. All rights reserved.
  Use of this source code is governed by a BSD-style license that
  can be found in the LICENSE file.
*/

#include <stdlib.h>
#include <string.h>
#include "stack.h"

bool clibds_stack_init_conf(stack_conf_t * const restrict conf_ptr,
                            size_t given_init_capacity, size_t given_exp_factor,
                            void * (* given_conf_mem_alloc)(size_t),
                            void (* given_conf_mem_free)(void *))
{
  if (conf_ptr == NULL)
    return false;

  return clibds_vec_init_conf(&conf_ptr->conf_stack_vec, given_init_capacity,
                              given_exp_factor, given_conf_mem_alloc, given_conf_mem_free);
}

bool clibds_stack_destroy_conf(stack_conf_t * const restrict conf_ptr)
{
  if (conf_ptr == NULL)
    return false;

  return clibds_vec_destroy_conf(&conf_ptr->conf_stack_vec);
}

bool clibds_stack_init_bysize(stack_t * const stack_ptr, size_t size_given,
                              stack_conf_t * const conf_ptr)
{
  if (stack_ptr == NULL)
    return false;

  return clibds_vec_init_bysize(&stack_ptr->stack_vec, size_given,
                                (conf_ptr != NULL) ? &conf_ptr->conf_stack_vec : NULL);
}

bool clibds_stack_push(stack_t * const stack_ptr, void * data_given)
{
  if (stack_ptr == NULL)
    return false;

  return clibds_vec_push(&stack_ptr->stack_vec, data_given);
}

size_t clibds_stack_push_from_array(stack_t * const stack, void * const arr, size_t nelem)
{
  if (stack == NULL)
    return false;

  return clibds_vec_push_from_array(&stack->stack_vec, arr, nelem);
}

size_t clibds_stack_push_from_list(stack_t * const stack, list_t * const list)
{
  if (stack == NULL)
    return false;

  return clibds_vec_push_from_list(&stack->stack_vec, list);
}

size_t clibds_stack_push_from_flist(stack_t * const stack, flist_t * const flist)
{
  if (stack == NULL)
    return false;

  return clibds_vec_push_from_flist(&stack->stack_vec, flist);
}

bool clibds_stack_pop(stack_t * const stack_ptr)
{
  if (stack_ptr == NULL)
    return false;

  return clibds_vec_pop(&stack_ptr->stack_vec);
}

void * clibds_stack_peek(stack_t * const stack_ptr)
{
  if (stack_ptr == NULL)
    return false;

  return clibds_vec_getlast(&stack_ptr->stack_vec);
}

size_t clibds_stack_clear(stack_t * const stack_ptr)
{
  if (stack_ptr == NULL)
    return 0;

  return clibds_vec_clear(&stack_ptr->stack_vec);
}

size_t clibds_stack_delete(stack_t * const stack_ptr)
{
  if (stack_ptr == NULL)
    return 0;

  return clibds_vec_delete(&stack_ptr->stack_vec);
}
