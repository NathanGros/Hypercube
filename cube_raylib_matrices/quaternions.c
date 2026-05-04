#include "quaternions.h"

quaternion_t quaternion_multiply(quaternion_t q1, quaternion_t q2) {
	quaternion_t res;
	res.angle = q1.angle*q2.angle - q1.x*q2.x -q1.y*q2.y - q1.z*q2.z;
	res.x = q1.angle*q2.x + q1.x*q2.angle + q1.y*q2.z - q1.z*q2.y;
	res.y = q1.angle*q2.y - q1.x*q2.z + q1.y*q2.angle + q1.z*q2.x;
	res.z = q1.angle*q2.z + q1.x*q2.y - q1.y*q2.x + q1.z*q2.angle;
	return res;
}

quaternion_t quaternion_conjugate(quaternion_t q) {
	return (quaternion_t) {q.angle, -q.x, -q.y, -q.z};
}

vector3_t quaternion_rotate_vector(quaternion_t q, vector3_t v) {
	// v' = q x v x q*
	quaternion_t v_quat = {0, v.x, v.y, v.z};
	quaternion_t rotated_v_quat = quaternion_multiply(quaternion_multiply(q, v_quat), quaternion_conjugate(q));
	return (vector3_t) {rotated_v_quat.x, rotated_v_quat.y, rotated_v_quat.z};
}

quaternion_t quaternion_from_axis_angle(vector3_t axis, double angle) {
	double half_angle = angle / 2;
	double sin_half = sin(half_angle);
	return (quaternion_t) {
		cos(half_angle),
		sin_half * axis.x,
		sin_half * axis.y,
		sin_half * axis.z
	};
}
