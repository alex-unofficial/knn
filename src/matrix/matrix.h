/* matrix.h - functions and macros for working with row-major matrices
 * Copyright (C) 2023  Alexandros Athanasiadis
 *
 * This file is part of knn
 *
 * knn is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * knn is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef MATRIX_H
#define MATRIX_H

#include "def.h"

/* function for creating a matrix with size rows * cols that works with vector instructions */
static inline elem_t *create_matrix(size_t rows, size_t cols) {
	return (elem_t *) _mm_malloc(rows * cols * sizeof(elem_t), VEC_ALIGN);
}

/* function to free memory of a matrix allocated using create_matrix */
static inline void delete_matrix(elem_t *m) {
	_mm_free(m);
}

/* macros for indexing a row-major matrix m of size rows * cols*/
#define MATRIX_ELEM(m, i, j, rows, cols) ((m)[(i) * (cols) + (j)])
#define MATRIX_ROW(m, i, rows, cols) ((m) + (i) * (cols))

#endif
