#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <math.h>

#include "sedm.h"

#include "matrix.h"
#include "def.h"

#define EPSILON 1e-1

int main(int argc, char** argv) {
	srand(time(NULL));

	printf("========= testing sedm ==========\n");

	// Set the dimensions of the matrices
	size_t n = 20000;
	size_t m = 20000;
	size_t d = 4096;

	printf("n = %lu\nm = %lu\nd = %lu\n", n, m, d);
	printf("\n");

	// Allocate memory for the matrices
	elem_t *X = create_matrix(n, d);
	elem_t *Y = create_matrix(m, d);
	elem_t *distance_matrix = create_matrix(n, m);
	elem_t *reference_distance_matrix = create_matrix(n, m);

	// Initialize the matrices with random values
	for (size_t i = 0; i < n; i++) {
		for (size_t j = 0; j < d; j++) {
			MATRIX_ELEM(X, i, j, n, d) = (elem_t) rand() / RAND_MAX;
		}
	}
	for (size_t i = 0; i < m; i++) {
		for (size_t j = 0; j < d; j++) {
			MATRIX_ELEM(Y, i, j, m, d) = (elem_t) rand() / RAND_MAX;
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

	printf("checking for errors:\n");
	// Compare the distance matrices
   	int errors = 0;
	for (size_t i = 0; i < n; i++) {
		for (size_t j = 0; j < m; j++) {
			if (fabs(MATRIX_ELEM(distance_matrix, i, j, n, m) 
				   - MATRIX_ELEM(reference_distance_matrix, i, j, n, m)) > EPSILON) {

				printf("%d: Difference at (%lu, %lu): %0.4f (calculated) vs %0.4f (reference)\n",
					++errors, i, j, MATRIX_ELEM(distance_matrix, i, j, n, m), 
					MATRIX_ELEM(reference_distance_matrix, i, j, n, m));
			}
		}
	}

	printf("found %d errors\n", errors);
	printf("\n");

	printf("The simple version took %0.2f ms\n", sedm_simp_time);
	printf("The compound version took %0.2f ms\n", sedm_comp_time);
	printf("The relative speedup is %0.0f\%\n", 100.0 * sedm_comp_time / sedm_simp_time);
	printf("\n");

	// Free memory
	delete_matrix(X);
	delete_matrix(Y);
	delete_matrix(distance_matrix);
	delete_matrix(reference_distance_matrix);

	if(!errors)
		return 0;
	else
		return 1;
}
