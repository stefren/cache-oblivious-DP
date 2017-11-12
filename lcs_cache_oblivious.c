#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./lcs.h"

#ifndef START_CAPACITY
#define START_CAPACITY 64
#endif

void lcs_output_boundary(lcs_input_t* input, char* dp, int width, int x_index, int y_index, int x_len, int y_len);
lcs_result_t lcs_recursive(boundary_t X, boundary_t Y, boundary_t T, boundary_t L, int i, int j);

void lcs_cache_oblivious(lcs_input_t* input) {
  int width = input->x_len;
  int height = input->y_len;
  char* x = input->x;
  char* y = input->y;
  char* dp = (char *) calloc(1, width * height);
  lcs_output_boundary(input, dp, width, 0, 0, width, height);

  printf("Longest subsequence: %d\n", get_char_at_indices(dp, width, width-1, height-1));
  printf("\n");
}

lcs_result_t lcs_recursive(boundary_t X, boundary_t Y, boundary_t T, boundary_t L, int i, int j) {
  lcs_result_t result = (lcs_result_t) { .buf = NULL, .length = 0, .capacity = START_CAPACITY };
  if (T.length == 1 && L.length == 1) {
    if (X.start[0] == Y.start[0]) {
    }
    return result;
  } else {
    boundary_t new_X = (boundary_t) {.start = X.start, .length = X.length/2 };
    return result; 
  }
}

void lcs_output_boundary(lcs_input_t* input, char* dp, int width, int x_index, int y_index, int x_len, int y_len) {
  if (x_len == 1 && y_len == 1) {
    if (input->x[x_index] == input->y[y_index]) {
      set_char_at_indices(dp, width, x_index, y_index, get_char_at_indices(dp, width, x_index-1, y_index-1)); 
    } else if (get_char_at_indices(dp, width, x_index, y_index-1) > get_char_at_indices(dp, width, x_index-1, y_index)) {
      set_char_at_indices(dp, width, x_index, y_index, get_char_at_indices(dp, width, x_index, y_index-1));
    } else {
      set_char_at_indices(dp, width, x_index, y_index, get_char_at_indices(dp, width, x_index-1, y_index));
    }
  } else {
    lcs_output_boundary(input, dp, width, x_index, y_index, (x_len+1)/2, (y_len+1)/2);
    lcs_output_boundary(input, dp, width, x_index + (x_len+1)/2, y_index, x_len/2, (y_len+1)/2);
    lcs_output_boundary(input, dp, width, x_index, y_index + (y_len+1)/2, (x_len+1)/2, y_len/2);
    lcs_output_boundary(input, dp, width, x_index + (x_len+1)/2, y_index + (y_len+1)/2, x_len/2, y_len/2);
  }
}