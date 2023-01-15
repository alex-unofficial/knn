/*  qselect.h - function declarations for qselect
 *  Copyright (C) 2023  Alexandros Athanasiadis
 *
 *  This file is part of knn
 *
 *  knn is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  knn is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef QSELECT_H
#define QSELECT_H

#include "def.h"

/* generates an array of indices starting at begin_idx 
 * of size size and stores it in ind */
void gen_indices(size_t *ind, size_t begin_idx, size_t size);

/* qselect selects the k-th smallest element in the array */
elem_t qselect(size_t k, elem_t *arr, size_t *ind, size_t size);

#endif
