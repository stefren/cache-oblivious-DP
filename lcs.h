#define MAXLINE 1024 /* max string size */

/* Represents an input x, y pair for LCS */
typedef struct {
  int x_len;
  int y_len;
  char* x;
  char* y;
} lcs_input_t;



void lcs_regular(lcs_input_t* input);

/* Return the character at indices i, j given a matrix stored as a contiguous
 * block of memory in row-major order.
 */
static inline char get_char_at_indices(char* dp_matrix, int row_size, int i, int j) {
  return dp_matrix[j*row_size + i];
}

/* Set the character at indices i, j given a matrix stored as a contiguous
 * block of memory in row-major order.
 */
static inline char set_char_at_indices(char* dp_matrix, int row_size, int i, int j, char value) {
  dp_matrix[j*row_size + i] = value;
  return 0;   // success
}
