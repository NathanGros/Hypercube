#include "drawing.h"

double screen_radius_px;
double screen_radius;

void init_draw() {
	int screenWidth = GetScreenWidth();
	int screenHeight = GetScreenHeight();
	screen_radius_px = screenHeight > screenWidth ? screenHeight / 2 : screenWidth / 2;
	screen_radius = tan((double) get_camera_fov() * PI / 180 / 2);
}

double perspective_length(double length, double distance) {
	if (distance == 0)
		distance = 0.00001;
	return length / distance / screen_radius * screen_radius_px;
}

double double_abs(double n) {
	return (n > 0) ? n : -n;
}

vector3_t cam_coordinates(vector3_t point, camera_t cam) {
	vector3_t translated_point = (vector3_t) {point.x - cam.position.x, point.y - cam.position.y, point.z - cam.position.z};
	matrix_t *translated_point_matrix = matrix_make(3, 1);
	matrix_set(translated_point_matrix, 0, 0, translated_point.x);
	matrix_set(translated_point_matrix, 1, 0, translated_point.y);
	matrix_set(translated_point_matrix, 2, 0, translated_point.z);
	matrix_t *rotated_point_matrix = matrix_multiply(get_inverse_transition_matrix(), translated_point_matrix);
	vector3_t rotated_point = (vector3_t) {
		matrix_get(rotated_point_matrix, 0, 0),
		matrix_get(rotated_point_matrix, 1, 0),
		matrix_get(rotated_point_matrix, 2, 0)
	};
	matrix_free(translated_point_matrix);
	matrix_free(rotated_point_matrix);
	return rotated_point;
}

void draw_point_3d(camera_t cam, vector3_t point, double radius, Color color) {
	// Change to cam coordinates
	vector3_t rotated_point = cam_coordinates(point, cam);

	// Compute screen position
	if (rotated_point.z <= 0)
		return;
	int screen_cam_x = GetScreenWidth() / 2;
	int screen_cam_y = GetScreenHeight() / 2;
	int screen_x = screen_cam_x - perspective_length(rotated_point.x, rotated_point.z);
	int screen_y = screen_cam_y - perspective_length(rotated_point.y, rotated_point.z);

	// Draw
	DrawCircle(screen_x, screen_y, perspective_length(radius, rotated_point.z), color);
}

vector2_t cam_to_screen(vector3_t p) {
	int screen_cam_x = GetScreenWidth() / 2;
	int screen_cam_y = GetScreenHeight() / 2;
	int screen_p_x = screen_cam_x - perspective_length(p.x, p.z);
	int screen_p_y = screen_cam_y - perspective_length(p.y, p.z);
	return (vector2_t) {screen_p_x, screen_p_y};
}

void draw_screen_line(vector3_t p1, vector3_t p2, double thickness) {
	vector2_t screen_point1 = cam_to_screen(p1);
	vector2_t screen_point2 = cam_to_screen(p2);
	double middle_distance = p2.z + (p1.z - p2.z) / 2;
	double screen_thickness = perspective_length(thickness, middle_distance);
	DrawLineEx((Vector2) {screen_point1.x, screen_point1.y}, (Vector2) {screen_point2.x, screen_point2.y}, screen_thickness, RED);
}

void draw_line_3d(camera_t cam, vector3_t point1, vector3_t point2, double thickness, Color color) {
	// Don't draw if same point
	if (point1.x == point2.x && point1.y == point2.y && point1.z == point2.z)
		return;

	// Change to cam coordinates
	vector3_t rotated_point1 = cam_coordinates(point1, cam);
	vector3_t rotated_point2 = cam_coordinates(point2, cam);

	// Compute screen position
	// Line is entirely out of FOV
	if (rotated_point1.z <= 0 && rotated_point2.z <= 0)
		return;

	// Line is entirely in FOV
	if (rotated_point1.z > 0 && rotated_point2.z > 0) {
		draw_screen_line(rotated_point1, rotated_point2, thickness);
		return;
	}

	// Line is partially out of FOV
	double diff_x = rotated_point2.x - rotated_point1.x;
	double diff_y = rotated_point2.y - rotated_point1.y;
	double to_zero_ratio = double_abs(rotated_point1.z) / (double_abs(rotated_point1.z) + double_abs(rotated_point2.z));
	double diff_x_to_zero = to_zero_ratio * diff_x;
	double diff_y_to_zero = to_zero_ratio * diff_y;
	vector3_t p0 = (vector3_t) {rotated_point1.x + diff_x_to_zero, rotated_point1.y + diff_y_to_zero, 0};
	if (rotated_point1.z <= 0) // Draw line from p0 to p2
		draw_screen_line(p0, rotated_point2, thickness);
	else // Draw line from p0 to p1
		draw_screen_line(p0, rotated_point1, thickness);
}
