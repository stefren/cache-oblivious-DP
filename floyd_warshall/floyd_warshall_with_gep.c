#include <stdio.h>
#include <string.h>
#include <getopt.h>

#include "../gep.h"
#include "floyd_warshall.h"

static uint64_t floyd_warshall_update(uint64_t* x, uint64_t* u, uint64_t* v, uint64_t* w) {
  if (*u == -1 || *v == -1 || *u + *v > *x) return *x;
  return *u + *v;
}

static bool floyd_warshall_update_exists(uint64_t n, uint64_t i_1, uint64_t i_2, uint64_t j_1,
                                         uint64_t j_2, uint64_t k_1, uint64_t k_2) {
  return true; 
}

void floyd_warshall_with_gep(dp_matrix_t* X) {
  GEP(X, &floyd_warshall_update, &floyd_warshall_update_exists);
}