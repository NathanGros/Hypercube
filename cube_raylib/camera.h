#ifndef CAMERA_H
#define CAMERA_H

#include "vectors.h"
#include "quaternions.h"

typedef struct {
	vector3_t position;
	quaternion_t orientation;
} camera_t;

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
