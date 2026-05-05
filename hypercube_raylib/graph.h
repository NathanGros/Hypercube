#ifndef GRAPH_H
#define GRAPH_H

#include <stdlib.h>
#include "vectors.h"

typedef struct {
	int nb_vertices;
	vector4_t *vertices;
	int *adj_mat;
} graph4_t;

typedef struct {
	int nb_vertices;
	vector3_t *vertices;
	int *adj_mat;
} graph3_t;

graph4_t *graph4_make(int nb_vertices);
graph3_t *graph3_make(int nb_vertices);
void graph4_free(graph4_t *g);
void graph3_free(graph3_t *g);

#endif
