#ifndef MATRICES_H
#define MATRICES_H

#include <stdlib.h>
#include <math.h>

typedef struct {
	int m;
	int n;
	double *data;
} matrix_t;

matrix_t *matrix_make(int m, int n);
void matrix_free(matrix_t *m);
void matrix_set(matrix_t *m, int i, int j, double data);
double matrix_get(matrix_t *m, int i, int j);
matrix_t *matrix_copy(matrix_t *m);
matrix_t *matrix_add(matrix_t *m1, matrix_t *m2);
matrix_t *matrix_scalar_multiply(matrix_t *m, double scalar);
matrix_t *matrix_transpose(matrix_t *m);
matrix_t *matrix_multiply(matrix_t *m1, matrix_t *m2);
matrix_t *matrix_submatrix(matrix_t *m, int row_skip, int column_skip);
int matrix_determinant(matrix_t *m, double *det);
matrix_t *matrix_inverse(matrix_t *m);

#endif
