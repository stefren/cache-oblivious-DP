#include "../gep.h"

#define MAXLINE 1024 /* max string size */

/* Represents an input x, y pair for LCS 
 *   x and y are pointers to the char arrays and
 *   x_len and y_len are the respective lengths of each input string 
 */
typedef struct {
  uint64_t x_len;
  uint64_t y_len;
  char* x;
  char* y;
} lcs_input_t;

/* We need to use a structure to store the output subsequence because the cache
 * oblivious algorithm generates the subsequence recursively.
 * We use a singly linked list that stores the head and tail of the list so 
 * that we can perform O(1) merges. 
 * 
 * An LCS result node represents a node in one of these linked lists.
 */
struct lcs_result_node_t {
  char character;
  struct lcs_result_node_t* next;
};
 
typedef struct lcs_result_node_t lcs_result_node_t;

/* A linked list containing a pointer to the first and last nodes, as well as
 * the size of the list 
 */
struct lcs_result_t {
  lcs_result_node_t* head;
  lcs_result_node_t* tail;
  uint64_t size;
};

typedef struct lcs_result_t lcs_result_t;

/* Inputs:
 *  X - a dp_matrix_t with entries initialized to all 0's, with dimensions 
 *      width corresponding to input->x_len and width corresponding to input->y_len 
 *  input - an lcs_input which has pointers to the two input strings and the lengths 
 * 
 * Post-condition: X is updated with X[i][j] is the answer to the subproblem LCS
 * of the substrings of inputs x[0...i] and y[0...j] 
 */
void lcs_naive(dp_matrix_t* X, lcs_input_t* input);

/* Inputs:
 *  X - a dp_matrix_t with entries initialized to all 0's, with dimensions 
 *      width corresponding to input->x_len and width corresponding to input->y_len 
 *  input - an lcs_input which has pointers to the two input strings and the lengths 
 * 
 * Post-condition: X is updated with X[i][j] is the answer to the subproblem LCS
 * of the substrings of inputs x[0...i] and y[0...j] 
 */
void lcs_cache_oblivious(dp_matrix_t* X, lcs_input_t* input);

/* Free the LCS result. Iterate through the nodes in the result and free each
 * node, and then free the list.
 */
static void free_lcs_result(lcs_result_t* result) {
  lcs_result_node_t* node = result->head;
  while (node != NULL) {
    lcs_result_node_t* nextNode = node->next;
    free(node);
    node = nextNode;
  }
}

/* Put the merged data of the first and second result into the second result 
 * and frees the first result. We put the results into the second result 
 * because the cache-oblivious algorithm typically merges in new results from
 * the front.
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
