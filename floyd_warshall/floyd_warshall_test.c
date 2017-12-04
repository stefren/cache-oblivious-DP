#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <assert.h>
#include "../fasttime.h"
#include "./floyd_warshall.h"

#define DEFAULT_LENGTH 1 << 10
static void usage(void) {
  fprintf(stderr, "Usage: floyd_warshall [-f <file>] [-r] \n");
  fprintf(stderr, "\t-f <file> Use <file> as the input file. \n");
  fprintf(stderr, "\t-r Randomly initialize the matrix \n");
}

int main(int argc, char** argv) {
  char* input_file;

  dp_matrix_t* X;
  dp_matrix_t* Y;

  /*
   * Read and interpret the command line arguments
   */
  if (argv[1] == NULL) {
    usage();
    exit(1);
  }
  char c = 0;
  while ((c = getopt(argc, argv, "rf:")) != EOF) {
    switch (c) {
    case 'f':
      input_file = (char*) malloc(1024);
      if (input_file == NULL) {
        printf("ERROR: malloc failed in main");
      }
      input_file = strdup(optarg);

      FILE* file;
      if ((file = fopen(input_file, "r")) == NULL) {
        printf("ERROR: Could not open file");
      }
      uint64_t num_vertices;
      uint64_t num_edges;

      fscanf(file, "%ld", &num_vertices);
      fscanf(file, "%ld", &num_edges);

      uint64_t length = num_vertices;
      X = init(length, length);
      Y = init(length, length);

      uint64_t i, j, w;
      for (i = 0; i < length; i++) {
        for (j = 0; j < length; j++) {
          set_entry(X, i, j, -1);
          set_entry(Y, i, j, -1);
        }
      }

      for (i = 0; i < length; i++) {
        set_entry(X, i, i, 0);
        set_entry(Y, i, j, 0);
      }

      for (uint64_t k = 0; k < num_edges; k++) {
        fscanf(file, "%ld%ld%ld", &i, &j, &w);
        set_entry(X, i, j, w);
        set_entry(Y, i, j, w);
      }
      break;
    case 'r':
      X = init(DEFAULT_LENGTH, DEFAULT_LENGTH);
      Y = init(DEFAULT_LENGTH, DEFAULT_LENGTH);
      for (uint64_t i = 0; i < DEFAULT_LENGTH; i++) {
        for (uint64_t j = 0; j < DEFAULT_LENGTH; j++) {
          set_entry(X, i, j, -1);
          set_entry(Y, i, j, -1);
        }
      }
      for (uint64_t i = 0; i < DEFAULT_LENGTH * DEFAULT_LENGTH; i++) {
        uint64_t x = rand() % DEFAULT_LENGTH;
        uint64_t y = rand() % DEFAULT_LENGTH;
        uint64_t rand_value = rand();
        set_entry(X, x, y, rand_value);
        set_entry(Y, x, y, rand_value);
      }
      break;
    default:
      usage();
      exit(1);
    }
  }

  /* Run Floyd-Warshall with GEP on X */
  fasttime_t start_time, end_time;
  double elapsed;
  start_time = gettime();
  floyd_warshall_with_gep(X);
  end_time = gettime();
  elapsed = tdiff(start_time, end_time);
  printf("Cache-Oblivious: %f seconds\n", elapsed);

  /* Run naive Floyd-Warshall on Y */
  start_time = gettime();
  floyd_warshall_naive(Y);
  end_time = gettime();
  elapsed = tdiff(start_time, end_time);
  printf("Naive: %f seconds\n", elapsed);

  #ifndef NDEBUG
  for (i = 0; i < length * length; i++) {
    assert(X->entries[i] == Y->entries[i]);
  }
  printf("\033[0;32mTESTS PASSED\033[0m All elements the same. \n");
  #endif
}
