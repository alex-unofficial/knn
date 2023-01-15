#ifndef QSELECT_H
#define QSELECT_H

#include "def.h"

/* generates an array of indices starting at begin_idx 
 * of size size and stores it in ind */
void gen_indices(size_t *ind, size_t begin_idx, size_t size);

/* qselect selects the k-th smallest element in the array */
elem_t qselect(size_t k, elem_t *arr, size_t *ind, size_t size);

#endif
