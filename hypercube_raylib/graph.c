#include "graph.h"

graph4_t *graph4_make(int nb_vertices) {
	graph4_t *g = malloc(sizeof(graph4_t));
	g->nb_vertices = nb_vertices;
	g->vertices = malloc(nb_vertices * sizeof(vector4_t));
	g->adj_mat = malloc(nb_vertices * nb_vertices * sizeof(int));
	for (int i = 0; i < g->nb_vertices * g->nb_vertices; i++) {
		g->adj_mat[i] = 0;
	}
	return g;
}

graph3_t *graph3_make(int nb_vertices) {
	graph3_t *g = malloc(sizeof(graph3_t));
	g->nb_vertices = nb_vertices;
	g->vertices = malloc(nb_vertices * sizeof(vector3_t));
	g->adj_mat = malloc(nb_vertices * nb_vertices * sizeof(int));
	for (int i = 0; i < g->nb_vertices * g->nb_vertices; i++) {
		g->adj_mat[i] = 0;
	}
	return g;
}

void graph4_free(graph4_t *g) {
	free(g->vertices);
	free(g->adj_mat);
	free(g);
}

void graph3_free(graph3_t *g) {
	free(g->vertices);
	free(g->adj_mat);
	free(g);
}
