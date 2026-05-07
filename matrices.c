#include "matrices.h"

matrix_t *matrix_make(int m, int n) {
	if (m < 1 || n < 1)
		return NULL;
	matrix_t *matrix = malloc(sizeof(matrix_t));
	matrix->m = m;
	matrix->n = n;
	matrix->data = malloc(m * n * sizeof(double));
	return matrix;
}

void matrix_free(matrix_t *m) {
	free(m->data);
	free(m);
}

void matrix_set(matrix_t *m, int i, int j, double data) {
	m->data[i * m->n + j] = data;
}

double matrix_get(matrix_t *m, int i, int j) {
	return m->data[i * m->n + j];
}

matrix_t *matrix_copy(matrix_t *m) {
	matrix_t *m2 = matrix_make(m->m, m->n);
	for (int i = 0; i < m->m * m->n; i++) {
		m2->data[i] = m->data[i];
	}
	return m2;
}

matrix_t *matrix_add(matrix_t *m1, matrix_t *m2) {
	if (m1->m != m2->m || m1->n != m2->n)
		return NULL;
	matrix_t *m3 = matrix_make(m1->m, m1->n);
	for (int i = 0; i < m1->m * m1->n; i++) {
		m3->data[i] = m1->data[i] + m2->data[i];
	}
	return m3;
}

matrix_t *matrix_scalar_multiply(matrix_t *m, double scalar) {
	matrix_t *m2 = matrix_make(m->m, m->n);
	for (int i = 0; i < m->m * m->n; i++) {
		m2->data[i] = m->data[i] * scalar;
	}
	return m2;
}

matrix_t *matrix_transpose(matrix_t *m) {
	matrix_t *m2 = matrix_make(m->n, m->m);
	for (int i = 0; i < m->m; i++) {
		for (int j = 0; j < m->n; j++) {
			matrix_set(m2, j, i, matrix_get(m, i, j));
		}
	}
	return m2;
}

matrix_t *matrix_multiply(matrix_t *m1, matrix_t *m2) {
	if (m1->n != m2->m)
		return NULL;
	matrix_t *m3 = matrix_make(m1->m, m2->n);
	for (int i = 0; i < m3->m; i++) {
		for (int j = 0; j < m3->n; j++) {
			double sum = 0;
			for (int k = 0; k < m1->n; k++) {
				sum += matrix_get(m1, i, k) * matrix_get(m2, k, j);
			}
			matrix_set(m3, i, j, sum);
		}
	}
	return m3;
}

matrix_t *matrix_submatrix(matrix_t *m, int row_skip, int column_skip) {
	if (row_skip > m->m || column_skip > m->n)
		return NULL;
	matrix_t *m2 = matrix_make(m->m - 1, m->n - 1);
	if (m2 == NULL)
		return NULL;
	int i_offset = 0;
	for (int i = 0; i < m->m; i++) {
		if (i == row_skip) {
			i_offset++;
			continue;
		}
		int j_offset = 0;
		for (int j = 0; j < m->n; j++) {
			if (j == column_skip) {
				j_offset++;
				continue;
			}
			matrix_set(m2, (i-i_offset), j-j_offset, matrix_get(m, i, j));
		}
	}
	return m2;
}

int matrix_cofactor(matrix_t *m, int row, int column, double *cofactor) {
	if (m->m != m->n)
		return 1;
	matrix_t *sub_m = matrix_submatrix(m, row, column);
	double minor;
	matrix_determinant(sub_m, &minor);
	matrix_free(sub_m);
	*cofactor = minor * pow(-1, row + column);
	return 0;
}

int matrix_determinant(matrix_t *m, double *det) {
	// M has to be a square matrix
	if (m->m != m->n)
		return 1;
	if (m->m == 2) {
		*det = matrix_get(m, 0, 0) * matrix_get(m, 1, 1)
			- matrix_get(m, 1, 0) * matrix_get(m, 0, 1);
		return 0;
	}
	if (m->m == 1) {
		*det = matrix_get(m, 0, 0);
		return 0;
	}
	double res = 0;
	for (int i = 0; i < m->n; i++) {
		double cofactor;
		matrix_cofactor(m, 0, i, &cofactor);
		res += matrix_get(m, 0, i) * cofactor;
	}
	*det = res;
	return 0;
}

matrix_t *matrix_inverse(matrix_t *m) {
	// M has to be a square matrix
	if (m->m != m->n)
		return NULL;
	matrix_t *adjugate = matrix_make(m->m, m->n);
	for (int i = 0; i < m->m; i++) {
		for (int j = 0; j < m->n; j++) {
			double cofactor;
			matrix_cofactor(m, j, i, &cofactor);
			matrix_set(adjugate, i, j, cofactor);
		}
	}
	double determinant;
	matrix_determinant(m, &determinant);
	if (determinant == 0) {
		matrix_free(adjugate);
		return NULL;
	}
	matrix_t *inverse = matrix_scalar_multiply(adjugate, 1 / determinant);
	matrix_free(adjugate);
	return inverse;
}
