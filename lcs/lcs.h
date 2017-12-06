#include "../gep.h"

#define MAXLINE 1024 /* max string size */

/* Represents an input x, y pair for LCS */
typedef struct {
  uint64_t x_len;
  uint64_t y_len;
  char* x;
  char* y;
} lcs_input_t;

struct lcs_result_node_t {
  char character;
  struct lcs_result_node_t* next;
};
 
typedef struct lcs_result_node_t lcs_result_node_t;

struct lcs_result_t {
  lcs_result_node_t* head;
  lcs_result_node_t* tail;
  uint64_t size;
};

typedef struct lcs_result_t lcs_result_t;

void lcs_naive(dp_matrix_t* X, lcs_input_t* input);
void lcs_cache_oblivious(dp_matrix_t* X, lcs_input_t* input);

/* Free the LCS result */
static void free_lcs_result(lcs_result_t* result) {
  lcs_result_node_t* node = result->head;
  while (node != NULL) {
    lcs_result_node_t* nextNode = node->next;
    free(node);
    node = nextNode;
  }
}

/* Puts the data into the second result and frees the first result
 */
static void merge_lcs_result(lcs_result_t* first, lcs_result_t* second) {
  if (second->head == NULL) {
    second->head = first->head;
    second->tail = first->tail;
    second->size = first->size;
    return;
  }
  if (first->size == 0) {
    return;
  }
  first->tail->next = second->head;
  second->head = first->head;
  second->size += first->size;
}
