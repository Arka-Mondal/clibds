/*
  Copyright (c) 2022, Arka Mondal. All rights reserved.
  Use of this source code is governed by a BSD-style license that
  can be found in the LICENSE file.
*/

#include <stdio.h>
#include <stdlib.h>
#include "pqueue.h"

int cmp_data(const void *, const void *);

int main(void)
{
  bool status;
  int vals[6] = {5, 8, 3, 4, 9, 1}, i;

  // declare a priority queue type variable
  pqueue_t new_pqueue;

  // initialize the priority queue
  initialize_pq(&new_pqueue, int, cmp_data);

  for (i = 0; i < 6; i++)
  {
    // push the data into the priority queue
    status = push_pq(&new_pqueue, vals + i);
    if (status == false)  // basic error handling
      exit(EXIT_FAILURE);
  }

  // get the most prioritized element in the priority queue
  printf("top: %d\n", format_data(peek_pq(&new_pqueue), int));

  // pop the most prioritized element from the priority queue
  status = pop_pq(&new_pqueue);
  if (status == false)
    exit(EXIT_FAILURE);

  printf("top: %d\n", format_data(peek_pq(&new_pqueue), int));

  // delete the priority queue
  delete_pq(&new_pqueue);

  return 0;
}

/*
 * compares two elements and returns 1 if the priority of the first element
 * is greater than that of the second element.
*/

int cmp_data(const void * ptr1, const void * ptr2)
{
  const int * iptr1 = (const int *) ptr1;
  const int * iptr2 = (const int *) ptr2;

  return (*iptr1 > *iptr2) - (*iptr1 < *iptr2);
}
