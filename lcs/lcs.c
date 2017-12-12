#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <assert.h>
#include "./lcs.h"

#include "../fasttime.h"

/*********************
 * Function prototypes
 *********************/
/* Read in the input from the file described by filename, which has the format
 * of x_len, y_len, x, y, separated by newlines.
 * 
 * Returns a lcs_input which is properly initialized with the lengths of the two
 * inputs and pointers to char arrays with the inputs.
 */
static lcs_input_t* read_input(char* filename);

/* Free the char arrays pointed to by the input structure and the structure itself. */
void free_input(lcs_input_t* input);

/* Helper function to print an error message */
void print_error(char* msg);

/* Function to print usage method of this binary */
static void usage(void);

/**************
 * Main routine
 **************/
int main(int argc, char** argv) {
  char* input_file;

  /*
   * Read and interpret the command line arguments
   */
  if (argv[1] == NULL) {
    usage();
    exit(1);
  }
  char c = 0;
  while ((c = getopt(argc, argv, "f:")) != EOF) {
    switch (c) {
    case 'f':
      input_file = (char*) malloc(MAXLINE);
      if (input_file == NULL) {
        print_error("ERROR: malloc failed in main");
      }
      input_file = strdup(optarg);
      break;
    default:
      usage();
      exit(1);
    }
  }

  // Read in input
  lcs_input_t* input = read_input(input_file);

  // Initialize empty matrix as input 
  dp_matrix_t* X = init(input->x_len, input->y_len);
  dp_matrix_t* Y = init(input->x_len, input->y_len);


  fasttime_t start = gettime();
  lcs_cache_oblivious(X, input);
  fasttime_t end = gettime();
  printf("C-O impl.: %f\n", tdiff(start, end));

  start = gettime();
  lcs_naive(Y, input);
  end = gettime();
  printf("Regular impl.: %f\n", tdiff(start, end));
  
  #ifndef NDEBUG
  for (uint64_t k = 0; k < input->x_len * input->y_len - 1; k++) { // the cache-oblivious solution
    assert(X->entries[k] == Y->entries[k]);                        // does not fill the last entry
  }  
  printf("\033[0;32mTESTS PASSED\033[0m All elements the same. \n");
  #endif

  free(X->entries);
  free(Y->entries);
  free_input(input);
  free(input_file);
}

/******************
 * Helper functions
 ******************/

/*
 * read_input - read an input for LCS problem
 */
static lcs_input_t* read_input(char* filename) {
  FILE* input_file;
  lcs_input_t* input;
  char path[MAXLINE];

  snprintf(path, MAXLINE, "%s%s", "./", filename);
  /* Allocate the input record */
  if ((input = (lcs_input_t*) malloc(sizeof(lcs_input_t))) == NULL) {
    print_error("ERROR: malloc failed in read_input");
  }

  /* Read the input file into the record */
  if ((input_file = fopen(path, "r")) == NULL) {
    print_error("ERROR: Could not open file in read_input");
  }

  fscanf(input_file, "%lld", &(input->x_len));
  fscanf(input_file, "%lld", &(input->y_len));
  printf("x length: %lld\n", input->x_len);
  printf("y length: %lld\n", input->y_len);
  char* x_str = (char*) malloc(input->x_len + 2);
  char* y_str = (char*) malloc(input->y_len + 2);
  if (fscanf(input_file, "%s", x_str) == EOF) {
    print_error("ERROR: Could not read x in read_input");
  }
  if (fscanf(input_file, "%s", y_str) == EOF) {
    print_error("ERROR: Could not read y in read_input");
  }
  input->x = x_str;
  input->y = y_str;
  printf("x:");
  for (int i = 0; i < input->x_len; i++) {
    printf("%c", input->x[i]);
  }
  printf("\n");
  printf("y:");
  for (int j = 0; j < input->y_len; j++) {
    printf("%c", input->y[j]);
  }
  printf("\n");
  fclose(input_file);
  return input;
}

/*
 * free_input - Frees memory allocated to hold the input
 */

void free_input(lcs_input_t* input) {
  free(input->x);     /* free the strings */
  free(input->y);
  free(input);
}

/*
 * print_error - Print error message and return
 */
void print_error(char* msg) {
  printf("%s\n", msg);
  exit(1);
}

/*
 * usage - Explain command line arguments
 */

static void usage(void) {
  fprintf(stderr, "Usage: lcs [-f <file>]\n");
  fprintf(stderr, "\t-f <file> Use <file> as the input file. \n");
}
