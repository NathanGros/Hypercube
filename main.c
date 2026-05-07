#include <raylib.h>

#include "camera.h"
#include "drawing.h"
#include "vectors.h"
#include "graph.h"
#include "shapes.h"

#define WHEEL_SENSITIVITY 10
#define MOUSE_SENSITIVITY 0.15
#define CAM_MOVEMENT_SPEED 0.05
#define DRAW_VERTICES false

void Init(Color backgroundColor) {
	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);
	InitWindow(1500, 900, "Hypergraphs");
	ClearBackground(backgroundColor);
	SetTargetFPS(60);
}

int main() {
	// Init
	Color backgroundColor = (Color){200, 200, 220, 255};
	Init(backgroundColor);
	init_draw();
	init_camera();
	graph4_t *shapes = init_shapes();

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
