#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <time.h>

#include <math.h>

#include <mpi.h>
#include <omp.h>

#include "knn.h"
#include "matrix.h"
#include "def.h"

#define ROOT 0

#define INIT_MATRIX 1
#define KNN 2
#define PRINTING_DIST 3
#define PRINTING_IDX 4

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
	}

	MPI_Bcast(&error, 1, MPI_INT, ROOT, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);

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

	int n_threads = omp_get_max_threads();
	omp_set_num_threads(n_threads);

	elem_t *X = create_matrix(n_max, d);
	elem_t *Y = create_matrix(n_max, d);
	elem_t *Z = create_matrix(n_max, d);

	int i_begin = rank * N / n_processes;
	int i_end = min((rank + 1) * N / n_processes, N);

	int n = i_end - i_begin;

	size_t t = 4096; // TODO: change this
	
	/* initialize the matrices */
	
	int recv_size = 0;
	MPI_Request send_req = MPI_REQUEST_NULL, recv_req;
	MPI_Status send_status, recv_status;

	switch(rank) {
		case ROOT: ;
			FILE *file = fopen(filename, "r");

			error = 0;
			if(file == NULL) {
				error = errno;
			}

			MPI_Bcast(&error, 1, MPI_INT, ROOT, MPI_COMM_WORLD);
			MPI_Barrier(MPI_COMM_WORLD);

			if(error) {
				fprintf(stderr, "%s\n", strerror(error));
				MPI_Finalize();
				exit(error);
			}

			/* root process reads its own data from the input file */
			read_matrix(file, X, n, d);

			/* then it reads from the input file and sends 
			 * data to the corresponding process */
			for(int r = 1 ; r < n_processes ; r++) {
				int r_begin = r * N / n_processes;
				int r_end = min((r + 1) * N / n_processes, N);

				int r_n = r_end - r_begin;

				read_matrix(file, Y, r_n, d);

				MPI_Wait(&send_req, &send_status);
				MPI_Isend(Y, r_n * d, MPI_ELEM_T, r, INIT_MATRIX, MPI_COMM_WORLD, &send_req);

				SWAP(Y, Z);
			}
			fclose(file);

			break;

		default: ;
			MPI_Bcast(&error, 1, MPI_INT, ROOT, MPI_COMM_WORLD);
			MPI_Barrier(MPI_COMM_WORLD);

			if(error) {
				MPI_Finalize();
				exit(error);
			}

			/* other processes receive their data from the root process */
			MPI_Recv(X, n_max * d, MPI_ELEM_T, ROOT, INIT_MATRIX, MPI_COMM_WORLD, &recv_status);

			break;
	}

	/* also initialize Y = X */
	int m = n;

	#pragma omp parallel for simd
	for(size_t i = 0 ; i < m * d ; i++) {
		Y[i] = X[i];
	}
	
	if(rank == ROOT) MPI_Wait(&send_req, &send_status);

	MPI_Barrier(MPI_COMM_WORLD);
	struct timespec t_begin, t_end;
	clock_gettime(CLOCK_MONOTONIC, &t_begin);

	/* perform knn */
	knn_result *res = NULL;

	for(int q = 0 ; q < n_processes ; q++) {
		/* initiate non-blocking send and recieve */
		MPI_Isend(Y, m * d, MPI_ELEM_T, r_send, KNN, MPI_COMM_WORLD, &send_req);
		MPI_Irecv(Z, n_max * d, MPI_ELEM_T, r_recv, KNN, MPI_COMM_WORLD, &recv_req);

		/* calculate the appropriate Y_idx */
		int r = mod(rank + q, n_processes);
		size_t Y_idx = r * N / n_processes;

		/* perform the knn operation */
		res = knn(X, n, Y, Y_idx, m, d, k, t, &res);

		/* wait for send and recieve to complete */
		MPI_Wait(&send_req, &send_status);
		MPI_Wait(&recv_req, &recv_status);

		MPI_Get_count(&recv_status, MPI_ELEM_T, &recv_size);
		m = recv_size / d;

		/* swap Y and Z pointer locations*/
		SWAP(Y, Z);
	}

	MPI_Barrier(MPI_COMM_WORLD);
	clock_gettime(CLOCK_MONOTONIC, &t_end);

	/* free matrices */
	delete_matrix(X);
	delete_matrix(Y);
	delete_matrix(Z);

	/* print results */
	switch(rank) {
		case ROOT: ;
			/* print information */
			double time_elapsed = (t_end.tv_sec - t_begin.tv_sec) + (t_end.tv_nsec - t_begin.tv_nsec) / 1e9f;

			printf("%lu %lu %lu %d %d %lu %lf\n", N, d, k, n_processes, n_threads, n_max, time_elapsed);
			
			/* create arrays to be used for printing */
			elem_t *dists= (elem_t *) malloc(n_max * k * sizeof(elem_t));
			elem_t *dists_swp = (elem_t *) malloc(n_max * k * sizeof(elem_t));

			size_t *idxs= (size_t *) malloc(n_max * k * sizeof(size_t));
			size_t *idxs_swp = (size_t *) malloc(n_max * k * sizeof(size_t));

			#pragma omp parallel for simd
			for(int i = 0 ; i < n * k ; i++) {
				dists[i] = res->n_dist[i];
				idxs[i] = res->n_idx[i];
			}

			MPI_Request recv_dist, recv_idx;
			MPI_Status dist_status, idx_status;

			/* root process recieves the results of the other 
			 * processes and prints them one by one */
			for(int r = 1 ; r < n_processes ; r++) {
				MPI_Irecv(dists_swp, n_max * k, MPI_ELEM_T, r, PRINTING_DIST, MPI_COMM_WORLD, &recv_dist);
				MPI_Irecv(idxs_swp, n_max * k, MPI_SIZE_T, r, PRINTING_IDX, MPI_COMM_WORLD, &recv_idx);

				for(size_t i = 0 ; i < n ; i++) {
					for(size_t j = 0 ; j < k ; j++) {
						printf("%lu:%0.2f ", 
								MATRIX_ELEM(idxs, i, j, n, k), MATRIX_ELEM(dists, i, j, n, k));
					}
					printf("\n");
				}

				MPI_Wait(&recv_dist, &dist_status);
				MPI_Wait(&recv_idx, &idx_status);

				MPI_Get_count(&dist_status, MPI_ELEM_T, &recv_size);

				n = recv_size / k;

				SWAP(dists, dists_swp);
				SWAP(idxs, idxs_swp);
				
			} for(size_t i = 0 ; i < n ; i++) {
				for(size_t j = 0 ; j < k ; j++) {
					printf("%lu:%0.2f ", 
							MATRIX_ELEM(idxs, i, j, n, k), MATRIX_ELEM(dists, i, j, n, k));
				}
				printf("\n");
			}

			/* free printing arrays */
			free(dists_swp);
			free(idxs_swp);

			break;

		default: ;
			/* other processes send their data to the root process */
			MPI_Isend(res->n_dist, n * k, MPI_ELEM_T, ROOT, PRINTING_DIST, MPI_COMM_WORLD, &send_req);
			MPI_Send(res->n_idx, n * k, MPI_SIZE_T, ROOT, PRINTING_IDX, MPI_COMM_WORLD);
			MPI_Wait(&send_req, &send_status);

			break;
	}

	MPI_Barrier(MPI_COMM_WORLD);

	/* free knn results */
	delete_knn(res);

	MPI_Finalize();
}
