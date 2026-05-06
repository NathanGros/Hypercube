#include "graph.h"

graph4_t *graph4_make(int nb_vertices) {
	graph4_t *g = malloc(sizeof(graph4_t));
	g->nb_vertices = nb_vertices;
	g->vertices = malloc(nb_vertices * sizeof(vector4_t));
	g->colors = malloc(nb_vertices * sizeof(Color));
	g->adj_mat = malloc(nb_vertices * nb_vertices * sizeof(int));
	for (int i = 0; i < g->nb_vertices * g->nb_vertices; i++) {
		g->adj_mat[i] = 0;
	}
	return g;
}

void graph4_free(graph4_t *g) {
	free(g->vertices);
	free(g->colors);
	free(g->adj_mat);
	free(g);
}

void graph4_add_vertices(graph4_t *g, int n) {
	vector4_t *new_vertices = malloc((g->nb_vertices+n) * sizeof(vector4_t));
	for (int i = 0; i < g->nb_vertices; i++) {
		new_vertices[i] = g->vertices[i];
	}
	if (g->nb_vertices != 0)
		free(g->vertices);
	g->vertices = new_vertices;

	Color *new_colors = malloc((g->nb_vertices+n) * sizeof(Color));
	for (int i = 0; i < g->nb_vertices; i++) {
		new_colors[i] = g->colors[i];
	}
	if (g->nb_vertices != 0)
		free(g->colors);
	g->colors = new_colors;

	int *new_adj_mat = malloc((g->nb_vertices+n)*(g->nb_vertices+n) * sizeof(int));
	for (int i = 0; i < (g->nb_vertices + n) * (g->nb_vertices + n); i++) {
		new_adj_mat[i] = 0;
	}
	for (int i = 0; i < g->nb_vertices; i++) {
		for (int j = 0; j < g->nb_vertices; j++) {
			new_adj_mat[i * g->nb_vertices + j + i * n] = g->adj_mat[i * g->nb_vertices + j];
		}
	}
	if (g->nb_vertices != 0)
		free(g->adj_mat);
	g->adj_mat = new_adj_mat;

	g->nb_vertices += n;
}
