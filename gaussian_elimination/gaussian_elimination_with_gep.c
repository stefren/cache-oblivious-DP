#include <stdio.h>
#include <string.h>
#include <getopt.h>

#include "../gep.h"

static void usage(void) {
  fprintf(stderr, "Usage: gaussian_elimination [-f <file>]\n");
  fprintf(stderr, "\t-f <file> Use <file> as the input file. \n");
}

static uint64_t gaussian_elimination_update(uint64_t* x, uint64_t* u, uint64_t* v, uint64_t* w) {
  return *x - (*u * *v) / *w ;
  
}

static bool gaussian_elimination_update_exists(uint64_t n, uint64_t i_1, uint64_t i_2, uint64_t j_1, 
                                               uint64_t j_2, uint64_t k_1, uint64_t k_2) {
  return (k_1 < n - 2 && k_1 < i_2 && i_1 < n - 1 && k_1 < j_2 && j_1 < n);
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

  uint64_t length;
  
  fscanf(file, "%lld", &length);
  
  dp_matrix_t* X = init(length, length);
  
  uint64_t i, j, w;
  for (i = 0; i < length; i++) {
    for (j = 0; j < length; j++) {
      fscanf(file, "%lld", &w);
      set_entry(X, i, j, w);
    }
  }

  gep_update(X, &gaussian_elimination_update, &gaussian_elimination_update_exists);
  
  for (i = 0; i < length; i++) {
    for (j = 0; j < i; j++) {
      printf("0 ");
    }
    for (j = i; j < length; j++) {
      printf("%lld ", get_entry(X, i, j));
    }
      printf("\n");
  }
}