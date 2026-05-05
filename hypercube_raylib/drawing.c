#include "drawing.h"

double screen_radius_px;
double screen_radius;

void init_draw() {
	int screenWidth = GetScreenWidth();
	int screenHeight = GetScreenHeight();
	screen_radius_px = screenHeight > screenWidth ? screenHeight / 2 : screenWidth / 2;
	screen_radius = tan((double) get_camera_fov() * PI / 180 / 2);
}

double double_abs(double n) {
	return (n > 0) ? n : -n;
}

double perspective_length_3d(double length, double distance) {
	if (distance == 0)
		distance = 0.00001;
	return length / (double_abs(distance));
}

double perspective_length_2d(double length, double distance) {
	if (distance == 0)
		distance = 0.00001;
	return length / (distance * screen_radius);
}

vector3_t hyperspace_to_space(vector4_t p) {
	return (vector3_t) {
		p.x,
		p.y,
		p.z
	};
}

vector2_t space_to_screen(vector3_t p) {
	int screen_cam_x = GetScreenWidth() / 2;
	int screen_cam_y = GetScreenHeight() / 2;
	return (vector2_t) {
		screen_cam_x - screen_radius_px * perspective_length_2d(p.x, p.z),
		screen_cam_y - screen_radius_px * perspective_length_2d(p.y, p.z)
	};
}

void draw_point_3d(camera_t cam, vector3_t point, double radius, Color color) {
	if (point.z <= 0)
		return;
	// Compute screen position
	vector2_t screen_p = space_to_screen(point);
	// Draw
	DrawCircle(screen_p.x, screen_p.y, 1.1 * radius, BLACK);
	DrawCircle(screen_p.x, screen_p.y, radius, color);
}

void draw_point_4d(camera_t cam, vector4_t point, double radius, Color color) {
	// Project to 3d space
	vector3_t space_p = hyperspace_to_space(point);
	draw_point_3d(cam, space_p, radius * screen_radius_px / (screen_radius * vector4_norm(point)), color);
}

void draw_screen_line(vector3_t p1, vector3_t p2, vector4_t midpoint, double thickness, Color color) {
	vector2_t screen_p1 = space_to_screen(p1);
	vector2_t screen_p2 = space_to_screen(p2);
	double screen_thickness = thickness * screen_radius_px / (screen_radius * vector4_norm(midpoint));
	DrawLineEx((Vector2) {screen_p1.x, screen_p1.y}, (Vector2) {screen_p2.x, screen_p2.y}, 1.5 * screen_thickness, BLACK);
	DrawLineEx((Vector2) {screen_p1.x, screen_p1.y}, (Vector2) {screen_p2.x, screen_p2.y}, screen_thickness, color);
}

void draw_line_4d(camera_t cam, vector4_t point1, vector4_t point2, double thickness, Color color) {
	// Don't draw if same point
	if (point1.w == point2.w && point1.x == point2.x && point1.y == point2.y && point1.z == point2.z)
		return;

	// Project to 3d space
	vector3_t space_p1 = hyperspace_to_space(point1);
	vector3_t space_p2 = hyperspace_to_space(point2);

	// Compute screen position
	// Line is entirely out of FOV
	if (space_p1.z <= 0 && space_p2.z <= 0)
		return;

	// Line is entirely in FOV
	if (space_p1.z > 0 && space_p2.z > 0) {
		vector4_t midpoint = (vector4_t) {
			(point1.w + point2.w) / 2,
			(point1.x + point2.x) / 2,
			(point1.y + point2.y) / 2,
			(point1.z + point2.z) / 2,
		};
		draw_screen_line(space_p1, space_p2, midpoint, thickness, color);
		return;
	}

	// Line is partially out of FOV
	double diff_x = space_p2.x - space_p1.x;
	double diff_y = space_p2.y - space_p1.y;
	double to_zero_ratio = double_abs(space_p1.z) / (double_abs(space_p1.z) + double_abs(space_p2.z));
	double diff_x_to_zero = to_zero_ratio * diff_x;
	double diff_y_to_zero = to_zero_ratio * diff_y;
	vector3_t p0 = (vector3_t) {space_p1.x + diff_x_to_zero, space_p1.y + diff_y_to_zero, 0};
	if (space_p1.z <= 0) { // Draw line from p0 to p2
		vector4_t midpoint = (vector4_t) {
			(point1.w + point2.w) / 2,
			(point1.x + point2.x) / 2,
			(point1.y + point2.y) / 2,
			(point1.z + point2.z) / 2,
		};
		draw_screen_line(p0, space_p2, midpoint, thickness, color);
	} else { // Draw line from p0 to p1
		vector4_t midpoint = (vector4_t) {
			(point1.w + point2.w) / 2,
			(point1.x + point2.x) / 2,
			(point1.y + point2.y) / 2,
			(point1.z + point2.z) / 2,
		};
		draw_screen_line(p0, space_p1, midpoint, thickness, color);
	}
}

graph4_t *rotate_graph(graph4_t *g) {
	graph4_t *g2 = graph4_make(g->nb_vertices);
	for (int i = 0; i < g->nb_vertices; i++) {
		g2->vertices[i] = camera_rotate_point(g->vertices[i]);
	}
	g2->adj_mat = malloc(g->nb_vertices * g->nb_vertices * sizeof(int));
	for (int i = 0; i < g->nb_vertices * g->nb_vertices; i++) {
		g2->adj_mat[i] = g->adj_mat[i];
	}
	return g2;
}

void sort_depths(vector4_t *vertices, int *depths_indices, int size) {
    for (int j = 1; j < size; j++) {
        int key = j;
        int i = j - 1;
        while (i > -1 && vector3_norm(hyperspace_to_space(vertices[depths_indices[i]])) > vector3_norm(hyperspace_to_space(vertices[key]))) {
            depths_indices[i + 1] = depths_indices[i];
            i = i - 1;
        }
        depths_indices[i + 1] = key;
    }
}

void draw_graph_4d(camera_t cam, graph4_t *graph, Color color) {
	double edge_thickness = 0.03;
	double vertex_radius = 0.1;
	int n = graph->nb_vertices;

	// Rotate graph
	graph4_t *rotated_graph = rotate_graph(graph);

	// Order vertices by 3d depth
	int *depth_indices = malloc(n * sizeof(int));
	for (int i = 0; i < n; i++) {
		depth_indices[i] = i;
	}
	sort_depths(rotated_graph->vertices, depth_indices, n);
	int *new_adj_mat = malloc(n * n * sizeof(int));
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			new_adj_mat[i * n + j] = rotated_graph->adj_mat[depth_indices[i] * n + depth_indices[j]];
		}
	}
	free(rotated_graph->adj_mat);
	rotated_graph->adj_mat = new_adj_mat;
	vector4_t *new_vertices = malloc(n * sizeof(vector4_t));
	for (int i = 0; i < n; i++) {
		new_vertices[i] = rotated_graph->vertices[depth_indices[i]];
	}
	free(rotated_graph->vertices);
	rotated_graph->vertices = new_vertices;
	free(depth_indices);

	// draw each vertex with edges coming towards the camera
	for (int i = n - 1; i > -1; i--) {
		draw_point_4d(cam, rotated_graph->vertices[i], vertex_radius, WHITE);
		for (int j = 0; j < n; j++) {
			if (rotated_graph->adj_mat[i * n + j] == 1)
				draw_line_4d(cam, rotated_graph->vertices[i], rotated_graph->vertices[j], edge_thickness, color);
		}
	}

	graph4_free(rotated_graph);
}
