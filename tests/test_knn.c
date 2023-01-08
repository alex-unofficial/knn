#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "knn.h"
#include "matrix.h"
#include "def.h"

int int_pow(int base, int power) {
	int result = 1;

	while(power > 0) {
		if(power & 0x01) {
			result *= base;
		}

		base *= base;
		power >>= 1;
	}

	return result;
}

int main(int argc, char **argv) {

	printf("========== testing knn ==========\n");

	int side_len = 10;
	int d = 4;

	int t = 100;

	size_t n = int_pow(side_len, d);
	int k = 2 * d + 1;

	printf("side = %d\nn = %d\nd = %d\nk = %d\n",side_len, n, d, k);
	printf("\n");
	
	elem_t *X = create_matrix(n, d);

	for(size_t i = 0 ; i < n ; i++) {

		size_t idx = i;
		for(int j = 0 ; j < d ; j++) {
			MATRIX_ELEM(X, i, j, n, d) = (elem_t) (idx % side_len);
			idx /= side_len;
		}
	}

	knn_result *res = knn(X, n, X, n, d, k, t);

	printf("checking for errors:\n");

	int errors = 0;
	for(size_t i = 0 ; i < n ; i++) {
		int coords[d];
		for(int j = 0 ; j < d ; j++)
			coords[j] = (int) MATRIX_ELEM(X, i, j, n, d);

		size_t *knn_i = MATRIX_ROW(res->n_idx, i, n, k);

		{ // first nearest neighbour
			int n_coords[d];
			for(int j = 0 ; j < d ; j++)
				n_coords[j] = (int) MATRIX_ELEM(X, knn_i[0], j, n, d);

			int error = 0;
			for(int j = 0 ; j < d ; j++) {
				if(n_coords[j] != coords[j]) {
					error = 1;
					break;
				}
			}

			if(error) {
				printf("%3d: error at [ ", ++errors);
				for(int j = 0 ; j < d ; j++)
					printf("%2d ", coords[j]);

				printf("]: first neighbour is [ ");
				for(int j = 0 ; j < d ; j++)
					printf("%2d ", n_coords[j]);

				printf("]\n");
			}
		}

		int is_internal = 1;
		for(int j = 0 ; j < d ; j++) {
			if(coords[j] == 0 || coords[j] == side_len - 1)
				is_internal = 0;
		}

		if(is_internal) {

			for(int l = 1 ; l < k ; l++) {
				int n_coords[d];
				for(int j = 0 ; j < d ; j++)
					n_coords[j] = (int) MATRIX_ELEM(X, knn_i[l], j, n, d);

				int sum = 0;
				for(int j = 0 ; j < d ; j++) {
					sum += abs(coords[j] - n_coords[j]);
				}

				if(sum != 1) {
					printf("%3d: error at [ ", ++errors);
					for(int j = 0 ; j < d ; j++)
						printf("%2d ", coords[j]);

					printf("]: neighbour %d is [ ", l);
					for(int j = 0 ; j < d ; j++)
						printf("%2d ", n_coords[j]);

					printf("]\n");
				}
			}
		}
	}

	printf("found %d errors\n", errors);
	printf("\n");
	
	delete_matrix(X);
	delete_knn(res);

	if(!errors)
		return 0;
	else
		return 1;
}
