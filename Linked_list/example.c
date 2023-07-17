/*
  Copyright (c) 2023, Arka Mondal. All rights reserved.
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
  clibds_list_init(&new_list, int, NULL);

  // push each element one by one at the end of the list
  clibds_list_pushback_from_array(&new_list, arr, 6);

  // iterate over each element in the list starting from the beginning of the list
  clibds_list_foreach(&new_list, it)
    printf("%d ", clibds_list_format_data(it, int));
  putchar('\n');

  // push an element at the beginning of the list
  clibds_list_pushfront(&new_list, &value1);

  // push an element at a particular index of the list
  clibds_list_pushatindex(&new_list, 2, &value2);

  // check the size of the list
  printf("list size: %zu\n", clibds_list_size(&new_list));

  // pop an element from the end of the list
  clibds_list_popback(&new_list);

  clibds_list_foreach(&new_list, it)
    printf("%d ", clibds_list_format_data(it, int));
  putchar('\n');

  // delete the list
  clibds_list_delete(&new_list);

  return 0;
}
