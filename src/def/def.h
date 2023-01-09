#ifndef DEF_H
#define DEF_H

#include <immintrin.h>

typedef float elem_t;
#define MPI_ELEM MPI_FLOAT

#define VEC_T __m256
#define VEC_SIZE 8
#define VEC_ALIGN 32

#define VEC_ZERO _mm256_setzero_ps
#define VEC_LOAD _mm256_load_ps
#define VEC_FMADD _mm256_fmadd_ps
#define VEC_SUB _mm256_sub_ps

#define VEC_SUM(v) ((v)[0] + (v)[1] + (v)[2] + (v)[3] + (v)[4] + (v)[5] + (v)[6] + (v)[7])

#define min(x, y) (((x) < (y)) ? (x) : (y))
#define max(x, y) (((x) > (y)) ? (x) : (y))

#define SWAP(a, b) {typeof(a) temp = (a); (a) = (b); (b) = temp;}

#define mod(x, N) (((x) < 0) ? ((x) % (N) + (N)) : ((x) % (N)))

#endif
