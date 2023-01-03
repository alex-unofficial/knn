#include "qselect.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

elem_t qselect(size_t k, elem_t *arr, size_t size) {

	size_t start_idx = 0, end_idx = size;

	while(true) {
		if(start_idx == end_idx - 1) 
			return arr[start_idx];

		size_t pivot_idx = start_idx + rand() % (end_idx - start_idx);
		elem_t pivot_val = arr[pivot_idx];

		SWAP(arr[pivot_idx], arr[end_idx - 1]);

		size_t store_idx = start_idx;

		for(size_t i = start_idx ; i < end_idx - 1 ; i++) {
			if(arr[i] <= pivot_val) {
				SWAP(arr[store_idx], arr[i]);
				store_idx += 1;
			}
		}

		SWAP(arr[end_idx - 1], arr[store_idx]);

		pivot_idx = store_idx;

		if(k == pivot_idx + 1) 
			return arr[k - 1];
		else if(k < pivot_idx + 1) 
			end_idx = pivot_idx;
		else 
			start_idx = pivot_idx + 1;
	}
}
