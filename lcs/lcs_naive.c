#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./lcs.h"

void backtrack(dp_matrix_t* X, lcs_input_t* input, lcs_result_t* result, uint64_t i, uint64_t j);

void lcs_naive(dp_matrix_t* X, lcs_input_t* input) {
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
  lcs_result_t* result = (lcs_result_t*) malloc(sizeof(lcs_result_t));
  result->size = 0;
  result->head = NULL;
  result->tail = NULL;
  backtrack(X, input, result, X->width-1, X->height-1);
  printf("Longest subsequence: %llu\n", result->size);
  for (lcs_result_node_t* node = result->head; node != NULL; node = node->next) {
    printf("%c", node->character);
  }
  printf("\n");
  free_lcs_result(result);
}

 void backtrack(dp_matrix_t* X, lcs_input_t* input, lcs_result_t* result, uint64_t i, uint64_t j) {
   if (i == -1 || j == -1) {
     return;
   }
   if (input->x[i] == input->y[j]) {
     backtrack(X, input, result, i-1, j-1);
     lcs_result_node_t* new_node = malloc(sizeof(lcs_result_node_t));
     new_node->character = input->x[i];
     new_node->next = NULL;
     if (result->head == NULL) {
       result->head = new_node;
       result->tail = new_node;
       result->size = 1;
     } else {
       result->tail->next = new_node;
       result->tail = new_node;
       result->size++;
     }
   } else if (get_entry(X, i, j-1) > get_entry(X, i-1, j)) {
     backtrack(X, input, result, i, j-1);
   } else {
     backtrack(X, input, result, i-1, j);
	 }
 }
