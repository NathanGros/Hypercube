#ifndef GRAPH_H
#define GRAPH_H

#include <raylib.h>
#include <stdlib.h>
#include "vectors.h"

typedef struct {
	int nb_vertices;
	vector4_t *vertices;
	Color *colors;
	int *adj_mat;
} graph4_t;

graph4_t *graph4_make(int nb_vertices);
void graph4_free(graph4_t *g);
graph4_t *graph4_merge(graph4_t *g1, graph4_t *g2);

#endif
