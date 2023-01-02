#ifndef MATRIX_H
#define MATRIX_H

#include "def.h"

typedef struct Matrix {
	// size of matrix
	size_t rows;
	size_t cols;

	// data in matrix 
	elem_t *data;
} matrix;

matrix *create_matrix(size_t rows, size_t cols);
void delete_matrix(matrix *m);

#define MATRIX_ELEM(m, i, j, rows, cols) ((m)[(i) * (cols) + (j)])
#define MATRIX_ROW(m, i, rows, cols) ((m) + (i) * (cols))

#endif
