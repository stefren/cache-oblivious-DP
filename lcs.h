#define MAXLINE 1024 /* max string size */

/* Represents an input x, y pair for LCS */
typedef struct {
  uint64_t x_len;
  uint64_t y_len;
  char* x;
  char* y;
} lcs_input_t;

typedef struct {
  char* start;
  uint64_t length;
} boundary_t; 

typedef struct {
  boundary_t* bottom;
  boundary_t* right;
} boundary_result_t;

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

void lcs_regular(lcs_input_t* input);
void lcs_cache_oblivious(lcs_input_t* input);

/* Return the character at indices i, j given a matrix stored as a contiguous
 * block of memory in row-major order.
 */
static uint64_t get_char_at_indices(uint64_t* dp_matrix, int row_size, int i, int j) {
  if (j < 0 || i < 0 || i > row_size) {
    return 0;
  }
  return dp_matrix[j*row_size + i];
}

/* Set the character at indices i, j given a matrix stored as a contiguous
 * block of memory in row-major order.
 */
static uint64_t set_char_at_indices(uint64_t* dp_matrix, int row_size, int i, int j, char value) {
  if (j < 0 || i < 0 || i > row_size) {
    return -1;
  }
  dp_matrix[j*row_size + i] = value;
  return 0;   // success
}
