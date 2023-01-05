#ifndef MATRIX_H
#define MATRIX_H

#include "def.h"

static inline elem_t *create_matrix(size_t rows, size_t cols) {
	return (elem_t *) _mm_malloc(rows * cols * sizeof(elem_t), VEC_ALIGN);
}

static inline void delete_matrix(elem_t *m) {
	_mm_free(m);
}

#define MATRIX_ELEM(m, i, j, rows, cols) ((m)[(i) * (cols) + (j)])
#define MATRIX_ROW(m, i, rows, cols) ((m) + (i) * (cols))

#endif
