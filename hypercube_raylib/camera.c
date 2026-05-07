#include "camera.h"

double camera_fov = 110;
camera_t cam;

vector4_t world_forward = (vector4_t) {0, 0, 0, 1};
vector4_t world_left = (vector4_t) {0, 1, 0, 0};
vector4_t world_up = (vector4_t) {0, 0, 1, 0};
vector4_t world_in = (vector4_t) {1, 0, 0, 0};

vector4_t rotated_world_origin;
vector4_t rotated_world_w;
vector4_t rotated_world_x;
vector4_t rotated_world_y;
vector4_t rotated_world_z;

matrix_t *transition_matrix = NULL;
matrix_t *inverse_transition_matrix = NULL;

vector4_t cam_coordinates(vector4_t point, camera_t cam) {
	vector4_t translated_point = (vector4_t) {point.w - cam.position.w, point.x - cam.position.x, point.y - cam.position.y, point.z - cam.position.z};
	matrix_t *translated_point_matrix = matrix_make(4, 1);
	matrix_set(translated_point_matrix, 0, 0, translated_point.w);
	matrix_set(translated_point_matrix, 1, 0, translated_point.x);
	matrix_set(translated_point_matrix, 2, 0, translated_point.y);
	matrix_set(translated_point_matrix, 3, 0, translated_point.z);
	matrix_t *rotated_point_matrix = matrix_multiply(inverse_transition_matrix, translated_point_matrix);
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

void update_world_vectors() {
	rotated_world_origin = cam_coordinates((vector4_t) {0, 0, 0, 0}, cam);
	vector4_t negated_rotated_world_origin = vector4_scalar_multiply(rotated_world_origin, -1);
	rotated_world_w = vector4_add(cam_coordinates((vector4_t) {1, 0, 0, 0}, cam), negated_rotated_world_origin);
	rotated_world_x = vector4_add(cam_coordinates((vector4_t) {0, 1, 0, 0}, cam), negated_rotated_world_origin);
	rotated_world_y = vector4_add(cam_coordinates((vector4_t) {0, 0, 1, 0}, cam), negated_rotated_world_origin);
	rotated_world_z = vector4_add(cam_coordinates((vector4_t) {0, 0, 0, 1}, cam), negated_rotated_world_origin);
}

void update_transition_matrix() {
	if (transition_matrix == NULL)
		transition_matrix = matrix_make(4, 4);
	matrix_set(transition_matrix, 0, 0, cam.in.w);
	matrix_set(transition_matrix, 1, 0, cam.in.x);
	matrix_set(transition_matrix, 2, 0, cam.in.y);
	matrix_set(transition_matrix, 3, 0, cam.in.z);
	matrix_set(transition_matrix, 0, 1, cam.left.w);
	matrix_set(transition_matrix, 1, 1, cam.left.x);
	matrix_set(transition_matrix, 2, 1, cam.left.y);
	matrix_set(transition_matrix, 3, 1, cam.left.z);
	matrix_set(transition_matrix, 0, 2, cam.up.w);
	matrix_set(transition_matrix, 1, 2, cam.up.x);
	matrix_set(transition_matrix, 2, 2, cam.up.y);
	matrix_set(transition_matrix, 3, 2, cam.up.z);
	matrix_set(transition_matrix, 0, 3, cam.forward.w);
	matrix_set(transition_matrix, 1, 3, cam.forward.x);
	matrix_set(transition_matrix, 2, 3, cam.forward.y);
	matrix_set(transition_matrix, 3, 3, cam.forward.z);
	// The inverse of a rotation matrix is its transpose
	if (inverse_transition_matrix != NULL)
		matrix_free(inverse_transition_matrix);
	inverse_transition_matrix = matrix_transpose(transition_matrix);
	update_world_vectors();
}

matrix_t *get_transition_matrix() {
	return transition_matrix;
}

matrix_t *get_inverse_transition_matrix() {
	return inverse_transition_matrix;
}

vector4_t camera_rotate_point(vector4_t point) {
	vector4_t rotated_point = vector4_add(rotated_world_origin,
		vector4_add(vector4_scalar_multiply(rotated_world_w, point.w),
			vector4_add(vector4_scalar_multiply(rotated_world_x, point.x),
				vector4_add(vector4_scalar_multiply(rotated_world_y, point.y),
					vector4_scalar_multiply(rotated_world_z, point.z)
				)
			)
		)
	);
	return rotated_point;
}

void init_camera() {
	cam.position = (vector4_t) {0, 0, 0, 0};
	cam.forward = (vector4_t) world_forward;
	cam.left = (vector4_t) world_left;
	cam.up = (vector4_t) world_up;
	cam.in = (vector4_t) world_in;
	update_transition_matrix();
}

void end_camera() {
	matrix_free(transition_matrix);
	matrix_free(inverse_transition_matrix);
}

void set_camera_fov(double fov) {
	camera_fov = fov;
}

double get_camera_fov() {
	return camera_fov;
}

camera_t get_camera() {
	return cam;
}

vector4_t rotate_world_vector(matrix_t *rotation_matrix, vector4_t world_v) {
	matrix_t *world_mat = matrix_make(4, 1);
	matrix_set(world_mat, 0, 0, world_v.w);
	matrix_set(world_mat, 1, 0, world_v.x);
	matrix_set(world_mat, 2, 0, world_v.y);
	matrix_set(world_mat, 3, 0, world_v.z);
	matrix_t *world_rotated_mat = matrix_multiply(rotation_matrix, world_mat);
	matrix_free(world_mat);
	matrix_t *cam_rotated_mat = matrix_multiply(transition_matrix, world_rotated_mat);
	matrix_free(world_rotated_mat);
	vector4_t cam_v = (vector4_t) {
		matrix_get(cam_rotated_mat, 0, 0),
		matrix_get(cam_rotated_mat, 1, 0),
		matrix_get(cam_rotated_mat, 2, 0),
		matrix_get(cam_rotated_mat, 3, 0)
	};
	matrix_free(cam_rotated_mat);
	return cam_v;
}

matrix_t *get_rotation_matrix_yz(double angle) {
	matrix_t *rotation_matrix = matrix_make(4, 4);
	matrix_set(rotation_matrix, 0, 0, cos(angle));
	matrix_set(rotation_matrix, 0, 1, -sin(angle));
	matrix_set(rotation_matrix, 0, 2, 0);
	matrix_set(rotation_matrix, 0, 3, 0);
	matrix_set(rotation_matrix, 1, 0, sin(angle));
	matrix_set(rotation_matrix, 1, 1, cos(angle));
	matrix_set(rotation_matrix, 1, 2, 0);
	matrix_set(rotation_matrix, 1, 3, 0);
	matrix_set(rotation_matrix, 2, 0, 0);
	matrix_set(rotation_matrix, 2, 1, 0);
	matrix_set(rotation_matrix, 2, 2, 1);
	matrix_set(rotation_matrix, 2, 3, 0);
	matrix_set(rotation_matrix, 3, 0, 0);
	matrix_set(rotation_matrix, 3, 1, 0);
	matrix_set(rotation_matrix, 3, 2, 0);
	matrix_set(rotation_matrix, 3, 3, 1);
	return rotation_matrix;
}

matrix_t *get_rotation_matrix_wz(double angle) {
	matrix_t *rotation_matrix = matrix_make(4, 4);
	matrix_set(rotation_matrix, 0, 0, 1);
	matrix_set(rotation_matrix, 0, 1, 0);
	matrix_set(rotation_matrix, 0, 2, 0);
	matrix_set(rotation_matrix, 0, 3, 0);
	matrix_set(rotation_matrix, 1, 0, 0);
	matrix_set(rotation_matrix, 1, 1, cos(angle));
	matrix_set(rotation_matrix, 1, 2, -sin(angle));
	matrix_set(rotation_matrix, 1, 3, 0);
	matrix_set(rotation_matrix, 2, 0, 0);
	matrix_set(rotation_matrix, 2, 1, sin(angle));
	matrix_set(rotation_matrix, 2, 2, cos(angle));
	matrix_set(rotation_matrix, 2, 3, 0);
	matrix_set(rotation_matrix, 3, 0, 0);
	matrix_set(rotation_matrix, 3, 1, 0);
	matrix_set(rotation_matrix, 3, 2, 0);
	matrix_set(rotation_matrix, 3, 3, 1);
	return rotation_matrix;
}

matrix_t *get_rotation_matrix_wx(double angle) {
	matrix_t *rotation_matrix = matrix_make(4, 4);
	matrix_set(rotation_matrix, 0, 0, 1);
	matrix_set(rotation_matrix, 0, 1, 0);
	matrix_set(rotation_matrix, 0, 2, 0);
	matrix_set(rotation_matrix, 0, 3, 0);
	matrix_set(rotation_matrix, 1, 0, 0);
	matrix_set(rotation_matrix, 1, 1, 1);
	matrix_set(rotation_matrix, 1, 2, 0);
	matrix_set(rotation_matrix, 1, 3, 0);
	matrix_set(rotation_matrix, 2, 0, 0);
	matrix_set(rotation_matrix, 2, 1, 0);
	matrix_set(rotation_matrix, 2, 2, cos(angle));
	matrix_set(rotation_matrix, 2, 3, -sin(angle));
	matrix_set(rotation_matrix, 3, 0, 0);
	matrix_set(rotation_matrix, 3, 1, 0);
	matrix_set(rotation_matrix, 3, 2, sin(angle));
	matrix_set(rotation_matrix, 3, 3, cos(angle));
	return rotation_matrix;
}

matrix_t *get_rotation_matrix_xy(double angle) {
	matrix_t *rotation_matrix = matrix_make(4, 4);
	matrix_set(rotation_matrix, 0, 0, cos(angle));
	matrix_set(rotation_matrix, 0, 1, 0);
	matrix_set(rotation_matrix, 0, 2, 0);
	matrix_set(rotation_matrix, 0, 3, sin(angle));
	matrix_set(rotation_matrix, 1, 0, 0);
	matrix_set(rotation_matrix, 1, 1, 1);
	matrix_set(rotation_matrix, 1, 2, 0);
	matrix_set(rotation_matrix, 1, 3, 0);
	matrix_set(rotation_matrix, 2, 0, 0);
	matrix_set(rotation_matrix, 2, 1, 0);
	matrix_set(rotation_matrix, 2, 2, 1);
	matrix_set(rotation_matrix, 2, 3, 0);
	matrix_set(rotation_matrix, 3, 0, -sin(angle));
	matrix_set(rotation_matrix, 3, 1, 0);
	matrix_set(rotation_matrix, 3, 2, 0);
	matrix_set(rotation_matrix, 3, 3, cos(angle));
	return rotation_matrix;
}

matrix_t *get_rotation_matrix_xz(double angle) {
	matrix_t *rotation_matrix = matrix_make(4, 4);
	matrix_set(rotation_matrix, 0, 0, cos(angle));
	matrix_set(rotation_matrix, 0, 1, 0);
	matrix_set(rotation_matrix, 0, 2, -sin(angle));
	matrix_set(rotation_matrix, 0, 3, 0);
	matrix_set(rotation_matrix, 1, 0, 0);
	matrix_set(rotation_matrix, 1, 1, 1);
	matrix_set(rotation_matrix, 1, 2, 0);
	matrix_set(rotation_matrix, 1, 3, 0);
	matrix_set(rotation_matrix, 2, 0, sin(angle));
	matrix_set(rotation_matrix, 2, 1, 0);
	matrix_set(rotation_matrix, 2, 2, cos(angle));
	matrix_set(rotation_matrix, 2, 3, 0);
	matrix_set(rotation_matrix, 3, 0, 0);
	matrix_set(rotation_matrix, 3, 1, 0);
	matrix_set(rotation_matrix, 3, 2, 0);
	matrix_set(rotation_matrix, 3, 3, 1);
	return rotation_matrix;
}

matrix_t *get_rotation_matrix_wy(double angle) {
	matrix_t *rotation_matrix = matrix_make(4, 4);
	matrix_set(rotation_matrix, 0, 0, 1);
	matrix_set(rotation_matrix, 0, 1, 0);
	matrix_set(rotation_matrix, 0, 2, 0);
	matrix_set(rotation_matrix, 0, 3, 0);
	matrix_set(rotation_matrix, 1, 0, 0);
	matrix_set(rotation_matrix, 1, 1, cos(angle));
	matrix_set(rotation_matrix, 1, 2, 0);
	matrix_set(rotation_matrix, 1, 3, -sin(angle));
	matrix_set(rotation_matrix, 2, 0, 0);
	matrix_set(rotation_matrix, 2, 1, 0);
	matrix_set(rotation_matrix, 2, 2, 1);
	matrix_set(rotation_matrix, 2, 3, 0);
	matrix_set(rotation_matrix, 3, 0, 0);
	matrix_set(rotation_matrix, 3, 1, sin(angle));
	matrix_set(rotation_matrix, 3, 2, 0);
	matrix_set(rotation_matrix, 3, 3, cos(angle));
	return rotation_matrix;
}

void camera_rotate_wx(double angle) {
	matrix_t *rotation_matrix = get_rotation_matrix_wx(angle);
	cam.forward = rotate_world_vector(rotation_matrix, world_forward);
	cam.left = rotate_world_vector(rotation_matrix, world_left);
	cam.up = rotate_world_vector(rotation_matrix, world_up);
	cam.in = rotate_world_vector(rotation_matrix, world_in);
	matrix_free(rotation_matrix);
	update_transition_matrix();
}

void camera_rotate_wy(double angle) {
	matrix_t *rotation_matrix = get_rotation_matrix_wy(angle);
	cam.forward = rotate_world_vector(rotation_matrix, world_forward);
	cam.left = rotate_world_vector(rotation_matrix, world_left);
	cam.up = rotate_world_vector(rotation_matrix, world_up);
	cam.in = rotate_world_vector(rotation_matrix, world_in);
	matrix_free(rotation_matrix);
	update_transition_matrix();
}

void camera_rotate_wz(double angle) {
	matrix_t *rotation_matrix = get_rotation_matrix_wz(angle);
	cam.forward = rotate_world_vector(rotation_matrix, world_forward);
	cam.left = rotate_world_vector(rotation_matrix, world_left);
	cam.up = rotate_world_vector(rotation_matrix, world_up);
	cam.in = rotate_world_vector(rotation_matrix, world_in);
	matrix_free(rotation_matrix);
	update_transition_matrix();
}

void camera_rotate_xy(double angle) {
	matrix_t *rotation_matrix = get_rotation_matrix_xy(angle);
	cam.forward = rotate_world_vector(rotation_matrix, world_forward);
	cam.left = rotate_world_vector(rotation_matrix, world_left);
	cam.up = rotate_world_vector(rotation_matrix, world_up);
	cam.in = rotate_world_vector(rotation_matrix, world_in);
	matrix_free(rotation_matrix);
	update_transition_matrix();
}

void camera_rotate_xz(double angle) {
	matrix_t *rotation_matrix = get_rotation_matrix_xz(angle);
	cam.forward = rotate_world_vector(rotation_matrix, world_forward);
	cam.left = rotate_world_vector(rotation_matrix, world_left);
	cam.up = rotate_world_vector(rotation_matrix, world_up);
	cam.in = rotate_world_vector(rotation_matrix, world_in);
	matrix_free(rotation_matrix);
	update_transition_matrix();
}

void camera_rotate_yz(double angle) {
	matrix_t *rotation_matrix = get_rotation_matrix_yz(angle);
	cam.forward = rotate_world_vector(rotation_matrix, world_forward);
	cam.left = rotate_world_vector(rotation_matrix, world_left);
	cam.up = rotate_world_vector(rotation_matrix, world_up);
	cam.in = rotate_world_vector(rotation_matrix, world_in);
	matrix_free(rotation_matrix);
	update_transition_matrix();
}

void camera_move_forward(double distance) {
	cam.position.w += distance * cam.forward.w;
	cam.position.x += distance * cam.forward.x;
	cam.position.y += distance * cam.forward.y;
	cam.position.z += distance * cam.forward.z;
	update_world_vectors();
}

void camera_move_left(double distance) {
	cam.position.w += distance * cam.left.w;
	cam.position.x += distance * cam.left.x;
	cam.position.y += distance * cam.left.y;
	cam.position.z += distance * cam.left.z;
	update_world_vectors();
}

void camera_move_up(double distance) {
	cam.position.w += distance * cam.up.w;
	cam.position.x += distance * cam.up.x;
	cam.position.y += distance * cam.up.y;
	cam.position.z += distance * cam.up.z;
	update_world_vectors();
}

void camera_move_in(double distance) {
	cam.position.w += distance * cam.in.w;
	cam.position.x += distance * cam.in.x;
	cam.position.y += distance * cam.in.y;
	cam.position.z += distance * cam.in.z;
	update_world_vectors();
}
