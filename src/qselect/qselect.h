#ifndef QSELECT_H
#define QSELECT_H

#include "def.h"

#define SWAP(a, b) {typeof(a) temp = (a); (a) = (b); (b) = temp;}

void gen_indices(size_t *ind, size_t size);

elem_t qselect(size_t k, elem_t *arr, size_t *ind, size_t size);

#endif
