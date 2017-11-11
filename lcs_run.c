#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "./fasttime.h"
#include "./lcs.h"
#include "./lcs_implementation.c"

/*********************
 * Function prototypes
 *********************/
static lcs_input_t* read_input(char* filename);
void free_input(lcs_input_t* input);

void print_error(char* msg);
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
      input_file = (char*) malloc(sizeof(char));
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

  lcs_input_t* input = read_input(input_file);
  // run implementations here
  fasttime_t start = gettime();
  lcs_regular(input);
  fasttime_t end = gettime();
  free_input(input);
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

  fscanf(input_file, "%d", &(input->x_len));
  fscanf(input_file, "%d", &(input->y_len));
  printf("x length: %d\n", input->x_len);
  printf("y length: %d\n", input->y_len);
  char* x_str = (char*) malloc(input->x_len * sizeof(char));
  char* y_str = (char*) malloc(input->y_len * sizeof(char));
  if (fgets(x_str, input->x_len, input_file) == NULL) {
    print_error("ERROR: Could not read x in read_input");
  }
  if (fgets(y_str, input->y_len, input_file) == NULL) {
    print_error("ERROR: Could not read y in read_input");
  }
  input->x = x_str;
  input->y = y_str;
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
