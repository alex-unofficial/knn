#ifndef KNN_H
#define KNN_H

#include "def.h"

/* knn result struct.
 *
 * knn_result is the return type of the knn function.
 * for a set of input points it stores the index of, and the distances to 
 * the k-nearest neighbours of each point.
 *
 * parameters:
 * m (size_t): the number of elements for which knn was computed
 * k (size_t): the number of nearest neighbours that were found for each neighbour
 *
 * arrays:
 * n_idx (size_t *): array of size m * k.
 *                   each row contains the indices of the k nearest 
 *                   neighbours of the corresponding input element.
 *
 * n_dist (size_t *): array of size m * k.
 *                    each row contains the distances to the k nearest 
 *                    neighbours of the corresponding input element.
 */
typedef struct knn_result {
	size_t *n_idx;
	elem_t *n_dist;

	size_t m;
	size_t k;
} knn_result;

/* knn computes the K nearest neighbours in Y of each element in X */
knn_result *knn(
	const elem_t *X, size_t n, const elem_t *Y, size_t Y_begin, size_t m, 
	size_t d, size_t k, size_t t, knn_result **prev_result);

/* frees the memory for a knn_result struct */
void delete_knn(knn_result *k);

#endif
