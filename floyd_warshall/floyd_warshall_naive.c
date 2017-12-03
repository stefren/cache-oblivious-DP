#include <stdio.h>
#include <string.h>
#include <getopt.h>

#include "../gep.h"
#include "floyd_warshall.h"

/* Naive implementation of Floyd-Warshall */
void floyd_warshall_naive(dp_matrix_t* X) {
  for (uint64_t i = 0; i < X->width; i++) {
    for (uint64_t j = 0; j < X->width; j++) {
      for (uint64_t k = 0; k < X->width; k++) {
        if (get_entry(X, i, k) != -1 && get_entry(X, k, j) != -1 && 
            get_entry(X, i, k) + get_entry(X, k, j) < get_entry(X, i, j)) {
          set_entry(X, i, j, get_entry(X, i, k) + get_entry(X, k, j));
        }
      }
    }
  }
}
