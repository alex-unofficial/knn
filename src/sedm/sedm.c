#include <omp.h>
#include <gsl/gsl_cblas.h>

#include "matrix.h"
#include "def.h"

#define min(x, y) (((x) < (y)) ? (x) : (y))

matrix *sedm_comp(const matrix *X, const matrix *Y) {

	// Getting the matrix sizes
	size_t n = X->rows;
	size_t m = Y->rows;
	size_t d = X->cols;

	// Initializing the distance matrix
	matrix *distance_matrix = create_matrix(n, m);

	// Setting the number of threads for OpenMP to be the maximum possible
	int threadnum = omp_get_max_threads();
	omp_set_num_threads(threadnum);

	// Calculate X_sqrd and Y_sqrd in parallel using SIMD instructions
	elem_t *X_sqrd = (elem_t*) malloc(n * sizeof(elem_t));
	elem_t *Y_sqrd = (elem_t*) malloc(m * sizeof(elem_t));

	#pragma omp parallel for
	for (size_t i = 0; i < n; i++) {
		VEC_T x_sqrd = VEC_ZERO();
		for (size_t k = 0; k < d - d % VEC_SIZE; k += VEC_SIZE) {
			VEC_T x = VEC_LOAD(MATRIX_ROW(X, i) + k);
			x_sqrd = VEC_FMADD(x, x, x_sqrd);
		}

		elem_t sum = x_sqrd[0] + x_sqrd[1] + x_sqrd[2] + x_sqrd[3] 
				  + x_sqrd[4] + x_sqrd[5] + x_sqrd[6] + x_sqrd[7];

		for (size_t k = d - d % VEC_SIZE; k < d; k++) {
			elem_t x = MATRIX_ELEM(X, i, k);
			sum += x * x;
		}

		X_sqrd[i] = sum;
	}
	

	#pragma omp parallel for
	for (size_t j = 0; j < m; j++) {
		VEC_T y_sqrd = VEC_ZERO();
		for (size_t k = 0; k < d - d % VEC_SIZE; k += VEC_SIZE) {
			VEC_T y = VEC_LOAD(MATRIX_ROW(Y, j) + k);
			y_sqrd = VEC_FMADD(y, y, y_sqrd);
		}
		
		elem_t sum = y_sqrd[0] + y_sqrd[1] + y_sqrd[2] + y_sqrd[3] 
				  + y_sqrd[4] + y_sqrd[5] + y_sqrd[6] + y_sqrd[7];

		for (int k = d - d % VEC_SIZE; k < d; k++) {
			elem_t y = MATRIX_ELEM(Y, j, k);
			sum += y * y;
		}

		Y_sqrd[j] = sum;
	}

	// Allocate memory for XY
	matrix *XY = create_matrix(n, m);

	// Calculate XY using BLAS subroutine
	#pragma omp parallel for
	for(int t = 0 ; t < threadnum ; t++) {
		size_t i_begin = t * n / threadnum;
		size_t i_end = min((t + 1) * n / threadnum, n);

		elem_t *Xi = MATRIX_ROW(X, i_begin);
		elem_t *XYi = MATRIX_ROW(XY, i_begin);

		cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasTrans, 
				i_end - i_begin, m, d, 
				1.0, Xi, d, Y->data, d, 
				0.0, XYi, m);
	}

	// Calculate the squared Euclidean distance matrix in parallel
	#pragma omp parallel for
	for (size_t i = 0; i < n; i++) {
		for (size_t j = 0; j < m; j++) {
			MATRIX_ELEM(distance_matrix, i, j) = X_sqrd[i] - 2 * MATRIX_ELEM(XY, i, j) + Y_sqrd[j];
		}
	}

	// Free memory
	free(X_sqrd);
	free(Y_sqrd);
	delete_matrix(XY);

	return distance_matrix;
}

matrix *sedm_simp(const matrix *X, const matrix *Y) {

	// Getting the matrix sizes
	size_t n = X->rows;
	size_t m = Y->rows;
	size_t d = X->cols;

	// Initializing the distance matrix
	matrix *distance_matrix = create_matrix(n, m);

	// Setting the number of threads for OpenMP to be the maximum possible
	int threadnum = omp_get_max_threads();
	omp_set_num_threads(threadnum);

	// Calculate the squared Euclidean distance matrix
	#pragma omp parallel for
	for (size_t i = 0; i < n; i++) {
		for (size_t j = 0; j < m; j++) {
			VEC_T v_sum = VEC_ZERO();

			for (size_t k = 0; k < d - d % VEC_SIZE; k += VEC_SIZE) {
				// Load VEC_SIZE values for the vectors at positions i and j in X and Y
				VEC_T x = VEC_LOAD(MATRIX_ROW(X, i) + k);
				VEC_T y = VEC_LOAD(MATRIX_ROW(Y, j) + k);

				// Compute the squared difference between Xik and Yjk and add it to the sum
				VEC_T diff = VEC_SUB(x, y);
				v_sum = VEC_FMADD(diff, diff, v_sum);
			}

			elem_t sum = v_sum[0] + v_sum[1] + v_sum[2] + v_sum[3] 
					  + v_sum[4] + v_sum[5] + v_sum[6] + v_sum[7];

			for(size_t k = d - d % VEC_SIZE; k < d; k++) {
				elem_t x = MATRIX_ELEM(X, i, k);
				elem_t y = MATRIX_ELEM(Y, j, k);

				elem_t diff = x - y;
				
				sum += diff * diff;
			}

			MATRIX_ELEM(distance_matrix, i, j) = sum;
		}
	}

	return distance_matrix;
}
