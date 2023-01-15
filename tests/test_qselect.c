/* test_qselect - test for the qselect method for various input arrays
 * Copyright (C) 2023  Alexandros Athanasiadis
 *
 * This file is part of knn
 *
 * knn is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * knn is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>

#include "qselect.h"

#include "def.h"

int main(int argc, char **argv) {

	printf("======== testing qselect ========\n");

	printf("array_1 = [1..N]\n");
	printf("\n");

	int N = 10000;
	int k = 150;

	printf("N = %d\nk = %d\n", N, k);
	printf("\n");

	elem_t arr[N];
	size_t ind[N];

	for(int i = 0 ; i < N ; i++) arr[i] = (elem_t) (i + 1);
	gen_indices(ind, 0, N);

	for(int i = 0 ; i < N ; i++) {
		int j = rand() % N;
		SWAP(arr[i], arr[j]);
		SWAP(ind[i], ind[j]);
	}

	elem_t r = qselect(k, arr, ind, N);

	printf("checking for errors:\n");

	int errors1 = 0;

	if(r != (elem_t) k) 
		printf("%d: wrong k-th smallest element: %0.1f returned vs %0.1f true\n", ++errors1, r, (elem_t) k);

	// using BubbleSort to sort the k first elements
	for (int i = 0; i < k - 1; i++)
		for (int j = 0; j < k - i - 1; j++)
			if (arr[j] > arr[j + 1]) {
				SWAP(arr[j], arr[j + 1]);
				SWAP(ind[j], ind[j + 1]);
			}

	for(int i = 0 ; i < k ; i++) {
		if(arr[i] != (elem_t) (i + 1))
			printf("%d: error arr[%d] = %0.1f != %0.1f\n", ++errors1, i, arr[i], (elem_t) (i + 1));
	}
	
	printf("found %d errors\n", errors1);
	printf("\n");
	printf("---------------------------------\n");
	
	printf("array_2 = [1, 2, 3, 3, 4, 5, 6]\n");
	printf("\n");

	N = 7;
	k = 4;

	printf("N = %d\nk = %d\n", N, k);
	printf("\n");

	elem_t arr2[] = {1.0, 2.0, 3.0, 3.0, 4.0, 5.0, 6.0};
	size_t ind2[N];

	gen_indices(ind2, 0, N);

	for(int i = 0 ; i < N ; i++) {
		int j = rand() % N;
		SWAP(arr2[i], arr2[j]);
		SWAP(ind2[i], ind2[j]);
	}

	r = qselect(k, arr2, ind2, N);

	printf("checking for errors:\n");
	int errors2 = 0;

	if(r != 3.0) 
		printf("%d: wrong k-th smallest element: %0.1f returned vs %0.1f true\n", ++errors2, r, 3.0);

	for (int i = 0; i < k - 1; i++)
		for (int j = 0; j < k - i - 1; j++)
			if (arr2[j] > arr2[j + 1]) {
				SWAP(arr2[j], arr2[j + 1]);
				SWAP(ind2[j], ind2[j + 1]);
			}


	if(arr2[0] != 1.0)
		printf("%d: error arr[%d] = %0.1f != %0.1f\n", ++errors2, 0, arr2[0], 1.0);
	if(arr2[1] != 2.0)
		printf("%d: error arr[%d] = %0.1f != %0.1f\n", ++errors2, 1, arr2[1], 2.0);
	if(arr2[2] != 3.0)
		printf("%d: error arr[%d] = %0.1f != %0.1f\n", ++errors2, 2, arr2[2], 3.0);
	if(arr2[3] != 3.0)
		printf("%d: error arr[%d] = %0.1f != %0.1f\n", ++errors2, 3, arr2[3], 3.0);
	
	printf("found %d errors\n", errors2);
	printf("\n");

	printf("---------------------------------\n");

	printf("array_3 = [1, 2, 3, 3, 4, 5, 6]\n");
	printf("\n");

	N = 7;
	k = 3;

	printf("N = %d\nk = %d\n", N, k);
	printf("\n");

	for(int i = 0 ; i < N ; i++) {
		int j = rand() % N;
		SWAP(arr2[i], arr2[j]);
		SWAP(ind2[i], ind2[j]);
	}

	r = qselect(k, arr2, ind2, N);

	printf("checking for errors:\n");
	int errors3 = 0;

	if(r != 3.0) 
		printf("%d: wrong k-th smallest element: %0.1f returned vs %0.1f true\n", ++errors3, r, 3.0);

	for (int i = 0; i < k - 1; i++)
		for (int j = 0; j < k - i - 1; j++)
			if (arr2[j] > arr2[j + 1]) {
				SWAP(arr2[j], arr2[j + 1]);
				SWAP(ind2[j], ind2[j + 1]);
			}

	if(arr2[0] != 1.0)
		printf("%d: error arr[%d] = %0.1f != %0.1f\n", ++errors3, 0, arr2[0], 1.0);
	if(arr2[1] != 2.0)
		printf("%d: error arr[%d] = %0.1f != %0.1f\n", ++errors3, 1, arr2[1], 2.0);
	if(arr2[2] != 3.0)
		printf("%d: error arr[%d] = %0.1f != %0.1f\n", ++errors3, 2, arr2[2], 3.0);
	
	printf("found %d errors\n", errors3);

	printf("\n");

	if(!errors1 && !errors2 && !errors3)
		return 0;
	else
		return 1 ;
}
