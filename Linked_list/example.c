/*
  Copyright (c) 2022, Arka Mondal. All rights reserved.
  Use of this source code is governed by a BSD-style license that
  can be found in the LICENSE file.
*/

#include <stdio.h>
#include "linkedlist.h"

int main(void)
{
  int arr[6] = {12, 6, 84, 63, 55, 34}, i, value1, value2;

  // declare a list type variable
  list_t new_list;

  value1 = 32;
  value2 = 105;

  // initialize the list
  initialize_list(&new_list, int);

  // push each element one by one at the end of the list
  for (i = 0; i < 6; i++)
    pushback_list(&new_list, arr + i);

  // iterate over each element in the list starting from the beginning of the list
  for_each_in_list(&new_list, it)
    printf("%d ", format_data(it->data, int));
  putchar('\n');

  // push an element at the beginning of the list
  pushfront_list(&new_list, &value1);

  // push an element at a particular index of the list
  pushatindex_list(&new_list, 2, &value2);

  // check the size of the list
  printf("list size: %zu\n", new_list.size);

  // pop an element from the end of the list
  popback_list(&new_list);

  for_each_in_list(&new_list, it)
    printf("%d ", format_data(it->data, int));
  putchar('\n');

  // delete the list
  delete_list(&new_list);

  return 0;
}
