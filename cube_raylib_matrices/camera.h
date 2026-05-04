#ifndef CAMERA_H
#define CAMERA_H

#include "vectors.h"
#include "matrices.h"

typedef struct {
	vector3_t position;
	vector3_t forward;
	vector3_t left;
	vector3_t up;
} camera_t;

void update_transition_matrix();
matrix_t *get_transition_matrix();
matrix_t *get_inverse_transition_matrix();
void init_camera();
void set_camera_fov(double fov);
double get_camera_fov();
camera_t get_camera();
void camera_rotate_pitch(double angle);
void camera_rotate_yaw(double angle);
void camera_rotate_roll(double angle);
void camera_move_forward(double distance);
void camera_move_up(double distance);
void camera_move_left(double distance);

#endif
