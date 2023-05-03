/*
  Copyright (c) 2023, Arka Mondal. All rights reserved.
  Use of this source code is governed by a BSD-style license that
  can be found in the LICENSE file.
*/

#include <stdio.h>
#include "stack.h"

int main(void)
{
  int arr[6] = {21, 55, 46, 73, 91, 10}, i;

  // declare a stack type variable
  stack_t new_stack;

  // initialize the stack
  clibds_stack_init(&new_stack, int, NULL);

  // push values into the stack
  for (i = 0; i < 6; i++)
    clibds_stack_push(&new_stack, arr + i);

  clibds_stack_foreach(&new_stack, it)
  {
    printf("%d ", *(int *) *it);
  }
  putchar('\n');

  printf("Stack size: %zu\n", clibds_stack_size(&new_stack));

  // pop a value from the stack
  clibds_stack_pop(&new_stack);

  clibds_stack_foreach(&new_stack, it)
  {
    printf("%d ", *(int *) *it);
  }
  putchar('\n');

  // delete the stack
  clibds_stack_delete(&new_stack);

  return 0;
}
