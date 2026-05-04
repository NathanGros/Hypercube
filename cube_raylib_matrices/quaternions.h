#ifndef QUATERNIONS_H
#define QUATERNIONS_H

#include <math.h>
#include "vectors.h"

typedef struct {
	double angle;
	double x;
	double y;
	double z;
} quaternion_t;

quaternion_t quaternion_multiply(quaternion_t q1, quaternion_t q2);
quaternion_t quaternion_conjugate(quaternion_t q);
vector3_t quaternion_rotate_vector(quaternion_t q, vector3_t v);
quaternion_t quaternion_from_axis_angle(vector3_t axis, double angle);

#endif
