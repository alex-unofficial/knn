/* sedm.h - function declarations for the SEDM methods
 * Copyright (C) 2023  Alexandros Athanasiadis
 *
 * This file is part of knn
 *
 * knn is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * knn is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
