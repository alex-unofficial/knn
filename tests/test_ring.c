#include <mpi.h>
#include <stdio.h>
#include <math.h>

#include "def.h"

int main(int argc, char** argv) {
	
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    int p, r;
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &r);

	if(r == 0) printf("========== testing ring =========\n");

	int N = 1000;
	int n_max = (int)ceil((float)N / (float)p);
	
	int *X = (int *) malloc(n_max * sizeof(int));
	int *Y = (int *) malloc(n_max * sizeof(int));

	int i_begin = r * N / p;
	int i_end = min((r + 1) * N / p, N);

	int n = i_end - i_begin;

	// initializing X
	for(int i = 0 ; i < n ; i++) {
		X[i] = i_begin + i;
	}

	int r_send = mod(r - 1, p);
	int r_recv = mod(r + 1, p);

	if(r == 0) printf("checking for errors:\n");
	MPI_Barrier(MPI_COMM_WORLD);

	int errors = 0;
	int idx = 0;
	for(int k = 0 ; k < p ; k++) {
		MPI_Request send_req, recv_req;
		MPI_Isend(X, n, MPI_INT, r_send, 0, MPI_COMM_WORLD, &send_req);
		MPI_Irecv(Y, n_max, MPI_INT, r_recv, 0, MPI_COMM_WORLD, &recv_req);

		// Doing work
		for(int i = 0 ; i < n ; i++) {
			if(mod(X[i] - i_begin, N) != idx) {
				printf("%d >> error %3d: wrong value for k = %d, at X[%d] = %2d\n", r, ++errors, k, i, X[i]);
			}

			idx++;
		}

		MPI_Status send_status, recv_status;
		MPI_Wait(&send_req, &send_status);
		MPI_Wait(&recv_req, &recv_status);

		MPI_Get_count(&recv_status, MPI_INT, &n);
		SWAP(X, Y);
	}

	if(idx != N)
		printf("%d >> error %3d: did not read all values, final idx = %d\n", r, ++errors, idx);

	int total_errors;
	MPI_Reduce(&errors, &total_errors, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if(r == 0) printf("found %d errors\n", total_errors);

    // Finalize the MPI environment.
    MPI_Finalize();
}
