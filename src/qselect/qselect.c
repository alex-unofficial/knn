/*  qselect.c - implementation of quickselect algorithm
 *  Copyright (C) 2023  Alexandros Athanasiadis
 *
 *  This file is part of knn
 *
 *  knn is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  knn is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "qselect.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* qselect selects the k-th smallest element in the array
 *
 * inputs:
 * k (size_t): index of the array
 * arr (elem_t *): the input array. violatile
 * ind (size_t *): the indexes of the array. violatile
 * size (size_t): the size of the input array
 *
 * returns:
 * the k-th smallest element in the array
 *
 * at the end of the process the k-th smallest element
 * will be at index k in the array and all elements smaller
 * than it will be to the left of it, while larger ones will
 * be to the right. the ind array will change along with the
 * input array to correspond with the changing indexes.
 */
elem_t qselect(size_t k, elem_t *arr, size_t *ind, size_t size) {

	/* initialize the start and end of the array */
	size_t start_idx = 0, end_idx = size;

	/* this will run until it find the k smallest element */
	while(true) {
		/* if start = end the k-th smallest element is arr[start] */
		if(start_idx == end_idx - 1) 
			return arr[start_idx];

		/* select a random pivot to minize chance of worst-case complexity */
		size_t pivot_idx = start_idx + rand() % (end_idx - start_idx);
		elem_t pivot_val = arr[pivot_idx];

		/* move the pivot to the end */
		SWAP(arr[pivot_idx], arr[end_idx - 1]);
		SWAP(ind[pivot_idx], ind[end_idx - 1]);

		/* procedure to move all elements smaller than the pivot to its left
		 * and all elements larger than the pivot to its right*/
		size_t store_idx = start_idx;
		for(size_t i = start_idx ; i < end_idx - 1 ; i++) {
			if(arr[i] <= pivot_val) {
				SWAP(arr[store_idx], arr[i]);
				SWAP(ind[store_idx], ind[i]);
				store_idx += 1;
			}
		}

		/* move pivot to the correct position */
		SWAP(arr[end_idx - 1], arr[store_idx]);
		SWAP(ind[end_idx - 1], ind[store_idx]);

		pivot_idx = store_idx;

		/* if the new pivot position is k then the pivot is the k-th smallest element */
		if(k == pivot_idx + 1) 
			return arr[k - 1];
		/* if the new pivot position is larger than k then repeat for the elements left of the pivot */
		else if(k < pivot_idx + 1) 
			end_idx = pivot_idx;
		/* if the new pivot position is smaller than k then repeat for the elements right of the pivot */
		else 
			start_idx = pivot_idx + 1;
	}
}

/* generates an array of indices starting at begin_idx 
 * of size size and stores it in ind */
void gen_indices(size_t *ind, size_t begin_idx, size_t size) {
	for(size_t i = 0 ; i < size ; i++) ind[i] = begin_idx + i;
}
