#ifndef SEDM_H
#define SEDM_H

#include "def.h"

/* calls one of the input methods below */
#define sedm(X, n, Y, m, d, D) (sedm_simp((X), (n), (Y), (m), (d), (D)))

/* sedm_comp computes the SEDM between the input matrices X and Y
 * using a compound method and CBLAS libraries. */
void sedm_comp(const elem_t *X, size_t n, const elem_t *Y, size_t m, size_t d, elem_t *distance_matrix);

/* sedm_simp computes the SEDM between the input matrices X and Y using a simple method. */
void sedm_simp(const elem_t *X, size_t n, const elem_t *Y, size_t m, size_t d, elem_t *distance_matrix);

#endif
