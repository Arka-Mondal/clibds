/*
  Copyright (c) 2022, Arka Mondal. All rights reserved.
  Use of this source code is governed by a BSD-style license that
  can be found in the LICENSE file.
*/

#include <stdio.h>
#include "queue.h"

int main(void)
{
  int arr[6] = {42, 38, 74, 22, 30, 19}, i;

  // declare a queue type variable
  queue_t new_queue;

  // initialize the queue
  initialize_queue(&new_queue, int);

  // push values into the queue
  for (i = 0; i < 6; i++)
    push_queue(&new_queue, arr + i);

  // iterate over each element in the stack starting from the queue front
  for_each_in_queue(&new_queue, it)
    printf("%d ", format_data(it->data, int));
  putchar('\n');

  // queue peek
  printf("queue front: %d\n", format_data(peek_queue(&new_queue), int));

  // pop from the queue
  pop_queue(&new_queue);

  // iterate over each element in the stack starting from the queue front
  for_each_in_queue(&new_queue, it)
    printf("%d ", format_data(it->data, int));
  putchar('\n');

  // check the size of the queue
  printf("queue size: %zu\n", new_queue.size);

  // delete the queue
  delete_queue(&new_queue);

  return 0;
}
