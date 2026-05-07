#include <raylib.h>

#include "camera.h"
#include "drawing.h"
#include "vectors.h"
#include "graph.h"

#define WHEEL_SENSITIVITY 10
#define MOUSE_SENSITIVITY 0.15
#define CAM_MOVEMENT_SPEED 0.05
#define DRAW_VERTICES false

graph4_t *make_cube(vector4_t origin) {
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
		cube->colors[i] = BLUE;
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

graph4_t *make_pyramid(vector4_t origin) {
	graph4_t *pyramid = graph4_make(5);
	pyramid->vertices[0] = vector4_add((vector4_t) {0.0, 0.0, 0.0, 0.0}, origin);
	pyramid->vertices[1] = vector4_add((vector4_t) {1.0, 0.0, 0.0, 0.0}, origin);
	pyramid->vertices[2] = vector4_add((vector4_t) {0.5, 0.866025, 0.0, 0.0}, origin);
	pyramid->vertices[3] = vector4_add((vector4_t) {0.5, 0.288675, 0.816497, 0.0}, origin);
	pyramid->vertices[4] = vector4_add((vector4_t) {0.5, 0.288675, 0.204124, 0.774597}, origin);
	for (int i = 0; i < 5; i++) {
		pyramid->colors[i] = RED;
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

void Init(Color backgroundColor) {
	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);
	InitWindow(1500, 900, "Hypergraphs");
	ClearBackground(backgroundColor);
	SetTargetFPS(60);
}

int main() {
	// Init
	Color backgroundColor = (Color){255, 255, 255, 255};
	Init(backgroundColor);
	init_draw();
	init_camera();

	// Shapes
	graph4_t *shapes = graph4_make(0);
	vector4_t cube_origin = (vector4_t) {0, 0, 0, 0};
	graph4_t *cube = make_cube(cube_origin);
	shapes = graph4_merge(shapes, cube);
	vector4_t pyramid_origin = (vector4_t) {2, 2, 2, 2};
	graph4_t *pyramid = make_pyramid(pyramid_origin);
	shapes = graph4_merge(shapes, pyramid);
	
	DisableCursor();
	while (!WindowShouldClose()) {
		// Controls
		Vector2 mouseDelta = GetMouseDelta();
		double pitch_delta = mouseDelta.y;
		double yaw_delta = mouseDelta.x;
		// Pitch
		if (pitch_delta != 0) {
			double angle_delta = pitch_delta * MOUSE_SENSITIVITY * GetFrameTime();
			if (IsKeyDown(KEY_LEFT_CONTROL))
				camera_rotate_xz(angle_delta);
			else
				camera_rotate_wx(angle_delta);
		}
		// Yaw
		if (yaw_delta != 0) {
			double angle_delta = -1 * yaw_delta * MOUSE_SENSITIVITY * GetFrameTime();
			if (IsKeyDown(KEY_LEFT_CONTROL))
				camera_rotate_yz(-angle_delta);
			else
				camera_rotate_wy(-angle_delta);
		}
		// Roll
		float wheel_move = GetMouseWheelMove();
		if (wheel_move != 0) {
			double angle_delta = WHEEL_SENSITIVITY * -wheel_move * GetFrameTime();
			if (IsKeyDown(KEY_LEFT_CONTROL))
				camera_rotate_xy(angle_delta);
			else
				camera_rotate_wz(angle_delta);
		}
		// Movement
		if (IsKeyDown(KEY_W)) camera_move_forward(CAM_MOVEMENT_SPEED);
		if (IsKeyDown(KEY_S)) camera_move_forward(-CAM_MOVEMENT_SPEED);
		if (IsKeyDown(KEY_A)) camera_move_left(CAM_MOVEMENT_SPEED);
		if (IsKeyDown(KEY_D)) camera_move_left(-CAM_MOVEMENT_SPEED);
		if (IsKeyDown(KEY_SPACE)) camera_move_up(CAM_MOVEMENT_SPEED);
		if (IsKeyDown(KEY_LEFT_SHIFT)) camera_move_up(-CAM_MOVEMENT_SPEED);
		if (IsKeyDown(KEY_Q)) camera_move_in(CAM_MOVEMENT_SPEED);
		if (IsKeyDown(KEY_E)) camera_move_in(-CAM_MOVEMENT_SPEED);

		// Drawing
		BeginDrawing();
		ClearBackground(backgroundColor);
		draw_graph_4d(get_camera(), shapes, DRAW_VERTICES);
		EndDrawing();
	}
	CloseWindow();

	// De-init
	graph4_free(shapes);
	end_camera();
	return 0;
}
