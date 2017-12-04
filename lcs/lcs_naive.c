#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./lcs.h"

void backtrack(lcs_input_t* input, lcs_result_t* result, uint64_t* dp, int64_t i, int64_t j);

void lcs_naive(dp_matrix_t* X) {
  uint64_t width = X->width;
  uint64_t height = X->height;
  uint64_t i, j;
  for (i = 0; i < X->width; i++) {
    for (j = 0; j < X->height; j++) {
      if (get_entry(X, i, j) == -1) {
        set_entry(X, i, j, get_entry(X, i-1, j-1) + 1);
      } else if (get_entry(X, i, j-1) >= get_entry(X, i-1, j)) {
        set_entry(X, i, j, get_entry(X, i, j-1));
      } else {
        set_entry(X, i, j, get_entry(X, i-1, j));
      }
    }
  }
  printf("Longest subsequence: %llu\n", get_entry(X, width - 1, height - 1));
//  lcs_result_t* result = (lcs_result_t*) malloc(sizeof(lcs_result_t);
//  backtrack(input, &result, dp, width-1, height-1);
//  printf("Longest subsequence: %llu\n", result.size);
//  printf("\n");
//  for (lcs_result_node_t* node = result.head; node != NULL; node = node->next) {
//    printf("%c", node->character);
//  }
//  printf("\n");
//  free(dp);
//  free_lcs_result(&result);
}

// void backtrack(dp_matrix_t* X, lcs_result_t* result, uint64_t* dp, int64_t i, int64_t j) {
//   if (i < 0 || j < 0) {
//     return;
//   }
//   if (X == ) {
//     backtrack(input, result, dp, i-1, j-1);
//     lcs_result_node_t* new_node = malloc(sizeof(lcs_result_node_t));
//     new_node->character = input->x[i];
//     new_node->next = NULL;
//     if (result->head == NULL) {
//       result->head = new_node;
//       result->tail = new_node;
//       result->size = 1;
//     } else {
//       result->tail->next = new_node;
//       result->tail = new_node;
//       result->size++;
//     }
//   } else if (get_entry(X, i, j-1) > get_char_at_indices(X, i-1, j)) {
//     backtrack(input, result, dp, i, j-1);
//   } else {
//     backtrack(input, result, dp, i-1, j);
//   }
// }
