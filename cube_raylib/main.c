#include <raylib.h>
#include <stdio.h>

#include "camera.h"
#include "drawing.h"

#define WHEEL_SENSITIVITY 10
#define MOUSE_SENSITIVITY 0.15
#define CAM_MOVEMENT_SPEED 0.05

vector3_t cube[8];

void init_cube() {
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				cube[4*i + 2*j + k] = (vector3_t) {i, j, k};
			}
		}
	}
}

void draw_cube(camera_t cam) {
	double edge_thickness = 0.03;
	double vertex_radius = 0.1;
	// Edges
	draw_line_3d(cam, cube[0], cube[1], edge_thickness, RED);
	draw_line_3d(cam, cube[0], cube[2], edge_thickness, RED);
	draw_line_3d(cam, cube[0], cube[4], edge_thickness, RED);
	draw_line_3d(cam, cube[3], cube[2], edge_thickness, RED);
	draw_line_3d(cam, cube[3], cube[1], edge_thickness, RED);
	draw_line_3d(cam, cube[3], cube[7], edge_thickness, RED);
	draw_line_3d(cam, cube[5], cube[4], edge_thickness, RED);
	draw_line_3d(cam, cube[5], cube[7], edge_thickness, RED);
	draw_line_3d(cam, cube[5], cube[1], edge_thickness, RED);
	draw_line_3d(cam, cube[6], cube[7], edge_thickness, RED);
	draw_line_3d(cam, cube[6], cube[4], edge_thickness, RED);
	draw_line_3d(cam, cube[6], cube[2], edge_thickness, RED);
	// Vertices
	for (int i = 0; i < 8; i++) {
		draw_point_3d(cam, cube[i], vertex_radius, BLACK);
	}
}

void print_cube(vector3_t *cube) {
	for (int i = 0; i < 8; i++) {
		printf("Vertex %d:", i);
		printf("x: %.2f, y: %.2f, z: %.2f\n", cube[i].x, cube[i].y, cube[i].z);
	}
}

void Init(Color backgroundColor) {
	InitWindow(1500, 900, "Window");
	ClearBackground(backgroundColor);
	SetTargetFPS(60);
}

int main() {
	// Init
	Color backgroundColor = (Color){255, 255, 255, 255};
	Init(backgroundColor);
	init_draw();
	init_camera();
	init_cube();

	DisableCursor();
	while (!WindowShouldClose()) {
		// Controls
		Vector2 mouseDelta = GetMouseDelta();
		double pitch_delta = mouseDelta.y;
		double yaw_delta = mouseDelta.x;
		// Pitch
		if (pitch_delta != 0) {
			double angle_delta = pitch_delta * MOUSE_SENSITIVITY * GetFrameTime();
			camera_rotate_pitch(angle_delta);
		}
		// Yaw
		if (yaw_delta != 0) {
			double angle_delta = -1 * yaw_delta * MOUSE_SENSITIVITY * GetFrameTime();
			camera_rotate_yaw(angle_delta);
		}
		// Roll
		float wheel_move = GetMouseWheelMove();
		if (wheel_move != 0) {
			double angle_delta = WHEEL_SENSITIVITY * -wheel_move * GetFrameTime();
			camera_rotate_roll(angle_delta);
		}
		// Movement
		if (IsKeyDown(KEY_W)) camera_move_forward(CAM_MOVEMENT_SPEED);
		if (IsKeyDown(KEY_S)) camera_move_forward(-CAM_MOVEMENT_SPEED);
		if (IsKeyDown(KEY_A)) camera_move_left(CAM_MOVEMENT_SPEED);
		if (IsKeyDown(KEY_D)) camera_move_left(-CAM_MOVEMENT_SPEED);
		if (IsKeyDown(KEY_SPACE)) camera_move_up(CAM_MOVEMENT_SPEED);
		if (IsKeyDown(KEY_LEFT_CONTROL)) camera_move_up(-CAM_MOVEMENT_SPEED);

		// Drawing
		BeginDrawing();
		ClearBackground(backgroundColor);
		draw_cube(get_camera());
		EndDrawing();
	}
	CloseWindow();

	// De-init
	return 0;
}
