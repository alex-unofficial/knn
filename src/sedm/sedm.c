#include <omp.h>
#include <gsl/gsl_cblas.h>

#include "sedm.h"

#include "matrix.h"
#include "def.h"

#define min(x, y) (((x) < (y)) ? (x) : (y))

void sedm_comp(float *X, int n, float *Y, int m, int d, float *distance_matrix) {

	// Setting the number of threads for OpenMP to be the maximum possible
	int threadnum = omp_get_max_threads();
	omp_set_num_threads(threadnum);

	// Calculate X_sqrd and Y_sqrd in parallel using SIMD instructions
	float *X_sqrd = (float*) malloc(n * sizeof(float));
	float *Y_sqrd = (float*) malloc(m * sizeof(float));

	#pragma omp parallel for
	for (int i = 0; i < n; i++) {
		VEC_T x_sqrd = VEC_ZERO();
		for (int k = 0; k < d - d % VEC_SIZE; k += VEC_SIZE) {
			VEC_T x = VEC_LOAD(MATRIX_ROW(X, i, n, d) + k);
			x_sqrd = VEC_FMADD(x, x, x_sqrd);
		}

		float sum = x_sqrd[0] + x_sqrd[1] + x_sqrd[2] + x_sqrd[3] 
				  + x_sqrd[4] + x_sqrd[5] + x_sqrd[6] + x_sqrd[7];

		for (int k = d - d % VEC_SIZE; k < d; k++) {
			float x = MATRIX_ELEM(X, i, k, n, d);
			sum += x * x;
		}

		X_sqrd[i] = sum;
	}
	

	#pragma omp parallel for
	for (int j = 0; j < m; j++) {
		VEC_T y_sqrd = VEC_ZERO();
		for (int k = 0; k < d - d % VEC_SIZE; k += VEC_SIZE) {
			VEC_T y = VEC_LOAD(MATRIX_ROW(Y, j, m, d) + k);
			y_sqrd = VEC_FMADD(y, y, y_sqrd);
		}
		
		float sum = y_sqrd[0] + y_sqrd[1] + y_sqrd[2] + y_sqrd[3] 
				  + y_sqrd[4] + y_sqrd[5] + y_sqrd[6] + y_sqrd[7];

		for (int k = d - d % VEC_SIZE; k < d; k++) {
			float y = MATRIX_ELEM(Y, j, k, m, d);
			sum += y * y;
		}

		Y_sqrd[j] = sum;
	}

	// Allocate memory for XY
	float *XY = (float*) malloc(n * m * sizeof(float));

	// Calculate XY using BLAS subroutine
	#pragma omp parallel for
	for(int t = 0 ; t < threadnum ; t++) {
		int i_begin = t * n / threadnum;
		int i_end = min((t + 1) * n / threadnum, n);

		float *Xi = MATRIX_ROW(X, i_begin, n, d);
		float *XYi = MATRIX_ROW(XY, i_begin, n, m);

		cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasTrans, 
				i_end - i_begin, m, d, 
				1.0, Xi, d, Y, d, 
				0.0, XYi, m);
	}

	// Calculate the squared Euclidean distance matrix in parallel
	#pragma omp parallel for
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			MATRIX_ELEM(distance_matrix, i, j, n, m) = X_sqrd[i] - 2 * MATRIX_ELEM(XY, i, j, n, m) + Y_sqrd[j];
		}
	}

	// Free memory
	free(X_sqrd);
	free(Y_sqrd);
	free(XY);
}

void sedm_simp(float *X, int n, float *Y, int m, int d, float *distance_matrix) {

	// Setting the number of threads for OpenMP to be the maximum possible
	int threadnum = omp_get_max_threads();
	omp_set_num_threads(threadnum);

	// Calculate the squared Euclidean distance matrix
	#pragma omp parallel for
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			VEC_T v_sum = VEC_ZERO();

			for (int k = 0; k < d - d % VEC_SIZE; k += VEC_SIZE) {
				// Load VEC_SIZE values for the vectors at positions i and j in X and Y
				VEC_T x = VEC_LOAD(MATRIX_ROW(X, i, n, d) + k);
				VEC_T y = VEC_LOAD(MATRIX_ROW(Y, j, m, d) + k);

				// Compute the squared difference between Xik and Yjk and add it to the sum
				VEC_T diff = VEC_SUB(x, y);
				v_sum = VEC_FMADD(diff, diff, v_sum);
			}

			float sum = v_sum[0] + v_sum[1] + v_sum[2] + v_sum[3] 
					  + v_sum[4] + v_sum[5] + v_sum[6] + v_sum[7];

			for(int k = d - d % VEC_SIZE; k < d; k++) {
				float x = MATRIX_ELEM(X, i, k, n, d);
				float y = MATRIX_ELEM(Y, j, k, m, d);

				float diff = x - y;
				
				sum += diff * diff;
			}

			MATRIX_ELEM(distance_matrix, i, j, n, m) = sum;
		}
	}
}
