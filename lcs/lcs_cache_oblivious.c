#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "./lcs.h"

void lcs_output_boundary(dp_matrix_t* X, uint64_t x_index, uint64_t y_index, uint64_t x_len, uint64_t y_len);
lcs_result_t lcs_recursive(lcs_input_t* input, uint64_t* dp, int64_t width, int64_t x_index, int64_t y_index, int64_t* i, int64_t* j, int64_t x_len, int64_t y_len);

void lcs_cache_oblivious(dp_matrix_t* X) {
  lcs_output_boundary(X, 0, 0, X->width, X->height);
  printf("Longest subsequence: %llu\n", get_entry(X, X->width-1, X->height-1));
//  lcs_result_t result = lcs_recursive(input, dp, width, 0, 0, &i, &j, width, height);
//  printf("Longest subsequence: %llu\n", result.size);
//  lcs_result_node_t* node = result.head;
//  for (lcs_result_node_t* node = result.head; node != NULL; node = node->next) {
//    printf("%c", node->character);
//  }
//  printf("\n");
//  free(dp);
//  free_lcs_result(&result);
}

//lcs_result_t lcs_recursive(lcs_input_t* input, uint64_t* dp, int64_t width, int64_t x_index, int64_t y_index, int64_t* i, int64_t* j, int64_t x_len, int64_t y_len) {
//  assert(x_index <= *i && *i < x_index + x_len && y_index <= *j && *j < y_index + y_len);
//  lcs_result_t result = (lcs_result_t) { .head = NULL, .tail = NULL, .size = 0 };
//  if (x_len == 0 || y_len == 0) return result;
//  if (x_len == 1 && y_len == 1) {
//    if (input->x[*i] == input->y[*j]) {
//      lcs_result_node_t* newNode = malloc(sizeof(lcs_result_node_t));
//      newNode->character = input->x[*i];
//      newNode->next = NULL;
//      result.head = newNode;
//      result.tail = newNode;
//      result.size = 1;
//      (*i)--;
//      (*j)--; 
//    } else if (get_char_at_indices(dp, width, *i, *j-1) >= get_char_at_indices(dp, width, *i-1, *j)) {
//      (*j)--;
//    } else { 
//      (*i)--;
//    }
//    return result;
//  } else {
//    lcs_output_boundary(input, dp, width, x_index, y_index, (x_len+1)/2, (y_len+1)/2);
//    if ((*j == y_index + y_len - 1 && (x_index + (x_len+1)/2 <= *i && *i < x_index + x_len)) || 
//        (*i == x_index + x_len - 1 && (y_index + (y_len+1)/2 <= *j && *j < y_index + y_len))) {
//      lcs_output_boundary(input, dp, width, x_index + (x_len+1)/2, y_index, x_len/2, (y_len+1)/2);
//      lcs_output_boundary(input, dp, width, x_index, y_index + (y_len+1)/2, (x_len+1)/2, y_len/2);
//      lcs_result_t new_result = lcs_recursive(input, dp, width, x_index + (x_len+1)/2, y_index + (y_len+1)/2, i, j, x_len/2, y_len/2);
//      merge_lcs_result(&new_result, &result); 
//    }
//
//    if ((*j == y_index + y_len - 1 && (x_index <= *i && *i < x_index + (x_len+1)/2)) || 
//        (*i == x_index + (x_len-1)/2 && (y_index + (y_len+1)/2 <= *j && *j < y_index + y_len))) {
//      lcs_result_t new_result = lcs_recursive(input, dp, width, x_index, y_index + (y_len+1)/2, i, j, (x_len+1)/2, y_len/2);
//      merge_lcs_result(&new_result, &result); 
//    }
//    
//    if  ((*j == y_index + (y_len-1)/2 && (x_index + (x_len+1)/2 <= *i && *i < x_index + x_len)) || 
//        (*i == x_index + x_len - 1 && (y_index <= *j && *j < y_index + (y_len+1)/2))) {
//      lcs_result_t new_result = lcs_recursive(input, dp, width, x_index + (x_len+1)/2, y_index, i, j, x_len/2, (y_len+1)/2);
//      merge_lcs_result(&new_result, &result); 
//    }
//
//    if  ((*j == y_index + (y_len-1)/2 && (x_index <= *i && *i < x_index + (x_len+1)/2)) || 
//        (*i == x_index + (x_len-1)/2 && (y_index <= *j && *j < y_index + (y_len+1)/2))) {
//      lcs_result_t new_result = lcs_recursive(input, dp, width, x_index, y_index, i, j, (x_len+1)/2, (y_len+1)/2);
//      merge_lcs_result(&new_result, &result); 
//    }
//    return result;
//  }
//}

void lcs_output_boundary(dp_matrix_t* X, uint64_t x_index, uint64_t y_index, uint64_t x_len, uint64_t y_len) {
  if (x_len == 0 || y_len == 0) return;
  if (x_len == 1 && y_len == 1) {
    if (get_entry(X, x_index, y_index) == -1) {
      set_entry(X, x_index, y_index, get_entry(X, x_index-1, y_index-1)+1);
    } else if (get_entry(X, x_index, y_index-1) >= get_entry(X, x_index-1, y_index)) {
      set_entry(X, x_index, y_index, get_entry(X, x_index, y_index-1));
    } else {
      set_entry(X, x_index, y_index, get_entry(X, x_index-1, y_index));
    }

  } else {
    lcs_output_boundary(X, x_index, y_index, (x_len+1)/2, (y_len+1)/2);
    lcs_output_boundary(X, x_index + (x_len+1)/2, y_index, x_len/2, (y_len+1)/2);
    lcs_output_boundary(X, x_index, y_index + (y_len+1)/2, (x_len+1)/2, y_len/2);
    lcs_output_boundary(X, x_index + (x_len+1)/2, y_index + (y_len+1)/2, x_len/2, y_len/2);
  }
}