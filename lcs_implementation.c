#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./lcs.h"

void backtrack(lcs_input_t* input, uint64_t* dp, int64_t i, int64_t j);

void lcs_regular(lcs_input_t* input) {
  uint64_t width = input->x_len;
  uint64_t height = input->y_len;
  char* x = input->x;
  char* y = input->y;
  uint64_t* dp = calloc(sizeof(uint64_t), width * height);
  uint64_t i, j;
  for (i = 0; i < width; i++) {
    for (j = 0; j < height; j++) {
      if (x[i] == y[j]) {
        set_char_at_indices(dp, width, i, j, get_char_at_indices(dp, width, i-1, j-1) + 1);
      } else if (get_char_at_indices(dp, width, i, j-1) >= get_char_at_indices(dp, width, i-1, j)) {
        set_char_at_indices(dp, width, i, j, get_char_at_indices(dp, width, i, j-1));
      } else {
        set_char_at_indices(dp, width, i, j, get_char_at_indices(dp, width, i-1, j));
      }
    }
  }

  printf("Longest subsequence: %llu\n", get_char_at_indices(dp, width, width-1, height-1));
  backtrack(input, dp, width-1, height-1);
  printf("\n");
}

void backtrack(lcs_input_t* input, uint64_t* dp, int64_t i, int64_t j) {
  if (i < 0 || j < 0) {
    return;
  }
  if (input->x[i] == input->y[j]) {
    backtrack(input, dp, i-1, j-1);
    putchar(input->x[i]);
  } else if (get_char_at_indices(dp, input->x_len, i, j-1) > get_char_at_indices(dp, input->x_len, i-1, j)) {
    backtrack(input, dp, i, j-1);
  } else {
    backtrack(input, dp, i-1, j);
  }
}
