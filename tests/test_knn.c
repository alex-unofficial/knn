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

	int side_len = 100;
	int d = 2;

	int n = int_pow(side_len, d);
	int k = 2 * d + 1;

	printf("side = %d\nn = %d\nd = %d\nk = %d\n",side_len, n, d, k);
	printf("\n");
	
	matrix *X = create_matrix(n, d);

	// this works only for d = 2
	for(int x = 0 ; x < side_len ; x++) {
		for(int y = 0 ; y < side_len ; y++) {

			int i = x * side_len + y;

			MATRIX_ELEM(X->data, i, 0, n, d) = (elem_t) x;
			MATRIX_ELEM(X->data, i, 1, n, d) = (elem_t) y;
		}
	}

	knn_result *res = knn(X->data, n, X->data, n, d, k);

	printf("checking for errors:\n");

	int errors = 0;
	for(int i = 0 ; i < n ; i++) {
		int x = (int) MATRIX_ELEM(X->data, i, 0, n, d);
		int y = (int) MATRIX_ELEM(X->data, i, 1, n, d);

		size_t *knn_i = MATRIX_ROW(res->n_idx, i, n, k);

		{ // first nearest neighbour
			int n_x = (int) MATRIX_ELEM(X->data, knn_i[0], 0, n, d);
			int n_y = (int) MATRIX_ELEM(X->data, knn_i[0], 1, n, d);

			if(!(x == n_x && y == n_y))
				printf("%3d: error at (%2d, %2d): first neighbout is (%2d, %2d)\n", ++errors, x, y, n_x, n_y);
		}

		if((0 < x && x < side_len - 1) && (0 < y && y < side_len - 1)) {

			for(int j = 1 ; j < k ; j++) {
				int n_x = (int) MATRIX_ELEM(X->data, knn_i[j], 0, n, d);
				int n_y = (int) MATRIX_ELEM(X->data, knn_i[j], 1, n, d);

				if(!((abs(x - n_x) == 1 && abs(y - n_y) == 0) || (abs(y - n_y) == 1 && abs(x - n_x) == 0)))
					printf("%3d: error at (%2d, %2d): neighbour %d is (%2d, %2d)\n", 
							++errors, x, y, j, n_x, n_y);
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
