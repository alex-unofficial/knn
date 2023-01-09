#ifndef KNN_H
#define KNN_H

#include "def.h"

typedef struct knn_result {
	size_t *n_idx;
	elem_t *n_dist;

	size_t m;
	size_t k;
} knn_result;

knn_result *knn(const elem_t *X, size_t n, const elem_t *Y, size_t Y_begin, size_t m, int d, int k, int t);
void delete_knn(knn_result *k);

#endif
