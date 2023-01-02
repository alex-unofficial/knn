#ifndef SEDM_H
#define SEDM_H

#include "matrix.h"

#define sedm(X, Y) (sedm_simp((X), (Y)))

matrix *sedm_comp(const matrix *X, const matrix *Y);
matrix *sedm_simp(const matrix *X, const matrix *Y);

#endif
