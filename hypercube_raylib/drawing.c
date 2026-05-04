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

vector4_t cam_coordinates(vector4_t point, camera_t cam) {
	vector4_t translated_point = (vector4_t) {point.w - cam.position.w, point.x - cam.position.x, point.y - cam.position.y, point.z - cam.position.z};
	matrix_t *translated_point_matrix = matrix_make(4, 1);
	matrix_set(translated_point_matrix, 0, 0, translated_point.w);
	matrix_set(translated_point_matrix, 1, 0, translated_point.x);
	matrix_set(translated_point_matrix, 2, 0, translated_point.y);
	matrix_set(translated_point_matrix, 3, 0, translated_point.z);
	matrix_t *rotated_point_matrix = matrix_multiply(get_inverse_transition_matrix(), translated_point_matrix);
	vector4_t rotated_point = (vector4_t) {
		matrix_get(rotated_point_matrix, 0, 0),
		matrix_get(rotated_point_matrix, 1, 0),
		matrix_get(rotated_point_matrix, 2, 0),
		matrix_get(rotated_point_matrix, 3, 0)
	};
	matrix_free(translated_point_matrix);
	matrix_free(rotated_point_matrix);
	return rotated_point;
}

vector3_t hyperspace_to_space(vector4_t p) {
	return (vector3_t) {
		perspective_length_3d(p.x, p.w),
		perspective_length_3d(p.y, p.w),
		perspective_length_3d(p.z, p.w)
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

void draw_point_4d(camera_t cam, vector4_t point, double radius, Color color) {
	// Change to cam coordinates
	vector4_t rotated_point = cam_coordinates(point, cam);

	// Project to 3d space
	vector3_t space_p = hyperspace_to_space(rotated_point);
	if (space_p.z <= 0)
		return;
	// Compute screen position
	vector2_t screen_p = space_to_screen(space_p);

	// Draw
	DrawCircle(screen_p.x, screen_p.y, radius * screen_radius_px / (screen_radius * norm_vector4(rotated_point)), color);
}

void draw_screen_line(vector3_t p1, vector3_t p2, vector4_t midpoint, double thickness, Color color) {
	vector2_t screen_p1 = space_to_screen(p1);
	vector2_t screen_p2 = space_to_screen(p2);
	double screen_thickness = thickness * screen_radius_px / (screen_radius * norm_vector4(midpoint));
	DrawLineEx((Vector2) {screen_p1.x, screen_p1.y}, (Vector2) {screen_p2.x, screen_p2.y}, screen_thickness, color);
}

void draw_line_4d(camera_t cam, vector4_t point1, vector4_t point2, double thickness, Color color) {
	// Don't draw if same point
	if (point1.w == point2.w && point1.x == point2.x && point1.y == point2.y && point1.z == point2.z)
		return;

	// Change to cam coordinates
	vector4_t rotated_point1 = cam_coordinates(point1, cam);
	vector4_t rotated_point2 = cam_coordinates(point2, cam);

	// Project to 3d space
	vector3_t space_p1 = hyperspace_to_space(rotated_point1);
	vector3_t space_p2 = hyperspace_to_space(rotated_point2);

	// Compute screen position
	// Line is entirely out of FOV
	if (space_p1.z <= 0 && space_p2.z <= 0)
		return;

	// Line is entirely in FOV
	if (space_p1.z > 0 && space_p2.z > 0) {
		vector4_t midpoint = (vector4_t) {
			(rotated_point1.w + rotated_point2.w) / 2,
			(rotated_point1.x + rotated_point2.x) / 2,
			(rotated_point1.y + rotated_point2.y) / 2,
			(rotated_point1.z + rotated_point2.z) / 2,
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
			(0 + rotated_point2.w) / 2,
			(p0.x + rotated_point2.x) / 2,
			(p0.y + rotated_point2.y) / 2,
			(p0.z + rotated_point2.z) / 2,
		};
		draw_screen_line(p0, space_p2, midpoint, thickness, color);
	} else { // Draw line from p0 to p1
		vector4_t midpoint = (vector4_t) {
			(rotated_point1.w + 0) / 2,
			(rotated_point1.x + p0.x) / 2,
			(rotated_point1.y + p0.y) / 2,
			(rotated_point1.z + p0.z) / 2,
		};
		draw_screen_line(p0, space_p1, midpoint, thickness, color);
	}
}
