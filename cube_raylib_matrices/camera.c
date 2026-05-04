#include "camera.h"

double camera_fov = 110;
camera_t cam;

vector3_t world_forward = (vector3_t) {0, 0, 1};
vector3_t world_left = (vector3_t) {1, 0, 0};
vector3_t world_up = (vector3_t) {0, 1, 0};

matrix_t *transition_matrix = NULL;
matrix_t *inverse_transition_matrix = NULL;

void update_transition_matrix() {
	if (transition_matrix == NULL)
		transition_matrix = matrix_make(3, 3);
	matrix_set(transition_matrix, 0, 0, cam.left.x);
	matrix_set(transition_matrix, 1, 0, cam.left.y);
	matrix_set(transition_matrix, 2, 0, cam.left.z);
	matrix_set(transition_matrix, 0, 1, cam.up.x);
	matrix_set(transition_matrix, 1, 1, cam.up.y);
	matrix_set(transition_matrix, 2, 1, cam.up.z);
	matrix_set(transition_matrix, 0, 2, cam.forward.x);
	matrix_set(transition_matrix, 1, 2, cam.forward.y);
	matrix_set(transition_matrix, 2, 2, cam.forward.z);
	// The inverse of a rotation matrix is its transpose
	inverse_transition_matrix = matrix_transpose(transition_matrix);
}

matrix_t *get_transition_matrix() {
	return transition_matrix;
}

matrix_t *get_inverse_transition_matrix() {
	return inverse_transition_matrix;
}

void init_camera() {
	cam.position = (vector3_t) {0.5, 0.5, 0.5};
	cam.forward = (vector3_t) {0, 0, 1};
	cam.left = (vector3_t) {1, 0, 0};
	cam.up = (vector3_t) {0, 1, 0};
	update_transition_matrix();
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

vector3_t rotate_world_vector(matrix_t *rotation_matrix, vector3_t world_v) {
	matrix_t *world_mat = matrix_make(3, 1);
	matrix_set(world_mat, 0, 0, world_v.x);
	matrix_set(world_mat, 1, 0, world_v.y);
	matrix_set(world_mat, 2, 0, world_v.z);
	matrix_t *world_rotated_mat = matrix_multiply(rotation_matrix, world_mat);
	matrix_free(world_mat);
	matrix_t *cam_rotated_mat = matrix_multiply(transition_matrix, world_rotated_mat);
	matrix_free(world_rotated_mat);
	vector3_t cam_v = (vector3_t) {
		matrix_get(cam_rotated_mat, 0, 0),
		matrix_get(cam_rotated_mat, 1, 0),
		matrix_get(cam_rotated_mat, 2, 0)
	};
	matrix_free(cam_rotated_mat);
	return cam_v;
}

void camera_rotate_pitch(double angle) {
	matrix_t *rotation_matrix = matrix_make(3, 3);
	matrix_set(rotation_matrix, 0, 0, 1);
	matrix_set(rotation_matrix, 0, 1, 0);
	matrix_set(rotation_matrix, 0, 2, 0);
	matrix_set(rotation_matrix, 1, 0, 0);
	matrix_set(rotation_matrix, 1, 1, cos(angle));
	matrix_set(rotation_matrix, 1, 2, -sin(angle));
	matrix_set(rotation_matrix, 2, 0, 0);
	matrix_set(rotation_matrix, 2, 1, sin(angle));
	matrix_set(rotation_matrix, 2, 2, cos(angle));

	cam.forward = rotate_world_vector(rotation_matrix, world_forward);
	cam.left = rotate_world_vector(rotation_matrix, world_left);
	cam.up = rotate_world_vector(rotation_matrix, world_up);

	matrix_free(rotation_matrix);
	update_transition_matrix();
}

void camera_rotate_yaw(double angle) {
	matrix_t *rotation_matrix = matrix_make(3, 3);
	matrix_set(rotation_matrix, 0, 0, cos(angle));
	matrix_set(rotation_matrix, 0, 1, 0);
	matrix_set(rotation_matrix, 0, 2, sin(angle));
	matrix_set(rotation_matrix, 1, 0, 0);
	matrix_set(rotation_matrix, 1, 1, 1);
	matrix_set(rotation_matrix, 1, 2, 0);
	matrix_set(rotation_matrix, 2, 0, -sin(angle));
	matrix_set(rotation_matrix, 2, 1, 0);
	matrix_set(rotation_matrix, 2, 2, cos(angle));

	cam.forward = rotate_world_vector(rotation_matrix, world_forward);
	cam.left = rotate_world_vector(rotation_matrix, world_left);
	cam.up = rotate_world_vector(rotation_matrix, world_up);

	matrix_free(rotation_matrix);
	update_transition_matrix();
}

void camera_rotate_roll(double angle) {
	matrix_t *rotation_matrix = matrix_make(3, 3);
	matrix_set(rotation_matrix, 0, 0, cos(angle));
	matrix_set(rotation_matrix, 0, 1, -sin(angle));
	matrix_set(rotation_matrix, 0, 2, 0);
	matrix_set(rotation_matrix, 1, 0, sin(angle));
	matrix_set(rotation_matrix, 1, 1, cos(angle));
	matrix_set(rotation_matrix, 1, 2, 0);
	matrix_set(rotation_matrix, 2, 0, 0);
	matrix_set(rotation_matrix, 2, 1, 0);
	matrix_set(rotation_matrix, 2, 2, 1);

	cam.forward = rotate_world_vector(rotation_matrix, world_forward);
	cam.left = rotate_world_vector(rotation_matrix, world_left);
	cam.up = rotate_world_vector(rotation_matrix, world_up);

	matrix_free(rotation_matrix);
	update_transition_matrix();
}

void camera_move_forward(double distance) {
	cam.position.x += distance * cam.forward.x;
	cam.position.y += distance * cam.forward.y;
	cam.position.z += distance * cam.forward.z;
}

void camera_move_left(double distance) {
	cam.position.x += distance * cam.left.x;
	cam.position.y += distance * cam.left.y;
	cam.position.z += distance * cam.left.z;
}

void camera_move_up(double distance) {
	cam.position.x += distance * cam.up.x;
	cam.position.y += distance * cam.up.y;
	cam.position.z += distance * cam.up.z;
}
