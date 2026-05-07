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

graph4_t *graph4_merge(graph4_t *g1, graph4_t *g2) {
	graph4_t *g3 = graph4_make(g1->nb_vertices + g2->nb_vertices);

	for (int i = 0; i < g1->nb_vertices; i++) {
		g3->vertices[i] = g1->vertices[i];
	}
	for (int i = 0; i < g2->nb_vertices; i++) {
		g3->vertices[g1->nb_vertices + i] = g2->vertices[i];
	}

	for (int i = 0; i < g1->nb_vertices; i++) {
		g3->colors[i] = g1->colors[i];
	}
	for (int i = 0; i < g2->nb_vertices; i++) {
		g3->colors[g1->nb_vertices + i] = g2->colors[i];
	}

	for (int i = 0; i < g1->nb_vertices; i++) {
		for (int j = 0; j < g1->nb_vertices; j++) {
			g3->adj_mat[i * (g3->nb_vertices) + j] = g1->adj_mat[i * g1->nb_vertices + j];
		}
	}
	for (int i = 0; i < g2->nb_vertices; i++) {
		for (int j = 0; j < g2->nb_vertices; j++) {
			g3->adj_mat[(g1->nb_vertices * g3->nb_vertices) + i * (g3->nb_vertices) + (g1->nb_vertices + j)] = g2->adj_mat[i * g2->nb_vertices + j];
		}
	}
	
	graph4_free(g1);
	graph4_free(g2);
	return g3;
}
