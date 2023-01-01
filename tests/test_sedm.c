#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <math.h>

#include "sedm.h"

#include "matrix.h"
#include "simd.h"

#define EPSILON 1e-3

int main(int argc, char** argv) {
	srand(time(NULL));

	// Set the dimensions of the matrices
	int n = 1000;
	int m = 6000;
	int d = 512;

	// Allocate memory for the matrices
	float *X = (float*) _mm_malloc(n * d * sizeof(float), VEC_ALIGN);
	float *Y = (float*) _mm_malloc(m * d * sizeof(float), VEC_ALIGN);
	float *distance_matrix = (float*) malloc(n * m * sizeof(float));
	float *reference_distance_matrix = (float*) malloc(n * m * sizeof(float));

	// Initialize the matrices with random values
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < d; j++) {
			MATRIX_ELEM(X, i, j, n, d) = (float) rand() / RAND_MAX;
		}
	}
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < d; j++) {
			MATRIX_ELEM(Y, i, j, m, d) = (float) rand() / RAND_MAX;
		}
	}

	// Calculate the squared Euclidean distance matrix using both functions
	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);
	sedm_comp(X, n, Y, m, d, distance_matrix);
	clock_gettime(CLOCK_MONOTONIC, &end);
	double sedm_comp_time = (double) (end.tv_sec - start.tv_sec) * 1000.0 + (double) (end.tv_nsec - start.tv_nsec) / 1000000.0;

	clock_gettime(CLOCK_MONOTONIC, &start);
	sedm_simp(X, n, Y, m, d, reference_distance_matrix);
	clock_gettime(CLOCK_MONOTONIC, &end);
	double sedm_simp_time = (double) (end.tv_sec - start.tv_sec) * 1000.0 + (double) (end.tv_nsec - start.tv_nsec) / 1000000.0;

	// Compare the distance matrices
   	int equal = 1;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (fabs(MATRIX_ELEM(distance_matrix, i, j, n, m) 
				   - MATRIX_ELEM(reference_distance_matrix, i, j, n, m)) > EPSILON) {

				equal = 0;

				printf("Difference at (%d, %d): %0.4f (calculated) vs %0.4f (reference)\n", i, j, 
					MATRIX_ELEM(distance_matrix, i, j, n, m), 
					MATRIX_ELEM(reference_distance_matrix, i, j, n, m));
			}
		}
	}

	if (equal) {
		printf("The distance matrices are equal.\n");
	} else {
		printf("\nThe distance matrices are not equal.\n");
	}

	printf("The simple version took %0.2f ms\n", sedm_simp_time);
	printf("The compound version took %0.2f ms\n", sedm_comp_time);
	printf("The relative speedup is %0.0f\%\n", 100.0 * sedm_comp_time / sedm_simp_time);

	// Free memory
	_mm_free(X);
	_mm_free(Y);
	free(distance_matrix);
	free(reference_distance_matrix);

	return 0;
}
