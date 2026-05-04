#include <raylib.h>

#include "camera.h"
#include "drawing.h"

#define WHEEL_SENSITIVITY 10
#define MOUSE_SENSITIVITY 0.15
#define CAM_MOVEMENT_SPEED 0.05

vector4_t cube1[16];
vector4_t cube2[16];

void init_cube(vector4_t cube[16], vector4_t cube_origin) {
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				for (int l = 0; l < 2; l++) {
					cube[8*i + 4*j + 2*k + l] = (vector4_t) {cube_origin.w+i, cube_origin.x+j, cube_origin.y+k, cube_origin.z+l};
				}
			}
		}
	}
}

void draw_cube(camera_t cam, vector4_t cube[16], Color color) {
	double edge_thickness = 0.03;
	double vertex_radius = 0.1;
	// Edges
	draw_line_4d(cam, cube[0], cube[1], edge_thickness, color);
	draw_line_4d(cam, cube[0], cube[2], edge_thickness, color);
	draw_line_4d(cam, cube[0], cube[4], edge_thickness, color);
	draw_line_4d(cam, cube[0], cube[8], edge_thickness, color);
	draw_line_4d(cam, cube[3], cube[2], edge_thickness, color);
	draw_line_4d(cam, cube[3], cube[1], edge_thickness, color);
	draw_line_4d(cam, cube[3], cube[7], edge_thickness, color);
	draw_line_4d(cam, cube[3], cube[11], edge_thickness, color);
	draw_line_4d(cam, cube[6], cube[7], edge_thickness, color);
	draw_line_4d(cam, cube[6], cube[4], edge_thickness, color);
	draw_line_4d(cam, cube[6], cube[2], edge_thickness, color);
	draw_line_4d(cam, cube[6], cube[14], edge_thickness, color);
	draw_line_4d(cam, cube[12], cube[13], edge_thickness, color);
	draw_line_4d(cam, cube[12], cube[14], edge_thickness, color);
	draw_line_4d(cam, cube[12], cube[8], edge_thickness, color);
	draw_line_4d(cam, cube[12], cube[4], edge_thickness, color);
	draw_line_4d(cam, cube[9], cube[8], edge_thickness, color);
	draw_line_4d(cam, cube[9], cube[11], edge_thickness, color);
	draw_line_4d(cam, cube[9], cube[13], edge_thickness, color);
	draw_line_4d(cam, cube[9], cube[1], edge_thickness, color);
	draw_line_4d(cam, cube[5], cube[4], edge_thickness, color);
	draw_line_4d(cam, cube[5], cube[7], edge_thickness, color);
	draw_line_4d(cam, cube[5], cube[1], edge_thickness, color);
	draw_line_4d(cam, cube[5], cube[13], edge_thickness, color);
	draw_line_4d(cam, cube[10], cube[11], edge_thickness, color);
	draw_line_4d(cam, cube[10], cube[8], edge_thickness, color);
	draw_line_4d(cam, cube[10], cube[14], edge_thickness, color);
	draw_line_4d(cam, cube[10], cube[2], edge_thickness, color);
	draw_line_4d(cam, cube[15], cube[14], edge_thickness, color);
	draw_line_4d(cam, cube[15], cube[13], edge_thickness, color);
	draw_line_4d(cam, cube[15], cube[11], edge_thickness, color);
	draw_line_4d(cam, cube[15], cube[7], edge_thickness, color);
	// Vertices
	for (int i = 0; i < 16; i++) {
		draw_point_4d(cam, cube[i], vertex_radius, BLACK);
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

	vector4_t cube1_origin = (vector4_t) {0, 0, 0, 0};
	init_cube(cube1, cube1_origin);
	vector4_t cube2_origin = (vector4_t) {1, 1, 1, 1};
	init_cube(cube2, cube2_origin);
	
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
		// draw_cube(get_camera(), cube1, RED);
		draw_cube(get_camera(), cube2, BLUE);
		EndDrawing();
	}
	CloseWindow();

	// De-init
	return 0;
}
