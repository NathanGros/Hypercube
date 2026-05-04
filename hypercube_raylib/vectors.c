#include "vectors.h"

double norm_vector4(vector4_t v) {
	return sqrt(v.w*v.w + v.x*v.x + v.y*v.y + v.z*v.z);
}

vector4_t vector4_add(vector4_t v1, vector4_t v2) {
	return (vector4_t) {
		v1.w + v2.w,
		v1.x + v2.x,
		v1.y + v2.y,
		v1.z + v2.z
	};
}

vector4_t vector4_scalar_multiply(vector4_t v, double n) {
	return (vector4_t) {
		v.w * n,
		v.x * n,
		v.y * n,
		v.z * n
	};
}
