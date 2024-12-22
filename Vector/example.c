/*
  Copyright (c) 2023, Arka Mondal. All rights reserved.
  Use of this source code is governed by a BSD-style license that
  can be found in the LICENSE file.
*/

#include <stdio.h>
#include "vector.h"

int main(void)
{
  int arr[6] = {6, 7, 1, 2, 8, 3};
  vector_t new_vec;

  // initialize vector with default configuration
  // equivalent to: clibds_vec_init(&new_vec, int, NULL);
  clibds_vec_init(&new_vec, int);

  /*for (int i = 0; i < 6; i++)
    clibds_vec_push(&new_vec, arr + i);*/
  clibds_vec_push_from_array(&new_vec, arr, 6);

  clibds_vec_foreach(&new_vec, it)
    printf("%d ", clibds_vec_fmt_data(it, int));
  putchar('\n');

  clibds_vec_insert(&new_vec, 0, &(int){5});

  clibds_vec_foreach(&new_vec, it)
    printf("%d ", clibds_vec_fmt_data(it, int));
  putchar('\n');

  printf("Size: %zu\n", clibds_vec_size(&new_vec));

  printf("%s\n", (clibds_vec_remove(&new_vec, 2)) ? "true" : "false");

  clibds_vec_foreach(&new_vec, it)
    printf("%d ", clibds_vec_fmt_data(it, int));
  putchar('\n');

  printf("Size: %zu\n", clibds_vec_size(&new_vec));

  clibds_vec_delete(&new_vec);

  return 0;
}
