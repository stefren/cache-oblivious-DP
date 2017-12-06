#include <stdio.h>

#include "./gep.h"

#define CACHE_SIZE_KB 256
 
/* An implementation of the Gaussian elimination paradigm for simple dynamic programming problems */

uint64_t get_entry(dp_matrix_t* X, uint64_t i, uint64_t j) {
  if (i > X->width || j > X->height) {
    return 0;  // return 0 if the entry is out of range
  }
  return X->entries[j*X->width + i];
}

void set_entry(dp_matrix_t* X, uint64_t i, uint64_t j, uint64_t value) {
  if (i >= X->width || j >= X->height) {
    return;
  }
  X->entries[j*X->width + i] = value;
}

dp_matrix_t* init(uint64_t width, uint64_t height) {
  dp_matrix_t* X = malloc(sizeof(dp_matrix_t));
  uint64_t* matrix = (uint64_t*) calloc(sizeof(uint64_t), width * height);
  X->width = width;
  X->height = height;
  X->entries = matrix;
  return X;
} 

void recursive_GEP(dp_matrix_t* X, uint64_t i_1, uint64_t i_2, 
                   uint64_t j_1, uint64_t j_2, uint64_t k_1, uint64_t k_2,
                   UPDATE_F update_f, UPDATE_EXISTS_F update_exists_f) {
//  printf("%lld %lld %lld %lld %lld %lld\n", i_1, i_2, j_1, j_2, k_1, k_2);
  if (update_exists_f(X->width, i_1, i_2, j_1, j_2, k_1, k_2) == false || k_2 < k_1 || j_2 < j_1 || i_2 < i_1) {
    return;
  }
  if (k_1 == k_2 && i_1 == i_2 && j_1 == j_2) { // base case
    uint64_t x = get_entry(X, i_1, j_1); uint64_t u = get_entry(X, i_1, k_1);
    uint64_t v = get_entry(X, k_1, j_1);
    uint64_t w = get_entry(X, k_1, k_1);
//    printf("%lld, %lld, %lld\n", i_1, j_1, k_1);    
    set_entry(X, i_1, j_1, update_f(x, u, v, w));
  } else {
    uint64_t i_m = (i_1 + i_2)/2;
    uint64_t j_m = (j_1 + j_2)/2;
    uint64_t k_m = (k_1 + k_2)/2;
    
    // forward pass
    recursive_GEP(X, i_1, i_m, j_1, j_m, k_1, k_m, update_f, update_exists_f);  
    recursive_GEP(X, i_m + 1, i_2, j_1, j_m, k_1, k_m, update_f, update_exists_f);  
    recursive_GEP(X, i_1, i_m, j_m + 1, j_2, k_1, k_m, update_f, update_exists_f);  
    recursive_GEP(X, i_m + 1, i_2, j_m + 1, j_2, k_1, k_m, update_f, update_exists_f);  

    // backward pass
    recursive_GEP(X, i_m + 1, i_2, j_m + 1, j_2, k_m + 1, k_2, update_f, update_exists_f);  
    recursive_GEP(X, i_1, i_m, j_m + 1, j_2, k_m + 1, k_2, update_f, update_exists_f);  
    recursive_GEP(X, i_m + 1, i_2, j_1, j_m, k_m + 1, k_2, update_f, update_exists_f);  
    recursive_GEP(X, i_1, i_m, j_1, j_m, k_m + 1, k_2, update_f, update_exists_f);  
  }
}

void recursive_parallel_GEP(dp_matrix_t* X, uint64_t i_1, uint64_t i_2, 
                   uint64_t j_1, uint64_t j_2, uint64_t k_1, uint64_t k_2,
                   UPDATE_F update_f, UPDATE_EXISTS_F update_exists_f) {
//  printf("%lld %lld %lld %lld %lld %lld\n", i_1, i_2, j_1, j_2, k_1, k_2);
  if (update_exists_f(X->width, i_1, i_2, j_1, j_2, k_1, k_2) == false || k_2 < k_1 || j_2 < j_1 || i_2 < i_1) {
    return;
  }
  if (k_1 == k_2 && i_1 == i_2 && j_1 == j_2) { // base case
    uint64_t x = get_entry(X, i_1, j_1); uint64_t u = get_entry(X, i_1, k_1);
    uint64_t v = get_entry(X, k_1, j_1);
    uint64_t w = get_entry(X, k_1, k_1);
//    printf("%lld, %lld, %lld\n", i_1, j_1, k_1);    
    set_entry(X, i_1, j_1, update_f(x, u, v, w));
  } else {
    uint64_t i_m = (i_1 + i_2)/2;
    uint64_t j_m = (j_1 + j_2)/2;
    uint64_t k_m = (k_1 + k_2)/2;
    
    // forward pass
    recursive_GEP(X, i_1, i_m, j_1, j_m, k_1, k_m, update_f, update_exists_f);  
    #pragma omp parallel
    {
    recursive_GEP(X, i_m + 1, i_2, j_1, j_m, k_1, k_m, update_f, update_exists_f);  
    recursive_GEP(X, i_1, i_m, j_m + 1, j_2, k_1, k_m, update_f, update_exists_f);  
    }
    recursive_GEP(X, i_m + 1, i_2, j_m + 1, j_2, k_1, k_m, update_f, update_exists_f);  

    // backward pass
    recursive_GEP(X, i_m + 1, i_2, j_m + 1, j_2, k_m + 1, k_2, update_f, update_exists_f);  
    #pragma omp parallel
    {
    recursive_GEP(X, i_1, i_m, j_m + 1, j_2, k_m + 1, k_2, update_f, update_exists_f);  
    recursive_GEP(X, i_m + 1, i_2, j_1, j_m, k_m + 1, k_2, update_f, update_exists_f);  
    }
    recursive_GEP(X, i_1, i_m, j_1, j_m, k_m + 1, k_2, update_f, update_exists_f);  
  }
}

void recursive_split_longest_GEP(dp_matrix_t* X, uint64_t i_1, uint64_t i_2, 
                   uint64_t j_1, uint64_t j_2, uint64_t k_1, uint64_t k_2,
                   UPDATE_F update_f, UPDATE_EXISTS_F update_exists_f, bool reverse) {
//  printf("%lld %lld %lld %lld %lld %lld\n", i_1, i_2, j_1, j_2, k_1, k_2);
  if (update_exists_f(X->width, i_1, i_2, j_1, j_2, k_1, k_2) == false) {
    return;
  }
  if (k_1 == k_2 && i_1 == i_2 && j_1 == j_2) { // base case
//    printf("%lld, %lld, %lld\n ", i_1, j_1, k_1);
    uint64_t x = get_entry(X, i_1, j_1);
    uint64_t u = get_entry(X, i_1, k_1);
    uint64_t v = get_entry(X, k_1, j_1);
    uint64_t w = get_entry(X, k_1, k_1);
    set_entry(X, i_1, j_1, update_f(x, u, v, w));
  } else {
    uint64_t i_diff = i_2 - i_1;
    uint64_t j_diff = j_2 - j_1;
    uint64_t k_diff = k_2 - k_1;
    if (!reverse) {
      if (k_diff >= j_diff && k_diff >= i_diff) {
        uint64_t k_m = (k_1 + k_2)/2;
        recursive_split_longest_GEP(X, i_1, i_2, j_1, j_2, k_1, k_m, update_f, update_exists_f, false);  
        recursive_split_longest_GEP(X, i_1, i_2, j_1, j_2, k_m + 1, k_2, update_f, update_exists_f, true);  
      } else if (j_diff >= i_diff && j_diff >= k_diff) {
        uint64_t j_m = (j_1 + j_2)/2;
        recursive_split_longest_GEP(X, i_1, i_2, j_1, j_m, k_1, k_2, update_f, update_exists_f, false);  
        recursive_split_longest_GEP(X, i_1, i_2, j_m + 1, j_2, k_1, k_2, update_f, update_exists_f, false);  
      } else {
        uint64_t i_m = (i_1 + i_2)/2;
        recursive_split_longest_GEP(X, i_1, i_m, j_1, j_2, k_1, k_2, update_f, update_exists_f, false);  
        recursive_split_longest_GEP(X, i_m + 1, i_2, j_1, j_2, k_1, k_2, update_f, update_exists_f, false);  
      }
    } else {
      if (k_diff >= j_diff && k_diff >= i_diff) {
        uint64_t k_m = (k_1 + k_2)/2;
        recursive_split_longest_GEP(X, i_1, i_2, j_1, j_2, k_1, k_m, update_f, update_exists_f, false);  
        recursive_split_longest_GEP(X, i_1, i_2, j_1, j_2, k_m + 1, k_2, update_f, update_exists_f, true);  
      } else if (j_diff >= i_diff && j_diff >= k_diff) {
        uint64_t j_m = (j_1 + j_2)/2;
        recursive_split_longest_GEP(X, i_1, i_2, j_m + 1, j_2, k_1, k_2, update_f, update_exists_f, true);  
        recursive_split_longest_GEP(X, i_1, i_2, j_1, j_m, k_1, k_2, update_f, update_exists_f, true);  
      } else {
        uint64_t i_m = (i_1 + i_2)/2;
        recursive_split_longest_GEP(X, i_m + 1, i_2, j_1, j_2, k_1, k_2, update_f, update_exists_f, true);  
        recursive_split_longest_GEP(X, i_1, i_m, j_1, j_2, k_1, k_2, update_f, update_exists_f, true);  
      }
    }
  }
}
uint64_t f_w(uint64_t x, uint64_t u, uint64_t v, uint64_t w) {
  if (u == -1 || v == -1 || x < u + v) return x;
  return u + v;
}

void iterative(dp_matrix_t* X, uint64_t i_1, uint64_t i_2, 
                   uint64_t j_1, uint64_t j_2, uint64_t k_1, uint64_t k_2,
                   UPDATE_F update_f, UPDATE_EXISTS_F update_exists_f) {
  for (uint64_t k = k_1; k <= k_2; k++) {
    for (uint64_t i = i_1; i <= i_2; i++) {
      for (uint64_t j = j_1; j <= j_2; j++) {
        if (update_exists_f(X->width, i, i, j, j, k, k)) {
          uint64_t x = get_entry(X, i, j);
          uint64_t u = get_entry(X, i, k);
          uint64_t v = get_entry(X, k, j);
          uint64_t w = get_entry(X, k, k);
          set_entry(X, i, j, f_w(x, u, v, w));
        }
      }
    }
  }
}

uint64_t GEP(dp_matrix_t* X, UPDATE_F update_f, UPDATE_EXISTS_F update_exists_f) {
  uint64_t length = X->width;
//  recursive_split_longest_GEP(X, 0, length - 1, 0, length - 1, 0, length - 1, update_f, update_exists_f, false);
//  recursive_parallel_GEP(X, 0, length - 1, 0, length - 1, 0, length - 1, update_f, update_exists_f);
//  recursive_GEP(X, 0, length - 1, 0, length - 1, 0, length - 1, update_f, update_exists_f);
  iterative(X, 0, length - 1, 0, length - 1, 0, length - 1, update_f, update_exists_f);
  return get_entry(X, length - 1, length - 1);
}

