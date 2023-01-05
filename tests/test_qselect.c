#include <stdio.h>
#include <stdlib.h>

#include "qselect.h"

#include "def.h"

int main(int argc, char **argv) {

	printf("===== testing qselect =====\n");

	printf("\n");

	printf("--- array I: [1..N]\n");

	int N = 10000;
	int k = 320;

	printf("N = %d\nk = %d\n", N, k);

	elem_t arr[N];
	size_t ind[N];

	for(int i = 0 ; i < N ; i++) arr[i] = (elem_t) (i + 1);
	gen_indices(ind, N);

	for(int i = 0 ; i < N ; i++) {
		int j = rand() % N;
		SWAP(arr[i], arr[j]);
		SWAP(ind[i], ind[j]);
	}

	elem_t r = qselect(k, arr, ind, N);

	if(r != (elem_t) k) 
		printf("wrong k-th smallest element: %0.1f returned vs %0.1f true\n", r, (elem_t) k);
	else
		printf("correctly returned the k-th smallest element\n");

	for (int i = 0; i < k - 1; i++)
		for (int j = 0; j < k - i - 1; j++)
			if (arr[j] > arr[j + 1]) {
				SWAP(arr[j], arr[j + 1]);
				SWAP(ind[j], ind[j + 1]);
			}

	printf("checking for errors:\n");

	int errors = 0;
	for(int i = 0 ; i < k ; i++) {
		if(arr[i] != (elem_t) (i + 1))
			printf("%d: error arr[%d] = %0.1f != %0.1f\n", ++errors, i, arr[i], (elem_t) (i + 1));
	}
	
	printf("found %d errors\n", errors);
	
	printf("\n");

	printf("--- array II: [1, 2, 3, 3, 4, 5, 6]\n");

	N = 7;
	k = 4;

	printf("N = %d\nk = %d\n", N, k);

	elem_t arr2[] = {1.0, 2.0, 3.0, 3.0, 4.0, 5.0, 6.0};
	size_t ind2[N];

	gen_indices(ind2, N);

	for(int i = 0 ; i < N ; i++) {
		int j = rand() % N;
		SWAP(arr2[i], arr2[j]);
		SWAP(ind2[i], ind2[j]);
	}

	r = qselect(k, arr2, ind2, N);

	if(r != 3.0) 
		printf("wrong k-th smallest element: %0.1f returned vs %0.1f true\n", r, 3.0);
	else
		printf("correctly returned the k-th smallest element\n");

	for (int i = 0; i < k - 1; i++)
		for (int j = 0; j < k - i - 1; j++)
			if (arr2[j] > arr2[j + 1]) {
				SWAP(arr2[j], arr2[j + 1]);
				SWAP(ind2[j], ind2[j + 1]);
			}

	printf("checking for errors:\n");

	errors = 0;
	if(arr2[0] != 1.0)
		printf("%d: error arr[%d] = %0.1f != %0.1f\n", ++errors, 0, arr2[0], 1.0);
	if(arr2[1] != 2.0)
		printf("%d: error arr[%d] = %0.1f != %0.1f\n", ++errors, 1, arr2[1], 2.0);
	if(arr2[2] != 3.0)
		printf("%d: error arr[%d] = %0.1f != %0.1f\n", ++errors, 2, arr2[2], 3.0);
	if(arr2[3] != 3.0)
		printf("%d: error arr[%d] = %0.1f != %0.1f\n", ++errors, 3, arr2[3], 3.0);
	
	printf("found %d errors\n", errors);

	printf("\n");

	N = 7;
	k = 3;

	printf("N = %d\nk = %d\n", N, k);

	for(int i = 0 ; i < N ; i++) {
		int j = rand() % N;
		SWAP(arr2[i], arr2[j]);
		SWAP(ind2[i], ind2[j]);
	}

	r = qselect(k, arr2, ind2, N);

	if(r != 3.0) 
		printf("wrong k-th smallest element: %0.1f returned vs %0.1f true\n", r, 3.0);
	else
		printf("correctly returned the k-th smallest element\n");

	for (int i = 0; i < k - 1; i++)
		for (int j = 0; j < k - i - 1; j++)
			if (arr2[j] > arr2[j + 1]) {
				SWAP(arr2[j], arr2[j + 1]);
				SWAP(ind2[j], ind2[j + 1]);
			}

	printf("checking for errors:\n");

	errors = 0;
	if(arr2[0] != 1.0)
		printf("%d: error arr[%d] = %0.1f != %0.1f\n", ++errors, 0, arr2[0], 1.0);
	if(arr2[1] != 2.0)
		printf("%d: error arr[%d] = %0.1f != %0.1f\n", ++errors, 1, arr2[1], 2.0);
	if(arr2[2] != 3.0)
		printf("%d: error arr[%d] = %0.1f != %0.1f\n", ++errors, 2, arr2[2], 3.0);
	
	printf("found %d errors\n", errors);

	printf("\n");

	return 0;
}
