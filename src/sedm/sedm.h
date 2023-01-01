#ifndef SEDM_H
#define SEDM_H

#define sedm(X, n, Y, m, d, D) (sedm_simp((X), (n), (Y), (m), (d), (D)))

void sedm_comp(float *X, int n, float *Y, int m, int d, float *distance_matrix);
void sedm_simp(float *X, int n, float *Y, int m, int d, float *distance_matrix);

#endif
