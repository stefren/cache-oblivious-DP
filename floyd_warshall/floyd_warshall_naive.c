#include <stdio.h>
#include <string.h>
#include <getopt.h>

#include "../gep.h"

static void usage(void) {
  fprintf(stderr, "Usage: floyd_warshall_naive [-f <file>]\n");
  fprintf(stderr, "\t-f <file> Use <file> as the input file. \n");
}

/* Naive implementation of Floyd-Warshall */
void floyd_warshall_naive(dp_matrix_t* X) {
  for (k = 0; k < X->width; k++) {
    for (i = 0; i < X->width; i++) {
      for (j = 0; j < X->width; j++) {
        if (get_entry(X, i, k) != -1 && get_entry(X, k, j) != -1 && 
            get_entry(X, i, k) + get_entry(X, k, j) < get_entry(X, i, j)) {
          set_entry(X, i, j, get_entry(X, i, k) + get_entry(X, k, j));
        }
      }
    }
  }
}

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
      input_file = (char*) malloc(1024);
      if (input_file == NULL) {
        printf("ERROR: malloc failed in main");
      }
      input_file = strdup(optarg);
      break;
    default: 
      usage();
      exit(1);
    }
  }
  
  FILE* file;
  if ((file = fopen(input_file, "r")) == NULL) {
    printf("ERROR: Could not open file");
  }
  uint64_t num_vertices;
  uint64_t num_edges;
  
  fscanf(file, "%lld", &num_vertices);
  fscanf(file, "%lld", &num_edges);
  
  uint64_t length = num_vertices;
  dp_matrix_t* X = init(length);
  
  uint64_t i, j, w;
  for (i = 0; i < length; i++) {
    for (j = 0; j < length; j++) {
      set_entry(X, i, j, -1);
    }
  }

  for (i = 0; i < length; i++) {
    set_entry(X, i, i, 0);
  }

  for (uint64_t k = 0; k < num_edges; k++) {
    fscanf(file, "%lld%lld%lld", &i, &j, &w);
    set_entry(X, i, j, w);
  }

  floyd_warshall_naive(X);
  
  for (i = 0; i < length * length; i++) {
    printf("%lld, %lld: %lld\n", i/length, i%length, X->entries[i]);
  }
}