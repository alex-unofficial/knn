#ifndef SIMD_H
#define SIMD_H

#include <immintrin.h>

typedef float elem_t;

#define VEC_T __m256
#define VEC_SIZE 8
#define VEC_ALIGN 32

#define VEC_ZERO _mm256_setzero_ps
#define VEC_LOAD _mm256_load_ps
#define VEC_FMADD _mm256_fmadd_ps
#define VEC_SUB _mm256_sub_ps

#endif
