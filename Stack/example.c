/*
  Copyright (c) 2022, Arka Mondal. All rights reserved.
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
  initialize_stack(&new_stack, int);

  // push values into the stack
  for (i = 0; i < 6; i++)
    push_stack(&new_stack, arr + i);

  // iterate over each element in the stack starting from the stack top
  for_each_in_stack(&new_stack, it)
    printf("%d ", format_data(it->data, int));
  putchar('\n');

  // pop an element from the stack
  pop_stack(&new_stack);

  // stack peek
  printf("stack top (after the pop_stack()): %d\n", format_data(peek_stack(&new_stack), int));

  // check the size of the stack
  printf("stack size: %zu\n", new_stack.size);

  // delete the stack
  delete_stack(&new_stack);

  return 0;
}
