#ifndef SEDM_H
#define SEDM_H

#include "def.h"

#define sedm(X, n, Y, m, d, D) (sedm_simp((X), (n), (Y), (m), (d), (D)))

void sedm_comp(const elem_t *X, size_t n, const elem_t *Y, size_t m, size_t d, elem_t *distance_matrix);
void sedm_simp(const elem_t *X, size_t n, const elem_t *Y, size_t m, size_t d, elem_t *distance_matrix);

#endif
