#include "knn.h"

#include <omp.h>

#include "sedm.h"
#include "qselect.h"
#include "matrix.h"
#include "def.h"

#ifndef MAX_MEM
#define MAX_MEM (1 << 30)
#endif

knn_result *knn(const elem_t *X, size_t n, const elem_t *Y, size_t m, int d, int k) {

	// the result will be stored in res.
	knn_result *res = (knn_result *) malloc(sizeof(knn_result));

	res->m = n;
	res->k = k;

	res->n_idx = (size_t *) malloc(n * k * sizeof(size_t));
	res->n_dist = (elem_t *) malloc(n * k * sizeof(elem_t));

	// setting up OpenMP to use the maximum available threads
	int threadnum = omp_get_max_threads();
	omp_set_num_threads(threadnum);

	int t = min(MAX_MEM / (m * (sizeof(size_t) + sizeof(elem_t))), n);

	elem_t *D = create_matrix(t, m);
	size_t *ind = (size_t *) malloc(t * m * sizeof(size_t));

	// slice X into parts of size t in order to not use a lot of memory.
	for(size_t X_begin = 0 ; X_begin < n ; X_begin += t) {
		size_t X_end = min(X_begin + t, n);

		size_t slice_size = X_end - X_begin;

		const elem_t *X_slice = MATRIX_ROW(X, X_begin, n, d);

		// calculate the SEDM for X_slice and Y
		sedm(X_slice, slice_size, Y, m, d, D);

		// in parallel for all rows in the slice perform knn
		#pragma omp parallel for
		for(int tid = 0 ; tid < slice_size ; tid++) {
			elem_t *Di = MATRIX_ROW(D, tid, t, m);
			size_t *ind_i = MATRIX_ROW(ind, tid, t, m);

			gen_indices(ind_i, m);

			// using QuickSelect to find the k smallest elements in Di
			qselect(k, Di, ind_i, m);

			// sorting the first k elements in Di using BubbleSort
			for(size_t i = 0 ; i < k - 1 ; i++) {
				for(size_t j = 0 ; j < k - i - 1 ; j++) {
					if(Di[j] > Di[j + 1]) {
						SWAP(Di[j], Di[j + 1]);
						SWAP(ind_i[j], ind_i[j + 1]);
					}
				}
			}

			// store the results in res
			for(size_t j = 0 ; j < k ; j++) {
				MATRIX_ELEM(res->n_dist, X_begin + tid, j, n, k) = Di[j];
				MATRIX_ELEM(res->n_idx, X_begin + tid, j, n, k) = ind_i[j];
			}
		}
	}

	delete_matrix(D);
	free(ind);

	return res;
}

void delete_knn(knn_result *knn) {
	free(knn->n_idx);
	free(knn->n_dist);

	free(knn);
}
