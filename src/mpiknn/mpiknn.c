#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <mpi.h>
#include <omp.h>

#include "knn.h"
#include "matrix.h"
#include "def.h"

#define ROOT 0

#define INIT_MATRIX 1
#define KNN 2
#define PRINTING 3

int read_matrix(FILE *file, elem_t *X, size_t n, size_t d) {
	for(size_t i = 0 ; i < n ; i++) {
		for(size_t j = 0 ; j < d ; j++) {
			int k = fscanf(file, "%f", &MATRIX_ELEM(X, i, j, n, d));

			if(!k && feof(file)) {
				return 1;
			}
		}
	}

	return 0;
}

int main(int argc, char **argv) {

	MPI_Init(&argc, &argv);

	int n_processes, rank;
	MPI_Comm_size(MPI_COMM_WORLD, &n_processes);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int error = 0;
	if(rank == ROOT) {
		if(argc < 5) {
			error = 1;
			fprintf(stderr, "not enough arguments.\nusage: %s file.dat k N d\n", argv[0]);
		}

		MPI_Bcast(&error, 1, MPI_INT, ROOT, MPI_COMM_WORLD);
	}

	if(error) {
		MPI_Finalize();
		exit(error);
	}

	/* initialize variables */

	char *filename = argv[1];
	size_t k = atoi(argv[2]);
	size_t N = atoi(argv[3]);
	size_t d = atoi(argv[4]);
	
	size_t n_max = (size_t)ceil((double)N/(double)n_processes);

	int r_send = mod(rank - 1, n_processes);
	int r_recv = mod(rank + 1, n_processes);

	elem_t *X = create_matrix(n_max, d);
	elem_t *Y = create_matrix(n_max, d);
	elem_t *Z = create_matrix(n_max, d);

	int i_begin = rank * N / n_processes;
	int i_end = min((rank + 1) * N / n_processes, N);

	int n = i_end - i_begin;

	size_t t = 100; // TODO: change this
	
	/* initialize the matrices */
	
	int recv_size = 0;
	MPI_Request send_req = MPI_REQUEST_NULL, recv_req;
	MPI_Status send_status, recv_status;

	switch(rank) {
		case ROOT:
			/* root process reads from filename and sends 
			 * data in the ring to be distrubuted */
			FILE *file = fopen(filename, "r");

			for(int r = 0 ; r < n_processes ; r++) {
				int r_begin = r * N / n_processes;
				int r_end = min((r + 1) * N / n_processes, N);

				int r_n = r_end - r_begin;

				read_matrix(file, X, r_n, d);

				MPI_Wait(&send_req, &send_status);
				MPI_Isend(X, r_n * d, MPI_ELEM, r_send, INIT_MATRIX, MPI_COMM_WORLD, &send_req);

				SWAP(X, Y);

			}

			fclose(file);

			MPI_Recv(X, n_max * d, MPI_ELEM, r_recv, INIT_MATRIX, MPI_COMM_WORLD, &recv_status);
			MPI_Get_count(&recv_status, MPI_ELEM, &recv_size);

			break;

		default:
			/* other processes pass on the received data until
			 * they recieve their own */
			for(int r = 0 ; r < rank ; r++) {
				MPI_Recv(X, n_max * d, MPI_ELEM, r_recv, INIT_MATRIX, MPI_COMM_WORLD, &recv_status);
				MPI_Get_count(&recv_status, MPI_ELEM, &recv_size);

				MPI_Wait(&send_req, &send_status);
				MPI_Isend(X, recv_size, MPI_ELEM, r_send, INIT_MATRIX, MPI_COMM_WORLD, &send_req);
				
				SWAP(X, Y);
			}

			MPI_Recv(X, n_max * d, MPI_ELEM, r_recv, INIT_MATRIX, MPI_COMM_WORLD, &recv_status);
			MPI_Get_count(&recv_status, MPI_ELEM, &recv_size);

			break;
	}

	error = 0;
	if(recv_size != n * d) {
		error = 1;
		MPI_Bcast(&error, 1, MPI_INT, rank, MPI_COMM_WORLD);
	}

	MPI_Barrier(MPI_COMM_WORLD);

	if(error) {
		if(rank == ROOT) fprintf(stderr, "recieved size different than expected.\n");
		MPI_Finalize();
		exit(error);
	}

	/* also initialize Y = X */
	int m = n;

	#pragma omp parallel for simd
	for(size_t i = 0 ; i < m * d ; i++) {
		Y[i] = X[i];
	}

	MPI_Barrier(MPI_COMM_WORLD);

	/* perform knn */
	knn_result *res = NULL;

	for(int q = 0 ; q < n_processes ; q++) {
		/* initiate non-blocking send and recieve */
		MPI_Isend(Y, m * d, MPI_ELEM, r_send, KNN, MPI_COMM_WORLD, &send_req);
		MPI_Irecv(Z, n_max * d, MPI_ELEM, r_recv, KNN, MPI_COMM_WORLD, &recv_req);

		/* calculate the appropriate Y_idx */
		int r = mod(rank + q, n_processes);
		size_t Y_idx = r * N / n_processes;

		/* perform the knn operation */
		res = knn(X, n, Y, Y_idx, m, d, k, t, &res);

		/* wait for send and recieve to complete */
		MPI_Wait(&send_req, &send_status);
		MPI_Wait(&recv_req, &recv_status);

		MPI_Get_count(&recv_status, MPI_ELEM, &recv_size);
		m = recv_size / d;

		/* swap Y and Z pointer locations*/
		SWAP(Y, Z);
	}

	MPI_Barrier(MPI_COMM_WORLD);

	/* free matrices */
	delete_matrix(X);
	delete_matrix(Y);
	delete_matrix(Z);

	/* create arrays to be used for printing */
	elem_t *dists = (elem_t *) malloc(n_max * k * sizeof(elem_t));
	elem_t *swp = (elem_t *) malloc(n_max * k * sizeof(elem_t));

	#pragma omp parallel for simd
	for(size_t i = 0 ; i < n * k ; i++) {
		dists[i] = res->n_dist[i];
	}

	/* free knn results */
	delete_knn(res);

	/* print results */
	switch(rank) {
		case ROOT:
			/* root process recieves the results of the other 
			 * processes and prints them one by one */
			for(int r = 0 ; r < n_processes - 1 ; r++) {
				MPI_Irecv(swp, n_max * k, MPI_ELEM, r_recv, PRINTING, MPI_COMM_WORLD, &recv_req);

				for(size_t i = 0 ; i < n ; i++) {
					for(size_t j = 0 ; j < k ; j++) {
						printf("%0.2f ", MATRIX_ELEM(dists, i, j, n, k));
					}
					printf("\n");
				}

				MPI_Wait(&recv_req, &recv_status);

				MPI_Get_count(&recv_status, MPI_ELEM, &recv_size);
				n = recv_size / k;

				SWAP(dists, swp);
			}

			for(size_t i = 0 ; i < n ; i++) {
				for(size_t j = 0 ; j < k ; j++) {
					printf("%0.2f ", MATRIX_ELEM(dists, i, j, n, k));
				}
				printf("\n");
			}

			break;

		default:
			/* other processes send their data to the next process
			 * then pass on the data from all previous processes */
			send_req = MPI_REQUEST_NULL;
			for(int r = 0 ; r < n_processes - rank - 1 ; r++) {
				MPI_Wait(&send_req, &send_status);
				MPI_Isend(dists, n * k, MPI_ELEM, r_send, PRINTING, MPI_COMM_WORLD, &send_req);

				MPI_Recv(swp, n_max * k, MPI_ELEM, r_recv, PRINTING, MPI_COMM_WORLD, &recv_status);

				MPI_Get_count(&recv_status, MPI_ELEM, &recv_size);
				n = recv_size / k;

				SWAP(dists, swp);
			}

			MPI_Wait(&send_req, &send_status);
			MPI_Send(dists, n * k, MPI_ELEM, r_send, PRINTING, MPI_COMM_WORLD);

			break;
	}

	MPI_Barrier(MPI_COMM_WORLD);

	/* free printing arrays */
	free(dists);
	free(swp);

	MPI_Finalize();
}
