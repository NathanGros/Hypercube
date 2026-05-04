#include "vectors.h"

vector3_t vector_normalize(vector3_t v) {
	double length = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
	return (vector3_t) {
		v.x / length,
		v.y / length,
		v.z / length
	};
}
