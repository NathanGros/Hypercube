#include "camera.h"

double camera_fov = 110;
camera_t cam;

vector3_t world_forward = (vector3_t) {0, 0, 1};
vector3_t world_left = (vector3_t) {1, 0, 0};
vector3_t world_up = (vector3_t) {0, 1, 0};

void init_camera() {
	cam.position = (vector3_t) {4.5, 0.5, 0.5};
	cam.orientation = (quaternion_t) {1, 0, 0, 0};
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

void camera_rotate_pitch(double angle) {
	vector3_t local_left = vector_normalize(quaternion_rotate_vector(cam.orientation, world_left));
	quaternion_t pitch_rotation = quaternion_from_axis_angle(local_left, angle);
	cam.orientation = quaternion_multiply(pitch_rotation, cam.orientation);
}

void camera_rotate_yaw(double angle) {
	vector3_t local_up = vector_normalize(quaternion_rotate_vector(cam.orientation, world_up));
	quaternion_t yaw_rotation = quaternion_from_axis_angle(local_up, angle);
	cam.orientation = quaternion_multiply(yaw_rotation, cam.orientation);
}

void camera_rotate_roll(double angle) {
	vector3_t local_forward = vector_normalize(quaternion_rotate_vector(cam.orientation, world_forward));
	quaternion_t roll_rotation = quaternion_from_axis_angle(local_forward, angle);
	cam.orientation = quaternion_multiply(roll_rotation, cam.orientation);
}

void camera_move_forward(double distance) {
	vector3_t local_forward = vector_normalize(quaternion_rotate_vector(cam.orientation, world_forward));
	cam.position.x += distance * local_forward.x;
	cam.position.y += distance * local_forward.y;
	cam.position.z += distance * local_forward.z;
}

void camera_move_left(double distance) {
	vector3_t local_left = vector_normalize(quaternion_rotate_vector(cam.orientation, world_left));
	cam.position.x += distance * local_left.x;
	cam.position.y += distance * local_left.y;
	cam.position.z += distance * local_left.z;
}

void camera_move_up(double distance) {
	vector3_t local_up = vector_normalize(quaternion_rotate_vector(cam.orientation, world_up));
	cam.position.x += distance * local_up.x;
	cam.position.y += distance * local_up.y;
	cam.position.z += distance * local_up.z;
}
