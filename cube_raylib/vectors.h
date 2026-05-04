#ifndef VECTORS_H
#define VECTORS_H

#include <math.h>

typedef struct {
	double x;
	double y;
	double z;
} vector3_t;

typedef struct {
	double x;
	double y;
} vector2_t;

vector3_t vector_normalize(vector3_t v);

#endif
