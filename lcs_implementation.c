#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./lcs.h"

void backtrack(lcs_input_t* input, lcs_result_t* result, uint64_t* dp, int64_t i, int64_t j);

void lcs_regular(lcs_input_t* input) {
  uint64_t width = input->x_len;
  uint64_t height = input->y_len;
  char* x = input->x;
  char* y = input->y;
  uint64_t* dp = calloc(sizeof(uint64_t), width * height);
  lcs_result_t result = { .head = NULL, .tail = NULL, .size = 0 }; 
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

  backtrack(input, &result, dp, width-1, height-1);
  printf("Longest subsequence: %llu\n", result.size);
  printf("\n");
  for (lcs_result_node_t* node = result.head; node != NULL; node = node->next) {
    printf("%c", node->character);
  }
  printf("\n");
  free(dp);
  free_lcs_result(result);
}

void backtrack(lcs_input_t* input, lcs_result_t* result, uint64_t* dp, int64_t i, int64_t j) {
  if (i < 0 || j < 0) {
    return;
  }
  if (input->x[i] == input->y[j]) {
    backtrack(input, result, dp, i-1, j-1);
    lcs_result_node_t* new_node = malloc(sizeof(lcs_result_node_t));
    new_node->character = input->x[i];
    new_node->next = NULL;
    if (result->head == NULL) {
      result->head = new_node;
      result->tail = new_node;
      result->size = 1;
    } else {
      result->tail->next = new_node;
      result->size++;
    }
  } else if (get_char_at_indices(dp, input->x_len, i, j-1) > get_char_at_indices(dp, input->x_len, i-1, j)) {
    backtrack(input, result, dp, i, j-1);
  } else {
    backtrack(input, result, dp, i-1, j);
  }
}
