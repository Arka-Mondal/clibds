/*
  Copyright (c) 2023, Arka Mondal. All rights reserved.
  Use of this source code is governed by a BSD-style license that
  can be found in the LICENSE file.
*/

#include <stdio.h>
#include <stdlib.h>
#include "rbtree.h"

int comp_data(void *, void *);

int main(void)
{
  int data_arr[9] = {8, 3, 10, 1, 6, 14, 4, 7, 13}, order[9];
  rbtree_t new_tree;
  size_t count, i;

  clibds_rbt_initialize(&new_tree, int, comp_data);

  /* for adding custom memory allocator
   * ----------------------------------
   * clibds_rbt_initialize(&new_tree, int, comp_data,
   *                       .conf_mem_alloc = alloc_fn, .conf_mem_free = free_fn);
  */

  if (clibds_rbt_insert_from_array(&new_tree, data_arr, 9) == 0)
  {
    fputs("Insertion failed.\n", stderr);
    exit(EXIT_FAILURE);
  }

  count = clibds_rbt_getdata(&new_tree, order, POSTORDER);

  for (i = 0; i < count; i++)
    printf("%d ", order[i]);
  putchar('\n');

  clibds_rbt_remove(&new_tree, &(int){14});

  count = clibds_rbt_getdata(&new_tree, order, POSTORDER);

  for (i = 0; i < count; i++)
    printf("%d ", order[i]);
  putchar('\n');

  clibds_rbt_delete(&new_tree);

  return 0;
}

int comp_data(void * data1, void * data2)
{
  static int * ptr1, * ptr2;

  ptr1 = (int *) data1;
  ptr2 = (int *) data2;

  return (*ptr1 > *ptr2) - (*ptr1 < *ptr2);
}
