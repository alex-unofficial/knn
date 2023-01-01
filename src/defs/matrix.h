#ifndef MATRIX_H
#define MATRIX_H

#define MATRIX_ELEM(matrix, i, j, rows, cols) ((matrix)[(i) * (cols) + (j)])
#define MATRIX_ROW(matrix, i, rows, cols) ((matrix) + (i) * (cols))

#endif
