/*
  Copyright (c) 2023, Arka Mondal. All rights reserved.
  Use of this source code is governed by a BSD-style license that
  can be found in the LICENSE file.
*/

#include <stdio.h>
#include "stack.h"

int main(void)
{
  int arr[6] = {21, 55, 46, 73, 91, 10};

  // declare a stack type variable
  stack_t new_stack;

  // initialize stack with default configuration
  // equivalent to: clibds_stack_init(&new_stack, int, NULL);
  clibds_stack_init(&new_stack, int);

  // push values into the stack
  clibds_stack_push_from_array(&new_stack, arr, 6);

  clibds_stack_foreach(&new_stack, it)
    printf("%d ", clibds_stack_fmt_data(it, int));
  putchar('\n');

  printf("Stack size: %zu\n", clibds_stack_size(&new_stack));

  // pop a value from the stack
  clibds_stack_pop(&new_stack);

  clibds_stack_foreach(&new_stack, it)
    printf("%d ", clibds_stack_fmt_data(it, int));
  putchar('\n');

  // delete the stack
  clibds_stack_delete(&new_stack);

  return 0;
}
