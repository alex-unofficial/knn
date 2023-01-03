#ifndef QSELECT_H
#define QSELECT_H

#include "def.h"

elem_t qselect(size_t k, elem_t *arr, size_t size);

#define SWAP(a, b) {typeof(a) temp = (a); (a) = (b); (b) = temp;}

#endif
