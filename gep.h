#include <stdbool.h>
#include <inttypes.h>
#include <stdlib.h>

#ifndef GEP_H
#define GEP_H

typedef struct {
  uint64_t width;
  uint64_t height;
  uint64_t* entries;
} dp_matrix_t;

dp_matrix_t* init(uint64_t width, uint64_t height);

uint64_t get_entry(dp_matrix_t* X, uint64_t i, uint64_t j);
uint64_t* get_entry_addr(dp_matrix_t* X, uint64_t i, uint64_t j);
void set_entry(dp_matrix_t* X, uint64_t i, uint64_t j, uint64_t value);

/* Template for the update function, which defines c[i,j] in terms of x = c[i,j],
 * u = c[i,k], v = c[k,j], and w = c[k,k]
 */
typedef uint64_t (*UPDATE_F)(uint64_t* x, uint64_t* u, uint64_t* v, uint64_t* w);

/* Template for the update_exists function, which should return false if there
 * is no update to be made on the range of i's, j's, and k's. 
 * 
 * n is the length of the dp_matrix (assumed to be square). 
 */
typedef bool (*UPDATE_EXISTS_F)(uint64_t n, uint64_t i_1, uint64_t i_2, uint64_t j_1, 
                                uint64_t j_2, uint64_t k_1, uint64_t k_2);

uint64_t GEP(dp_matrix_t* X, UPDATE_F update_f, UPDATE_EXISTS_F update_exists_f);

#endif /* GEP_H */
