#include "shapes.h"

graph4_t *make_cube(vector4_t origin, Color color) {
	int nb_vertices = 16; // A 4D cube has 16 vertices
	graph4_t *cube = graph4_make(16);

	// Adding the 16 vertices with their coordinates in a smart way
	for (int i = 0; i < 16; i++) {
		cube->vertices[i] = (vector4_t) {
			origin.w + (i >> 3) % 2 - 0.5,
			origin.x + (i >> 2) % 2 - 0.5,
			origin.y + (i >> 1) % 2 - 0.5,
			origin.z + i % 2 - 0.5,
		};
	}

	/* Adjacency matrix of the vertices.
	If there is a 1 on line 0 column 2, it means there is an edge between vertex 0 and vertex 2.
	If there is a zero then there is no edge.
	This matrix has to be symmetrical. */
	/* // Old way. Now hardcoded to better understand what's going on.
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 4; j++) {
			int n = i;
			if ((n >> j) % 2 == 1)
				n -= 1 << j;
			else
				n += 1 << j;
			cube->adj_mat[i * 16 + n] = 1;
		}
	} */
	int adj_mat[16 * 16] = {
/*      0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 */
/* 0 */ 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
/* 1 */ 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
/* 2 */ 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0,
/* 3 */ 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0,
/* 4 */ 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0,
/* 5 */ 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0,
/* 6 */ 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0,
/* 7 */ 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
/* 8 */ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0,
/* 9 */ 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0,
/* 10*/ 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0,
/* 11*/ 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1,
/* 12*/ 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0,
/* 13*/ 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1,
/* 14*/ 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1,
/* 15*/ 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0
	};
	// Adding the matrix above to the graph (Don't modify this)
	for (int i = 0; i < nb_vertices*nb_vertices; i++) {
			cube->adj_mat[i] = adj_mat[i];
	}

	// Adding color to all vertices (Don't modify this)
	for (int i = 0; i < nb_vertices; i++) {
		cube->colors[i] = color;
	}

	return cube;
}

graph4_t *make_pyramid(vector4_t origin, Color color) {
	int nb_vertices = 5; // A 4D pyramid has 5 vertices
	graph4_t *pyramid = graph4_make(nb_vertices);

	// Adding the 5 vertices with their coordinates
	pyramid->vertices[0] = vector4_add((vector4_t) {0.0, 0.0, 0.0, 0.0}, origin);
	pyramid->vertices[1] = vector4_add((vector4_t) {1.0, 0.0, 0.0, 0.0}, origin);
	pyramid->vertices[2] = vector4_add((vector4_t) {0.5, 0.866025, 0.0, 0.0}, origin);
	pyramid->vertices[3] = vector4_add((vector4_t) {0.5, 0.288675, 0.816497, 0.0}, origin);
	pyramid->vertices[4] = vector4_add((vector4_t) {0.5, 0.288675, 0.204124, 0.774597}, origin);

	/* Adjacency matrix of the vertices.
	If there is a 1 on line 0 column 2, it means there is an edge between vertex 0 and vertex 2.
	If there is a zero then there is no edge.
	This matrix has to be symmetrical. */
	int adj_mat[5 * 5] = {
/*      0  1  2  3  4 */
/* 0 */ 0, 1, 1, 1, 1,
/* 1 */ 1, 0, 1, 1, 1,
/* 2 */ 1, 1, 0, 1, 1,
/* 3 */ 1, 1, 1, 0, 1,
/* 4 */ 1, 1, 1, 1, 0
	};
	// Adding the matrix above to the graph (Don't modify this)
	for (int i = 0; i < nb_vertices*nb_vertices; i++) {
			pyramid->adj_mat[i] = adj_mat[i];
	}

	// Adding color to all vertices (Don't modify this)
	for (int i = 0; i < pyramid->nb_vertices; i++) {
		pyramid->colors[i] = color;
	}
	
	return pyramid;
}



graph4_t *init_shapes() {
	graph4_t *shapes = graph4_make(0);

	// Cube
	vector4_t cube_origin = (vector4_t) {0, 0, 0, 0};
	Color cube_color = (Color) {90, 90, 255, 255}; // RGBA
	graph4_t *cube = make_cube(cube_origin, cube_color);
	shapes = graph4_merge(shapes, cube);

	// Template example
	// Pyramid
	vector4_t pyramid_origin = (vector4_t) {2, 2, 2, 2}; // The origin
	Color pyramid_color = (Color) {255, 0, 0, 255}; // The color of the edges in RGBA format
	graph4_t *pyramid = make_pyramid(pyramid_origin, pyramid_color); // The function to create the shape, make your own one above !
	shapes = graph4_merge(shapes, pyramid); // Adding the pyramid to the rest of the shapes

	return shapes;
}
