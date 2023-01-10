#ifndef DEF_H
#define DEF_H

#include <stdint.h>
#include <limits.h>

#include <immintrin.h>

typedef float elem_t;

#define MPI_ELEM_T MPI_FLOAT

#if SIZE_MAX == UCHAR_MAX
   #define MPI_SIZE_T MPI_UNSIGNED_CHAR
#elif SIZE_MAX == USHRT_MAX
   #define MPI_SIZE_T MPI_UNSIGNED_SHORT
#elif SIZE_MAX == UINT_MAX
   #define MPI_SIZE_T MPI_UNSIGNED
#elif SIZE_MAX == ULONG_MAX
   #define MPI_SIZE_T MPI_UNSIGNED_LONG
#elif SIZE_MAX == ULLONG_MAX
   #define MPI_SIZE_T MPI_UNSIGNED_LONG_LONG
#endif

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
