#ifndef VECTORS_H
#define VECTORS_H

#include <math.h>

typedef struct {
	double w;
	double x;
	double y;
	double z;
} vector4_t;

typedef struct {
	double x;
	double y;
	double z;
} vector3_t;

typedef struct {
	double x;
	double y;
} vector2_t;

double norm_vector4(vector4_t v);

#endif
