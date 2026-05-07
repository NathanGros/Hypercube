#include "shapes.h"

graph4_t *make_cube(vector4_t origin, Color color) {
	graph4_t *cube = graph4_make(16);
	for (int i = 0; i < 16; i++) {
		cube->vertices[i] = (vector4_t) {
			origin.w + (i >> 3) % 2 - 0.5,
			origin.x + (i >> 2) % 2 - 0.5,
			origin.y + (i >> 1) % 2 - 0.5,
			origin.z + i % 2 - 0.5,
		};
	}
	for (int i = 0; i < 16; i++) {
		cube->colors[i] = color;
	}
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 4; j++) {
			int n = i;
			if ((n >> j) % 2 == 1)
				n -= 1 << j;
			else
				n += 1 << j;
			cube->adj_mat[i * 16 + n] = 1;
		}
	}
	return cube;
}

graph4_t *make_pyramid(vector4_t origin, Color color) {
	graph4_t *pyramid = graph4_make(5);
	pyramid->vertices[0] = vector4_add((vector4_t) {0.0, 0.0, 0.0, 0.0}, origin);
	pyramid->vertices[1] = vector4_add((vector4_t) {1.0, 0.0, 0.0, 0.0}, origin);
	pyramid->vertices[2] = vector4_add((vector4_t) {0.5, 0.866025, 0.0, 0.0}, origin);
	pyramid->vertices[3] = vector4_add((vector4_t) {0.5, 0.288675, 0.816497, 0.0}, origin);
	pyramid->vertices[4] = vector4_add((vector4_t) {0.5, 0.288675, 0.204124, 0.774597}, origin);
	for (int i = 0; i < 5; i++) {
		pyramid->colors[i] = color;
	}
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (i == j)
				continue;
			pyramid->adj_mat[i * 5 + j] = 1;
		}
	}
	return pyramid;
}
