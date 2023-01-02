#include <stdlib.h>

#include "matrix.h"
#include "def.h"

matrix *create_matrix(size_t rows, size_t cols) {
	matrix *m = (matrix *)malloc(sizeof(matrix));

	m->rows = rows;
	m->cols = cols;

	m->data = (elem_t *) _mm_malloc(rows * cols * sizeof(elem_t), VEC_ALIGN);

	return m;
}

void delete_matrix(matrix *m) {
	_mm_free(m->data);
	free(m);
}
