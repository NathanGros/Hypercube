#include "vectors.h"

double norm_vector4(vector4_t v) {
	return sqrt(v.w*v.w + v.x*v.x + v.y*v.y + v.z*v.z);
}
