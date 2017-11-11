#define MAXLINE 1024 /* max string size */

/* Represents an input x, y pair for LCS */
typedef struct {
  int x_len;
  int y_len;
  char* x;
  char* y;
} lcs_input_t;
