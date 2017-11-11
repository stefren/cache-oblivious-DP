#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void backtrack(lcs_input_t* input, char* dp_matrix, int i, int j);

void lcs_regular(lcs_input_t* input) {
  int x_len = input->x_len;
  int y_len = input->y_len;
  char* x = input->x;
  char* y = input->y;
  char dp_matrix[x_len][y_len];
  int i, j;
  for (i = 0; i <= x_len; i++) {
    dp_matrix[i][0] = 0;
  }
  for (j = 0; j <= y_len; j++) {
    dp_matrix[0][j] = 0;
  }
  for (i = 1; i <= x_len; i++) {
    for (j = 1; j <= y_len; j++) {
      if (x[i] == y[j]) {
        dp_matrix[i][j] = dp_matrix[i-1][j-1] + 1;
      } else {
        if (dp_matrix[i][j-1] > dp_matrix[i-1][j]) {
          dp_matrix[i][j] = dp_matrix[i][j-1];
        } else {
          dp_matrix[i][j] = dp_matrix[i-1][j];
        }
      }
    }
  }
  backtrack(input, &dp_matrix, x_len, y_len);
}

void backtrack(lcs_input_t* input, char* dp_matrix, int i, int j) {
  int x_len = input->x_len;
  int y_len = input->y_len;
  char* x = input->x;
  char* y = input->y;
  if (i == 0 || j == 0) {
    printf("Longest subsequence: \n");
    return;
  }
  if (x[i] == y[i]) {
    backtrack(input, dp_matrix, i, j-1);
    printf("%d", x[i]);
  }
  if (dp_matrix[i][j-1] > dp_matrix[i-1][j]) {
    backtrack(input, dp_matrix, i-1, j);
  } else {
    backtrack(input, dp_matrix, i, j-1);
  }
}
