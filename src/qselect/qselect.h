#ifndef QSELECT_H
#define QSELECT_H

#include "def.h"

void gen_indices(size_t *ind, size_t begin_idx, size_t size);

elem_t qselect(size_t k, elem_t *arr, size_t *ind, size_t size);

#endif
