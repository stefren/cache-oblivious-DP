#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./lcs.h"

#ifndef START_CAPACITY
#define START_CAPACITY 64
#endif

/* Puts the data into the first result and frees the second result
 */
void merge_lcs_result(lcs_result_t* first, lcs_result_t* second) {
  if (!(first->capacity > first->length + second->length)) {
    first->capacity *= 2;
    first->buf = realloc(first->buf, first->capacity);
  }
  first->length += second->length;
  for (int i = 0; i < second->length; i++) {
    first->buf[first->length+i] = second->buf[i];
  }
  if (second->capacity > 0) { 
    free(second->buf);
  }
}

void lcs_output_boundary(lcs_input_t* input, char* dp, int width, int x_index, int y_index, int x_len, int y_len);
lcs_result_t lcs_recursive(lcs_input_t* input, char* dp, int width, int x_index, int y_index, int* i, int* j, int x_len, int y_len);

void lcs_cache_oblivious(lcs_input_t* input) {
  int width = input->x_len;
  int height = input->y_len;
  char* x = input->x; 
  char* y = input->y;
  char* dp = (char *) calloc(1, width * height);
  int i = width-1;
  int j = height-1;
//  lcs_output_boundary(input, dp, width, 0, 0, width, height);
  lcs_recursive(input, dp, width, 0, 0, &i, &j, width, height);
  printf("Longest subsequence: %d\n", get_char_at_indices(dp, width, width-1, height-1));
  printf("\n");
  for (int k = 0; k < width; k++) {
    for (int h = 0; h < height; h++) {
      printf("%d ", get_char_at_indices(dp, width, k, h));
    }
    printf("\n");
  }
  free(dp);
}

lcs_result_t lcs_recursive(lcs_input_t* input, char* dp, int width, int x_index, int y_index, int* i, int* j, int x_len, int y_len) {
  char* result_buf = malloc(START_CAPACITY);
  lcs_result_t result = (lcs_result_t) { .buf = result_buf, .length = 0, .capacity = START_CAPACITY };
  if (x_len == 0 || y_len == 0) return result;
  if (x_len == 1 && y_len == 1) {
    if (input->x[*i] == input->y[*j]) {
      *i--;
      *j--; 
      result.buf[0] = input->x[*i];
      result.length++;
    }
    return result;
  } else {
    lcs_output_boundary(input, dp, width, x_index, y_index, (x_len+1)/2, (y_len+1)/2);
    if ((*j == y_index + y_len - 1 && (x_index + (x_len+1)/2 <= *i || *i < x_index + x_len)) || 
        (*i == x_index + x_len - 1 && (y_index + (y_len+1)/2 <= *j || *j < y_index + y_len))) {
      lcs_output_boundary(input, dp, width, x_index + (x_len+1)/2, y_index, x_len/2, (y_len+1)/2);
      lcs_output_boundary(input, dp, width, x_index, y_index + (y_len+1)/2, (x_len+1)/2, y_len/2);
      lcs_result_t new_result = lcs_recursive(input, dp, width, x_index + (x_len+1)/2, y_index + (y_len+1)/2, i, j, x_len/2, y_len/2);
      merge_lcs_result(&new_result, &result); 
      result = new_result;
    }
    if ((*j == y_index + y_len - 1 && (x_index + (x_len+1)/2 <= *i || *i < x_index + x_len)) || 
        (*i == x_index + (x_len+1)/2 && (y_index <= *j || *j < y_index + (y_len+1)/2))) {
      lcs_result_t new_result = lcs_recursive(input, dp, width, x_index + (x_len+1)/2, y_index, i, j, x_len/2, (y_len+1)/2);
      merge_lcs_result(&new_result, &result); 
      result = new_result;
    }
    
    if  ((*j == y_index + (y_len+1)/2 && (x_index <= *i || *i < y_index + (x_len+1)/2)) || 
        (*i == x_index + x_len - 1 && (y_index + (y_len+1)/2 <= *j || *j < y_index + y_len))) {
      lcs_result_t new_result = lcs_recursive(input, dp, width, x_index, y_index + (y_len+1)/2, i, j, (x_len+1)/2, y_len/2);
      merge_lcs_result(&new_result, &result); 
      result = new_result;
    }


    if  ((*j == y_index + (y_len+1)/2 && (x_index <= *i || *i < y_index + (x_len+1)/2)) || 
        (*i == x_index + (x_len+1)/2 && (x_index <= *j || *j < x_index + (x_len+1)/2))) {
      lcs_result_t new_result = lcs_recursive(input, dp, width, x_index, y_index, i, j, (x_len+1)/2, (y_len+1)/2);
      merge_lcs_result(&new_result, &result); 
      result = new_result;
    }
    return result;
  }
}

void lcs_output_boundary(lcs_input_t* input, char* dp, int width, int x_index, int y_index, int x_len, int y_len) {

//  printf("%d, %d, %d, %d\n", x_index, y_index, x_len, y_len);
  if (x_len == 0 || y_len == 0) return;
  if (x_len == 1 && y_len == 1) {
    if (input->x[x_index] == input->y[y_index]) {
      set_char_at_indices(dp, width, x_index, y_index, get_char_at_indices(dp, width, x_index-1, y_index-1));
    } else if (get_char_at_indices(dp, width, x_index, y_index-1) >= get_char_at_indices(dp, width, x_index-1, y_index)) {
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
