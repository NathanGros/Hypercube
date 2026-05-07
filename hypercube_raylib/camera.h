#ifndef CAMERA_H
#define CAMERA_H

#include "vectors.h"
#include "matrices.h"

typedef struct {
	vector4_t position;
	vector4_t forward;
	vector4_t left;
	vector4_t up;
	vector4_t in;
} camera_t;

void update_transition_matrix();
matrix_t *get_transition_matrix();
matrix_t *get_inverse_transition_matrix();
void init_camera();
void end_camera();
void set_camera_fov(double fov);
double get_camera_fov();
camera_t get_camera();
void camera_rotate_wx(double angle);
void camera_rotate_wy(double angle);
void camera_rotate_wz(double angle);
void camera_rotate_xy(double angle);
void camera_rotate_xz(double angle);
void camera_rotate_yz(double angle);
void camera_move_forward(double distance);
void camera_move_up(double distance);
void camera_move_left(double distance);
void camera_move_in(double distance);
vector4_t camera_rotate_point(vector4_t point);

#endif
